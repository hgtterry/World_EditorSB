/****************************************************************************************/
/*  Fusion.h                                                                            */
/*                                                                                      */
/*  Author:       Jim Mischel, Ken Baird, Jeff Lomax                                    */
/*  Description:  Genesis world editor header file                                      */
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
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef FUSION_H
#define FUSION_H

#include "resource.h"       // main symbols
#include "MainFrm.h"
#include "activationwatch.h"
#include "prefs.h"

#include "brush.h"
#include "Entity.h"

#include <afxmt.h>	// required for single-instance checking

#include "AB_App.h"
/////////////////////////////////////////////////////////////////////////////
// CFusionApp:
// See FUSION.cpp for the implementation of this class
//
// Define our standard template so we can use these other places
#define TEMPLATE_RESOURCE_ID			IDR_FUSIONTYPE
#define TEMPLATE_DOC_CLASS				RUNTIME_CLASS( CFusionDoc )
#define TEMPLATE_CHILD_FRAME_CLASS		RUNTIME_CLASS( CChildFrame )
#define TEMPLATE_VIEW_CLASS				RUNTIME_CLASS( CFusionView )

#define FUSION_INIFILE_NAME "WrldEdit.ini"

class CFusionApp : public CWinApp
{
public:
	void InitUserPreferences(CMainFrame* pMainFrame);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CFusionApp();
	~CFusionApp();
	CFusionDoc * GetActiveFusionDoc( void ) const ;

	const Prefs * GetPreferences( void ) { return pResolvedPrefs; } ;
	Prefs * GetPreferencesNormal( void ) { return pPrefs; } ;
	CEvent* pNewInstanceEvent;
	CEvent* pShutdownEvent;

	int NumCopiedBrushes;
	int NumCopiedEntities;
	Brush** CopiedBrushes;
	CEntity** CopiedEntities;

	void CFusionApp::ClearClipboard();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFusionApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFusionApp)
	afx_msg void Open_TxlEditor();
	afx_msg void Open_Preferences();
	afx_msg void Show_ImGui_Surface();
	afx_msg void Open_FaceProperties();
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void CloseDocument();
	afx_msg BOOL OnOpenRecentFile (UINT nID);
	afx_msg void OnPreferences();
	afx_msg void OnUpdatePreferences(CCmdUI* pCmdUI);
	afx_msg void OnHelpHowdoi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnHelp( );
	afx_msg void OnHelpIndex ();
private:
	CMainFrame* pMainFrame;
	CActivationWatch* pWatcher;

	/*
	  pPrefs points to the preferences that were read from the INI file, and
	  are modified by the preferences dialog.  pResolvedPrefs is a preferences
	  structure in which the relative path names have been resolved.  A pointer
	  to this structure is returned by GetPreferences.  The paths are resolved
	  in this structure when the preferences are read, and whenever they are changed.
	*/
	Prefs *pPrefs;
	Prefs *pResolvedPrefs;

	void CommandLineExport( CString* pMapFileName );
	BOOL ProcessCommandLine();
	CMultiDocTemplate* pDocTemplate;
	BOOL IsFirstInstance();
	CFusionDoc* pFusionDoc;
	void ResolvePreferencesPaths ();
};

/////////////////////////////////////////////////////////////////////////////
#endif // Prevent multiple inclusion