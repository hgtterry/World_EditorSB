/*
Copyright (c) 2023 World Editor -- HGT Software W.T.Flanigan H.C.Flanigan

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "stdafx.h"
#include "AB_App.h"
#include "A_MainView.h"


#include "FUSIONDoc.h"
#include "FUSIONView.h"

#include <float.h>
#include <level.h>
#include "units.h"

#pragma warning(disable : 4100)

A_MainView::A_MainView(void)
{
	m_pDoc = NULL;
}

A_MainView::~A_MainView(void)
{
}

// *************************************************************************
// *	  						Declerations		Genesis				   *
// *************************************************************************

#define fdoc_SHOW_ALL_GROUPS -1

typedef struct tagBrushDrawData
{
	const Box3d	*	pViewBox ;
	CDC *		pDC ;
	ViewVars *	v ;
	int GroupId;
	CFusionDoc *pDoc;
	BrushFlagTest FlagTest;
	Ogre::uint32		Color;
} BrushDrawData ;

static void fdocDrawEntity(CEntity const* pEnt, ViewVars  const* v,CDC* pDC, EntityTable const *pEntityDefs, BOOL	bShowUI)
{
	#define ENTITY_SIZE (8.0f)  // 16" across
	geVec3d VecOrigin;
	geVec3d EntSizeWorld;	// entity size in world space

	POINT EntPosView;
	POINT EntSizeView;
	POINT EntWidthHeight ;
	POINT OriginView;

	POINT TopLeft, BottomRight;
	POINT TopRight, BottomLeft;

	static const float COS45	= (float)cos (M_PI/4.0f);
	static const float SIN45	= (float)sin (M_PI/4.0f);
	static const float MCOS45	= (float)cos (-(M_PI/4.0f));
	static const float MSIN45	= (float)sin (-(M_PI/4.0f));

	// compute entity size in view coordinates
	geVec3d_Set (&EntSizeWorld, ENTITY_SIZE, ENTITY_SIZE, ENTITY_SIZE);
	EntSizeView = Render_OrthoWorldToView ( v, &EntSizeWorld);
	geVec3d_Clear (&VecOrigin);
	OriginView = Render_OrthoWorldToView ( v, &VecOrigin);
	// This one is the width and height of the Entity
	EntWidthHeight.x = std::max( OriginView.x, EntSizeView.x ) - std::min( OriginView.x, EntSizeView.x ) ;
	EntWidthHeight.y = std::max( OriginView.y, EntSizeView.y ) - std::min( OriginView.y, EntSizeView.y ) ;
	
	// This can have negative numbers
	EntSizeView.x -= OriginView.x;
	EntSizeView.y -= OriginView.y;

	// entity's position in the view
	EntPosView = Render_OrthoWorldToView ( v, &(pEnt->mOrigin));

	{
		// Draw an X at the entity's position...
		TopLeft.x = EntPosView.x - EntSizeView.x;
		TopLeft.y = EntPosView.y - EntSizeView.y;
		BottomRight.x = EntPosView.x + EntSizeView.x;
		BottomRight.y = EntPosView.y + EntSizeView.y;
		TopRight.x = BottomRight.x;
		TopRight.y = TopLeft.y;
		BottomLeft.x = TopLeft.x;
		BottomLeft.y = BottomRight.y;

		pDC->MoveTo (TopLeft);
		pDC->LineTo (BottomRight);
		pDC->MoveTo (TopRight);
		pDC->LineTo (BottomLeft);

	}

	// and then show the aiming arrow and arc stuff...
	if( bShowUI )
	{
		POINT		ArcTopLeft, ArcBottomRight;
		POINT		ptDirSlope ;		// Slope of the "Direction" line
		POINT		ptRotationPoint ;	// Point near end of "Direction" line we rotate to get arrowhead points
		POINT		ptRelRotatePoint ;	// Rotation points about zero
		POINT		ptPlus45 ;			// Final Arrowhead point
		POINT		ptMinus45 ;			// Final Arrowhead point
		POINT		ptStart ;			// Start point for Arc
		POINT		ptEnd ;				// End point of Arc
		float		fPercentIntoLine ;	// Distance into Direction line for rotation point
		float		fDirLength ;		// Direction line length
		float		fEntityLength ;		// Entity length
		float		fAngleToTarget ;	// Radians of arc midpoint
		geFloat		fRadius ;
		geVec3d		Angles ;
		geXForm3d	Xfm ;
		geVec3d		VecTarg ;
		float		fArc ;
		POINT		LineEndView;
		geBoolean	bUIAvailable ;

		// Get the Radius and the Angle  ONE of these must be present to show UI
		bUIAvailable = GE_FALSE ;
		if( pEnt->GetRadius( &fRadius, pEntityDefs ) == GE_FALSE )
			fRadius = 100.0f ;
		else
			bUIAvailable = GE_TRUE ;

		if( pEnt->GetAngles( &Angles, pEntityDefs ) == GE_FALSE )
			geVec3d_Clear( &Angles ) ;
		else
			bUIAvailable = GE_TRUE ;

		if( bUIAvailable == GE_FALSE )
			return ;

		// The camera angles are given in camera coordinates rather than
		// world coordinates (don't ask).
		// So we convert them here.
		if (pEnt->IsCamera ())
		{
			geVec3d_Set(&Angles, Angles.Z, (-Angles.Y-M_PI/2.0f), Angles.X);
		}

		geXForm3d_SetEulerAngles( &Xfm, &Angles ) ;
		geVec3d_Set( &VecTarg, fRadius, 0.0f, 0.0f ) ;
		geXForm3d_Transform( &Xfm, &VecTarg, &VecTarg ) ;
		geVec3d_Add( &(pEnt->mOrigin), &VecTarg, &VecTarg ) ;

		LineEndView = Render_OrthoWorldToView ( v, &VecTarg );

		// Draw to the end point
		pDC->MoveTo( EntPosView ) ;
		pDC->LineTo( LineEndView ) ;

		ptDirSlope.x = LineEndView.x - EntPosView.x ;	// Slope of Direction line
		ptDirSlope.y = LineEndView.y - EntPosView.y ;
				
		fDirLength = sqrt((float) (ptDirSlope.x*ptDirSlope.x) + (ptDirSlope.y*ptDirSlope.y)) ;	// Length of Direction line
		fEntityLength = sqrt((float) (EntSizeView.x*EntSizeView.x)+(EntSizeView.y*EntSizeView.y)) ;
		fEntityLength *= 2 ;	// Arrow 2x entity size
		fPercentIntoLine = 1.0f - (fEntityLength / fDirLength ) ;
		ptRotationPoint.x = (long)(ptDirSlope.x * fPercentIntoLine) ;
		ptRotationPoint.y = (long)(ptDirSlope.y * fPercentIntoLine) ;
		ptRotationPoint.x += EntPosView.x ;
		ptRotationPoint.y += EntPosView.y ;

		ptRelRotatePoint.x = ptRotationPoint.x - LineEndView.x ;
		ptRelRotatePoint.y = ptRotationPoint.y - LineEndView.y ;

		ptPlus45.x = (long)(ptRelRotatePoint.x * COS45 - ptRelRotatePoint.y * SIN45 ) ;
		ptPlus45.y = (long)(ptRelRotatePoint.y * COS45 + ptRelRotatePoint.x * SIN45 ) ;
		ptMinus45.x = (long)(ptRelRotatePoint.x * MCOS45 - ptRelRotatePoint.y * MSIN45 ) ;
		ptMinus45.y = (long)(ptRelRotatePoint.y * MCOS45 + ptRelRotatePoint.x * MSIN45 ) ;

		ptPlus45.x += LineEndView.x ;
		ptPlus45.y += LineEndView.y ;
		ptMinus45.x += LineEndView.x ;
		ptMinus45.y += LineEndView.y ;

		pDC->LineTo( ptPlus45 ) ;
		pDC->LineTo( ptMinus45 ) ;
		pDC->LineTo( LineEndView ) ;

		if( pEnt->GetArc( &fArc, pEntityDefs ) == GE_FALSE )
		{
			fArc = 0.0f ;	// All Directions
		}
		if( fArc != 0.0f )			// Draw the arc
		{
			fArc = 2*M_PI - fArc;
			fArc /= 2.0f ;	// We need half the angle
			EntSizeView.x *= 3; 
			EntSizeView.y *= 3;
			EntWidthHeight.x *= 3 ;
			EntWidthHeight.y *= 3 ;
			// Arc BB is an enlarged Entity BB
			ArcTopLeft.x		= EntPosView.x - EntSizeView.x;
			ArcTopLeft.y		= EntPosView.y - EntSizeView.y;
			ArcBottomRight.x	= EntPosView.x + EntSizeView.x;
			ArcBottomRight.y	= EntPosView.y + EntSizeView.y;
		
			fAngleToTarget = atan2((float) ptDirSlope.y, ptDirSlope.x ) ;	// Angle line leaves
			fAngleToTarget += M_PI ;	// The other side is where the angle starts
			
			ptStart.x = (long)((EntWidthHeight.x) * cos( fAngleToTarget + fArc )) ;
			ptStart.y = (long)((EntWidthHeight.y) * sin( fAngleToTarget + fArc )) ;
			ptEnd.x = (long)((EntWidthHeight.x) * cos( fAngleToTarget - fArc )) ;
			ptEnd.y = (long)((EntWidthHeight.y) * sin( fAngleToTarget - fArc )) ;
			ptStart.x += EntPosView.x ;
			ptStart.y += EntPosView.y ;
			ptEnd.x += EntPosView.x ;
			ptEnd.y += EntPosView.y ;

			// If Start and end point are different
			if( !(ptStart.x == ptEnd.x && ptStart.y == ptEnd.y) )
			{
				pDC->Arc
				( 
					ArcTopLeft.x, ArcTopLeft.y, ArcBottomRight.x, ArcBottomRight.y, 
					ptStart.x, ptStart.y, 
					ptEnd.x, ptEnd.y
				);
			}

			// Draw the two rays out the same distance as the Direction
			ptStart.x = (long)((fDirLength) * cos( fAngleToTarget + fArc )) ;
			ptStart.y = (long)((fDirLength) * sin( fAngleToTarget + fArc )) ;
			ptStart.x += EntPosView.x ;
			ptStart.y += EntPosView.y ;
			pDC->MoveTo( EntPosView ) ;
			pDC->LineTo( ptStart ) ;

			ptEnd.x = (long)((fDirLength) * cos( fAngleToTarget - fArc )) ;
			ptEnd.y = (long)((fDirLength) * sin( fAngleToTarget - fArc )) ;
			ptEnd.x += EntPosView.x ;
			ptEnd.y += EntPosView.y ;
			pDC->MoveTo( EntPosView ) ;
			pDC->LineTo( ptEnd ) ;
		}// Arc for this entity exists
	}
}/* fdocDrawEntity */

static geBoolean fdocBrushNotDetail (const Brush *b)
{
	return !Brush_IsDetail (b);
}

static geBoolean fdocBrushIsSubtract (const Brush *b)
{
	return (Brush_IsSubtract (b) && !Brush_IsHollowCut (b));
}


static geBoolean BrushDrawSelFacesOrtho(Brush *pBrush, void *lParam)
{
	BrushDrawData	*pData;

	pData	=(BrushDrawData *)lParam;

	Render_RenderBrushSelFacesOrtho(pData->v, pBrush, pData->pDC->m_hDC);

	return	GE_TRUE;
}

static geBoolean BrushDrawSheetFacesOrtho(Brush *pBrush, void *lParam)
{
	BrushDrawData	*pData;

	if(Brush_IsSheet(pBrush))
	{
		pData	=(BrushDrawData *)lParam;

		Render_RenderBrushSheetFacesOrtho(pData->v, pBrush, pData->pDC->m_hDC);
	}
	return	GE_TRUE;
}

static geBoolean EntityDraw( CEntity& Entity, void * lParam )
{
	
	BrushDrawData *pData;

	pData = (BrushDrawData *)lParam;

	if( Entity.GetGroupId () != pData->GroupId )
	{
		return GE_TRUE ;
	}

	if ( (Entity.IsSelected() == GE_FALSE ) && pData->pDoc->EntityIsVisible( &Entity ) )
	{
		fdocDrawEntity (&Entity, pData->v, pData->pDC, Level_GetEntityDefs (App->CLSB_Doc->pLevel), GE_FALSE );
	}

	return GE_TRUE ;
}

#define	VectorToSUB(a, b)			(*((((geFloat *)(&a))) + (b)))


// *************************************************************************
// *	  						RenderOrthoView		Genesis				   *
// *************************************************************************
void A_MainView::RenderOrthoView(ViewVars *v, CDC *pDC) // hgtterry Render to views
{
	//App->Say("RenderOrthoView");
	CMainFrame *m_pMainFrame;
	m_pMainFrame		=(CMainFrame *)AfxGetMainWnd(); // MFC POO

	m_pDoc = (CFusionDoc*)m_pMainFrame->GetCurrentDoc();

	int				inidx, i;
	GroupIterator	gi;
	int				GroupId;
	BrushDrawData	brushDrawData ;
	BrushList		*BList;

	if (App->CLSB_Doc->pLevel == NULL)
	{
		return;
	}

	CPen	PenAllItems (PS_SOLID, 1, RGB(255,255,255));
	CPen	PenCutBrush (PS_SOLID, 1, RGB(255,155,0));
	CPen	PenDetailBrush (PS_DASH, 1, RGB(255, 255, 255));
	CPen	PenSelected (PS_SOLID, 1, RGB(0,255,255));
	CPen	PenTemplate (PS_SOLID, 1, RGB(0,0,255));
	CPen	PenHintBrush (PS_SOLID, 1, RGB(0, 100, 0));
	CPen	PenClipBrush (PS_SOLID, 1, RGB(128, 0, 128));
	CPen	PenSheetFaces (PS_SOLID, 1, RGB(255, 255, 0));
	CPen	PenSelectedFaces (PS_SOLID, 1, RGB(255,0,255));
	CPen	PenCamera (PS_SOLID, 1, RGB(0,255,0));

	geVec3d		XTemp;
	Box3d ViewBox;
	int GroupVis = Level_GetGroupVisibility (App->CLSB_Doc->pLevel);

	BList = Level_GetBrushes (App->CLSB_Doc->pLevel);
	inidx	=Render_GetInidx(v);
	
	Box3d_SetBogusBounds (&ViewBox);

	Render_ViewToWorld(v, 0, 0, &XTemp);

	Box3d_AddPoint (&ViewBox, XTemp.X, XTemp.Y, XTemp.Z);

	Render_ViewToWorld(v, Render_GetWidth(v), Render_GetHeight(v), &XTemp);
	Box3d_AddPoint (&ViewBox, XTemp.X, XTemp.Y, XTemp.Z);

	VectorToSUB(ViewBox.Min, inidx)	=-FLT_MAX;
	VectorToSUB(ViewBox.Max, inidx)	=FLT_MAX;

	brushDrawData.pViewBox = &ViewBox ;
	brushDrawData.pDC = pDC ;
	brushDrawData.v = v ;
	brushDrawData.pDoc = m_pDoc;//this;
	brushDrawData.GroupId = 0;
	brushDrawData.FlagTest = NULL;

	// Initialize oldpen.  Don't initialize it anywhere else!
	CPen BlackPen (PS_SOLID, 1, RGB (0, 0, 0));
	CPen * const oldpen = pDC->SelectObject (&BlackPen);

	{
		float GridSize, GridSnapSize;
		const Prefs *pPrefs = m_pDoc->GetPrefs();

		CPen	PenGrid (PS_SOLID, 1, Prefs_GetGridColor (pPrefs));
		CPen	PenSnapGrid (PS_SOLID, 1, Prefs_GetSnapGridColor (pPrefs));


		GridSize = Render_GetFineGrid(v, (Level_GetGridType (App->CLSB_Doc->pLevel) == GridTexel) ? GRID_TYPE_TEXEL : GRID_TYPE_METRIC);

	
		
		if (Level_GetGridType (App->CLSB_Doc->pLevel) == GridMetric)
		{
			GridSize /= 2.54f;
		}

		GridSnapSize = Level_GetGridSnapSize (App->CLSB_Doc->pLevel);

		{
			if (GridSnapSize >= GridSize)
			{
				// render snap grid
				pDC->SelectObject (&PenSnapGrid);
			   Render_RenderOrthoGridFromSize (v, GridSnapSize, pDC->m_hDC);
			}

			pDC->SelectObject (&PenGrid);
			Render_RenderOrthoGridFromSize (v, 128, pDC->m_hDC);

		}

		pDC->SelectObject (oldpen);
	}

	GroupListType *Groups = Level_GetGroups (App->CLSB_Doc->pLevel);

	GroupId	=Group_GetFirstId(Groups, &gi);
	while(GroupId != NO_MORE_GROUPS )
	{
	    brushDrawData.FlagTest = ::fdocBrushNotDetail;
		brushDrawData.GroupId = GroupId;
		if( (GroupVis == Group_ShowAll) ||
			((GroupVis == Group_ShowCurrent) && (GroupId == App->CLSB_Doc->mCurrentGroup)) ||
			((GroupVis == Group_ShowVisible) && (Group_IsVisible (Groups, GroupId)))
		)
		{
			const GE_RGBA * pRGB ;
			pRGB = Group_GetColor( Groups, GroupId ) ;
			CPen	PenThisGroup(PS_SOLID, 1, RGB(pRGB->r,pRGB->g,pRGB->b));

			pDC->SelectObject (&PenThisGroup);
			Level_EnumLeafBrushes (App->CLSB_Doc->pLevel, &brushDrawData, BrushDraw); // Draw Brushes
			
			if(App->CLSB_Doc->mShowEntities == GE_TRUE )
			{
				Level_EnumEntities (App->CLSB_Doc->pLevel, &brushDrawData, EntityDraw);
			}

			// render cut brushes
			pDC->SelectObject (&PenCutBrush);
			brushDrawData.FlagTest = fdocBrushIsSubtract;
			Level_EnumLeafBrushes (App->CLSB_Doc->pLevel, &brushDrawData, BrushDraw);

			// details
			if (m_pDoc->bShowDetailBrushes)
			{
				// detail brushes
				pDC->SelectObject (&PenDetailBrush);
				brushDrawData.FlagTest = Brush_IsDetail;
				Level_EnumLeafBrushes (App->CLSB_Doc->pLevel, &brushDrawData, BrushDraw);
			}

			// hints
			if(m_pDoc->bShowHintBrushes)
			{
				// Hint brushes
				pDC->SelectObject (&PenHintBrush);
				brushDrawData.FlagTest = Brush_IsHint;
				Level_EnumLeafBrushes (App->CLSB_Doc->pLevel, &brushDrawData, BrushDraw);
			}


			// clip
			if(m_pDoc->bShowClipBrushes)
			{
				// Hint brushes
				pDC->SelectObject (&PenClipBrush);
				brushDrawData.FlagTest = Brush_IsClip;
				Level_EnumLeafBrushes (App->CLSB_Doc->pLevel, &brushDrawData, BrushDraw);
			}

			pDC->SelectObject (&PenAllItems);
		}

		GroupId	=Group_GetNextId(Groups, &gi);	
	}

	brushDrawData.GroupId = fdoc_SHOW_ALL_GROUPS;
	brushDrawData.FlagTest = NULL;
	{
		CEntityArray *Entities = Level_GetEntities (App->CLSB_Doc->pLevel);
		int NumSelBrushes = SelBrushList_GetSize (App->CLSB_Doc->pSelBrushes);
				
		// render selected brushes and entities
		pDC->SelectObject(&PenSelected);

		for(i=0;i < NumSelBrushes;i++)
		{
			Brush *pBrush;

			pBrush = SelBrushList_GetBrush (App->CLSB_Doc->pSelBrushes, i);
			if (m_pDoc->fdocShowBrush (pBrush, &ViewBox))
			{
				if(Brush_IsMulti (pBrush))
				{
					BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList (pBrush), &brushDrawData, BrushDraw);
				}
				else
				{
					Render_RenderBrushFacesOrtho(v, pBrush, pDC->m_hDC);
				}
			}
		}

		for(i=0;i < Entities->GetSize();i++)
		{
			CEntity *pEnt;

			pEnt = &(*Entities)[i];

			if (pEnt->IsSelected ())
			{
				fdocDrawEntity (pEnt, v, pDC, Level_GetEntityDefs (App->CLSB_Doc->pLevel), (i==App->CLSB_Doc->mCurrentEntity) ? GE_TRUE : GE_FALSE ) ;
			}
		}
	}

	{
		// render sheet faces
		pDC->SelectObject (&PenSheetFaces);
		BrushList_EnumLeafBrushes(BList, &brushDrawData, BrushDrawSheetFacesOrtho);
	}
	{
		// render selected faces
		pDC->SelectObject (&PenSelectedFaces);
		BrushList_EnumLeafBrushes(BList, &brushDrawData, BrushDrawSelFacesOrtho);
	}
	{
		// template brush/entity
		pDC->SelectObject (&PenTemplate);

		if((App->CLSB_Doc->mModeTool==ID_TOOLS_TEMPLATE)||
			(App->CLSB_Doc->mModeTool==ID_TOOLS_CAMERA && App->CLSB_Doc->GetSelState()==NOSELECTIONS))
		{
			if(!App->CLSB_Doc->TempEnt)
			{
				if (Brush_TestBoundsIntersect(App->CLSB_Doc->CurBrush, &ViewBox))
				{
					if(Brush_IsMulti(App->CLSB_Doc->CurBrush))
					{
						BrushList_EnumLeafBrushes(App->CL_Brush->Brush_GetBrushList(App->CLSB_Doc->CurBrush), &brushDrawData, BrushDraw);
					}
					else
					{
						Render_RenderBrushFacesOrtho(v, App->CLSB_Doc->CurBrush, pDC->m_hDC);
					}
				}
			}
			else
			{
				fdocDrawEntity (&App->CLSB_Doc->mRegularEntity, v, pDC, Level_GetEntityDefs (App->CLSB_Doc->pLevel), GE_FALSE );
			}
		}
	}

	// find and render the camera entity
	CEntity *pCameraEntity = App->CLSB_Camera_WE->FindCameraEntity();

	if ((pCameraEntity != NULL))
	{
		if (pCameraEntity->IsSelected())
			pDC->SelectObject (&PenSelected);
		else
			pDC->SelectObject (&PenCamera);

		fdocDrawEntity (pCameraEntity, v, pDC, Level_GetEntityDefs (App->CLSB_Doc->pLevel), GE_TRUE);
	}

	pDC->SelectObject(oldpen);

}

// *************************************************************************
// *	  						BrushDraw		Genesis					   *
// *************************************************************************
geBoolean A_MainView::BrushDraw( Brush *pBrush, void *lParam)
{
	BrushDrawData * pData = (BrushDrawData*)lParam ;
	CFusionDoc * pDoc = pData->pDoc;

	if ((pData->GroupId == fdoc_SHOW_ALL_GROUPS) || (Brush_GetGroupId (pBrush) == pData->GroupId))
	{
		if ((pData->FlagTest == NULL) || pData->FlagTest (pBrush))
		{
			if (pDoc->fdocShowBrush (pBrush, pData->pViewBox))
			{
				App->CL_Main_View->Render_RenderBrushFacesOrtho(pData->v, pBrush, pData->pDC->m_hDC);
			}
		}
	}

	return GE_TRUE ;
}

static POINT plist[64];

// *************************************************************************
// *	  			Render_RenderBrushFacesOrtho		Genesis			   *
// *************************************************************************
void A_MainView::Render_RenderBrushFacesOrtho( const ViewVars *Cam, Brush *b, HDC ViewDC)
{
	int	i, j;

	//assert (b != NULL);

	for(i=0;i < Brush_GetNumFaces(b);i++)
	{
		Face* f	= Brush_GetFace(b, i);
		const geVec3d* pnts	= Face_GetPoints(f);

		for(j=0; j < Face_GetNumPoints(f); j++)
		{
			plist[j] = App->CL_Main_View->Render_OrthoWorldToView(Cam, &pnts[j]);
		}

		plist[j] = plist[0]; 
		Polyline(ViewDC, plist, j+1);
	}
}

// *************************************************************************
// *	  			Render_OrthoWorldToView		Genesis					   *
// *************************************************************************
POINT A_MainView::Render_OrthoWorldToView(const ViewVars *v, geVec3d const *wp)
{
	POINT	sc = {0, 0};
	geVec3d ptView;

	switch (v->ViewType)
	{
		case VIEWTOP :
		{
			geVec3d_Subtract (wp, &v->CamPos, &ptView);
			geVec3d_Scale (&ptView, v->ZoomFactor, &ptView);
			sc.x = (int)(v->XCenter + ptView.X);
			sc.y = (int)(v->YCenter + ptView.Z);
			break;
		}
		case VIEWFRONT :
		{
			geVec3d_Subtract (wp, &v->CamPos, &ptView);
			geVec3d_Scale (&ptView, v->ZoomFactor, &ptView);
			sc.x = (int)(v->XCenter + ptView.X);
			sc.y = (int)(v->YCenter - ptView.Y);
			break;
		}
		case VIEWSIDE :
		{
			geVec3d_Subtract (wp, &v->CamPos, &ptView);
			geVec3d_Scale (&ptView, v->ZoomFactor, &ptView);
			sc.x = (int)(v->XCenter + ptView.Z);
			sc.y = (int)(v->YCenter - ptView.Y);
			break;
		}
		default :
//			assert (0);	// bad view type
			// I should be able to assert here, but I can't.
			// somebody is calling this function for the rendered
			// view.  Bad stuff, really, but I don't have time to change it.
			break;
	}
	return sc;
}

