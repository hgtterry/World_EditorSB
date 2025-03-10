/****************************************************************************************/
/*  FusionDoc.h                                                                         */
/*                                                                                      */
/*  Author:       Jim Mischel, Ken Baird, Jeff Lomax                                    */
/*  Description:  Genesis world editor header file                                      */
/*                                                                                      */
/*  Copyright (c) 1996-1999, Eclipse Entertainment, L.L.C.                              */
/*                                                                                      */
/*  The contents of this file are subject to the Genesis3D Public License               */
/*  Version 1.01 (the "License"); you may not use this file except in                   */
/*  compliance with the License. You may obtain a copy of the License at                */
/*  http://www.genesis3d.com                                                            */
/*                                                                                      */
/*  Software distributed under the License is distributed on an "AS IS"                 */
/*  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See                */
/*  the License for the specific language governing rights and limitations              */
/*  under the License.                                                                  */
/*                                                                                      */
/*  The Original Code is Genesis3D, released March 25, 1999.                            */
/*  Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved           */
/*                                                                                      */
/****************************************************************************************/
#ifndef FUSIONDOC_H
#define FUSIONDOC_H

#include <assert.h>

#include "resource.h"
#include "Fusion.h"
#include "level.h"

#include "FaceAttributesDialog.h"
#include "BrushAttributesDialog.h"
#include "SelFaceList.h"
#include "SelBrushList.h"
#include "EntityTable.h"

#include "mainfrm.h"
#include "undostack.h"


#define WINDOW_TOP_VIEW 0
#define WINDOW_FRONT_VIEW 1
#define WINDOW_SIDE_VIEW 2
#define MAX_SEL	1000
#define CURTOOL_NONE	(-1)

enum UpdateFlags
{
	REBUILD_QUICK				=1,
	UAV_RENDER_ONLY				=2,
	UAV_ACTIVE3DVIEW_ONLY		=4,
	UAV_NONACTIVE3DVIEWS_ONLY	=8,
	UAV_TEXTUREVIEW_ONLY		=16,
	UAV_GRID_ONLY				=32,
	UAV_TEXTUREBROWSER_ONLY		=64,
	UAV_ALLVIEWS				=128,
	UAV_ALL3DVIEWS				=256,
	UAV_THIS_GRID_ONLY			=512
};

enum
{
	fctNOTHING,
	fctENTITY,
	fctBRUSH
};

enum GridFlags
{
	USE_GRID			=1,
	CONSTRAIN_HOLLOWS	=2
};

enum SelectionState
{
	NOBRUSHES=0, ONEBRUSH=1, MULTIBRUSH=2, RESERVEDBRUSH=4,
	NOFACES=8, ONEFACE=16, MULTIFACE=32, RESERVEDFACE=64,
	NOENTITIES=128, ONEENTITY=256, MULTIENTITY=512, RESERVEDENTITY=1024,
	ONEBRUSHONLY=137, ONEBRUSH_ONEFACE=145, ONEENTITYONLY=264,
	ONEBRUSH_ONEENTITY=265, NOSELECTIONS=136,
	BRUSHCLEAR=7, FACECLEAR=120, ENTITYCLEAR=1920,
	ANYENTITY=1792, ANYBRUSH=7, ANYFACE=112
};

//bool callback for flag checking
typedef geBoolean (*BrushFlagTest)(const Brush *pBrush);

class CFusionView;

//class CMainFrame;

class CFusionDoc : public CDocument
{
protected: // create from serialization only
	CFusionDoc();
	DECLARE_SERIAL(CFusionDoc);

public:
	Level* pLevel;

	//dialogs / controls
	CFrameWnd *mpActiveViewFrame;
	
	geBoolean BrushIsVisible( const Brush * pBrush ) const ;
	geBoolean EntityIsVisible( const CEntity *pEntity ) const ;
	void TempCopySelectedBrushes();
	
	
	void SnapScaleNearest(int sides, int inidx, ViewVars *v);
	
	void SetCurrentBrushTexture();
	BOOL IsEntitySelected();
	void DeleteSelectedEntities();
	
	
	void CopySelectedBrushes();
	void SetSelectedEntity( int ID );
	void AdjustEntityAngle( const ViewVars * v, const geFloat dx ) ;
	void AdjustEntityArc( const ViewVars * v, const geFloat dx ) ;
	void AdjustEntityRadius( const geVec3d *pVec ) ;

	void CenterThingInActiveView( Brush* pBrush, CEntity* pEntity );
	void SelectAllBrushes();
	void SelectAllEntities();
	

	


	void OnBrushAddtoworld();
	
	void CreateEntity(char const *pEntityName);
	

	// creates a template entity with which the user specifies a location for any objects
	// they place
	void CreateObjectTemplate();

	void CreateStaircase();
	void CreateCylinder();
	void CreateSpheroid();
	void CreateCube();
	void CreateCone();
	void CreateArch();

	void SetSelectedBrush();
	void SetSelectedFace(int PolyNum);
	void ResetSelectedBrush();
	void ResetSelectedFace(int PolyNum = 0);
	BOOL OneBrushSelectedOnly(void);

	void RotateSelectedBrushes(geVec3d const *v);
	void RotateSelectedBrushesDirect(geVec3d const *v);


	void PlaceTemplateEntity3D(CPoint point, ViewVars *v);
	void MoveToNearest(void);


	void NullBrushAttributes();
	
	void UpdateCameraEntity( const ViewVars *v ) ;
	void SetRenderedViewCamera( const geVec3d * pVec, const geVec3d * pPRY ) ;
	geBoolean GetCursorInfo(char *info, int MaxSize);
	void NullFaceAttributes();
	void FaceAttributesDialog ();
//	void UpdateAllViews(int Mode, CView* pSender, BOOL Override = FALSE );
	int SubtractBrushFromList(Brush& Brush);
	void ResetSelectedFaceAttributes ();
	
	

	
	
//	void ChangeGridSize(geFloat Increment, CView* pSender );
	void UpdateGridInformation();
	void MoveEntity(geVec3d *);
	

	void DoneRotateBrush();
	void OnSelectedTypeCmdUI(CCmdUI* pCmdUI);
	int AreBrushesSelected();
	void UpdateEntityOrigins();
// changed QD Actors
	void UpdateEntityActors();
// end change
	int CanRedo();
	void SaveBrushUndo();
	int CanUndo();
	void RenderWorld(ViewVars *v, CDC* pDC);
	
	
	void ShearBrush(geVec3d*);
	void ResetSelectedBrushAttributes();
	void BrushListToMeters(void);
	void BrushListToTexels(void);
	void MakeSelectedBrushNewest(void);
	

	void SetDefaultBrushTexInfo(Brush *);
	void SelectOrthoRect(CPoint ptStart, CPoint ptEnd, ViewVars *v) ;
	void SelectRay(CPoint point, ViewVars *v);
	WadFileEntry* GetDibBitmap(const char *Name);
	
	int	MakeNewBrushGroup ( CWnd * pParent );
	int FillBrushGroupCombo (CComboBox &cb);
	int DoBrushGroupEditor (CComboBox &cb);
	void AddSelToGroup( void ) ;
	void RemovesSelFromGroup( void ) ;
	

	// places the specified object down into the level at the specified location
	geBoolean PlaceObject( const char *Objectname, const geVec3d *location );
	geBoolean ImportFile (const char *PathName, const geVec3d *location);

	void SelectEntity (CEntity *pEnt);
	void DeselectEntity (CEntity *pEnt);

	geBoolean	ValidateEntities( void ) ;
	geBoolean	ValidateBrushes( void ) ;
	void		SelectTab( int nTabIndex ) ;

	geBoolean BrushIsSelected (Brush const *pBrush);

	
	
	geBoolean	fdocShowBrush( Brush const *b, Box3d const *ViewBox ) ;

	geBoolean	bShowLeakFinder(void) { return bShowLeak; }
	void		SetShowLeakFinder(geBoolean bShow) { bShowLeak=bShow; }
	geBoolean	IsLeakFileLoaded(void) { return bLeakLoaded; }

	
	
	int			GetNumLeakPoints(void) { return NumLeakPoints; }
	void CompileDone (CompilerErrorEnum CompileRslt);
	void UpdateFaceAttributesDlg (void);
	void UpdateBrushAttributesDlg (void);
	const Prefs *GetPrefs (void);
	void	RebuildTrees(void);
	void	InvalidateDrawTreeOriginalFaces(void);

	void ExportMapFile(const char *FileName);
// changed QD 11/03
	void ExportTo3dtv1_32(const char *FileName);
// changed QD 12/03
	void ExportTo3ds(const char *FileName, int ExpSelected, geBoolean ExpLights, geBoolean ExpFiles);

// end change


	const char* GetObjectName3D(CPoint point, ViewVars *v);

	CEntity* GetSelectedEntity();

	void OnUpdateViewType(CCmdUI* pCmdUI);

	CFusionView* GetCameraView();

	

	CString LastTemplateTypeName;

	
	void ScaleSelectedBrushes(geVec3d *ScaleVector);


public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFusionDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	
	void OnToolsTemplate();
	virtual ~CFusionDoc();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFusionDoc)
	afx_msg void OnToolsUsegrid();
	afx_msg void OnUpdateToolsUsegrid(CCmdUI* pCmdUI);
	afx_msg void OnToolsGridsettings();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEntitiesEditor();
	afx_msg void OnEntitiesShow();
	afx_msg void OnUpdateEntitiesShow(CCmdUI* pCmdUI);
	afx_msg void OnViewShowAllGroups();
	afx_msg void OnViewShowCurrentGroup();
	afx_msg void OnViewShowVisibleGroups();
	afx_msg void OnUpdateViewShowVisibleGroups(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewShowAllGroups(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewShowCurrentGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushAdjustmentmode(CCmdUI* pCmdUI);
	afx_msg void OnBrushSelectedCopytocurrent();
	afx_msg void OnBrushSelectedDelete();
	afx_msg void OnGbspnowater();
	afx_msg void OnUpdateFaceAdjustmentmode(CCmdUI* pCmdUI);
	afx_msg void OnConstrainhollows();
	afx_msg void OnUpdateConstrainhollows(CCmdUI* pCmdUI);
	afx_msg void OnGeneralselect();
	afx_msg void OnUpdateGeneralselect(CCmdUI* pCmdUI);
	public:
	afx_msg void OnBrushSubtractfromworld();
	protected:
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrushSubtractfromworld(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsBrushShowassociatedentity(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntitiesEditor(CCmdUI* pCmdUI);
	afx_msg void OnNewLibObject();
	
	afx_msg void SB_OnFileOpen_3dt();

	afx_msg void OnFileOpen_2Boxes();
	afx_msg void On_New_Project();

	afx_msg void OnToolsBrushAdjustmentmode();
	afx_msg void OnToolsFaceAdjustmentmode();
	afx_msg void OnUpdateBrushPrimitives(CCmdUI* pCmdUI);
	afx_msg void OnBrushPrimitivesCube();
	afx_msg void OnBrushPrimitivesSpheroid();
	afx_msg void OnBrushPrimitivesCylinder();
	afx_msg void OnBrushPrimitivesStaircase();
	afx_msg void OnViewShowClip();
	afx_msg void OnUpdateViewShowClip(CCmdUI* pCmdUI);
	afx_msg void OnViewShowDetail();
	afx_msg void OnUpdateViewShowDetail(CCmdUI* pCmdUI);
	afx_msg void OnViewShowHint();
	afx_msg void OnUpdateViewShowHint(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnBrushPrimitivesArch();
	afx_msg void OnBrushPrimitivesCone();
	afx_msg void OnFileImport();
	afx_msg void OnFileImportActor();
	afx_msg void OnEntityVisibility();
	afx_msg void OnRebuildBsp();
	afx_msg void OnUpdateRebuildBsp(CCmdUI* pCmdUI);
	
	
	afx_msg void OnToolsToggleadjustmode();
	afx_msg void OnUpdateToolsToggleadjustmode(CCmdUI* pCmdUI);
	afx_msg void OnLeveloptions();

	public:
	afx_msg void OnCameraForward();
	afx_msg void OnCameraBack();
	afx_msg void OnCameraLeft();
	afx_msg void OnCameraRight();
	afx_msg void OnCameraLookUp();
	afx_msg void OnCameraLookDown();
	afx_msg void OnCameraUp();
	afx_msg void OnCameraDown();
	protected:

	afx_msg void OnFileExport();

	afx_msg void OnFileExportGDSB(); // hgtterry [100123]
	afx_msg void OnFileExportObject(); // WaveFront Object
	afx_msg void OnFileExport_Ogre3D_Mesh(); // Ogre3D Mesh

	afx_msg void OnUpdateFileExportGDSB(CCmdUI* pCmdUI); // hgtterry [100123]

	afx_msg void OnUpdateFileExport(CCmdUI* pCmdUI);
	afx_msg void OnViewTypeWireFrame();

	// EquitySB
	afx_msg void OnEquity_SetView(); // hgtterry [090123]
	afx_msg void Zero_Camera(); // hgtterry [090123]
	afx_msg void Start_QuickView();
	afx_msg void Start_CommandPanel();
	afx_msg void Start_RecentFiles();
	afx_msg void Save_SB();
	afx_msg void Save_As_SB();

	afx_msg void OnViewTypeTexture();
	afx_msg void OnUpdateViewTypeWireFrame(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTypeTexture(CCmdUI* pCmdUI);
	afx_msg void OnCameraCenteronselection();
	afx_msg void OnUpdateCameraCenteronselection(CCmdUI* pCmdUI);
	afx_msg void OnPlaceOmniLight();
	afx_msg void OnPlaceSpotLight();
	afx_msg void OnCameraGoto();
	afx_msg void OnModifyMove();
	afx_msg void OnUpdateModifyMove(CCmdUI* pCmdUI);
	afx_msg void OnModifyScale();
	afx_msg void OnUpdateModifyScale(CCmdUI* pCmdUI);
	afx_msg void OnModifyMovetocenterofview();
	afx_msg void OnUpdateModifyMovetocenterofview(CCmdUI* pCmdUI);
	afx_msg void OnToolsBrushMakenewest();
	afx_msg void OnUpdateToolsBrushMakenewest(CCmdUI* pCmdUI);
	afx_msg void OnLinkviewports();
	afx_msg void OnUpdateLinkviewports(CCmdUI* pCmdUI);
	afx_msg void OnTemplateSunlight();
	afx_msg void OnUpdateTemplateSunlight(CCmdUI* pCmdUI);
// changed QD Actors
	afx_msg void OnViewShowActors();
	afx_msg void OnUpdateViewShowActors(CCmdUI* pCmdUI);
// end change
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
	int			NumLeakPoints;
	geBoolean	bLeakLoaded, bShowLeak;
	geBoolean	LoadLeakFile(const char *Filename);
	geBoolean	LoadMapFile(const char *Filename);

public:
	char		LastPath[MAX_PATH];  //no cstrings for dialog action
public:
// changed QD Actors
	geBoolean	bShowClipBrushes, bShowDetailBrushes, bShowHintBrushes, bShowActors;
// end change
private:
//	CEntity *pCameraEntity;

	int DoCompileDialog (void);
	geBoolean WriteLevelToMap (const char *Filename);

	geBoolean Load( const char *FileName );  // Loads from filename

public:
	void UpdateSelectedModel (int MoveRotate, geVec3d const *pVecDelta);
private:

	// insures all of the Dib ID's in the brush are correct
	// returns GE_TRUE if all textures fixed up.
	// returns GE_FALSE if any texture was not found
	geBoolean FixUpBrushDibIDs( Brush *brush );

	// our current setting for our grid movement
	int mCurrentBrushId;
public:
	void RotateSelectedBrushList (SelBrushList *pList, geVec3d const *v);


public:
	void GetRotationPoint (geVec3d *pVec);

private:
	void CreateNewTemplateBrush (Brush *pBrush);
	void SetupDefaultFilename (void);

public:
	const char *FindTextureLibrary (char const *WadName);

private:
	Face *FindSelectedFace (void);
//	void DeleteBrushAttributes (void);
//	void DeleteFaceAttributes (void);
};

geBoolean BrushSelect( Brush *pBrush, Ogre::uint32 Action, Ogre::uint32 Data, void * pVoid ) ;
geBoolean BrushDraw( Brush *pBrush, Ogre::uint32 Action, Ogre::uint32 Data, void * pVoid ) ;
geBoolean EntitySelect( CEntity& Entity, Ogre::uint32 Action, Ogre::uint32 Data, void * pVoid ) ;
geBoolean EntityDraw( CEntity& Entity, Ogre::uint32 Action, Ogre::uint32 Data, void * pVoid ) ;

#endif

