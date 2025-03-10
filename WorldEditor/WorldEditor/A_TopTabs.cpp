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

//#define WINVER 0x0500
//
//#pragma comment(linker,"\"/manifestdependency:type='win32' \
//name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
//processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 

#include "StdAfx.h"
#include "AB_App.h"
#include "A_TopTabs.h"
#include "resource.h"

#include "FUSIONDoc.h"
#include "FUSIONView.h"

#include "Windows.h"
#include "wingdi.h"

SB_TopTabs::SB_TopTabs(void)
{
	Top_Tabs_Hwnd = nullptr;
	Brush_Modify_Panel_Hwnd = nullptr;
	File_Panel_Hwnd = nullptr;
	Test_Panel_Hwnd = nullptr;
	Equity_Panel_Hwnd = nullptr;
	Faces_Panel_Hwnd = nullptr;
	Camera_Panel_Hwnd = nullptr;

	Quick_Command_Started = 0;

	Textured_Flag = 0;
	Wired_Flag = 0;

	Header_File_Flag = 0;
	Header_BrushModify_Flag = 0;
	Header_Test_Flag = 0;
	Header_Equity_Flag = 0;
	Header_Faces_Flag = 0;
	Header_Camera_Flag = 0;

	Brush_Select_Flag = 0;
	Brush_MoveRotate_Flag = 0;
	Brush_Scale_Flag = 0;
	Brush_Shear_Flag = 0;

	OgreRunning_Flag = 0;
}

SB_TopTabs::~SB_TopTabs(void)
{
}

// *************************************************************************
// *	  		Start_Headers_Tabs:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
bool SB_TopTabs::Start_Headers_Tabs()
{
	if (Quick_Command_Started == 0)
	{
		Top_Tabs_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TOPTABS, App->MainHwnd, (DLGPROC)TB_Headers_Proc);

		App->CLSB_Panels->Centre_QC_Dialog();

		Start_BrushModify_Panel();
		Start_File_Tab();
		Start_Test_Tab();
		Start_Equity_Tab();
		Start_Faces_Tab();
		Start_Camera_Tab();

		Reset_Tabs_Buttons();
		Header_File_Flag = 1;
		RedrawWindow(Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		ShowWindow(File_Panel_Hwnd, SW_SHOW);

		Update_Dlg_Controls();

		Quick_Command_Started = 1;
	}
	
	return 1;
}

// *************************************************************************
// *			TB_Headers_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs::TB_Headers_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TB_MODIFY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TB_FILE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TB_TEST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TB_EQUITY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TB_FACES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TB_CAMERA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_TB_TEXTURED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TB_WIRED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BT_TB_SELECTALL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TB_DESELECTALL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));


		SetWindowLong(hDlg, GWL_EXSTYLE, GetWindowLong(hDlg, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hDlg, RGB(213, 222, 242), 230, LWA_ALPHA);

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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TB_TEXTURED && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TB_TEXTURED));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_TopTabs->Textured_Flag);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TB_WIRED && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TB_WIRED));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_TopTabs->Wired_Flag);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TB_SELECTALL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TB_SELECTALL));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TB_DESELECTALL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TB_DESELECTALL));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		// -------------------------------
		if (some_item->idFrom == IDC_BT_TB_MODIFY && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TB_MODIFY));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CLSB_TopTabs->Header_BrushModify_Flag);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TB_FILE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CLSB_TopTabs->Header_File_Flag);
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_TB_TEST2 && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TB_TEST2));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CLSB_TopTabs->Header_Test_Flag);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TB_EQUITY && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TB_EQUITY));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CLSB_TopTabs->Header_Equity_Flag);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TB_FACES && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TB_FACES));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CLSB_TopTabs->Header_Faces_Flag);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TB_CAMERA && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TB_CAMERA));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle_Tabs(item, App->CLSB_TopTabs->Header_Camera_Flag);
			}

			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		// ---------------- Headers

		if (LOWORD(wParam) == IDC_BT_TB_CAMERA)
		{
			App->CLSB_TopTabs->Reset_Tabs_Buttons();
			App->CLSB_TopTabs->Header_Camera_Flag = 1;
			RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			ShowWindow(App->CLSB_TopTabs->Camera_Panel_Hwnd, SW_SHOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TB_EQUITY)
		{
			App->CLSB_Equity->Go_Equity();
			/*App->CLSB_TopTabs->Reset_Tabs_Buttons();
			App->CLSB_TopTabs->Header_Equity_Flag = 1;
			RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			ShowWindow(App->CLSB_TopTabs->Equity_Panel_Hwnd, SW_SHOW);*/

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TB_TEST2)
		{
			App->CL_Test_View->Start_Main_View_Dlg();
			/*App->CLSB_TopTabs->Reset_Tabs_Buttons();
			App->CLSB_TopTabs->Header_Test_Flag = 1;
			RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			ShowWindow(App->CLSB_TopTabs->Test_Panel_Hwnd, SW_SHOW);*/

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TB_FACES)
		{
			App->CLSB_TopTabs->Reset_Tabs_Buttons();
			App->CLSB_TopTabs->Header_Faces_Flag = 1;
			RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			ShowWindow(App->CLSB_TopTabs->Faces_Panel_Hwnd, SW_SHOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TB_FILE)
		{
			App->CLSB_TopTabs->Reset_Tabs_Buttons();
			App->CLSB_TopTabs->Header_File_Flag = 1;
			RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			ShowWindow(App->CLSB_TopTabs->File_Panel_Hwnd, SW_SHOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TB_MODIFY)
		{
			App->CLSB_TopTabs->Reset_Tabs_Buttons();
			App->CLSB_TopTabs->Header_BrushModify_Flag = 1;
			RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			ShowWindow(App->CLSB_TopTabs->Brush_Modify_Panel_Hwnd, SW_SHOW);

			return TRUE;
		}

		// --------------------
		if (LOWORD(wParam) == IDC_BT_TB_TEXTURED)
		{
			App->CLSB_TopTabs->Reset_Render_Buttons();

			App->CLSB_TopTabs->Textured_Flag = 1;
			App->CL_Render_App->Render3D_Mode(ID_VIEW_TEXTUREVIEW);

			if (App->BR_True3D_Mode_Active == 1)
			{
				App->CLSB_Ogre_Setup->RenderListener->ShowFaces = 0;
				App->CLSB_Mesh_Mgr->World_Node->setVisible(true);
			}

			RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TB_WIRED)
		{
			App->CLSB_TopTabs->Reset_Render_Buttons();

			App->CLSB_TopTabs->Wired_Flag = 1;
			App->CL_Render_App->Render3D_Mode(ID_VIEW_3DWIREFRAME);

			if (App->BR_True3D_Mode_Active == 1)
			{
				App->CLSB_Ogre_Setup->RenderListener->ShowFaces = 1;
				App->CLSB_Mesh_Mgr->World_Node->setVisible(false);
			}

			RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TB_SELECTALL)
		{
			App->CLSB_Doc->SelectAll();
			App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);

			App->CL_TabsGroups_Dlg->Update_Dlg_SelectedBrushesCount();
			App->CL_TabsGroups_Dlg->Update_Dlg_Controls();
			App->CLSB_TopTabs->Update_Dlg_Controls();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TB_DESELECTALL)
		{
			App->CLSB_Doc->ResetAllSelections();
			App->CLSB_Doc->UpdateSelected();
			App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);

			App->CL_TabsGroups_Dlg->Update_Dlg_SelectedBrushesCount();
			App->CL_TabsGroups_Dlg->Update_Dlg_Controls();
			App->CLSB_TopTabs->Update_Dlg_Controls();

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CLSB_TopTabs->Quick_Command_Started = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CLSB_TopTabs->Quick_Command_Started = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  	Reset_Tabs_Buttons:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_TopTabs::Reset_Tabs_Buttons()
{
	Header_File_Flag = 0;
	Header_BrushModify_Flag = 0;
	Header_Test_Flag = 0;
	Header_Equity_Flag = 0;
	Header_Faces_Flag = 0;
	Header_Camera_Flag = 0;

	ShowWindow(Brush_Modify_Panel_Hwnd, SW_HIDE);
	ShowWindow(File_Panel_Hwnd, SW_HIDE);
	ShowWindow(Test_Panel_Hwnd, SW_HIDE);
	ShowWindow(Equity_Panel_Hwnd, SW_HIDE);
	ShowWindow(Faces_Panel_Hwnd, SW_HIDE);
	ShowWindow(Camera_Panel_Hwnd, SW_HIDE);

	RedrawWindow(Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  	Reset_Render_Buttons:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_TopTabs::Reset_Render_Buttons()
{
	Textured_Flag = 0;
	Wired_Flag = 0;

	RedrawWindow(Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *	  	Start_BrushModify_Panel:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
bool SB_TopTabs::Start_BrushModify_Panel()
{
	App->Get_Current_Document();

	Brush_Modify_Panel_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TB_PROPERTIES,Top_Tabs_Hwnd, (DLGPROC)BrushModify_Panel_Proc);
	return 1;
}

// *************************************************************************
// *		BrushModify_Panel_Proc:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs::BrushModify_Panel_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BTSCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTBSELECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTBMOVEROTATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTBSHEAR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

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

	case  WM_NOTIFY:
		{

		LPNMHDR some_item = (LPNMHDR)lParam;

			if (some_item->idFrom == IDC_BTTBSELECT && some_item->code == NM_CUSTOMDRAW)
			{
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
				App->Custom_Button_Toggle(item,App->CLSB_TopTabs->Brush_Select_Flag);
				return TRUE;
			}

			if (some_item->idFrom == IDC_BTTBMOVEROTATE && some_item->code == NM_CUSTOMDRAW)
			{
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
				bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BTTBMOVEROTATE));
				if (test == 0)
				{
					App->Custom_Button_Greyed(item);
				}
				else
				{
					App->Custom_Button_Toggle(item, App->CLSB_TopTabs->Brush_MoveRotate_Flag);
				}

				return TRUE;
			}

			if (some_item->idFrom == IDC_BTSCALE && some_item->code == NM_CUSTOMDRAW)
			{
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
				bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BTSCALE));
				if (test == 0)
				{
					App->Custom_Button_Greyed(item);
				}
				else
				{
					App->Custom_Button_Toggle(item, App->CLSB_TopTabs->Brush_Scale_Flag);
				}
				return TRUE;
			}

			if (some_item->idFrom == IDC_BTTBSHEAR && some_item->code == NM_CUSTOMDRAW)
			{
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
				bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BTTBSHEAR));
				if (test == 0)
				{
					App->Custom_Button_Greyed(item);
				}
				else
				{
					App->Custom_Button_Toggle(item, App->CLSB_TopTabs->Brush_Shear_Flag);
				}
				return TRUE;
			}

			return TRUE;
		}

	case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDC_BTTBMOVEROTATE)
			{
				App->CLSB_TopTabs->Reset_Brush_Buttons();
				App->CLSB_TopTabs->Brush_MoveRotate_Flag = 1;

				RedrawWindow(App->CLSB_TopTabs->Brush_Modify_Panel_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				App->CLSB_TopTabs->Select_MoveRotate();
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BTTBSHEAR)
			{
				App->CLSB_TopTabs->Reset_Brush_Buttons();
				App->CLSB_TopTabs->Brush_Shear_Flag = 1;

				RedrawWindow(App->CLSB_TopTabs->Brush_Modify_Panel_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				App->CLSB_TopTabs->Select_Shear();
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BTSCALE)
			{
				App->CLSB_TopTabs->Reset_Brush_Buttons();
				App->CLSB_TopTabs->Brush_Scale_Flag = 1;

				RedrawWindow(App->CLSB_TopTabs->Brush_Modify_Panel_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				App->CLSB_TopTabs->Select_Scale();
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BTTBSELECT)
			{
				App->CLSB_TopTabs->Reset_Brush_Buttons();
				App->CLSB_TopTabs->Brush_Select_Flag = 1;

				RedrawWindow(App->CLSB_TopTabs->Brush_Modify_Panel_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				App->CLSB_TopTabs->Select_Mode();
				return TRUE;
			}

			break;
		}

	}

	return FALSE;
}

// *************************************************************************
// *	  	Reset_Brush_Buttons:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_TopTabs::Reset_Brush_Buttons()
{
	Brush_Select_Flag = 0;
	Brush_MoveRotate_Flag = 0;
	Brush_Scale_Flag = 0;
	Brush_Shear_Flag = 0;

	RedrawWindow(Brush_Modify_Panel_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  		Start_File_Tab:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_TopTabs::Start_File_Tab()
{
	File_Panel_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TB_FILES, Top_Tabs_Hwnd, (DLGPROC)Top_File_Proc);

	return 1;
}

// *************************************************************************
// *			Top_File_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs::Top_File_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TBF_QUICKLOAD, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TABS_WORLDINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TBF_QUICKLOAD && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TABS_WORLDINFO && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDC_BT_TBF_QUICKLOAD)
			{
				/*App->CLSB_File_WE->New_File();
				return TRUE;*/
				
				char Txlpath[MAX_PATH];
				char path[MAX_PATH];

				strcpy(path,App->WorldEditor_Directory);
				strcat(path,"Levels\\First_Project_Prj\\Test.3dt");

				strcpy(Txlpath,App->WorldEditor_Directory);
				strcat(Txlpath,"Levels\\First_Project_Prj\\Equity.txl");

				strcpy(App->CLSB_File_WE->PathFileName_3dt,path);
				
				App->CLSB_File_WE->Quick_load_Flag = 1;

				bool Test = App->CLSB_File_WE->Start_Load(App->CLSB_File_WE->PathFileName_3dt,0);
				
				App->CLSB_File_WE->Quick_load_Flag = 0;

				if (Test == 1)
				{
					//if (App->New_Render == 1)
					//{
					//	if (App->CLSB_BR_Render->RB_Render_Started == 0)
					//	{
					//		App->CLSB_BR_Render->Update_Scene();
					//	}
					//	else
					//	{
					//		//App->CLSB_Doc->Render_WE_World = 0;
					//		//App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS | REBUILD_QUICK, NULL, TRUE);
					//		App->CLSB_BR_Render->Resize_3DView();

					//		App->CLSB_Mesh_Mgr->WE_Build_Brush_List(0);
					//		App->CLSB_Bullet->Create_Brush_Trimesh_XX(0);
					//		App->CLSB_Mesh_Mgr->WE_Convert_All_Texture_Groups();
					//		App->CLSB_Export_Ogre3D->Convert_ToOgre3D(1);
					//		App->CLSB_Ogre->OgreListener->CameraMode = Enums::CamDetached;
					//		//App->CLSB_Environment->SetSky(1);
					//	}
					//}

		
					App->Say("Loaded", App->CLSB_File_WE->PathFileName_3dt);
					App->CLSB_Panels->Set_Aplication_Dialogs_On();

					App->File_Loaded_Flag = 1;
					
				}

				

				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BT_TABS_WORLDINFO)
			{
				App->CLSB_Dialogs->Start_ListData(0);
				return TRUE;
			}

			break;
		}
	}

	return FALSE;
}

// *************************************************************************
// *	  		Start_Test_Tab:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_TopTabs::Start_Test_Tab()
{
	Test_Panel_Hwnd = NULL;

	Test_Panel_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TB_TEST, Top_Tabs_Hwnd, (DLGPROC)Top_Test_Proc);

	return 1;
}

// *************************************************************************
// *			Top_TestProc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs::Top_Test_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TB_NEWVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_WETEST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TB_NEWVIEW && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_WETEST && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_WETEST)
		{
			App->CLSB_Project_Create->Create_Ogre_Model(1);
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_TB_NEWVIEW)
		{
			App->CL_Test_View->Start_Main_View_Dlg();
			return TRUE;
		}

		break;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  		Start_Equity_Tab:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
bool SB_TopTabs::Start_Equity_Tab()
{
	Equity_Panel_Hwnd = NULL;

	Equity_Panel_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TB_EQUITY, Top_Tabs_Hwnd, (DLGPROC)Top_Equity_Proc);

	return 1;
}

// *************************************************************************
// *			Top_Equity_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs::Top_Equity_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TB_WORLDINFO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TB_NEWVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STARTEQUITY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TB_NEWVIEW && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TB_WORLDINFO && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_STARTEQUITY && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_OGRERUNNING && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_TopTabs->OgreRunning_Flag);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TB_NEWVIEW)
		{
			//App->CL_Main_View->Start_Main_View_Dlg();
			App->CL_Test_View->Start_Main_View_Dlg();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TB_WORLDINFO)
		{
			App->CLSB_Dialogs->Start_ListData(0);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_STARTEQUITY)
		{
			//App->Say("Coming Soon Check Updates");
			App->CLSB_Equity->Go_Equity();
			return TRUE;
		}

		break;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  		Start_Faces_Tab:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_TopTabs::Start_Faces_Tab()
{
	Faces_Panel_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TB_FACES, Top_Tabs_Hwnd, (DLGPROC)Top_Faces_Proc);

	return 1;
}

// *************************************************************************
// *			Top_Faces_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs::Top_Faces_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TT_ALL_BRUSH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTFACENEXT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTFACEPREVIOUS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		
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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TT_ALL_BRUSH && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTFACENEXT && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTFACEPREVIOUS && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TT_ALL_BRUSH)
		{
			App->CLSB_Doc->SelectAllFacesInBrushes();
			App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTFACENEXT)
		{
			App->Get_Current_Document();

			CFusionView* pView;
			POSITION		pos;
			pos = App->m_pDoc->GetFirstViewPosition();

			pView = (CFusionView*)App->m_pDoc->GetNextView(pos);

			pView->OnEditSelectFacesNext();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTFACEPREVIOUS)
		{
			App->Get_Current_Document();

			CFusionView* pView;
			POSITION		pos;
			pos = App->m_pDoc->GetFirstViewPosition();

			pView = (CFusionView*)App->m_pDoc->GetNextView(pos);

			pView->OnEditSelectFacesPrevious();
			return TRUE;
		}
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  		Start_Camera_Tab:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
bool SB_TopTabs::Start_Camera_Tab()
{
	Camera_Panel_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TB_CAMERA, Top_Tabs_Hwnd, (DLGPROC)Top_Camera_Proc);

	return 1;
}

// *************************************************************************
// *			Top_Faces_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs::Top_Camera_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TT_ZEROCAMRA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_MOVECAMRA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

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

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TT_ZEROCAMRA && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_MOVECAMRA && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TT_ZEROCAMRA)
		{
			App->CLSB_Camera_WE->Zero_Camera();
			App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_MOVECAMRA)
		{
			App->CLSB_Camera_WE->Start_Move_Camera();
			return TRUE;
		}

		break;
	}

	}

	return FALSE;
}
// *************************************************************************
// *		 	Select_Mode:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_TopTabs::Select_Mode()
{
	App->CLSB_Doc->mCurrentTool = CURTOOL_NONE;
	App->CLSB_Doc->mModeTool = ID_GENERALSELECT;
	App->CLSB_Doc->ConfigureCurrentTool();

}

// *************************************************************************
// *	  	Select_MoveRotate:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_TopTabs::Select_MoveRotate()
{
	CFusionView			*pView;

	App->Get_Current_Document();

	POSITION pos;
	pos = App->m_pDoc->GetFirstViewPosition();
	pView = (CFusionView*)App->m_pDoc->GetNextView(pos) ;

	int mode = pView->GetModeTool();

	if(mode == ID_TOOLS_TEMPLATE)
	{
		pView->SetTool( ID_TOOLS_BRUSH_MOVEROTATEBRUSH );
		App->CLSB_Doc->ConfigureCurrentTool();
	} 
	else 
	{
		if(pView->GetTool()==ID_TOOLS_BRUSH_MOVEROTATEBRUSH)
		{
			pView->SetTool(CURTOOL_NONE);
			pView->SetAdjustMode (ADJUST_MODE_FACE);
		} 
		else 
		{
			pView->SetTool(ID_TOOLS_BRUSH_MOVEROTATEBRUSH);
		}

		App->CLSB_Doc->ConfigureCurrentTool();
	}
}

// *************************************************************************
// *		 	Select_Scale:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_TopTabs::Select_Scale()
{
	App->Get_Current_Document();


	int mode = App->CLSB_Doc->mModeTool;// pView->GetModeTool();

	if(mode == ID_TOOLS_TEMPLATE)
	{
		App->CLSB_Doc->mCurrentTool = ID_TOOLS_BRUSH_SCALEBRUSH;
		App->CLSB_Doc->ConfigureCurrentTool();
	} 
	else 
	{
		if(App->CLSB_Doc->mCurrentTool == ID_TOOLS_BRUSH_SCALEBRUSH)
		{
			App->CLSB_Doc->mCurrentTool = CURTOOL_NONE;
			App->CLSB_Doc->mAdjustMode = ADJUST_MODE_FACE;
		} 
		else 
		{
			App->CLSB_Doc->mCurrentTool = ID_TOOLS_BRUSH_SCALEBRUSH;
		}

		App->CLSB_Doc->ConfigureCurrentTool();
	}
}

// *************************************************************************
// *		 	Select_Shear:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_TopTabs::Select_Shear()
{
	App->Get_Current_Document();

	CFusionView			*pView;

	POSITION pos;
	pos = App->m_pDoc->GetFirstViewPosition();
	pView = (CFusionView*)App->m_pDoc->GetNextView(pos) ;

	int mode = pView->GetModeTool();

	if(mode == ID_TOOLS_TEMPLATE)
	{
		pView->SetTool( ID_TOOLS_BRUSH_SHEARBRUSH);
		App->CLSB_Doc->ConfigureCurrentTool();
	} 
	else 
	{
		if(pView->GetTool() == ID_TOOLS_BRUSH_SHEARBRUSH)
		{
			pView->SetTool(CURTOOL_NONE);
			pView->SetAdjustMode (ADJUST_MODE_FACE);
		}
		else 
		{
			pView->SetTool(ID_TOOLS_BRUSH_SHEARBRUSH);
		}

		App->CLSB_Doc->ConfigureCurrentTool();
	}
}

// *************************************************************************
// *		  Update_Dlg_Controls:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_TopTabs::Update_Dlg_Controls()
{
	App->Get_Current_Document();

	int NumBrushes = App->CL_World->Get_Brush_Count();
	if (NumBrushes == 0)
	{
		if (App->CLSB_ViewMgrDlg->View_MgrDlg_Active == 1)
		{
			EnableWindow(GetDlgItem(App->CLSB_ViewMgrDlg->MgrDlg_hWnd, IDC_BT_EXPORT), 0);
		}
	}
	else
	{
		if (App->CLSB_ViewMgrDlg->View_MgrDlg_Active == 1)
		{
			EnableWindow(GetDlgItem(App->CLSB_ViewMgrDlg->MgrDlg_hWnd, IDC_BT_EXPORT), 1);
		}
	}

	int NumSelBrushes = SelBrushList_GetSize(App->CLSB_Doc->pSelBrushes);
	if (NumSelBrushes == 0)
	{
		EndDialog(App->CLSB_Brushes->Dimensions_Dlg_hWnd, 0);
		App->CLSB_Brushes->Dimensions_Dlg_Running = 0;
		
		EnableWindow(GetDlgItem(Brush_Modify_Panel_Hwnd, IDC_BTTBMOVEROTATE), 0);
		EnableWindow(GetDlgItem(Brush_Modify_Panel_Hwnd, IDC_BTSCALE), 0);
		EnableWindow(GetDlgItem(Brush_Modify_Panel_Hwnd, IDC_BTTBSHEAR), 0);

		App->CLSB_TopTabs->Reset_Brush_Buttons();
		App->CLSB_TopTabs->Brush_Select_Flag = 1;
		RedrawWindow(App->CLSB_TopTabs->Brush_Modify_Panel_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		if (App->CLSB_ViewMgrDlg->View_MgrDlg_Active == 1)
		{
			EnableWindow(GetDlgItem(App->CLSB_ViewMgrDlg->MgrDlg_hWnd, IDC_BT_SELECTED), 0);
		}
		
		//App->CLSB_TopTabs->Select_Mode();
	}
	else
	{
		EnableWindow(GetDlgItem(Brush_Modify_Panel_Hwnd, IDC_BTTBMOVEROTATE), 1);
		EnableWindow(GetDlgItem(Brush_Modify_Panel_Hwnd, IDC_BTSCALE), 1);
		EnableWindow(GetDlgItem(Brush_Modify_Panel_Hwnd, IDC_BTTBSHEAR), 1);

		if (App->CLSB_ViewMgrDlg->View_MgrDlg_Active == 1)
		{
			EnableWindow(GetDlgItem(App->CLSB_ViewMgrDlg->MgrDlg_hWnd, IDC_BT_SELECTED), 1);
		}

		Show_BrushPanel();
	}

	if (App->CLSB_Equity->EquitySB_Dialog_Visible == 0)
	{
		if (App->CLSB_ViewMgrDlg->View_MgrDlg_Active == 1)
		{
			EnableWindow(GetDlgItem(App->CLSB_ViewMgrDlg->MgrDlg_hWnd, IDC_BT_VIEWUPDATE), 0);
		}
	}
	else
	{
		if (App->CLSB_ViewMgrDlg->View_MgrDlg_Active == 1)
		{
			EnableWindow(GetDlgItem(App->CLSB_ViewMgrDlg->MgrDlg_hWnd, IDC_BT_VIEWUPDATE), 1);
		}
	}
}

// *************************************************************************
// *			 Show_BrushPanel:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_TopTabs::Show_BrushPanel()
{
	App->CLSB_TopTabs->Reset_Tabs_Buttons();
	App->CLSB_TopTabs->Header_BrushModify_Flag = 1;
	RedrawWindow(App->CLSB_TopTabs->Top_Tabs_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	ShowWindow(App->CLSB_TopTabs->Brush_Modify_Panel_Hwnd, SW_SHOW);
}
