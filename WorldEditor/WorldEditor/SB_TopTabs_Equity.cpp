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
#include "SB_TopTabs_Equity.h"

SB_TopTabs_Equity::SB_TopTabs_Equity(void)
{
	Tabs_TB_hWnd_Eq =	nullptr;
	Test_TB_hWnd =		nullptr;
	Camera_TB_hWnd =	nullptr;

	Render_Buttons_EQ_hWnd =	nullptr;
	GridHair_Buttons_hWnd =		nullptr;

	Toggle_Tabs_Test_Flag = 0;
	Toggle_Dimensions_Flag = 0;

	Toggle_Camera_First_Flag = 0;
	Toggle_Camera_Free_Flag = 0;
	Toggle_Camera_Model_Flag = 1;

	Toggle_Tabs_Camera_Flag = 1;

	Picking_Active_Flag = 0;
}

SB_TopTabs_Equity::~SB_TopTabs_Equity(void)
{
}

// *************************************************************************
// *			Start_Tabs:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_TopTabs_Equity::Start_Tabs(void)
{
	Start_GridHair_Buttons();
	Start_Render_Buttons_EQ();
	

	Start_Tabs_Headers();
	Start_Files_TB();
	Start_Camera_TB();

	Hide_Tabs();
	ShowWindow(Camera_TB_hWnd, SW_SHOW);
	Toggle_Tabs_Camera_Flag = 1;

	RedrawWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *		Start_Tabs_Headers:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_TopTabs_Equity::Start_Tabs_Headers(void)
{
	Tabs_TB_hWnd_Eq = nullptr;

	Tabs_TB_hWnd_Eq = CreateDialog(App->hInst, (LPCTSTR)IDD_TOPTABS_EQUITY, App->Equity_Dlg_hWnd, (DLGPROC)Tabs_Headers_Proc);
}

// *************************************************************************
// *		Tabs_Headers_Proc_Proc:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs_Equity::Tabs_Headers_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TT_TEST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_CAMERA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_UPDATE2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_UPDATE2 && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_UPDATE2));
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

		if (some_item->idFrom == IDC_BT_TT_TEST && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CLSB_TopTabs_Equity->Toggle_Tabs_Test_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_CAMERA && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle_Tabs(item, App->CLSB_TopTabs_Equity->Toggle_Tabs_Camera_Flag);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_TT_TEST)
		{
			App->CLSB_TopTabs_Equity->Hide_Tabs();
			ShowWindow(App->CLSB_TopTabs_Equity->Test_TB_hWnd, SW_SHOW);
			App->CLSB_TopTabs_Equity->Toggle_Tabs_Test_Flag = 1;

			RedrawWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			//App->CL_Ogre->Ogre_Listener->ImGui_Render_Tab = Enums::ImGui_Render_Group;
			//App->CL_Panels->Show_Panels(1);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TT_CAMERA)
		{
			App->CLSB_TopTabs_Equity->Hide_Tabs();
			ShowWindow(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, SW_SHOW);
			App->CLSB_TopTabs_Equity->Toggle_Tabs_Camera_Flag = 1;

			RedrawWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_TEST)
		{
			//Debug
			//App->CLSB_Export_Ogre3D->Convert_ToOgre3D(1);
			return 1;
		}

		if (LOWORD(wParam) == IDC_UPDATE2)
		{
			App->CLSB_Mesh_Mgr->Update_World();
			return 1;
		}

	}
	}
	return FALSE;
}

// *************************************************************************
// *			Hide_Tabs:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_TopTabs_Equity::Hide_Tabs(void)
{

	ShowWindow(App->CLSB_TopTabs_Equity->Test_TB_hWnd, SW_HIDE);
	ShowWindow(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, SW_HIDE);


	Toggle_Tabs_Test_Flag = 0;
	Toggle_Tabs_Camera_Flag = 0;


}

// *************************************************************************
// *			Start_Files_TB:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_TopTabs_Equity::Start_Files_TB(void)
{
	Test_TB_hWnd = nullptr;

	Test_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_EQ_TEST, Tabs_TB_hWnd_Eq, (DLGPROC)Files_TB_Proc);
}

// *************************************************************************
// *			Files_TB_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs_Equity::Files_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_UPDATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTBDIMENSIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_EQ_GROUPS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_UPDATE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_FIRST_MODEX));
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

		if (some_item->idFrom == IDC_BT_EQ_GROUPS && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTTBDIMENSIONS && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_TopTabs_Equity->Toggle_Dimensions_Flag);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_ETEST)
		{
			Debug
			//if (App->SBC_Scene->Scene_Loaded == 1)
			return 1;
		}
		
		if (LOWORD(wParam) == IDC_ADDTOSCENE)
		{
			App->CL_Brush->Brush_Create_From_Equity();
			return 1;
		}
		
		if (LOWORD(wParam) == IDC_BTTBDIMENSIONS)
		{

			//if (App->CLSB_Dimensions->Show_Position == 1)
			//{
			//	App->CLSB_Dimensions->Show_Position = 0;
			//	//App->CL_Panels->Show_Panels(1);
			//}
			//else
			//{
			//	App->CLSB_Dimensions->Show_Position = 1;
			//	//App->CL_Panels->Show_Panels(0);
			//}

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_EQ_GROUPS)
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				App->CLSB_Mesh_Mgr->Start_Brush_Viewer();
			}
			return 1;
		}

		return FALSE;
	}

	}
	return FALSE;
}

// *************************************************************************
// *			Start_Camera_TB:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_TopTabs_Equity::Start_Camera_TB(void)
{
	Camera_TB_hWnd = nullptr;

	Camera_TB_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TB_EQ_CAMERA, Tabs_TB_hWnd_Eq, (DLGPROC)Camera_TB_Proc);
}

// *************************************************************************
// *			Camera_TB_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs_Equity::Camera_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_TT_MODEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_FIRST_MODEX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_FREE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_RESETCAM, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TT_ZEROCAM, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CAMERASPEED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CAMERA_PICK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_TT_MODEL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TT_MODEL));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_TopTabs_Equity->Toggle_Camera_Model_Flag);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_FIRST_MODEX && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_FIRST_MODEX));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_TopTabs_Equity->Toggle_Camera_First_Flag);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_FREE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_FIRST_MODEX));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_TopTabs_Equity->Toggle_Camera_Free_Flag);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_RESETCAM && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TT_ZEROCAM && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CAMERASPEED && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_CAMERA_PICK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_TopTabs_Equity->Picking_Active_Flag);
			return CDRF_DODEFAULT;
		}

		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		
		if (LOWORD(wParam) == IDC_BT_TT_MODEL)
		{
			App->CLSB_TopTabs_Equity->Camera_Set_Model();
			return 1;
		}

		if (LOWORD(wParam) == IDC_FIRST_MODEX)
		{
			App->CLSB_TopTabs_Equity->Camera_Set_First();

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_TT_FREE)
		{
			App->CLSB_TopTabs_Equity->Camera_Set_Free();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_TT_RESETCAM)
		{
			App->CLSB_Camera_EQ->Reset_View();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_TT_ZEROCAM)
		{
			App->CLSB_Camera_EQ->Zero_View();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_CAMERASPEED)
		{
			App->CLSB_Dialogs->Start_Speed_Camera();
			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_CAMERA_PICK)
		{
			if (App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode == 1)
			{
				App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode = 0;
				App->CLSB_TopTabs_Equity->Picking_Active_Flag = 0;

				//App->CLSB_ImGui->Show_Face_Selection = 0;
			
				App->CLSB_Ogre_Setup->RenderListener->Show_Marker_Face = 0;
			}
			else
			{
				App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode = 1;
				App->CLSB_TopTabs_Equity->Picking_Active_Flag = 1;

				//App->CLSB_ImGui->Show_Face_Selection = 1;
				
				App->CLSB_Ogre_Setup->RenderListener->Show_Marker_Face = 1;
			}

			return 1;
		}
		
		return FALSE;
	}

	}
	return FALSE;
}

// *************************************************************************
// *			Camera_Set_Model:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_TopTabs_Equity::Camera_Set_Model(void)
{
	App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamModel);
	App->CLSB_TopTabs_Equity->Toggle_Camera_Model_Flag = 1;
	App->CLSB_TopTabs_Equity->Toggle_Camera_Free_Flag = 0;
	App->CLSB_TopTabs_Equity->Toggle_Camera_First_Flag = 0;

	RedrawWindow(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Camera_Set_Free:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_TopTabs_Equity::Camera_Set_Free(void)
{
	App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamDetached);
	App->CLSB_TopTabs_Equity->Toggle_Camera_Free_Flag = 1;
	App->CLSB_TopTabs_Equity->Toggle_Camera_Model_Flag = 0;
	App->CLSB_TopTabs_Equity->Toggle_Camera_First_Flag = 0;

	RedrawWindow(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *			Camera_Set_First:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_TopTabs_Equity::Camera_Set_First(void)
{
	if (App->CLSB_Scene_Data->Player_Added == 1)
	{
		App->CLSB_Ogre_Setup->OgreListener->CameraMode = Enums::CamFirst;
		App->CLSB_TopTabs_Equity->Toggle_Camera_First_Flag = 1;
		App->CLSB_TopTabs_Equity->Toggle_Camera_Model_Flag = 0;
		App->CLSB_TopTabs_Equity->Toggle_Camera_Free_Flag = 0;

		App->CLSB_Scene_Data->B_Player[0]->Player_Node->setVisible(false);

		RedrawWindow(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}

}

// *************************************************************************
// *	  	Start_GridHair_Buttons:- Terry and Hazel Flanigan 2024         *
// *************************************************************************
void SB_TopTabs_Equity::Start_GridHair_Buttons()
{
	GridHair_Buttons_hWnd = nullptr;

	GridHair_Buttons_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TB_GRIDHAIR, App->Equity_Dlg_hWnd, (DLGPROC)GridHair_Buttons_Proc);
	//Init_Bmps_Globals_EQ();
}

// *************************************************************************
// *	   GridHair_Buttons_Proc:- Terry and Hazel Flanigan 2024           *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs_Equity::GridHair_Buttons_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
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

		//-------------------------------------------------------- Show Grid
		if (LOWORD(wParam) == IDC_TBSHOWGRID)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_TBSHOWGRID);

			if (App->CLSB_Grid->ShowGridFlag == 1)
			{
				App->CLSB_Grid->Grid_SetVisible(0);
				App->CLSB_Grid->ShowGridFlag = 0;

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOff_Bmp);
			}
			else
			{
				App->CLSB_Grid->Grid_SetVisible(1);
				App->CLSB_Grid->ShowGridFlag = 1;

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOn_Bmp);

			}

			return TRUE;
		}

		//-------------------------------------------------------- Show Hair
		if (LOWORD(wParam) == IDC_TBSHOWHAIR)
		{
			HWND Temp = GetDlgItem(hDlg, IDC_TBSHOWHAIR);

			if (App->CLSB_Grid->ShowHair == 1)
			{
				App->CLSB_Grid->ShowHair = 0;
				App->CLSB_Grid->Hair_SetVisible(0);

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOff_Bmp);
			}
			else
			{
				App->CLSB_Grid->ShowHair = 1;
				App->CLSB_Grid->Hair_SetVisible(1);

				SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOn_Bmp);
			}

			return TRUE;
		}

		break;
	}
	return FALSE;
}


// *************************************************************************
// *	  	Hide_Render_Buttons_EQ:- Terry and Hazel Flanigan 2024         *
// *************************************************************************
void SB_TopTabs_Equity::Hide_Render_Buttons_EQ(bool Flag)
{
	if (Flag == 1)
	{
		EnableWindow(Render_Buttons_EQ_hWnd, 0);
		ShowWindow(Render_Buttons_EQ_hWnd, SW_HIDE);
	}
	else
	{
		EnableWindow(Render_Buttons_EQ_hWnd, 1);
		ShowWindow(Render_Buttons_EQ_hWnd, SW_SHOW);
	}
}

// *************************************************************************
// *	  	Start_Render_Buttonss_EQ:- Terry and Hazel Flanigan 2024       *
// *************************************************************************
void SB_TopTabs_Equity::Start_Render_Buttons_EQ()
{
	Render_Buttons_EQ_hWnd = nullptr;

	Render_Buttons_EQ_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TB_BUTTONS, App->Equity_Dlg_hWnd, (DLGPROC)Render_Buttons_Proc_EQ);
	Init_Bmps_Globals_EQ();
}

// *************************************************************************
// *	Render_Buttons_Procs_EQ:- Terry and Hazel Flanigan 2024            *
// *************************************************************************
LRESULT CALLBACK SB_TopTabs_Equity::Render_Buttons_Proc_EQ(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
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

		//-------------------------------------------------------- Full Screen
		if (LOWORD(wParam) == IDC_BTTB_FULLSCREEN)
		{
			App->CLSB_Scene_Data->Go_FullScreen_Mode();
			return TRUE;
		}
		//-------------------------------------------------------- Show Info
		if (LOWORD(wParam) == IDC_TBINFO2)
		{
		
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_TBSHOWFACES)
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_TBSHOWFACES);

				if (App->CLSB_Ogre_Setup->RenderListener->ShowFaces == 1)
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowFaces = 0;
					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);
				}
				else
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowFaces = 1;
					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOn_Bmp);
				}
			}

			return TRUE;
		}

		//-------------------------------------------------------- Show Textures
		if (LOWORD(wParam) == IDC_BTSHOWTEXTURES)
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_BTSHOWTEXTURES);

				if (App->CLSB_Ogre_Setup->RenderListener->ShowTextured == 1)
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowTextured = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOff_Bmp);
				}
				else
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowTextured = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);
				}
			}

			return TRUE;
		}

		//-------------------------------------------------------- Show Normals
		if (LOWORD(wParam) == IDC_BTSHOWNORMALS)
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_BTSHOWNORMALS);

				if (App->CLSB_Ogre_Setup->RenderListener->ShowNormals == 1)
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowNormals = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOff_Bmp);
				}
				else
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowNormals = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOn_Bmp);
				}
			}

			return TRUE;
		}

		//-------------------------------------------------------- Show Light
		if (LOWORD(wParam) == IDC_BTSHOWLIGHT2)
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_BTSHOWLIGHT2);

				if (App->CLSB_Ogre_Setup->RenderListener->Light_Activated == 1)
				{
					App->CLSB_Ogre_Setup->RenderListener->Light_Activated = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_LightsOff_Bmp);
				}
				else
				{
					App->CLSB_Ogre_Setup->RenderListener->Light_Activated = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_LightsOn_Bmp);
				}
			}

			return TRUE;
		}

		//-------------------------------------------------------- Show Points
		if (LOWORD(wParam) == IDC_BTSHOWPOINTS)
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_BTSHOWPOINTS);

				if (App->CLSB_Ogre_Setup->RenderListener->ShowPoints == 1)
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowPoints = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOff_Bmp);
				}
				else
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowPoints = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOn_Bmp);
				}
			}
			return TRUE;
		}

		//-------------------------------------------------------- Show Bones
		if (LOWORD(wParam) == IDC_BTSHOWBONES)
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_BTSHOWBONES);

				if (App->CLSB_Ogre_Setup->RenderListener->ShowBones == 1)
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowBones = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOff_Bmp);
				}
				else
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowBones = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOn_Bmp);
				}
			}

			return TRUE;
		}

		//-------------------------------------------------------- Show Bound Box
		if (LOWORD(wParam) == IDC_TBBOUNDBOX)
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				HWND Temp = GetDlgItem(hDlg, IDC_TBBOUNDBOX);

				if (App->CLSB_Ogre_Setup->RenderListener->ShowBoundingBox == 1)
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowBoundingBox = 0;
					//App->CLSB_TopBar->Toggle_BBox_Flag = 0;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOff_Bmp);
				}
				else
				{
					App->CLSB_Ogre_Setup->RenderListener->ShowBoundingBox = 1;
					//App->CLSB_TopBar->Toggle_BBox_Flag = 1;

					SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOn_Bmp);
				}
			}

			return TRUE;
		}
		break;
	}
	return FALSE;
}

// *************************************************************************
// *		Init_Bmps_Globalss_EQ:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void SB_TopTabs_Equity::Init_Bmps_Globals_EQ(void)
{

	HWND Temp = GetDlgItem(GridHair_Buttons_hWnd, IDC_TBSHOWGRID);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_GridOn_Bmp);

	Temp = GetDlgItem(GridHair_Buttons_hWnd, IDC_TBSHOWHAIR);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_HairOn_Bmp);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_TBINFO2);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_ModelInfo_Bmp);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_TBBOUNDBOX);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BBOff_Bmp);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_TBSHOWFACES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshOff_Bmp);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWTEXTURES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_TexturesOn_Bmp);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWPOINTS);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_MeshPointsOff_Bmp);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWBONES);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_BonesOff_Bmp);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWNORMALS);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_NormalsOff_Bmp);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWLIGHT2);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_LightsOff_Bmp);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTTB_FULLSCREEN);
	SendMessage(Temp, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)(HANDLE)App->Hnd_FullScreen_Bmp);

	HWND hTooltip_TB_2 = CreateWindowEx(0, TOOLTIPS_CLASS, "", TTS_ALWAYSTIP | TTS_BALLOON, 0, 0, 0, 0, App->MainHwnd, 0, App->hInst, 0);
	SendMessage(hTooltip_TB_2, TTM_SETMAXTIPWIDTH, 0, 150);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_TBINFO2);
	TOOLINFO ti8 = { 0 };
	ti8.cbSize = sizeof(ti8);
	ti8.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti8.uId = (UINT_PTR)Temp;
	ti8.lpszText = "Show Model Information";
	ti8.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti8);

	Temp = GetDlgItem(GridHair_Buttons_hWnd, IDC_TBSHOWHAIR);
	TOOLINFO ti9 = { 0 };
	ti9.cbSize = sizeof(ti9);
	ti9.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti9.uId = (UINT_PTR)Temp;
	ti9.lpszText = "Toggle Main Cross Hair";
	ti9.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti9);

	Temp = GetDlgItem(GridHair_Buttons_hWnd, IDC_TBSHOWGRID);
	TOOLINFO ti10 = { 0 };
	ti10.cbSize = sizeof(ti10);
	ti10.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti10.uId = (UINT_PTR)Temp;
	ti10.lpszText = "Toggle Main Grid";
	ti10.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti10);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_TBBOUNDBOX);
	TOOLINFO ti11 = { 0 };
	ti11.cbSize = sizeof(ti11);
	ti11.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti11.uId = (UINT_PTR)Temp;
	ti11.lpszText = "Toggle Bounding Box";
	ti11.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti11);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_TBSHOWFACES);
	TOOLINFO ti12 = { 0 };
	ti12.cbSize = sizeof(ti12);
	ti12.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti12.uId = (UINT_PTR)Temp;
	ti12.lpszText = "Toggle Show Faces";
	ti12.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti12);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWTEXTURES);
	TOOLINFO ti13 = { 0 };
	ti13.cbSize = sizeof(ti13);
	ti13.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti13.uId = (UINT_PTR)Temp;
	ti13.lpszText = "Toggle Show Textures";
	ti13.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti13);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWPOINTS);
	TOOLINFO ti14 = { 0 };
	ti14.cbSize = sizeof(ti14);
	ti14.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti14.uId = (UINT_PTR)Temp;
	ti14.lpszText = "Toggle Show Mesh Points";
	ti14.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti14);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWBONES);
	TOOLINFO ti15 = { 0 };
	ti15.cbSize = sizeof(ti15);
	ti15.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti15.uId = (UINT_PTR)Temp;
	ti15.lpszText = "Toggle Show Bones";
	ti15.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti15);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWNORMALS);
	TOOLINFO ti16 = { 0 };
	ti16.cbSize = sizeof(ti16);
	ti16.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti16.uId = (UINT_PTR)Temp;
	ti16.lpszText = "Toggle Show Normals";
	ti16.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti16);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTSHOWLIGHT2);
	TOOLINFO ti17 = { 0 };
	ti17.cbSize = sizeof(ti17);
	ti17.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti17.uId = (UINT_PTR)Temp;
	ti17.lpszText = "Toggle Show Light";
	ti17.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti17);

	Temp = GetDlgItem(Render_Buttons_EQ_hWnd, IDC_BTTB_FULLSCREEN);
	TOOLINFO ti18 = { 0 };
	ti18.cbSize = sizeof(ti11);
	ti18.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP;
	ti18.uId = (UINT_PTR)Temp;
	ti18.lpszText = "Full Screen Mode\r\nPress Esc to Exit";
	ti18.hwnd = App->MainHwnd;
	SendMessage(hTooltip_TB_2, TTM_ADDTOOL, 0, (LPARAM)&ti18);

}


