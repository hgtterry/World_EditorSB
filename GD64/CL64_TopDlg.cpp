/*
Copyright (c) GameDirector 2019 To 2024 HGT Software W.T.Flanigan H.C.Flanigan

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

#include "pch.h"
#include "CL64_App.h"
#include "resource.h"
#include "CL64_TopDlg.h"

CL64_TopDlg::CL64_TopDlg(void)
{
	TabsHwnd =		nullptr;
	Tabs_TB_hWnd =	nullptr;

	// Tab Options
	Debug_TB_hWnd = nullptr;

	Toggle_Tabs_Debug_Flag = 0;
}

CL64_TopDlg::~CL64_TopDlg(void)
{
}

// **************************************************************************
// *	  		 Start_TopBar:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_TopDlg::Start_TopBar()
{
	TabsHwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOPBAR, App->MainHwnd, (DLGPROC)TopBar_Proc);
	//Init_Bmps_Globals();
}

// *************************************************************************
// *			TopBar_Proc:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::TopBar_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_TopDlg->TabsHwnd = hDlg;

		App->CL_TopDlg->Start_Tabs_Headers();
		App->CL_TopDlg->Start_Debug_TB();

		/*/App->CL_TopBar->Start_Group_TB();
		App->CL_TopBar->Start_Model_TB();
		App->CL_TopBar->Start_Camera_TB();
		App->CL_TopBar->Start_Amimation_TB();*/

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_COMMAND:

		break;
	}
	return FALSE;
}

// *************************************************************************
// *		Start_Tabs_Headers:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_TopDlg::Start_Tabs_Headers(void)
{
	Tabs_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_TAB, TabsHwnd, (DLGPROC)Tabs_Headers_Proc);
}

// *************************************************************************
// *		Tabs_Headers_Proc:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::Tabs_Headers_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TDH_DEBUG, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		/*SendDlgItemMessage(hDlg, IDC_TBMODEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TBCAMERA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TBANIMATION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TBFILE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));*/

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TDH_DEBUG && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopDlg->Toggle_Tabs_Debug_Flag);
			return CDRF_DODEFAULT;
		}

		/*if (some_item->idFrom == IDC_TBMODEL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopBar->Toggle_Tabs_Model_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBCAMERA && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopBar->Toggle_Tabs_Camera_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBANIMATION && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopBar->Toggle_Tabs_Animation_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TBFILE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CL_TopBar->Toggle_Tabs_File_Flag);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TDH_DEBUG)
		{
			//App->CL_TopBar->Hide_Tabs();
			//ShowWindow(App->CL_TopBar->Files_TB_hWnd, SW_SHOW);
			App->CL_TopDlg->Toggle_Tabs_Debug_Flag = 1;

			RedrawWindow(App->CL_TopDlg->Tabs_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			//App->CL_Ogre->Ogre_Listener->ImGui_Render_Tab = Enums::ImGui_Render_Group;

			//App->CL_Panels->Show_Panels(1);

			return TRUE;
		}
	}
	}
	return FALSE;
}

// *************************************************************************
// *			Start_Debug_TB:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_TopDlg::Start_Debug_TB(void)
{
	Debug_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_DEBUG, Tabs_TB_hWnd, (DLGPROC)Debug_TB_Proc);
}

// *************************************************************************
// *			Debug_TB_Proc:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::Debug_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TD_DEBUG_RESETVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TD_DEBUG_IMGUIDEMO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TD_DEBUG_TESTCUBE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TD_DEBUG_IMGUIFPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->Brush_Tabs;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TD_DEBUG_RESETVIEW && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TD_DEBUG_IMGUIDEMO && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_ImGui->Show_ImGui_Demo);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TD_DEBUG_TESTCUBE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_Ogre->flag_Hide_Test_Cube);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TD_DEBUG_IMGUIFPS && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CL_ImGui->Show_FPS);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{

		if (LOWORD(wParam) == IDC_BT_TD_DEBUG_RESETVIEW)
		{
			App->CL_Camera->Reset_View();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_TD_DEBUG_IMGUIDEMO)
		{
			if (App->CL_ImGui->Show_ImGui_Demo == 1)
			{
				App->CL_ImGui->Show_ImGui_Demo = 0;
			}
			else
			{
				App->CL_ImGui->Show_ImGui_Demo = 1;
			}
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_TD_DEBUG_TESTCUBE)
		{
			if (App->CL_Ogre->flag_Hide_Test_Cube == 1)
			{
				App->CL_Ogre->flag_Hide_Test_Cube = 0;
			}
			else
			{
				App->CL_Ogre->flag_Hide_Test_Cube = 1;
			}

			App->CL_Ogre->Show_Test_Cube();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_TD_DEBUG_IMGUIFPS)
		{
			if (App->CL_ImGui->Show_FPS == 1)
			{
				App->CL_ImGui->Show_FPS = 0;
			}
			else
			{
				App->CL_ImGui->Show_FPS = 1;
			}
			return 1;
		}

		
		return FALSE;
	}

	}
	return FALSE;
}

