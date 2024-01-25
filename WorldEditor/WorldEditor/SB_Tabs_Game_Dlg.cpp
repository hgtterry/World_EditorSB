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
#include "resource.h"
#include "SB_Tabs_Game_Dlg.h"

SB_Tabs_Game_Dlg::SB_Tabs_Game_Dlg(void)
{
	Game_Tab_hDlg_Hwnd = nullptr;

	Toggle_Camera_First_Flag = 0;
	Toggle_Camera_Free_Flag = 1;
}

SB_Tabs_Game_Dlg::~SB_Tabs_Game_Dlg(void)
{
}

// *************************************************************************
// *	  	Start_Game_Settings:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Tabs_Game_Dlg::Start_Game_Settings()
{
	Game_Tab_hDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_3DSETTINGS, App->CLSB_TabsControl->Tabs_Control_Hwnd, (DLGPROC)Game_Settings_Proc);
}

// *************************************************************************
// *			 Game_Settings_Proc:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
LRESULT CALLBACK SB_Tabs_Game_Dlg::Game_Settings_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_3DUPDATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_PICK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_TRUE3D, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_FIRST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_FREE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_FULLSCREEN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_PREVIEWEDITOR, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		/*
		if (GetDlgItem(hDlg, IDC_ST_GD_GROUPS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_3DUPDATE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_3DUPDATE));
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

		if (some_item->idFrom == IDC_BT_PICK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_PICK));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode);
			}
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_TRUE3D && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_TRUE3D));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->BR_True3D_Mode_Active);
			}
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_FIRST && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_FIRST));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Tabs_True3D_Dlg->Toggle_Camera_First_Flag);
			}
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_FREE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_FREE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Tabs_True3D_Dlg->Toggle_Camera_Free_Flag);
			}
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_FULLSCREEN && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_FULLSCREEN));
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

		if (some_item->idFrom == IDC_BT_PREVIEWEDITOR && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_PREVIEWEDITOR));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Tabs_True3D_Dlg->GameEditor_Active_Flag);
			}

			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_PREVIEWEDITOR)
		{
			if (App->CLSB_Tabs_True3D_Dlg->GameEditor_Active_Flag == 1)
			{

				App->CLSB_Tabs_True3D_Dlg->GameEditor_Active_Flag = 0;
				RedrawWindow(App->CLSB_Tabs_True3D_Dlg->Game_Tab_hDlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

				ShowWindow(App->ListPanel, false);
				ShowWindow(App->CLSB_Properties->Properties_Dlg_hWnd, false);
				App->CLSB_Equity->Equity_Render_Mode = Enums::EQ_Mode_GameDirector;
				
				App->CLSB_Game_Editor->Hide_Game_Editor_Dialog();
				RedrawWindow(App->CLSB_Tabs_True3D_Dlg->Game_Tab_hDlg_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
			else
			{
				/*if (App->BR_True3D_Mode_Active == 1)
				{
					App->CLSB_BR_Render->Exit_BR_3D_Mode();
					App->BR_True3D_Mode_Active = 0;
					App->CLSB_ViewMgrDlg->Was_BR_True3D_Mode_Active = 1;
				}*/

				App->CLSB_Tabs_True3D_Dlg->GameEditor_Active_Flag = 1;
				App->CLSB_Game_Editor->Go_Game_Editor();
			}
			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BT_TRUE3D)
		{
			if (App->BR_True3D_Mode_Active == 1)
			{
				App->CLSB_BR_Render->Exit_BR_3D_Mode();
				App->CLSB_Tabs_True3D_Dlg->Set_Tabs_3DSettings_On(false);
			}
			else
			{
				App->CLSB_BR_Render->Start_BR_3D_Mode();
				App->CLSB_Tabs_True3D_Dlg->Set_Tabs_3DSettings_On(true);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_FREE)
		{
			/*App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamDetached);
			App->CLSB_Tabs_True3D_Dlg->Toggle_Camera_Free_Flag = 1;
			App->CLSB_Tabs_True3D_Dlg->Toggle_Camera_First_Flag = 0;

			if (App->CLSB_Scene->Player_Added == 1)
			{
				int f = App->CLSB_Scene->B_Player[0]->Phys_Body->getCollisionFlags();
				App->CLSB_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
			}

			RedrawWindow(App->CLSB_Tabs_True3D_Dlg->RB_3DSettings_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);*/

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_FIRST)
		{
			/*if (App->CLSB_Scene->Player_Added == 1)
			{
				App->CLSB_Tabs_True3D_Dlg->Toggle_Camera_First_Flag = 1;
				App->CLSB_Tabs_True3D_Dlg->Toggle_Camera_Free_Flag = 0;
				App->CLSB_Camera_EQ->Reset_Orientation();
				App->CLSB_Ogre->OgreListener->CameraMode = Enums::CamFirst;
				App->CLSB_TopTabs_Equity->Camera_Set_First();
			}

			RedrawWindow(App->CLSB_Tabs_True3D_Dlg->RB_3DSettings_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);*/

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_3DUPDATE)
		{
			App->CLSB_Mesh_Mgr->Update_World();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PICK)
		{
			if (App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode == 1)
			{
				App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode = 0;
				App->CLSB_Ogre_Setup->RenderListener->Show_Brush_Faces = 0;
				App->CLSB_Ogre_Setup->RenderListener->Show_Selected_Face = 0;
				App->CLSB_Ogre_Setup->RenderListener->Show_Marker_Face = 0;
			}
			else
			{
				App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode = 1;
				App->CLSB_Ogre_Setup->RenderListener->Show_Marker_Face = 1;
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_FULLSCREEN)
		{
			App->CLSB_Scene->Go_FullScreen_Mode();
			return TRUE;
		}
		
		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *	  	Show_Game_Dlg:- Terry and Hazel Flanigan 2023		           *
// *************************************************************************
void SB_Tabs_Game_Dlg::Show_Game_Dlg(bool Show)
{
	ShowWindow(Game_Tab_hDlg_Hwnd, Show);
}

// *************************************************************************
// *		Set_Control_Tabs_3DSettings:- Terry and Hazel Flanigan 2023    *
// *************************************************************************
void SB_Tabs_Game_Dlg::Set_Control_Tabs_3DSettings_On(bool flag)
{
	EnableWindow(GetDlgItem(App->CLSB_TabsControl->Tabs_Control_Hwnd, IDC_BT_3DSETTINGS), flag);
}

// *************************************************************************
// *		Set_Tabs_3DSettings:- Terry and Hazel Flanigan 2023            *
// *************************************************************************
void SB_Tabs_Game_Dlg::Set_Tabs_3DSettings_On(bool flag)
{
	/*EnableWindow(GetDlgItem(RB_3DSettings_Hwnd, IDC_BT_3DUPDATE), flag);
	EnableWindow(GetDlgItem(RB_3DSettings_Hwnd, IDC_BT_PICK), flag);
	EnableWindow(GetDlgItem(RB_3DSettings_Hwnd, IDC_BT_FIRST), flag);
	EnableWindow(GetDlgItem(RB_3DSettings_Hwnd, IDC_BT_FREE), flag);
	EnableWindow(GetDlgItem(RB_3DSettings_Hwnd, IDC_BT_FULLSCREEN), flag);

	RedrawWindow(RB_3DSettings_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);*/
}
