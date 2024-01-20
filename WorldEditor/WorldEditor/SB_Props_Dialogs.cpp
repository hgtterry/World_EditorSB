/*
Copyright (c) 2022 - 2024 Reality Factory Scene Builder -- HGT Software W.T.Flanigan H.C.Flanigan

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
#include "SB_Props_Dialogs.h"

SB_Props_Dialogs::SB_Props_Dialogs(void)
{
	Dimensions_Dlg_hWnd = NULL;
}

SB_Props_Dialogs::~SB_Props_Dialogs(void)
{
}

// **************************************************************************
// *	  		Start_Props_Dialogs:- Terry and Hazel Flanigan 2022			*
// **************************************************************************
void SB_Props_Dialogs::Start_Props_Dialogs()
{

	Start_Dialog_Dimensions();
	/*Start_Dialog_PhysicsReset();
	Start_Dialog_Debug();
	Start_Panels_Test_Dlg();
	Start_Area_PropsPanel();
	Start_Details_Goto_Dlg();
	Start_Materials_PropsPanel();*/

}

// *************************************************************************
// *	  Start_Dialog_Dimensions:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool SB_Props_Dialogs::Start_Dialog_Dimensions()
{
	Dimensions_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_PROPS_DIMENSIONS, App->CLSB_Properties->Properties_Dlg_hWnd, (DLGPROC)Dialog_Dimensions_Proc);

	//Init_Bmps_Dimensions();
	Hide_Dimensions_Dlg(0, 0);

	return 1;
}
// *************************************************************************
// *		Dialog_Text_Proc:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
LRESULT CALLBACK SB_Props_Dialogs::Dialog_Dimensions_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BT_POSITION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ROTATION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_SCALE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_LOCK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{

		if (GetDlgItem(hDlg, IDC_CK_LOCK) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			//return (UINT)App->Brush_Panel;
			return (LONG)App->AppBackground;
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		//return (LONG)App->Brush_Panel;
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_POSITION && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Dimensions->Show_Position);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_SCALE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Dimensions->Show_Scale);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_ROTATION && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Dimensions->Show_Rotation);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		//if (LOWORD(wParam) == IDC_CK_LOCK)
		//{
		//	HWND temp = GetDlgItem(hDlg, IDC_CK_LOCK);
		//	int test = SendMessage(temp, BM_GETCHECK, 0, 0);
		//	if (test == BST_CHECKED)
		//	{
		//		int EditCat = App->SBC_Properties->Edit_Category;
		//		//if (EditCat == Enums::FV_Edit_Object || EditCat == Enums::Edit_Teleport)
		//		{
		//			int Index = App->SBC_Properties->Current_Selected_Object;
		//			App->SBC_Scene->V_Object[Index]->Dimensions_Locked = 1;

		//			App->SBC_Scene->V_Object[Index]->Altered = 1;
		//			App->SBC_FileView->Mark_Altered(App->SBC_Scene->V_Object[Index]->FileViewItem);
		//			App->SBC_Scene->Scene_Modified = 1;
		//		}

		//		EnableWindow(GetDlgItem(hDlg, IDC_BT_POSITION), 0);
		//		EnableWindow(GetDlgItem(hDlg, IDC_BT_SCALE), 0);
		//		EnableWindow(GetDlgItem(hDlg, IDC_BT_ROTATION), 0);
		//		return 1;
		//	}
		//	else
		//	{
		//		int EditCat = App->SBC_Properties->Edit_Category;
		//		//if (EditCat == Enums::FV_Edit_Object || EditCat == Enums::Edit_Teleport)
		//		{
		//			int Index = App->SBC_Properties->Current_Selected_Object;
		//			App->SBC_Scene->V_Object[Index]->Dimensions_Locked = 0;

		//			App->SBC_Scene->V_Object[Index]->Altered = 1;
		//			App->SBC_FileView->Mark_Altered(App->SBC_Scene->V_Object[Index]->FileViewItem);
		//			App->SBC_Scene->Scene_Modified = 1;
		//		}

		//		if (App->SBC_Properties->Edit_Category == Enums::Edit_Area)
		//		{
		//			if (App->SBC_Properties->Edit_Category == Enums::Edit_Area)
		//			{
		//				App->SBC_Dialogs->YesNo("Edit Area", "Are You Sure", 1);
		//				bool Doit = App->SBC_Dialogs->Canceled;
		//				if (Doit == 1)
		//				{
		//					SendMessage(temp, BM_SETCHECK, 1, 0);
		//					return TRUE;
		//				}
		//			}
		//		}

		//		EnableWindow(GetDlgItem(hDlg, IDC_BT_POSITION), 1);
		//		EnableWindow(GetDlgItem(hDlg, IDC_BT_SCALE), 1);
		//		EnableWindow(GetDlgItem(hDlg, IDC_BT_ROTATION), 1);
		//		return 1;
		//	}
		//	return TRUE;
		//}

		if (LOWORD(wParam) == IDC_BT_POSITION)
		{

			if (App->CLSB_Dimensions->Show_Position == 1)
			{
				App->CLSB_Dimensions->Show_Position = 0;
				//App->CLSB_Markers->Hide_Axis_Marker();
			}
			else
			{
				//App->CLSB_Markers->Hide_Axis_Marker();

				App->CLSB_Dimensions->Prepare_Dimensions();
				App->CLSB_Dimensions->Show_Position = 1;
				App->CLSB_Dimensions->Show_Scale = 0;
				App->CLSB_Dimensions->Show_Rotation = 0;
			}

			RedrawWindow(App->CLSB_Props_Dialogs->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_SCALE)
		{

			if (App->CLSB_Dimensions->Show_Scale == 1)
			{
				App->CLSB_Dimensions->Show_Scale = 0;
				//App->CLSB_Markers->Hide_Axis_Marker();
			}
			else
			{
				//App->CLSB_Markers->Hide_Axis_Marker();

				App->CLSB_Dimensions->Prepare_Dimensions();
				App->CLSB_Dimensions->Show_Scale = 1;
				App->CLSB_Dimensions->Show_Position = 0;
				App->CLSB_Dimensions->Show_Rotation = 0;
			}

			RedrawWindow(App->CLSB_Props_Dialogs->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_ROTATION)
		{

			if (App->CLSB_Dimensions->Show_Rotation == 1)
			{
				App->CLSB_Dimensions->Show_Rotation = 0;
				//App->CLSB_Markers->Hide_Axis_Marker();
			}
			else
			{
				//App->CLSB_Markers->Hide_Axis_Marker();

				App->CLSB_Dimensions->Prepare_Dimensions();
				App->CLSB_Dimensions->Show_Rotation = 1;
				App->CLSB_Dimensions->Show_Position = 0;
				App->CLSB_Dimensions->Show_Scale = 0;
			}

			RedrawWindow(App->CLSB_Props_Dialogs->Dimensions_Dlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			return 1;
		}

		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	}
	return FALSE;
}

// *************************************************************************
// *		Hide_Dimensions_Dlg:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_Props_Dialogs::Hide_Dimensions_Dlg(bool Show, bool Lock_Dimensions)
{

	if (Lock_Dimensions == 1)
	{
		HWND temp = GetDlgItem(Dimensions_Dlg_hWnd, IDC_CK_LOCK);
		SendMessage(temp, BM_SETCHECK, 1, 0);

		EnableWindow(GetDlgItem(Dimensions_Dlg_hWnd, IDC_BT_POSITION), 0);
		EnableWindow(GetDlgItem(Dimensions_Dlg_hWnd, IDC_BT_SCALE), 0);
		EnableWindow(GetDlgItem(Dimensions_Dlg_hWnd, IDC_BT_ROTATION), 0);
	}
	else
	{
		HWND temp = GetDlgItem(Dimensions_Dlg_hWnd, IDC_CK_LOCK);
		SendMessage(temp, BM_SETCHECK, 0, 0);

		EnableWindow(GetDlgItem(Dimensions_Dlg_hWnd, IDC_BT_POSITION), 1);
		EnableWindow(GetDlgItem(Dimensions_Dlg_hWnd, IDC_BT_SCALE), 1);
		EnableWindow(GetDlgItem(Dimensions_Dlg_hWnd, IDC_BT_ROTATION), 1);
	}

	ShowWindow(Dimensions_Dlg_hWnd, Show);

}
