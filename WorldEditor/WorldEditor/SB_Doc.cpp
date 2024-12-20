#include "stdafx.h"
#include "AB_App.h"
#include "SB_Doc.h"

#include "FUSIONView.h"
#include "units.h"
#include "node.h"

#define MAX_PIXEL_SELECT_DIST (50)
#define MIN_ENTITY_SELECT_DIST (8.0f)
#define MAX_PIXEL_SELECT_THINGNAME (20)

// Maximum distance from entity in order for it to be selected.
// This is in world space coordinates and is used in rendered view only.
#define MAX_ENTITY_SELECT_DIST (16.0f)

#define CAMERA_MOVEMENT_DISTANCE (32.0f)
#define CAMERA_MOVEMENT_ANGLE (M_PI/16.0f)

typedef struct
{
    BOOL Select;
    int WhichGroup;
    CFusionDoc* pDoc;
} BrushSelectCallbackData;

SB_Doc::SB_Doc(void)
{
    pTempSelBrushes = NULL;
    mCurrentEntity = -1;
    mShowSelectedFaces = FALSE;
    IsNewDocument = 1;
    mShowEntities = GE_TRUE;
    mCurTextureSelection = 1;
    mActiveView = -1;
    mModeTool = ID_TOOLS_TEMPLATE;
    mShowBrush = TRUE;
    mCurrentTool = ID_TOOLS_BRUSH_MOVEROTATEBRUSH;
    mCurrentBitmap = 0;
    NumSelEntities = 0;
    mCurrentGroup = 0;

    Render_WE_World = 1;

    TempShearTemplate = NULL;
    pSelFaces = NULL;
    pSelBrushes = NULL;

    SelectLock = FALSE; 
    TempEnt = FALSE; 
    SelState = NOSELECTIONS;
    mShowSelectedBrushes = FALSE;

    mWorldBsp = NULL;
    mConstrainHollows = GE_TRUE,
    PlaceObjectFlag = FALSE;
    mAdjustMode = ADJUST_MODE_FACE;

    LeakPoints = NULL;

    pUndoStack = UndoStack_Create();
}

SB_Doc::~SB_Doc(void)
{
    if (pLevel != NULL)
    {
        Level_Destroy(&pLevel);
        pLevel = NULL;
    }

    if (mWorldBsp != NULL)
    {
        Node_ClearBsp(App->CLSB_Doc->mWorldBsp);
        mWorldBsp = NULL;
    }

    if (BTemplate != NULL)
    {
        Brush_Destroy(&BTemplate);
        BTemplate = NULL;
    }

    if (pUndoStack != NULL)
    {
        UndoStack_Destroy(&pUndoStack);
        pUndoStack = NULL;
    }

    if (pSelBrushes != NULL)
    {
        SelBrushList_Destroy(&pSelBrushes);
        pSelBrushes = NULL;
    }

    if (pTempSelBrushes != NULL)
    {
        SelBrushList_Destroy(&pTempSelBrushes);
        pTempSelBrushes = NULL;
    }

    if (LeakPoints != NULL)
    {
        //geRam_Free(LeakPoints);
        //LeakPoints = NULL;
    }

    if (pSelFaces != NULL)
    {
        SelFaceList_Destroy(&pSelFaces);
        pSelFaces = NULL;
    }

   // App->Say("Deleting SB_Doc");
}

// *************************************************************************
// *			        	AddBrushToWorld	                           	   *
// *************************************************************************
void SB_Doc::AddBrushToWorld()
{
    App->Get_Current_Document();

    if (TempEnt || !Brush_IsSubtract(CurBrush))
    {
        App->m_pDoc->OnBrushAddtoworld();
    }
    else
    {
        App->m_pDoc->OnBrushSubtractfromworld();
    }

    App->m_pDoc->SetModifiedFlag();
}

// *************************************************************************
// *			        	DeleteCurrentThing	                    	   *
// *************************************************************************
void SB_Doc::DeleteCurrentThing()
{
    App->Get_Current_Document();

    BOOL	ReBuild;

    if (mModeTool == ID_GENERALSELECT)
    {
        // set wait cursor
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

        ResetAllSelectedFaces();
        ReBuild = (GetSelState() & ANYBRUSH);

        DeleteSelectedBrushes();

        if (ReBuild && Level_RebuildBspAlways(pLevel))
        {
            UpdateAllViews(UAV_ALL3DVIEWS | REBUILD_QUICK, NULL, TRUE);
        }
        else
        {
            UpdateAllViews(UAV_ALL3DVIEWS, NULL);
        }

        // put cursor back
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
    }
}

// *************************************************************************
// *			        	DeleteSelectedBrushes	                	   *
// *************************************************************************
bool SB_Doc::DeleteSelectedBrushes()
{
    App->Get_Current_Document();

    geBoolean	bAlteredCurrentGroup = GE_FALSE;
    CEntityArray* Entities = Level_GetEntities(pLevel);

    for (int Ent = 0; Ent < Entities->GetSize() && (!(GetSelState() & NOENTITIES)); Ent++)
    {
        if ((*Entities)[Ent].IsSelected())
        {
            if ((*Entities)[Ent].IsCamera() == GE_FALSE)	// Exclude Cameras
            {
                if ((*Entities)[Ent].GetGroupId() == mCurrentGroup)
                {
                    bAlteredCurrentGroup = GE_TRUE;
                }

                DeleteEntity(Ent--);
                App->m_pDoc->SetModifiedFlag();
            }
        }
    }

    if (GetSelState() & ANYBRUSH)
    {
        int NumSelBrushes = SelBrushList_GetSize(pSelBrushes);
        for (int i = 0; i < NumSelBrushes; i++)
        {
            Brush* pBrush;

            pBrush = SelBrushList_GetBrush(pSelBrushes, 0);
           
            if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
                continue;
        
            if (Brush_GetGroupId(pBrush) == mCurrentGroup)
            {
                bAlteredCurrentGroup = GE_TRUE;
            }

            Level_RemoveBrush(pLevel, pBrush);
            SelBrushList_Remove(pSelBrushes, pBrush);
            Brush_Destroy(&pBrush);
        }

        //turn off any operation tools
        mCurrentTool = CURTOOL_NONE;

        App->m_pDoc->SetModifiedFlag();
    }

    // Deleting items removed group members so we must update the UI
    if (bAlteredCurrentGroup)
    {
        App->CL_TabsGroups_Dlg->Fill_ListBox();
    }

    UpdateSelected();

	return FALSE;
}

// *************************************************************************
// *			            	DeleteEntity	                    	   *
// *************************************************************************
void SB_Doc::DeleteEntity(int EntityIndex)
{
    App->Get_Current_Document();

    CEntityArray* Entities;

    // now delete the entity, we'll do the fixups later
    Entities = Level_GetEntities(pLevel);

    App->m_pDoc->DeselectEntity(&(*Entities)[EntityIndex]);
    // changed QD Actors
    Brush* b = (*Entities)[EntityIndex].GetActorBrush();
    if (b != NULL)
    {
        SelBrushList_Remove(pSelBrushes, b);
        Level_RemoveBrush(pLevel, b);
    }
    // end change
    Entities->RemoveAt(EntityIndex);
    SelState &= (~ENTITYCLEAR);
    SelState |= (NumSelEntities > 1) ? MULTIENTITY : (NumSelEntities + 1) << 7;
}

// *************************************************************************
// *			        	    OnEditDelete                           	   *
// *************************************************************************
void SB_Doc::OnEditDelete()
{
    App->Get_Current_Document();

    DeleteCurrentThing();
    App->m_pDoc->SetModifiedFlag();
}

// *************************************************************************
// *			        	    OnViewTypeWireFrame                    	   *
// *************************************************************************
void SB_Doc::OnViewTypeWireFrame()
{
    App->Get_Current_Document();

    App->m_pDoc->SetModifiedFlag();

    CFusionView* pFusionView = App->m_pDoc->GetCameraView();
    if (!pFusionView)
        return;

    pFusionView->OnViewType(ID_VIEW_3DWIREFRAME);
   
}

// *************************************************************************
// *			        	    OnViewTypeTexture                    	   *
// *************************************************************************
void SB_Doc::OnViewTypeTexture()
{
    App->Get_Current_Document();

    App->m_pDoc->SetModifiedFlag();

    CFusionView* pFusionView = App->m_pDoc->GetCameraView();
    if (!pFusionView)
        return;

    pFusionView->OnViewType(ID_VIEW_TEXTUREVIEW);

}


// *************************************************************************
// *			        	   SelectOrtho                             	   *
// *************************************************************************
void SB_Doc::SelectOrtho(CPoint point, ViewVars* v)
{
	App->Get_Current_Document();

	Brush* pMinBrush;
	CEntity* pMinEntity;
	geFloat Dist;
	int FoundThingType;

	if (IsSelectionLocked())
	{
		return;
	}

	// if Control key isn't pressed, then clear all current selections
	if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) == 0)
	{
		ResetAllSelections();
	}

	FoundThingType = FindClosestThing(&point, v, &pMinBrush, &pMinEntity, &Dist);

	if ((FoundThingType != fctNOTHING) && (Dist <= MAX_PIXEL_SELECT_DIST))
	{
		switch (FoundThingType)
		{
		case fctBRUSH:
		{
			DoBrushSelection(pMinBrush, brushSelToggle);
            if (App->CLSB_Brushes->Dimensions_Dlg_Running == 1)
            {
                App->CLSB_Brushes->Update_Pos_Dlg(App->CLSB_Brushes->Dimensions_Dlg_hWnd);
            }
			break;
		}
		case fctENTITY:
			DoEntitySelection(pMinEntity);
			break;
		default:
			// bad value returned from FindClosestThing
			assert(0);
		}
	}

	UpdateSelected();

	App->CLSB_TabsControl->Select_Brushes_Tab(0);
	App->CL_TabsGroups_Dlg->Get_Index(CurBrush);

	App->CL_TabsGroups_Dlg->Update_Dlg_Controls();
    App->CLSB_TopTabs->Update_Dlg_Controls();

}

static geBoolean fdocBrushCSGCallback(const Brush* pBrush, void* lParam)
{
    App->Get_Current_Document();

    return (App->m_pDoc->BrushIsVisible(pBrush) && (!Brush_IsHint(pBrush)) && (!Brush_IsClip(pBrush)));
}

// *************************************************************************
// *			                     DoneResize                       	   *
// *************************************************************************
void SB_Doc::DoneResize(int sides, int inidx)
{
    App->Get_Current_Document();

    mLastOp = BRUSH_SCALE;

    TempDeleteSelected();

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        if (Brush_IsMulti(CurBrush))
        {
            BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(CurBrush), Brush_GetModelId(CurBrush));
            BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(CurBrush), Brush_GetModelId(CurBrush), fdocBrushCSGCallback, NULL);
        }
        return;
    }

    int NumSelBrushes = SelBrushList_GetSize(pSelBrushes);
    for (int i = 0; i < NumSelBrushes; ++i)
    {
        Brush* pBrush;

        pBrush = SelBrushList_GetBrush(pSelBrushes, i);
       
        if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
            continue;
       
        Brush_ResizeFinal(pBrush, sides, inidx, &FinalScale);
        if (Brush_IsMulti(pBrush))
        {
            BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), Brush_GetModelId(pBrush));
            BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), Brush_GetModelId(pBrush), fdocBrushCSGCallback, NULL);
        }
    }

    UpdateSelected();
}

// *************************************************************************
// *			                 DoneMove                           	   *
// *************************************************************************
void SB_Doc::DoneMove(void)
{
    int	i;
    //	BrushList *BList = Level_GetBrushes (pLevel);

    mLastOp = BRUSH_MOVE;

    TempDeleteSelected();

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        if (TempEnt)
        {
            DoneMoveEntity();
        }
        else
        {
            Brush_Move(CurBrush, &FinalPos);
        }
        return;
    }
    else
    {
        int NumSelBrushes = SelBrushList_GetSize(pSelBrushes);
        for (i = 0; i < NumSelBrushes; i++)
        {
            Brush* pBrush;

            pBrush = SelBrushList_GetBrush(pSelBrushes, i);
           
            if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
                continue;
            
            Brush_Move(pBrush, &FinalPos);
        }

        if (GetSelState() & ANYENTITY)
        {
            DoneMoveEntity();
        }

        UpdateSelected();

        App->m_pDoc->UpdateSelectedModel(BRUSH_MOVE, &FinalPos);
    }
   
    geVec3d_Clear(&FinalPos);

}

// *************************************************************************
// *          DoneMoveEntity:- Terry and Hazel Flanigan 2023                *
// *************************************************************************
void SB_Doc::DoneMoveEntity(void)
{
    App->Get_Current_Document();

    int		i;
    float	SnapSize;
    CEntityArray* Entities = Level_GetEntities(pLevel);
    CEntity* pEnt;

    if (mCurrentTool == ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
    {
        if ((GetSelState() == ONEENTITYONLY) && Level_UseGrid(pLevel))
        {
            SnapSize = Level_GetGridSnapSize(pLevel);
        }
        else
        {
            SnapSize = 1.0f;
        }

        for (i = 0; i < Entities->GetSize(); i++)
        {
            pEnt = &(*Entities)[i];

            if (pEnt->IsSelected())
            {
                pEnt->DoneMove(SnapSize, Level_GetEntityDefs(pLevel));
                if (pEnt->IsCamera() == GE_TRUE)	// Camera Entity?
                    //				if( pEnt==pCameraEntity )	// Camera Entity?
                {
                    geVec3d	PitchRollYaw;

                    pEnt->GetAngles(&PitchRollYaw, Level_GetEntityDefs(pLevel));
                    App->m_pDoc->SetRenderedViewCamera(&(pEnt->mOrigin), &PitchRollYaw);
                    UpdateAllViews(UAV_RENDER_ONLY, NULL);
                }// Camera entity, update camera
            }// Entity Selected
        }// Loop thru Entities
    }
    else
    {
        if (mCurrentEntity < 0) //template
        {
            pEnt = &mRegularEntity;
        }
        else
        {
            pEnt = &(*Entities)[mCurrentEntity];
        }

        SnapSize = 1.0f;
        if (Level_UseGrid(pLevel))
        {
            SnapSize = Level_GetGridSnapSize(pLevel);
        }
        pEnt->DoneMove(SnapSize, Level_GetEntityDefs(pLevel));
    }
    App->m_pDoc->SetModifiedFlag();
}

// *************************************************************************
//          Lock_AllTextures:- Terry and Hazel Flanigan 2023           	   *
// *************************************************************************
void SB_Doc::Lock_AllTextures(void)
{
    // Texture Lock All Faces

    App->Get_Current_Document();

    SelectAll();
    UpdateAllViews(UAV_ALL3DVIEWS, NULL);

    Face* pFace;
    int NumberOfFaces;

    NumberOfFaces = SelFaceList_GetSize(pSelFaces);

    for (int i = 0; i < NumberOfFaces; ++i)
    {
        pFace = SelFaceList_GetFace(pSelFaces, i);
        Face_SetTextureLock(pFace, true);
    }

    ResetAllSelections();
    UpdateSelected();
    UpdateAllViews(UAV_ALL3DVIEWS, NULL);

}

static geBoolean fdocSelectEntity(CEntity& Ent, void* lParam)
{
    Ent.DeSelect();
    App->m_pDoc->SelectEntity(&Ent);
    return GE_TRUE;
}

static geBoolean fdocSelectBrush(Brush* pBrush, void* lParam)
{
    
    SelBrushList_Add(App->CLSB_Doc->pSelBrushes, pBrush);

    return GE_TRUE;
}

static geBoolean SelAllBrushFaces(Brush* pBrush, void* lParam)
{
    int iFace, nFaces;
    
    nFaces = Brush_GetNumFaces(pBrush);
    for (iFace = 0; iFace < nFaces; ++iFace)
    {
        Face* pFace;

        pFace = Brush_GetFace(pBrush, iFace);
        Face_SetSelected(pFace, GE_TRUE);
        SelFaceList_Add(App->CLSB_Doc->pSelFaces, pFace);
    }
    return GE_TRUE;
}

// *************************************************************************
// *              SelectAll:- Terry and Hazel Flanigan 2023           	   *
// *************************************************************************
void SB_Doc::SelectAll(void)
{
    App->Get_Current_Document();

    DoGeneralSelect();

    NumSelEntities = 0;
    Level_EnumEntities(pLevel, this, fdocSelectEntity);
    Level_EnumBrushes(pLevel, this, fdocSelectBrush);

    // Select all faces on all selected brushes
    int iBrush;
    int NumSelBrushes = SelBrushList_GetSize(pSelBrushes);

    for (iBrush = 0; iBrush < NumSelBrushes; ++iBrush)
    {
        Brush* pBrush;

        pBrush = SelBrushList_GetBrush(pSelBrushes, iBrush);

        if (Brush_IsMulti(pBrush))
        {
            BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(pBrush), this, SelAllBrushFaces);
        }
        else
        {
            SelAllBrushFaces(pBrush, this);
        }

    }

    UpdateSelected();

    ConfigureCurrentTool();
}

// *************************************************************************
// *             UpdateSelected:- Terry and Hazel Flanigan 2023            *
// *************************************************************************
void SB_Doc::UpdateSelected(void)
{
    App->Get_Current_Document();

    int		i;
    int NumSelFaces = SelFaceList_GetSize(pSelFaces);
    int NumSelBrushes = SelBrushList_GetSize(pSelBrushes);

    SelState = (NumSelBrushes > 1) ? MULTIBRUSH : NumSelBrushes;
    SelState |= (NumSelFaces > 1) ? MULTIFACE : (NumSelFaces + 1) << 3;
    SelState |= (NumSelEntities > 1) ? MULTIENTITY : (NumSelEntities + 1) << 7;


    if (mModeTool == ID_GENERALSELECT)
    {
        if (GetSelState() & ONEBRUSH)
        {
            CurBrush = SelBrushList_GetBrush(pSelBrushes, 0);
        }
        else
        {
            CurBrush = BTemplate;
        }
    }

    geVec3d_Clear(&SelectedGeoCenter);

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        if (TempEnt)
        {
            SelectedGeoCenter = mRegularEntity.mOrigin;
        }
        else
        {
            Brush_Center(CurBrush, &SelectedGeoCenter);
        }
    }
    else if (SelState != NOSELECTIONS)
    {
        Model* pModel;
        ModelInfo_Type* ModelInfo = Level_GetModelInfo(pLevel);

        pModel = ModelList_GetAnimatingModel(ModelInfo->Models);
        if (pModel != NULL)
        {
            // we're animating a model, so use its current position
            Model_GetCurrentPos(pModel, &SelectedGeoCenter);
        }
        else
        {
            if (NumSelBrushes)
            {
                SelBrushList_Center(pSelBrushes, &SelectedGeoCenter);
            }
            else if (NumSelEntities)
            {
                geVec3d EntitySelectionCenter = { 0.0f,0.0f,0.0f };

                CEntityArray* Entities;
                Entities = Level_GetEntities(pLevel);
                if (Entities)
                {
                    int NumEntities = Entities->GetSize();

                    for (int i = 0; i < NumEntities; i++)
                    {
                        if ((*Entities)[i].IsSelected())
                        {
                            geVec3d_Add(&EntitySelectionCenter, &(*Entities)[i].mOrigin, &EntitySelectionCenter);
                        }
                    }
                }

                geVec3d_Scale(&EntitySelectionCenter, 1 / (float)(NumSelEntities), &SelectedGeoCenter);
            }
        }
    }

    if (SelState & ONEENTITY)
    {
        CEntityArray* Entities = Level_GetEntities(pLevel);

        for (i = 0; i < Entities->GetSize() && !((*Entities)[i].IsSelected()); i++);
        mCurrentEntity = i;
    }
    else
    {
        mCurrentEntity = -1;
    }

    App->m_pDoc->UpdateFaceAttributesDlg();
    App->m_pDoc->UpdateBrushAttributesDlg();

    //assert( mpMainFrame->m_wndTabControls ) ;
    //assert( mpMainFrame->m_wndTabControls->GrpTab ) ;
    //mpMainFrame->m_wndTabControls->GrpTab->UpdateGroupSelection( ) ;

}

// *************************************************************************
// *           ResetAllSelections:- Terry and Hazel Flanigan 2023          *
// *************************************************************************
void SB_Doc::ResetAllSelections(void)
{
    ResetAllSelectedFaces();
    ResetAllSelectedBrushes();
    ResetAllSelectedEntities();
}


#pragma warning (disable:4100)
static geBoolean ResetSelectedFacesCB(Brush* b, void* pVoid)
{
    int	i;

    for (i = 0; i < Brush_GetNumFaces(b); i++)
    {
        Face* pFace;

        pFace = Brush_GetFace(b, i);
        Face_SetSelected(pFace, GE_FALSE);
    }
    return GE_TRUE;
}
#pragma warning (default:4100)

// *************************************************************************
// *          ResetAllSelectedFaces:- Terry and Hazel Flanigan 2023        *
// *************************************************************************
void SB_Doc::ResetAllSelectedFaces(void)
{
    BrushList_EnumLeafBrushes(Level_GetBrushes(pLevel), NULL, ResetSelectedFacesCB);
    SelFaceList_RemoveAll(pSelFaces);
}

// *************************************************************************
// *         ResetAllSelectedBrushes:- Terry and Hazel Flanigan 2023       *
// *************************************************************************
void SB_Doc::ResetAllSelectedBrushes(void)
{
    SelBrushList_RemoveAll(pSelBrushes);
    CurBrush = BTemplate;
}

static geBoolean fdocDeselectEntity(CEntity& Ent, void* lParam)
{
    CFusionDoc* pDoc = (CFusionDoc*)lParam;

    pDoc->DeselectEntity(&Ent);
    return GE_TRUE;
}

// *************************************************************************
// *         ResetAllSelectedEntities:- Terry and Hazel Flanigan 2023      *
// *************************************************************************
void SB_Doc::ResetAllSelectedEntities()
{
    DoGeneralSelect();
    Level_EnumEntities(pLevel, this, fdocDeselectEntity);
}

// *************************************************************************
// *         UpdateAllViews:- Terry and Hazel Flanigan 2023                *
// *************************************************************************
void SB_Doc::UpdateAllViews(int Mode, CView* pSender, BOOL Override)
{
    App->Get_Current_Document();

    if (App->m_pDoc->IsModified() && ((Mode & REBUILD_QUICK) && (Level_RebuildBspAlways(App->CLSB_Doc->pLevel))) || (Override))
    {
        App->m_pDoc->RebuildTrees();
    }
    else if ((Mode & REBUILD_QUICK) && (!Level_RebuildBspAlways(App->CLSB_Doc->pLevel)))
    {
        App->m_pDoc->InvalidateDrawTreeOriginalFaces();
    }

    if (Mode & REBUILD_QUICK)
        Mode &= ~REBUILD_QUICK;

    //	Do we want to redraw everything?
    if (Mode & UAV_ALLVIEWS)
    {
        App->m_pDoc->CDocument::UpdateAllViews(pSender);
        return;
    }

    POSITION pos = App->m_pDoc->GetFirstViewPosition();

    while (pos != NULL)
    {
        CView* pView = App->m_pDoc->GetNextView(pos);

        if (pView->IsKindOf(RUNTIME_CLASS(CFusionView)))
        {
            CFusionView* pFusionView = (CFusionView*)pView;
            CDC* pDC = pFusionView->GetDC();

            switch (Mode)
            {
            case UAV_ACTIVE3DVIEW_ONLY:

                if (pFusionView->GetParentFrame() == App->m_pDoc->mpActiveViewFrame)
                    pFusionView->Invalidate(TRUE);
                break;

            case UAV_NONACTIVE3DVIEWS_ONLY:

                if (pFusionView->GetParentFrame() != App->m_pDoc->mpActiveViewFrame)
                    pFusionView->Invalidate(TRUE);
                break;

            case UAV_TEXTUREVIEW_ONLY:

                if (pFusionView->mViewType == ID_VIEW_TEXTUREVIEW)
                    pFusionView->Invalidate(TRUE);
                break;

            case UAV_RENDER_ONLY:

                switch (pFusionView->mViewType)
                {
                case ID_VIEW_3DWIREFRAME:
                case ID_VIEW_TEXTUREVIEW:

                    pFusionView->Invalidate(TRUE);
                    break;

                default:
                    break;
                }
                break;

            case UAV_GRID_ONLY:

                switch (pFusionView->mViewType)
                {
                case ID_VIEW_TOPVIEW:
                case ID_VIEW_SIDEVIEW:
                case ID_VIEW_FRONTVIEW:

                    pFusionView->Invalidate(TRUE);
                    break;
                }
                break;


            case UAV_THIS_GRID_ONLY:
                if (pFusionView == pSender)
                {
                    switch (pFusionView->mViewType)
                    {
                    case ID_VIEW_TOPVIEW:
                    case ID_VIEW_SIDEVIEW:
                    case ID_VIEW_FRONTVIEW:
                        pFusionView->Invalidate(TRUE);
                        break;
                    }
                }
                break;

            case UAV_ALL3DVIEWS:

                pFusionView->Invalidate(TRUE);
                break;

            default:
                break;
            }

            pFusionView->ReleaseDC(pDC);
        }
    }
}

// *************************************************************************
// *         MoveSelectedBrushes:- Terry and Hazel Flanigan 2023           *
// *************************************************************************
void SB_Doc::MoveSelectedBrushes(geVec3d const* v)
{
    MoveSelectedBrushList(pTempSelBrushes, v);
}

// *************************************************************************
// *         MoveSelectedBrushList:- Terry and Hazel Flanigan 2023         *
// *************************************************************************
void SB_Doc::MoveSelectedBrushList(SelBrushList* pList,geVec3d const* v)
{
    int		i;
    int NumBrushes;
    mLastOp = BRUSH_MOVE;

    geVec3d_Add(&SelectedGeoCenter, v, &SelectedGeoCenter);
    geVec3d_Add(v, &FinalPos, &FinalPos);

    NumBrushes = SelBrushList_GetSize(pList);
    for (i = 0; i < NumBrushes; i++)
    {
        Brush* pBrush;

        pBrush = SelBrushList_GetBrush(pList, i);
        // changed QD Actors
        if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
            continue;
        // end change
        Brush_Move(pBrush, v);
    }

    CEntityArray* Entities = Level_GetEntities(pLevel);
    int NumEntities = Entities->GetSize();
    for (i = 0; i < NumEntities; i++)
    {
        if ((*Entities)[i].IsSelected())
        {
            (*Entities)[i].Move(v);
        }
    }
}

// *************************************************************************
// *          MoveTemplateBrush:- Terry and Hazel Flanigan 2023            *
// *************************************************************************
void SB_Doc::MoveTemplateBrush(geVec3d* v)
{
    mLastOp = BRUSH_MOVE;

    geVec3d_Add(&SelectedGeoCenter, v, &SelectedGeoCenter);
    geVec3d_Add(v, &FinalPos, &FinalPos);

    if (TempEnt)
    {
        mRegularEntity.Move(v);
    }
    else
    {
        geVec3d* pTemplatePos;

        Brush_Move(CurBrush, v);
        pTemplatePos = Level_GetTemplatePos(pLevel);
        Brush_Center(CurBrush, pTemplatePos);
    }
}

// *************************************************************************
// *          MoveSelectedClone:- Terry and Hazel Flanigan 2023            *
// *************************************************************************
void SB_Doc::MoveSelectedClone(geVec3d const* v)
{
    MoveSelectedBrushList(pSelBrushes, v);
}

// *************************************************************************
// *          RotateTemplateBrush:- Terry and Hazel Flanigan 2023          *
// *************************************************************************
void SB_Doc::RotateTemplateBrush(geVec3d* v)
{
    geXForm3d	rm;

    App->CLSB_Doc->mLastOp = BRUSH_ROTATE;

    geVec3d_Add(v, &FinalRot, &FinalRot);
    geXForm3d_SetEulerAngles(&rm, v);
    Brush_Rotate(CurBrush, &rm, &SelectedGeoCenter);
}

// *************************************************************************
// *         AddCameraEntityToLevel:- Terry and Hazel Flanigan 2023        *
// *************************************************************************
void SB_Doc::AddCameraEntityToLevel(void)
{
    CEntity* pCameraEntity = App->CLSB_Camera_WE->FindCameraEntity();
    if (!pCameraEntity)
    {
        // Make default camera entity
        CEntity CameraEntity;
        CString cstr;

        CreateEntityFromName("Camera", CameraEntity);
        cstr.LoadString(IDS_CAMERAENTITYNAME);
        CameraEntity.SetKeyValue("%name%", cstr);
        CameraEntity.SetOrigin(0.0f, 0.0f, 0.0f, Level_GetEntityDefs(App->CLSB_Doc->pLevel));
        Level_AddEntity(App->CLSB_Doc->pLevel, CameraEntity);

        //		pCameraEntity = FindCameraEntity();
    }
}

// *************************************************************************
// *         CreateEntityFromName:- Terry and Hazel Flanigan 2023          *
// *************************************************************************
geBoolean SB_Doc::CreateEntityFromName(char const* pEntityType, CEntity& NewEnt)
{
    assert(pEntityType != NULL);
    // get all properties for this entity type...
    EntityPropertiesList* pProps;

    pProps = EntityTable_GetEntityPropertiesFromName(Level_GetEntityDefs(App->CLSB_Doc->pLevel), pEntityType, ET_ALL);
    if (pProps == NULL)
    {
        return FALSE;
    }

    // Add key/value pairs for all of the properties...
    for (int PropNo = 0; PropNo < pProps->NumProps; ++PropNo)
    {
        EntityProperty* p = &(pProps->Props[PropNo]);

        NewEnt.SetKeyValue(p->pKey, p->pValue);

    }

    EntityTable_ReleaseEntityProperties(pProps);

    NewEnt.SetGroupId(0);
    NewEnt.UpdateOrigin(Level_GetEntityDefs(App->CLSB_Doc->pLevel));
    return TRUE;
}

// *************************************************************************
// *              DoneShear:- Terry and Hazel Flanigan 2023                *
// *************************************************************************
void SB_Doc::DoneShear(int sides, int inidx)
{
    App->Get_Current_Document();

    //	BrushList	*BList = Level_GetBrushes (pLevel);
    const Box3d* bx1, * bx2;
    int			snapside = 0;
    geFloat		bsnap;

    App->m_pDoc->SetModifiedFlag();

    mLastOp = BRUSH_SHEAR;

    bsnap = 1.0f;
    if (Level_UseGrid(pLevel))
    {
        bsnap = Level_GetGridSnapSize(pLevel);
    }

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        if (TempShearTemplate)	//can get here without shearing
        {						//by rapid clicking
            Brush_Destroy(&CurBrush);
            CurBrush = BTemplate = Brush_Clone(TempShearTemplate);
            Brush_ShearFinal(CurBrush, sides, inidx, &FinalScale);

            //check which side of the bounds changed
            bx1 = Brush_GetBoundingBox(CurBrush);
            bx2 = Brush_GetBoundingBox(TempShearTemplate);

            if (bx1->Max.X != bx2->Max.X)	snapside |= 2;
            if (bx1->Max.Y != bx2->Max.Y)	snapside |= 8;
            if (bx1->Max.Z != bx2->Max.Z)	snapside |= 32;
            if (bx1->Min.X != bx2->Min.X)	snapside |= 1;
            if (bx1->Min.Y != bx2->Min.Y)	snapside |= 4;
            if (bx1->Min.Z != bx2->Min.Z)	snapside |= 16;
            Brush_SnapShearNearest(CurBrush, bsnap, sides, inidx, snapside);
            Brush_Destroy(&TempShearTemplate);
        }
        return;
    }

    int NumSelBrushes = SelBrushList_GetSize(pSelBrushes);

    TempDeleteSelected();
    App->m_pDoc->TempCopySelectedBrushes();

    int i;

    for (i = 0; i < NumSelBrushes; ++i)
    {
        Brush* pBrush;
        Brush* tBrush;

        pBrush = SelBrushList_GetBrush(pSelBrushes, i);
        tBrush = SelBrushList_GetBrush(pTempSelBrushes, i);
        // changed QD Actors
        // don't shear ActorBrushes
        if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
            continue;
        // end change

        Brush_ShearFinal(pBrush, sides, inidx, &FinalScale);

        //check which side of the bounds changed
        bx1 = Brush_GetBoundingBox(pBrush);
        bx2 = Brush_GetBoundingBox(tBrush);

        if (bx1->Max.X != bx2->Max.X)	snapside |= 2;
        if (bx1->Max.Y != bx2->Max.Y)	snapside |= 8;
        if (bx1->Max.Z != bx2->Max.Z)	snapside |= 32;
        if (bx1->Min.X != bx2->Min.X)	snapside |= 1;
        if (bx1->Min.Y != bx2->Min.Y)	snapside |= 4;
        if (bx1->Min.Z != bx2->Min.Z)	snapside |= 16;

        Brush_SnapShearNearest(pBrush, bsnap, sides, inidx, snapside);
    }
    TempDeleteSelected();
    UpdateSelected();
}

// *************************************************************************
// *              DoneRotate:- Terry and Hazel Flanigan 2023              *
// *************************************************************************
void SB_Doc::DoneRotate(void)
{
    App->Get_Current_Document();

    int			i;
    geFloat	RSnap;
    geXForm3d		rm;
    geVec3d RotationPoint;
    geVec3d TemplateReversalRot;

    TemplateReversalRot = FinalRot;

    mLastOp = BRUSH_ROTATE;

    TempDeleteSelected();

    App->m_pDoc->TempCopySelectedBrushes();

    App->m_pDoc->GetRotationPoint(&RotationPoint);

    if ((App->CLSB_Doc->SelState & NOENTITIES) && Level_UseGrid(pLevel))
    {
        RSnap = Units_DegreesToRadians((float)Level_GetRotationSnap(pLevel));
        FinalRot.X = ((float)((int)(FinalRot.X / RSnap))) * RSnap;
        FinalRot.Y = ((float)((int)(FinalRot.Y / RSnap))) * RSnap;
        FinalRot.Z = ((float)((int)(FinalRot.Z / RSnap))) * RSnap;
    }

    if (mModeTool == ID_TOOLS_TEMPLATE)
        geVec3d_Subtract(&FinalRot, &TemplateReversalRot, &FinalRot);

    geXForm3d_SetEulerAngles(&rm, &FinalRot);

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        if (TempEnt)
        {
        }
        else
        {
            Brush_Rotate(CurBrush, &rm, &RotationPoint);
        }
        return;
    }

    int NumSelBrushes = SelBrushList_GetSize(pSelBrushes);

    for (i = 0; i < NumSelBrushes; i++)
    {
        Brush* pBrush;

        pBrush = SelBrushList_GetBrush(pTempSelBrushes, i);
        // changed QD Actors
        // don't rotate ActorBrushes
        if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
            continue;
        // end change

        Brush_Rotate(pBrush, &rm, &RotationPoint);
    }
    if (i < NumSelBrushes)
    {
        App->CLSB_Doc->TempDeleteSelected();
    }
    else
    {
        BrushList* BList = Level_GetBrushes(pLevel);
        for (i = 0; i < NumSelBrushes; i++)
        {
            // Replace the sel list brushes with the TSelList brushes
            Brush* TempBrush, * OldBrush;

            TempBrush = SelBrushList_GetBrush(pTempSelBrushes, 0);
            OldBrush = SelBrushList_GetBrush(pSelBrushes, 0);
            // changed QD Actors
            if (strstr(App->CL_Brush->Brush_GetName(OldBrush), ".act") != NULL)
            {
                BrushList_Remove(BList, TempBrush);
                SelBrushList_Remove(pTempSelBrushes, TempBrush);
                continue;
            }
            // end change

            BrushList_Remove(BList, TempBrush);
            BrushList_InsertAfter(BList, OldBrush, TempBrush);
            BrushList_Remove(BList, OldBrush);

            SelBrushList_Remove(pSelBrushes, OldBrush);
            SelBrushList_Remove(pTempSelBrushes, TempBrush);

            SelBrushList_Add(pSelBrushes, TempBrush);

            Brush_Destroy(&OldBrush);
        }
    }

    UpdateSelected();

    App->m_pDoc->UpdateSelectedModel(BRUSH_ROTATE, &FinalRot);

    geVec3d_Clear(&FinalRot);

    // Find the camera entity and update the rendered view's camera position
    {
        CEntity* pCameraEntity = App->CLSB_Camera_WE->FindCameraEntity();

        if (pCameraEntity != NULL)
        {
            geVec3d Angles;

            pCameraEntity->GetAngles(&Angles, Level_GetEntityDefs(pLevel));
            App->m_pDoc->SetRenderedViewCamera(&(pCameraEntity->mOrigin), &Angles);
        }
    }
}

// *************************************************************************
// *          TempDeleteSelected:- Terry and Hazel Flanigan 2023           *
// *************************************************************************
BOOL SB_Doc::TempDeleteSelected(void)
{
    BOOL	ret;
    int		i;
    int		NumTSelBrushes = SelBrushList_GetSize(pTempSelBrushes);

    for (ret = FALSE, i = 0; i < NumTSelBrushes; i++)
    {
        Brush* pBrush;

        pBrush = SelBrushList_GetBrush(pTempSelBrushes, 0);

        Level_RemoveBrush(pLevel, pBrush);
        SelBrushList_Remove(pTempSelBrushes, pBrush);
        Brush_Destroy(&pBrush);
        ret = TRUE;
    }
    return	ret;
}

// *************************************************************************
// *              BrushSelect:- Terry and Hazel Flanigan 2023              *
// *************************************************************************
void SB_Doc::BrushSelect(Brush* pBrush)
{
    // if the brush is already selected, then unselect it.
    // if not currently selected, then select it.
    if (!SelBrushList_Remove(pSelBrushes, pBrush))
    {
        SelBrushList_Add(pSelBrushes, pBrush);
    }
}

typedef struct FindClosestInfoTag
{
    CFusionDoc* pDoc;
    ViewVars* v;
    Brush** ppFoundBrush;
    geFloat* pMinEdgeDist;
    const POINT* ptFrom;
} FindClosestInfo;

static geFloat PointToLineDist
(
    POINT const* ptFrom,
    POINT const* ptLine1,
    POINT const* ptLine2
)
{
    geFloat xkj, ykj;
    geFloat xlk, ylk;
    geFloat denom;
    geFloat dist;

    xkj = (geFloat)(ptLine1->x - ptFrom->x);
    ykj = (geFloat)(ptLine1->y - ptFrom->y);
    xlk = (geFloat)(ptLine2->x - ptLine1->x);
    ylk = (geFloat)(ptLine2->y - ptLine1->y);
    denom = (xlk * xlk) + (ylk * ylk);
    if (denom < .0005f)
    {
        // segment ends coincide
        dist = xkj * xkj + ykj * ykj;
    }
    else
    {
        geFloat t;
        geFloat xfac, yfac;

        t = -(xkj * xlk + ykj * ylk) / denom;
        t = std::max(t, 0.0f);
        t = std::min(t, 1.0f);
        xfac = xkj + t * xlk;
        yfac = ykj + t * ylk;
        dist = xfac * xfac + yfac * yfac;
    }
    return (geFloat)sqrt(dist);
}

static geBoolean FindClosestBrushCB(Brush* pBrush, void* pVoid)
{
    FindClosestInfo* fci = (FindClosestInfo*)pVoid;

    if (fci->pDoc->BrushIsVisible(pBrush))
    {
        // for each face...
        for (int iFace = 0; iFace < Brush_GetNumFaces(pBrush); ++iFace)
        {
            POINT			pt1, pt2;
            Face* pFace = Brush_GetFace(pBrush, iFace);
            const geVec3d* FacePoints = Face_GetPoints(pFace);
            int				NumPoints = Face_GetNumPoints(pFace);

            // Starting with the edge formed by the last point and the first point,
            // determine distance from mouse cursor pos to the edge.
            pt1 = Render_OrthoWorldToView(fci->v, &FacePoints[NumPoints - 1]);
            for (int iPoint = 0; iPoint < NumPoints; ++iPoint)
            {
                geFloat Dist;

                pt2 = Render_OrthoWorldToView(fci->v, &FacePoints[iPoint]);
                Dist = PointToLineDist(fci->ptFrom, &pt1, &pt2);
                if (Dist < *fci->pMinEdgeDist)
                {
                    *fci->pMinEdgeDist = Dist;
                    *fci->ppFoundBrush = pBrush;
                }
                pt1 = pt2;	// next edge...
            }
        }
    }
    return GE_TRUE;
}

// *************************************************************************
// *            FindClosestThing:- Terry and Hazel Flanigan 2023           *
// *************************************************************************
int SB_Doc::FindClosestThing(POINT const* ptFrom,ViewVars* v,Brush** ppMinBrush,CEntity** ppMinEntity,geFloat* pDist)
{
    App->Get_Current_Document();

    int rslt;

    geBoolean FoundBrush;
    geFloat MinEdgeDist;
    Brush* pMinBrush;

    geBoolean FoundEntity;
    geFloat MinEntityDist;
    CEntity* pMinEntity;

    rslt = fctNOTHING;
    FoundBrush = FindClosestBrush(ptFrom, v, &pMinBrush, &MinEdgeDist);
    FoundEntity = FindClosestEntity(ptFrom, v, &pMinEntity, &MinEntityDist);


    if (FoundEntity)
    {
        if ((!FoundBrush) || (MinEntityDist < MinEdgeDist))
        {
            *pDist = MinEntityDist;
            if (ppMinEntity != NULL)
                *ppMinEntity = pMinEntity;
            rslt = fctENTITY;
        }
        else
        {
            *pDist = MinEdgeDist;
            if (ppMinBrush != NULL)
                *ppMinBrush = pMinBrush;
            rslt = fctBRUSH;
        }
    }
    else if (FoundBrush)
    {
        *pDist = MinEdgeDist;
        if (ppMinBrush != NULL)
            *ppMinBrush = pMinBrush;
        rslt = fctBRUSH;
    }
    return rslt;
}

// *************************************************************************
// *            FindClosestBrush:- Terry and Hazel Flanigan 2023           *
// *************************************************************************
geBoolean SB_Doc::FindClosestBrush(POINT const* ptFrom, ViewVars* v, Brush** ppFoundBrush, geFloat* pMinEdgeDist)
{
    App->Get_Current_Document();

    // determine the distance to the closest brush edge in the current view.
    FindClosestInfo	fci;

    *pMinEdgeDist = FLT_MAX;
    *ppFoundBrush = NULL;

    fci.pDoc = App->m_pDoc;
    fci.v = v;
    fci.ppFoundBrush = ppFoundBrush;
    fci.pMinEdgeDist = pMinEdgeDist;
    fci.ptFrom = ptFrom;

    BrushList_EnumLeafBrushes(Level_GetBrushes(App->CLSB_Doc->pLevel), &fci, ::FindClosestBrushCB);

    return	(*ppFoundBrush) ? GE_TRUE : GE_FALSE;
}

// *************************************************************************
// *            FindClosestEntity:- Terry and Hazel Flanigan 2023          *
// *************************************************************************
geBoolean SB_Doc::FindClosestEntity(POINT const* ptFrom,ViewVars* v,CEntity** ppMinEntity,geFloat* pMinEntityDist)
{
    App->Get_Current_Document();

    geBoolean rslt;
    CEntityArray* Entities;

    Entities = Level_GetEntities(pLevel);
    rslt = GE_FALSE;
    // determine distance to closest entity in the current view
    *pMinEntityDist = FLT_MAX;
    *ppMinEntity = NULL;
    for (int i = 0; i < Entities->GetSize(); ++i)
    {
        CEntity* pEnt;
        POINT EntPosView;
        geFloat Dist;
        int dx, dy;

        pEnt = &(*Entities)[i];
        if (App->m_pDoc->EntityIsVisible(pEnt))
        {
            EntPosView = Render_OrthoWorldToView(v, &pEnt->mOrigin);
            dx = EntPosView.x - ptFrom->x;
            dy = EntPosView.y - ptFrom->y;

            Dist = (geFloat)((dx * dx) + (dy * dy));
            if (Dist < *pMinEntityDist)
            {
                *pMinEntityDist = Dist;
                *ppMinEntity = pEnt;
                rslt = GE_TRUE;
            }
        }
    }

    if (rslt)
    {
        *pMinEntityDist = (geFloat)sqrt(*pMinEntityDist);
    }
    return rslt;
}

// *************************************************************************
// *          ReturnThingUnderPoint:- Terry and Hazel Flanigan 2023        *
// *************************************************************************
const char* SB_Doc::ReturnThingUnderPoint(CPoint point, ViewVars* v)
{
    Brush* pMinBrush;
    CEntity* pMinEntity;
    geFloat Dist;
    int FoundThingType;

    FoundThingType = FindClosestThing(&point, v, &pMinBrush, &pMinEntity, &Dist);
    if ((FoundThingType != fctNOTHING) && (Dist <= MAX_PIXEL_SELECT_THINGNAME))
    {
        switch (FoundThingType)
        {
        case fctBRUSH:
            return App->CL_Brush->Brush_GetName(pMinBrush);
            break;
        case fctENTITY:
            return pMinEntity->GetName();
            break;
        default:
            break;
        }
    }

    return "";
}

static geBoolean fdocSelectBrushesFromFaces(Brush* pBrush, void* lParam)
{
    CFusionDoc* pDoc = (CFusionDoc*)lParam;
    int iFace, nFaces;

    // if any of the brush's faces is selected, then select the brush.
    nFaces = Brush_GetNumFaces(pBrush);
    for (iFace = 0; iFace < nFaces; ++iFace)
    {
        Face* pFace;

        pFace = Brush_GetFace(pBrush, iFace);
        if (Face_IsSelected(pFace))
        {
            App->CLSB_Doc->DoBrushSelection(pBrush, brushSelAlways);
            break;
        }
    }
    return GE_TRUE;
}

// *************************************************************************
// *          SetAdjustmentMode:- Terry and Hazel Flanigan 2023            *
// *************************************************************************
void SB_Doc::SetAdjustmentMode(fdocAdjustEnum nCmdIDMode)
{
    App->Get_Current_Document();

    if (mAdjustMode == nCmdIDMode)
        return;

    if (nCmdIDMode == ADJUST_MODE_TOGGLE)
    {
        //		nCmdIDMode = (mAdjustMode == ADJUST_MODE_BRUSH) ? ADJUST_MODE_FACE : ADJUST_MODE_BRUSH;
        nCmdIDMode = ADJUST_MODE_FACE;
    }

    switch (nCmdIDMode)
    {
    case ADJUST_MODE_BRUSH:
        mAdjustMode = nCmdIDMode;

        // go through brush list and select any brush that has selected faces.
        // Ensure that all brushes in a locked group or model set are selected...
        Level_EnumLeafBrushes(pLevel, this, fdocSelectBrushesFromFaces);

        App->CLSB_Doc->ResetAllSelectedFaces();
        App->CLSB_Doc->UpdateSelected();

        //remove face attributes dialog if present...
//			DeleteFaceAttributes ();
        ConfigureCurrentTool();
        break;

    case ADJUST_MODE_FACE:
    {
        mAdjustMode = nCmdIDMode;

        // Select all faces on all selected brushes
        int iBrush;
        int NumSelBrushes = SelBrushList_GetSize(pSelBrushes);

        for (iBrush = 0; iBrush < NumSelBrushes; ++iBrush)
        {
            Brush* pBrush;

            pBrush = SelBrushList_GetBrush(pSelBrushes, iBrush);
            if (Brush_IsMulti(pBrush))
            {
                BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(pBrush), this, ::SelAllBrushFaces);
            }
            else
            {
                ::SelAllBrushFaces(pBrush, this);
            }
        }
        App->CLSB_Doc->UpdateSelected();
        //remove brush attributes dialog if present...
//			DeleteBrushAttributes ();
        ConfigureCurrentTool();
        break;
    }
    default:
        assert(0);		// bad mode (can't happen?)
        break;
    }
}

// *************************************************************************
// *          ConfigureCurrentTool:- Terry and Hazel Flanigan 2023         *
// *************************************************************************
void SB_Doc::ConfigureCurrentTool(void)
{
    BOOL	Redraw = FALSE;

    if (mModeTool == ID_TOOLS_CAMERA)
    {
        mCurrentTool = CURTOOL_NONE;
        mShowSelectedBrushes = (mAdjustMode == ADJUST_MODE_BRUSH);
        mShowSelectedFaces = (mAdjustMode == ADJUST_MODE_FACE);
        UpdateAllViews(UAV_ALL3DVIEWS, NULL);
        return;
    }

    switch (mAdjustMode)
    {
    case ADJUST_MODE_BRUSH:
        mShowSelectedFaces = FALSE;
        mShowSelectedBrushes = TRUE;

        UpdateSelected();
        Redraw = TRUE;
        break;

    case ADJUST_MODE_FACE:
        mShowSelectedFaces = TRUE;
        mShowSelectedBrushes = FALSE;

        UpdateSelected();
        Redraw = TRUE;
        break;

    default:
        assert(0);	// bad adjustment mode
        break;
    }

    switch (mCurrentTool)
    {
    case ID_TOOLS_BRUSH_MOVEROTATEBRUSH:
        if (mModeTool != ID_TOOLS_TEMPLATE)
        {
            mShowSelectedFaces = FALSE;
            mShowSelectedBrushes = TRUE;
            Redraw = TRUE;
        }
        else
        {
            mShowSelectedFaces = FALSE;
            mShowSelectedBrushes = FALSE;
        }
        break;

    case ID_TOOLS_BRUSH_SCALEBRUSH:
        if (mModeTool != ID_TOOLS_TEMPLATE)
        {
            Redraw = TRUE;
            mShowSelectedFaces = FALSE;
            mShowSelectedBrushes = TRUE;
        }
        else
        {
            mShowSelectedFaces = FALSE;
            mShowSelectedBrushes = FALSE;
        }
        break;

    }
    if (mModeTool == ID_TOOLS_TEMPLATE && TempEnt)
    {
        mCurrentEntity = -1;
    }

    Redraw = TRUE;
    if (Redraw)
    {
        UpdateAllViews(UAV_ALL3DVIEWS, NULL);
    }
}

// *************************************************************************
// *           DoGeneralSelect:- Terry and Hazel Flanigan 2023             *
// *************************************************************************
void SB_Doc::DoGeneralSelect(void)
{
    mCurrentTool = CURTOOL_NONE;
    mModeTool = ID_GENERALSELECT;
    ConfigureCurrentTool();
    //mpMainFrame->m_wndTabControls->m_pBrushEntityDialog->Update(this);
}

// *************************************************************************
// *           DoEntitySelection:- Terry and Hazel Flanigan 2023           *
// *************************************************************************
void SB_Doc::DoEntitySelection(CEntity* pEntity)
{
    App->Get_Current_Document();

    // an entity is closest.  Select/deselect it.
    int GroupId;
    geBoolean GroupLocked;
    GroupListType* Groups = Level_GetGroups(pLevel);

    assert(pEntity != NULL);

    GroupLocked = FALSE;
    GroupId = pEntity->GetGroupId();
    if (GroupId != 0)
    {
        GroupLocked = Group_IsLocked(Groups, GroupId);
    }


    if (pEntity->IsSelected())
    {
        if (GroupLocked)
        {
            // deselect entire group
            SelectGroupBrushes(FALSE, GroupId);
        }
        else
        {
            App->m_pDoc->DeselectEntity(pEntity);
        }
    }
    else
    {
        if (GroupLocked)
        {
            // select entire group
            SelectGroupBrushes(TRUE, GroupId);
        }
        else
        {
            App->m_pDoc->SelectEntity(pEntity);
        }
    }
}

// *************************************************************************
// *           DoBrushSelection:- Terry and Hazel Flanigan 2023            *
// *************************************************************************
void SB_Doc::DoBrushSelection(Brush* pBrush, BrushSel	nSelType) //	brushSelToggle | brushSelAlways)
{
    App->Get_Current_Document();

    int ModelId = 0;
    geBoolean ModelLocked;
    ModelInfo_Type* ModelInfo;
    GroupListType* Groups;
    int GroupId = 0;
    geBoolean GroupLocked;
    BrushList* BList;
    Brush* pBParent = NULL;

    ModelInfo = Level_GetModelInfo(pLevel);
    Groups = Level_GetGroups(pLevel);
    BList = Level_GetBrushes(pLevel);

    if (Brush_GetParent(BList, pBrush, &pBParent))
    {
        pBrush = pBParent;
    }

    ModelLocked = GE_FALSE;
    GroupLocked = FALSE;
    //	if(mAdjustMode != ADJUST_MODE_FACE)
    {
        // don't do this stuff if we're in face mode...
        ModelId = Brush_GetModelId(pBrush);
        if (ModelId != 0)
        {
            Model* pModel;

            pModel = ModelList_FindById(ModelInfo->Models, ModelId);
            if (pModel != NULL)
            {
                ModelLocked = Model_IsLocked(pModel);
            }
        }

        if (!ModelLocked)
        {
            GroupId = Brush_GetGroupId(pBrush);
            if (GroupId != 0)
            {
                GroupLocked = Group_IsLocked(Groups, GroupId);
            }
        }
    }

    if (nSelType == brushSelToggle && App->m_pDoc->BrushIsSelected(pBrush))
    {
        if (ModelLocked)
        {
            // model is locked, so deselect everything in the model
            SelectModelBrushes(FALSE, ModelId);
        }
        else if (GroupLocked)
        {
            // group is locked, so deselect entire group
            SelectGroupBrushes(FALSE, GroupId);
        }
        else
        {
            SelBrushList_Remove(pSelBrushes, pBrush);
           
            if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
            {
                CEntityArray* Entities = Level_GetEntities(pLevel);

                for (int i = 0; i < Entities->GetSize(); i++)
                {
                    Brush* b = (*Entities)[i].GetActorBrush();
                    if (b != NULL)
                        if (SelBrushList_Find(pSelBrushes, b))
                            if ((*Entities)[i].IsSelected())
                            {
                                (*Entities)[i].DeSelect();
                                --NumSelEntities;
                            }
                }
            }
        }
    }
    else
    {
        if (ModelLocked)
        {
            // model is locked, so select everything in the model
            SelectModelBrushes(TRUE, ModelId);
        }
        else if (GroupLocked)
        {
            // group is locked.  Select everything in the group
            SelectGroupBrushes(TRUE, GroupId);
        }
        else
        {
            SelBrushList_Add(pSelBrushes, pBrush);
      
            if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
            {
                CEntityArray* Entities = Level_GetEntities(pLevel);

                for (int i = 0; i < Entities->GetSize(); i++)
                {
                    Brush* b = (*Entities)[i].GetActorBrush();
                    if (b != NULL)
                        if (SelBrushList_Find(pSelBrushes, b))
                            if (!(*Entities)[i].IsSelected())
                            {
                                (*Entities)[i].Select();
                                ++NumSelEntities;
                            }
                }
            }  
        }
    }
}

typedef struct
{
    int ModelId;
    BOOL Select;
    CFusionDoc* pDoc;
} fdocBrushSelectData;

static geBoolean fdocSelectBrushCallback(Brush* pBrush, void* lParam)
{
    fdocBrushSelectData* pData;

    pData = (fdocBrushSelectData*)lParam;
    if (Brush_GetModelId(pBrush) == pData->ModelId)
    {
        if (pData->Select)
        {
            SelBrushList_Add(App->CLSB_Doc->pSelBrushes, pBrush);
        }
        else
        {
            SelBrushList_Remove(App->CLSB_Doc->pSelBrushes, pBrush);
        }
    }
    return GE_TRUE;
}

// *************************************************************************
// *           SelectModelBrushes:- Terry and Hazel Flanigan 2023          *
// *************************************************************************
void SB_Doc::SelectModelBrushes(BOOL Select,int ModelId)
{
    App->Get_Current_Document();

    fdocBrushSelectData bsData;

    bsData.Select = Select;
    bsData.ModelId = ModelId;
    bsData.pDoc = App->m_pDoc;

    // Go through the brush list and add all brushes that have
    // this model's id to the selection list.
    Level_EnumBrushes(App->CLSB_Doc->pLevel, &bsData, fdocSelectBrushCallback);

    UpdateSelected();
}

static geBoolean fdocBrushTextureScaleCallback(Brush* pBrush, void* lParam)
{
    const geFloat* pScaleVal = (geFloat*)lParam;

    Brush_SetTextureScale(pBrush, *pScaleVal);
    return GE_TRUE;
}

// *************************************************************************
// *         SetAllFacesTextureScale:- Terry and Hazel Flanigan 2023       *
// *************************************************************************
void SB_Doc::SetAllFacesTextureScale(geFloat ScaleVal)
{
    App->Get_Current_Document();

    if (SelBrushList_GetSize(pSelBrushes) > 0)
    {
        SelBrushList_Enum(pSelBrushes, fdocBrushTextureScaleCallback, &ScaleVal);
        if (Level_RebuildBspAlways(pLevel))
        {
            App->m_pDoc->RebuildTrees();
            UpdateAllViews(UAV_ALL3DVIEWS, NULL);
        }
    }
}


static geBoolean BrushSelect(Brush* pBrush, void* lParam)
{
    BrushSelectCallbackData* pData;

    pData = (BrushSelectCallbackData*)lParam;

    // select/deselect all group brushes
    if (Brush_GetGroupId(pBrush) == pData->WhichGroup)
    {
        if (pData->Select)
        {	// add this brush to the selected list
            SelBrushList_Add(App->CLSB_Doc->pSelBrushes, pBrush);
        }
        else
        {
            // remove this brush from the selection list
            SelBrushList_Remove(App->CLSB_Doc->pSelBrushes, pBrush);
        }
    }
    return GE_TRUE;
}


static geBoolean EntitySelect(CEntity& Entity, void* lParam)
{
    BrushSelectCallbackData* pData;
    CFusionDoc* pDoc;

    pData = (BrushSelectCallbackData*)lParam;
    pDoc = pData->pDoc;

    if (Entity.GetGroupId() == pData->WhichGroup)
    {
        if (pData->Select)
        {
            pDoc->SelectEntity(&Entity);
        }
        else
        {
            pDoc->DeselectEntity(&Entity);
        }
    }
    return GE_TRUE;
}

// *************************************************************************
// *            SelectGroupBrushes:- Terry and Hazel Flanigan 2023         *
// *************************************************************************
void SB_Doc::SelectGroupBrushes(BOOL Select,int WhichGroup)
{
    App->Get_Current_Document();

    BrushSelectCallbackData SelectData;

    SelectData.Select = Select;
    SelectData.WhichGroup = WhichGroup;
    SelectData.pDoc = App->m_pDoc;

    Level_EnumBrushes(pLevel, &SelectData, ::BrushSelect);
    Level_EnumEntities(pLevel, &SelectData, EntitySelect);
    UpdateSelected();		// update selection information
}

// *************************************************************************
// * Genesis      ShearSelected:- Terry and Hazel Flanigan 2023            *
// *************************************************************************
void SB_Doc::ShearSelected(float dx, float dy, int sides, int inidx)
{
    App->Get_Current_Document();

    mLastOp = BRUSH_SHEAR;

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        Brush_Destroy(&CurBrush);
        CurBrush = BTemplate = Brush_Clone(TempShearTemplate);
        Brush_ShearFixed(CurBrush, dx, dy, sides, inidx, &FinalScale, &ScaleNum);
    }
    else
    {
        int i;
        int NumSelBrushes;

        TempDeleteSelected();
        App->m_pDoc->TempCopySelectedBrushes();
        NumSelBrushes = SelBrushList_GetSize(pTempSelBrushes);
        for (i = 0; i < NumSelBrushes; ++i)
        {
            Brush* pBrush;

            pBrush = SelBrushList_GetBrush(pTempSelBrushes, i);
            Brush_ShearFixed(pBrush, dx, dy, sides, inidx, &FinalScale, &ScaleNum);
        }
    }

    App->m_pDoc->SetModifiedFlag();
}

// *************************************************************************
// * Genesis      ResizeSelected:- Terry and Hazel Flanigan 2023           *
// *************************************************************************
void SB_Doc::ResizeSelected(float dx, float dy, int sides, int inidx)
{
    mLastOp = BRUSH_SCALE;

    if (mModeTool == ID_TOOLS_TEMPLATE)
    {
        Brush_Resize(CurBrush, dx, dy, sides, inidx, &FinalScale, &ScaleNum);
        if (Brush_IsMulti(CurBrush))
        {
            BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(CurBrush), Brush_GetModelId(CurBrush));
            BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(CurBrush), Brush_GetModelId(CurBrush), ::fdocBrushCSGCallback, this);
        }
    }
    else
    {
        int i;
        int NumBrushes;

        NumBrushes = SelBrushList_GetSize(pTempSelBrushes);

        for (i = 0; i < NumBrushes; ++i)
        {
            Brush* pBrush;

            pBrush = SelBrushList_GetBrush(pTempSelBrushes, i);

            Brush_Resize(pBrush, dx, dy, sides, inidx, &FinalScale, &ScaleNum);
            if (Brush_IsMulti(pBrush))
            {
                BrushList_ClearCSGAndHollows((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), Brush_GetModelId(pBrush));
                BrushList_RebuildHollowFaces((BrushList*)App->CL_Brush->Brush_GetBrushList(pBrush), Brush_GetModelId(pBrush), ::fdocBrushCSGCallback, this);
            }
        }
    }
}

// *************************************************************************
// * Genesis    OnUpdateLinkviewports:- Terry and Hazel Flanigan 2023      *
// *************************************************************************
void SB_Doc::OnUpdateLinkviewports(CCmdUI* pCmdUI)
{
    if (Prefs_GetLinkViewports(((CFusionApp*)AfxGetApp())->GetPreferencesNormal()))
    {
        pCmdUI->SetCheck(TRUE);
        App->CLSB_ViewMgrDlg->LinkViews_Flag = 0;
    }
    else
    {
        pCmdUI->SetCheck(FALSE);
        App->CLSB_ViewMgrDlg->LinkViews_Flag = 1;
    }

    RedrawWindow(App->CLSB_ViewMgrDlg->MgrDlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// * Genesis      OnLinkviewports:- Terry and Hazel Flanigan 2023          *
// *************************************************************************
void SB_Doc::OnLinkviewports()
{
    Prefs* CurrentPrefs = ((CFusionApp*)AfxGetApp())->GetPreferencesNormal();

    if (Prefs_GetLinkViewports(CurrentPrefs))
    {
        Prefs_SetLinkViewports(CurrentPrefs, FALSE);
        return;
    }
    else
    {
        Prefs_SetLinkViewports(CurrentPrefs, TRUE);
        LinkViewports();
    }
}

// *************************************************************************
// * Genesis      LinkViewports:- Terry and Hazel Flanigan 2023            *
// *************************************************************************
void SB_Doc::LinkViewports()
{
    App->Get_Current_Document();


    if (!Prefs_GetLinkViewports(((CFusionApp*)AfxGetApp())->GetPreferencesNormal()))
        return;

    CView* pAView = NULL;
    CFusionView* pActiveView = NULL;

    CMDIChildWnd* pMDIChild = (CMDIChildWnd*)mpMainFrame->MDIGetActive();
    if (pMDIChild)
    {
        pAView = (CFusionView*)pMDIChild->GetActiveView();
        if (!pAView)
            return;

        if (pAView->IsKindOf(RUNTIME_CLASS(CFusionView)))
        {
            pActiveView = (CFusionView*)pAView;
        }
        else
            return;
    }

    if ((pActiveView->mViewType != ID_VIEW_TOPVIEW) &&
        (pActiveView->mViewType != ID_VIEW_FRONTVIEW) &&
        (pActiveView->mViewType != ID_VIEW_SIDEVIEW)
        )
        return;

    POSITION		pos;
    pos = App->m_pDoc->GetFirstViewPosition();
    CFusionView* pView;

    while (pos != NULL)
    {
        pView = (CFusionView*)App->m_pDoc->GetNextView(pos);

        if (pView != pActiveView)
        {
            switch (pActiveView->mViewType)
            {
            case ID_VIEW_TOPVIEW:

                switch (pView->mViewType)
                {
                case ID_VIEW_TOPVIEW:
                    pView->VCam->CamPos.X = pActiveView->VCam->CamPos.X;
                    pView->VCam->CamPos.Z = pActiveView->VCam->CamPos.Z;
                    break;

                case ID_VIEW_FRONTVIEW:
                    pView->VCam->CamPos.X = pActiveView->VCam->CamPos.X;
                    break;

                case ID_VIEW_SIDEVIEW:
                    pView->VCam->CamPos.Z = pActiveView->VCam->CamPos.Z;
                    break;
                }

                break;

            case ID_VIEW_FRONTVIEW:

                switch (pView->mViewType)
                {
                case ID_VIEW_TOPVIEW:
                    pView->VCam->CamPos.X = pActiveView->VCam->CamPos.X;
                    break;

                case ID_VIEW_FRONTVIEW:
                    pView->VCam->CamPos.X = pActiveView->VCam->CamPos.X;
                    pView->VCam->CamPos.Y = pActiveView->VCam->CamPos.Y;
                    break;

                case ID_VIEW_SIDEVIEW:
                    pView->VCam->CamPos.Y = pActiveView->VCam->CamPos.Y;
                    break;
                }

                break;

            case ID_VIEW_SIDEVIEW:

                switch (pView->mViewType)
                {
                case ID_VIEW_TOPVIEW:
                    pView->VCam->CamPos.Z = pActiveView->VCam->CamPos.Z;
                    break;

                case ID_VIEW_FRONTVIEW:
                    pView->VCam->CamPos.Y = pActiveView->VCam->CamPos.Y;
                    break;

                case VIEWSIDE:
                    pView->VCam->CamPos.Y = pActiveView->VCam->CamPos.Y;
                    pView->VCam->CamPos.Z = pActiveView->VCam->CamPos.Z;
                    break;
                }

                break;

            default:
                assert(0);
            }
        }
    }

    UpdateAllViews(UAV_GRID_ONLY, NULL, FALSE);
}

static geBoolean fdocUpdateFaceTextures(Face* pFace, void* lParam)
{
    CFusionDoc* pDoc = (CFusionDoc*)lParam;

    Face_SetTextureDibId(pFace, Level_GetDibId(App->CLSB_Doc->pLevel, Face_GetTextureName(pFace)));
    // changed QD 12/03
    const WadFileEntry* const pbmp = Level_GetWadBitmap(App->CLSB_Doc->pLevel, Face_GetTextureName(pFace));
    if (pbmp)
        Face_SetTextureSize(pFace, pbmp->Width, pbmp->Height);
    // end change
    return GE_TRUE;
}

static geBoolean fdocUpdateBrushFaceTextures(Brush* pBrush, void* pVoid)
{
    Brush_EnumFaces(pBrush, pVoid, ::fdocUpdateFaceTextures);
    return GE_TRUE;
}

// *************************************************************************
// * Genesis3D   UpdateAfterWadChange:- Terry and Hazel Flanigan 2023      *
// *************************************************************************
void SB_Doc::UpdateAfterWadChange()
{
    App->Get_Current_Document();

    App->m_pDoc->SetModifiedFlag();

    if (!Level_LoadWad(pLevel))
    {
        App->Say("Cant Load TXL File");
       /* CString Msg;
        AfxFormatString1(Msg, IDS_CANTLOADTXL, Level_GetWadPath(pLevel));
        AfxMessageBox(Msg, MB_OK + MB_ICONERROR);*/
    }

    // update textures tab
    mCurTextureSelection = 0;
    App->CLSB_TextureDialog->Fill_ListBox();

    // update all brush faces
    BrushList_EnumLeafBrushes(Level_GetBrushes(pLevel), this, ::fdocUpdateBrushFaceTextures);
    {
        // find the rendered view and set the wad size infos for it
        POSITION		pos;
        CFusionView* pView;

        pos = App->m_pDoc->GetFirstViewPosition();
        while (pos != NULL)
        {
            pView = (CFusionView*)App->m_pDoc->GetNextView(pos);
            if (Render_GetViewType(pView->VCam) & (VIEWSOLID | VIEWTEXTURE | VIEWWIRE))
            {
                Render_SetWadSizes(pView->VCam, Level_GetWadSizeInfos(pLevel));
                break;	// Only 1 rendered view for now
            }
        }
    }

    if (Level_RebuildBspAlways(pLevel))
    {
        App->m_pDoc->RebuildTrees();
        UpdateAllViews(UAV_ALL3DVIEWS, NULL);
    }
}

// *************************************************************************
// * Genesis3D   SelectAllFacesInBrushes:- Terry and Hazel Flanigan 2023   *
// *************************************************************************
void SB_Doc::SelectAllFacesInBrushes(void)
{
    DoGeneralSelect();

    // Select all faces on all selected brushes
    int iBrush;
    int NumSelBrushes = SelBrushList_GetSize(pSelBrushes);

    for (iBrush = 0; iBrush < NumSelBrushes; ++iBrush)
    {
        Brush* pBrush;

        pBrush = SelBrushList_GetBrush(pSelBrushes, iBrush);
        if (Brush_IsMulti(pBrush))
        {
            BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(pBrush), this, ::SelAllBrushFaces);
        }
        else
        {
            ::SelAllBrushFaces(pBrush, this);
        }
    }

    UpdateSelected();
    ConfigureCurrentTool();
}

typedef struct
{
    geVec3d	vp, wp;
    geFloat	MinBDist, CurDist;
    Face* HitFace, * CurFace;
    Brush* CurBrush;
    CFusionDoc* pDoc;
} SelectBrush3DCBData;

static geBoolean SelectBrush3DCB(Brush* b, void* lParam)
{
    SelectBrush3DCBData* pData = (SelectBrush3DCBData*)lParam;

    if (pData->pDoc->BrushIsVisible(b))
    {
        if (!(Brush_IsSubtract(b)))
        {
            Face* HitFace;
            float CurDist;

            HitFace = Brush_RayCast(b, &pData->vp, &pData->wp, &CurDist);
            if (HitFace != NULL)
            {
                pData->HitFace = HitFace;
                if (CurDist < pData->MinBDist)
                {
                    pData->CurDist = CurDist;
                    pData->MinBDist = pData->CurDist;
                    pData->CurBrush = b;
                    pData->CurFace = pData->HitFace;
                }
            }
        }
    }
    return	GE_TRUE;
}

// *************************************************************************
// * Genesis3D   SelectTextureFromFace3D:- Terry and Hazel Flanigan 2023   *
// *************************************************************************
void SB_Doc::SelectTextureFromFace3D(CPoint point, ViewVars* v)
{
    App->Get_Current_Document();

    BrushList* BList = Level_GetBrushes(App->CLSB_Doc->pLevel);
    SelectBrush3DCBData	bdat;

    Render_ViewToWorld(v, point.x, point.y, &bdat.vp);
    Render_BackRotateVector(v, &bdat.vp, &bdat.wp);
    Render_GetCameraPos(v, &bdat.vp);

    bdat.MinBDist = 999999.0f;
    bdat.CurBrush = NULL;
    bdat.pDoc = App->m_pDoc;
    BrushList_EnumCSGBrushes(BList, &bdat, SelectBrush3DCB);

    if (bdat.CurBrush)
    {
        if (bdat.CurFace)
        {
            char TextName[MAX_PATH];
            strcpy(TextName, Face_GetTextureName(bdat.CurFace));
           
            App->CLSB_TabsControl->Select_Texture_Tab(Face_GetTextureDibId(bdat.CurFace), TextName);

        }
    }
}

#define fdoc_SHOW_ALL_GROUPS -1

typedef struct tagBrushDrawData
{
    const Box3d* pViewBox;
    CDC* pDC;
    ViewVars* v;
    int GroupId;
    CFusionDoc* pDoc;
    BrushFlagTest FlagTest;
    Ogre::uint32		Color;
} BrushDrawData;

static geBoolean	IsBrushNotClipOrHint(const Brush* b)
{
    assert(b != NULL);

    return	(Brush_IsHint(b) || Brush_IsClip(b)) ? GE_FALSE : GE_TRUE;
}

static geBoolean	BrushDrawWire3dCB(Brush* pBrush, void* lParam)
{
    BrushDrawData* pData = (BrushDrawData*)lParam;
    CFusionDoc* pDoc = pData->pDoc;

    if ((pData->GroupId == fdoc_SHOW_ALL_GROUPS) || (Brush_GetGroupId(pBrush) == pData->GroupId))
    {
        if ((pData->FlagTest == NULL) || pData->FlagTest(pBrush))
        {
            if (pDoc->BrushIsVisible(pBrush))
            {
                Render_RenderBrushFaces(pData->v, pBrush, pData->Color);
            }
        }
    }
    return	GE_TRUE;
}

// *************************************************************************
// * Genesis3D   RenderWorld:- Terry and Hazel Flanigan 2023               *
// *************************************************************************
void SB_Doc::RenderWorld(ViewVars* v, CDC* pDC) // hgtterry RenderWorld
{

#define PEN_WHITE_COLOR RGB(255,255,255)
#define PEN_CYAN_COLOR  RGB(0,255,0)
#define PEN_BLUE_COLOR  RGB(0,0,255)
#define PEN_PURPLE_COLOR RGB(255,0,255)
#define PEN_YELLOW_COLOR RGB(255,255,0)
#define PEN_HINT_COLOR  RGB(0,100,0)
#define PEN_CLIP_COLOR  RGB(128,0,128)

    int				i;
    GNode* n;
    ModelIterator	mi;
    Model* pModel;
    geBoolean		DoBlit = GE_FALSE;
    BrushDrawData	brushDrawData;
    BrushList* BList;


    // Temporary Hack for not updating
//#pragma message ("This temporary hack needs to be fixed.  Perhaps a 'force redraw' flag?")
/*
    if (Level_RebuildBspAlways (pLevel) == GE_FALSE)
    {
        return ;
    }
*/
    if (App->CLSB_Doc->pLevel == NULL)
    {
        // must not be loaded yet...
        return;
    }

   /* BList = Level_GetBrushes(App->CLSB_Doc->pLevel);
    Render_UpdateViewPos(v);
    Render_SetUpFrustum(v);

    brushDrawData.pViewBox = NULL;
    brushDrawData.pDC = pDC;
    brushDrawData.v = v;
    brushDrawData.pDoc = this;
    brushDrawData.GroupId = fdoc_SHOW_ALL_GROUPS;
    brushDrawData.FlagTest = NULL;
    brushDrawData.Color = PEN_WHITE_COLOR;

    if ((Render_GetViewType(v) & (VIEWSOLID | VIEWTEXTURE)) && App->CLSB_Doc->mWorldBsp)
    {
        Render_RenderTree(v, App->CLSB_Doc->mWorldBsp, pDC->m_hDC, ZFILL);
        DoBlit = TRUE;
    }
    else
    {
        Render_ClearViewDib(v);
        brushDrawData.FlagTest = IsBrushNotClipOrHint;
        BrushList_EnumLeafBrushes(BList, &brushDrawData, ::BrushDrawWire3dCB);
    }*/
    //if (bShowClipBrushes)
    //{
    //    brushDrawData.Color = PEN_CLIP_COLOR;
    //    brushDrawData.FlagTest = Brush_IsClip;
    //    BrushList_EnumLeafBrushes(BList, &brushDrawData, ::BrushDrawWire3dZBufferCB);
    //}
    //if (bShowHintBrushes)
    //{
    //    brushDrawData.Color = PEN_HINT_COLOR;
    //    brushDrawData.FlagTest = Brush_IsHint;
    //    BrushList_EnumLeafBrushes(BList, &brushDrawData, ::BrushDrawWire3dZBufferCB);
    //}

    //CEntityArray* Entities = Level_GetEntities(App->CLSB_Doc->pLevel);

    //for (i = 0; i < Entities->GetSize(); i++)
    //{
    //    CEntity* pEnt;

    //    pEnt = &(*Entities)[i];

    //    if (!Render_PointInFrustum(v, &pEnt->mOrigin))
    //        continue;

    //    if (EntityIsVisible(pEnt) == GE_FALSE)
    //        continue;

    //    if (pEnt->IsSelected())
    //        continue;

    //    ::DrawEntity(pEnt, v, Level_GetEntityDefs(App->CLSB_Doc->pLevel));
    //}

    //if (!(App->CLSB_Doc->GetSelState() & NOENTITIES))
    //{
    //    for (i = 0; i < Entities->GetSize(); i++)
    //    {
    //        CEntity* pEnt = &(*Entities)[i];

    //        if (!(pEnt->IsSelected() && EntityIsVisible(pEnt)))
    //            continue;

    //        if (!Render_PointInFrustum(v, &pEnt->mOrigin))
    //            continue;

    //        const geBitmap* pBitmap;

    //        pBitmap = pEnt->GetBitmapPtr(Level_GetEntityDefs(App->CLSB_Doc->pLevel));

    //        Render_3DTextureZBufferOutline(v, &pEnt->mOrigin, pBitmap, RGB(0, 255, 255));
    //    }
    //}

    //if (DoBlit)
    //{
    //    ModelInfo_Type* ModelInfo = Level_GetModelInfo(App->CLSB_Doc->pLevel);

    //    //render the models
    //    pModel = ModelList_GetFirst(ModelInfo->Models, &mi);
    //    while (pModel != NULL)
    //    {
    //        n = Model_GetModelTree(pModel);
    //        if (n != NULL)
    //        {
    //            Render_RenderTree(v, n, pDC->m_hDC, ZBUFFER);
    //            DoBlit = TRUE;
    //        }
    //        pModel = ModelList_GetNext(ModelInfo->Models, &mi);
    //    }
    //}


    //brushDrawData.FlagTest = NULL;
    //brushDrawData.Color = PEN_CYAN_COLOR;

    //int NumSelBrushes = SelBrushList_GetSize(App->CLSB_Doc->pSelBrushes);
    //for (i = 0; i < NumSelBrushes; i++)
    //{
    //    Brush* pBrush;

    //    pBrush = SelBrushList_GetBrush(App->CLSB_Doc->pSelBrushes, i);
    //    // changed QD Actors
    //    if (!bShowActors)
    //        if (strstr(App->CL_Brush->Brush_GetName(pBrush), ".act") != NULL)
    //            continue;
    //    // end change
    //    if (Brush_IsMulti(pBrush))
    //    {
    //        BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(pBrush), &brushDrawData, ::BrushDrawWire3dCB);
    //    }
    //    else
    //    {
    //        Render_RenderBrushFaces(v, pBrush, brushDrawData.Color);
    //    }
    //}

    //if (!(App->CLSB_Doc->GetSelState() & NOFACES))
    //{
    //    brushDrawData.Color = PEN_PURPLE_COLOR;
    //    BrushList_EnumLeafBrushes(BList, &brushDrawData, ::BrushDrawWireSel3dCB);
    //}

    //if ((App->CLSB_Doc->mModeTool == ID_TOOLS_TEMPLATE) ||
    //    (App->CLSB_Doc->mModeTool == ID_TOOLS_CAMERA && App->CLSB_Doc->GetSelState() == NOSELECTIONS))
    //{
    //    brushDrawData.Color = PEN_BLUE_COLOR;
    //    if (!App->CLSB_Doc->TempEnt)
    //    {
    //        if (Brush_IsMulti(App->CLSB_Doc->CurBrush))
    //        {
    //            BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(App->CLSB_Doc->CurBrush), &brushDrawData, ::BrushDrawWire3dCB);
    //        }
    //        else
    //        {
    //            Render_RenderBrushFaces(v, App->CLSB_Doc->CurBrush, brushDrawData.Color);
    //        }
    //    }
    //    else
    //    {
    //        if (Render_PointInFrustum(v, &App->CLSB_Doc->mRegularEntity.mOrigin))
    //        {
    //            ::DrawEntity(&App->CLSB_Doc->mRegularEntity, v, Level_GetEntityDefs(App->CLSB_Doc->pLevel));
    //        }
    //    }
    //}

    ////	if (DoBlit)
    //{
    //    Render_BlitViewDib(v, pDC->m_hDC);
    //}
}