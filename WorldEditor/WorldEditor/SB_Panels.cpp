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
#include "SB_Panels.h"

SB_Panels::SB_Panels()
{
}

SB_Panels::~SB_Panels()
{
}

// *************************************************************************
// *	  	Centre_QC_Dialog:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Panels::Centre_QC_Dialog()
{
	RECT rcl;

	GetClientRect(App->MainHwnd, &rcl);
	int MX = rcl.right/2;
	int MY = rcl.bottom;
	int Top = rcl.top;

	GetClientRect(App->CLSB_TopTabs->Top_Tabs_Hwnd, &rcl);
	int PX = rcl.right/2;
	int PY = rcl.bottom;


	SetWindowPos(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, (MX-PX), Top + 100,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// *************************************************************************
// *		Move_FileView_Window:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_Panels::Move_FileView_Window(void)
{
	POINT p = { 0 };

	int Diff = MapWindowPoints(App->ViewGLhWnd, NULL, &p, 1);

	int PosX = p.x;
	int PosY = p.y;

	SetWindowPos(App->ListPanel, NULL, PosX + 0, PosY + 5,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

// *************************************************************************
// *			MovePhysicsPanel:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Panels::MovePhysicsView(void)
{

}

// *************************************************************************
// *			Place_GlobalGroups:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_Panels::Place_GlobalGroups(void)
{
	POINT p = { 0 };

	int Diff = MapWindowPoints(App->ViewGLhWnd, NULL, &p, 1);

	HDWP hdwp;

	RECT rect;
	GetWindowRect(App->ViewGLhWnd, &rect);

	int widthX = rect.right - rect.left;
	int heightY = rect.bottom - rect.top;

	int PosX = p.x;
	int PosY = p.y;

	SetWindowPos(App->CLSB_Properties->Properties_Dlg_hWnd, NULL, p.x + widthX - 255, PosY + 5,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// *************************************************************************
// *		Set_Aplication_Dialogs_Off:- Terry and Hazel Flanigan 2023     *
// *************************************************************************
void SB_Panels::Set_Aplication_Dialogs_Off(void)
{
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_TEXTURED), false);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_WIRED), false);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_SELECTALL), false);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_DESELECTALL), false);

	// ------------------ Top Tabs
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_EQUITY), true);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_MODIFY), false);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_FACES), false);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_CAMERA), false);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_TEST2), false);

	// ------------------ Viewer Manager
	EnableWindow(GetDlgItem(App->CLSB_ViewMgrDlg->MgrDlg_hWnd, IDC_BT_EXPORT), false);

	// ------------------ Templates Dialog
	EnableWindow(GetDlgItem(App->CLSB_Tabs_Templates_Dlg->TemplatesDlg_Hwnd, IDC_BT_EXPORTTEMPLATE), false);
	

	App->CLSB_TopTabs->Reset_Tabs_Buttons();
	App->CLSB_TopTabs->Header_File_Flag = 1;
	RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	ShowWindow(App->CLSB_TopTabs->File_Panel_Hwnd, SW_SHOW);

	App->CLSB_TopTabs->Update_Dlg_Controls();


	if (App->BR_True3D_Mode_Active == 1)
	{
		App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode = 0;
		App->CLSB_Ogre_Setup->RenderListener->Show_Brush_Faces = 0;
		App->CLSB_Ogre_Setup->RenderListener->Show_Selected_Face = 0;
		App->CLSB_Ogre_Setup->RenderListener->Show_Marker_Face = 0;

		App->CLSB_BR_Render->Exit_BR_3D_Mode();
		App->BR_True3D_Mode_Active = 0;
	}

	App->CLSB_ViewMgrDlg->RestoreAllPanes();

	App->CLSB_Tabs_True3D_Dlg->Set_Control_Tabs_3DSettings_On(false);
}

// *************************************************************************
// *		Set_Aplication_Dialogs_On:- Terry and Hazel Flanigan 2023     *
// *************************************************************************
void SB_Panels::Set_Aplication_Dialogs_On(void)
{
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_TEXTURED), true);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_WIRED), true);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_SELECTALL), true);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_DESELECTALL), true);

	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_EQUITY), true);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_MODIFY), true);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_FACES), true);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_CAMERA), true);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs->Top_Tabs_Hwnd, IDC_BT_TB_TEST2), true);

	// ------------------ Templates Dialog
	//EnableWindow(GetDlgItem(App->CLSB_Tabs_Templates_Dlg->TemplatesDlg_Hwnd, IDC_BT_EXPORTTEMPLATE), true);

	App->CLSB_Tabs_True3D_Dlg->Set_Control_Tabs_3DSettings_On(true);

}



