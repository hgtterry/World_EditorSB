/****************************************************************************************/
/*  FusionTabControls.h                                                                 */
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

#ifndef	FUSIONTABCONTROLS_H
#define FUSIONTABCONTROLS_H
#include <stdafx.h>
#include "resource.h"
#include "brush.h"


class CMainFrame;
class CModelDialog;

#define BRUSH_ENTITY_TAB	0
#define TEXTURE_TAB			1
#define GROUP_TAB			2
#define CONSOLE_TAB			3
#define MODEL_TAB			4

#define FTC_BORDER_SIZE_TOP		4
#define FTC_BORDER_SIZE_LEFT	4
#define FTC_BORDER_SIZE_RIGHT	8
#define FTC_BORDER_SIZE_BOTTOM	8


// CFusionTabControls window

class CFusionTabControls : public CTabCtrl
{
// Construction
public:
	void DisableAllTabDialogs();
	const char *GetCurrentTexture();
	WINDOWPLACEMENT m_WindowPlacement;
	int m_CurrentTab;
	CWnd *LastView;
	BOOL CreateTabs();
	CFusionTabControls( CDialogBar* pDBar );
	CModelDialog		*ModelTab;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFusionTabControls)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFusionTabControls();

	//moved public to grey the buttons
	void UpdateTextures (void);

	// Generated message map functions
public:
	CMainFrame* m_pMainFrame;
	//{{AFX_MSG(CFusionTabControls)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CDialogBar* m_pDialogBar;

};

/////////////////////////////////////////////////////////////////////////////

#endif
