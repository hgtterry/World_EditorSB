/*
Copyright (c) 2022 GameDirectorSB and EquityME -- HGTInflanite Software W.T.Flanigan H.C.Flanigan

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
#include "SB_Project_Create.h"

SB_Project_Create::SB_Project_Create()
{
	Dialog_cancelled_F = 0;
}

SB_Project_Create::~SB_Project_Create()
{
}

// *************************************************************************
// *	  		Start_New_Project:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_Project_Create::Start_New_Project()
{
	//App->CLSB_Scene_Data->Clear_Level();
	//App->CLSB_Scene_Data->Create_Resources_Group();

	Start_Save_Project_Dialog();

	if (App->CLSB_Project_Create->Dialog_cancelled_F == 1)
	{
		//App->Say("Cancelled");
	}
	else
	{
		Create_New_Project();
	}

}

// *************************************************************************
// *	  Start_Save_Project_Dialog:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_Project_Create::Start_Save_Project_Dialog()
{
	Dialog_cancelled_F = 0;

	DialogBox(App->hInst, (LPCTSTR)IDD_SB_NEW_PROJECT, App->MainHwnd, (DLGPROC)Save_Project_Dialog_Proc);
}

// *************************************************************************
// *		Save_Project_Dialog_Proc:- Terry and Hazel Flanigan 2024  	   *
// *************************************************************************
LRESULT CALLBACK SB_Project_Create::Save_Project_Dialog_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_STPJFOLDERPATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STPROJECTNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STLEVELNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STPATH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STPN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STLN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BTCHANGE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTCHANGELEVEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CK_SP_DESKTOP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));


		SendDlgItemMessage(hDlg, IDC_BTPJBROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STBANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CKQUICKLOAD, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_STPROJECTNAME, (LPCTSTR)App->CLSB_Project->m_Project_Name);
		SetDlgItemText(hDlg, IDC_STLEVELNAME, (LPCTSTR)App->CLSB_Project->m_Level_Name);
		
		SetDlgItemText(hDlg, IDC_STBANNER, (LPCTSTR)"Save Project As");

		strcpy(App->CLSB_Project->m_Project_Sub_Folder, App->CLSB_FileIO->DeskTop_Folder);
		strcat(App->CLSB_Project->m_Project_Sub_Folder, "\\");
		strcat(App->CLSB_Project->m_Project_Sub_Folder, App->CLSB_Project->m_Project_Name);
		strcat(App->CLSB_Project->m_Project_Sub_Folder, "_Prj");
		SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CLSB_Project->m_Project_Sub_Folder);

		HWND temp = GetDlgItem(hDlg, IDC_CKQUICKLOAD);
		SendMessage(temp, BM_SETCHECK, 1, 0);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{

		if (GetDlgItem(hDlg, IDC_STPJFOLDERPATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_STPROJECTNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_STLEVELNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_STPN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STLN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 255));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STPATH) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STBANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CKQUICKLOAD) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_SP_DESKTOP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BTCHANGE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTCHANGELEVEL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTPJBROWSE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDOK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDCANCEL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_CK_SP_DESKTOP)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SP_DESKTOP);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				strcpy(App->CLSB_Project->m_Project_Sub_Folder, App->CLSB_FileIO->DeskTop_Folder);
				strcat(App->CLSB_Project->m_Project_Sub_Folder, "\\");
				strcat(App->CLSB_Project->m_Project_Sub_Folder, App->CLSB_Project->m_Project_Name);
				strcat(App->CLSB_Project->m_Project_Sub_Folder, "_Prj");

				SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CLSB_Project->m_Project_Sub_Folder);

				//App->CLSB_Project->Directory_Changed_Flag = 1;

				EnableWindow(GetDlgItem(hDlg, IDC_BTPJBROWSE), 0);
				EnableWindow(GetDlgItem(hDlg, IDC_STPJFOLDERPATH), 0);

			}
			else
			{
				EnableWindow(GetDlgItem(hDlg, IDC_BTPJBROWSE), 1);
				EnableWindow(GetDlgItem(hDlg, IDC_STPJFOLDERPATH), 1);
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTPJBROWSE)
		{
			strcpy(App->CLSB_FileIO->BrowserMessage, "Select Folder To Place New Project a sub folder will be created");
			int Test = App->CLSB_FileIO->StartBrowser("");

			if (Test == 0) { return true; }

			strcpy(App->CLSB_Project->m_Project_Sub_Folder, App->CLSB_FileIO->szSelectedDir);
			strcat(App->CLSB_Project->m_Project_Sub_Folder, App->CLSB_Project->m_Project_Name);
			strcat(App->CLSB_Project->m_Project_Sub_Folder, "_Prj");

			SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CLSB_Project->m_Project_Sub_Folder);

			//App->CLSB_Project->Directory_Changed_Flag = 1;

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTCHANGE)
		{
			strcpy(App->CLSB_Dialogs->btext, "Change Project Name");
			strcpy(App->CLSB_Dialogs->Chr_Text, App->CLSB_Project->m_Project_Name);

			App->CLSB_Dialogs->Dialog_Text(10);

			if (App->CLSB_Dialogs->Canceled == 1)
			{
				return TRUE;
			}

			int len1 = strlen(App->CLSB_Project->m_Project_Sub_Folder);
			int len2 = strlen(App->CLSB_Project->m_Project_Name);
			App->CLSB_Project->m_Project_Sub_Folder[len1 - (len2 + 5)] = 0;


			strcpy(App->CLSB_Project->m_Project_Name, App->CLSB_Dialogs->Chr_Text);

			strcpy(App->CLSB_Project->m_Project_Sub_Folder, App->CLSB_Project->m_Project_Sub_Folder);
			strcat(App->CLSB_Project->m_Project_Sub_Folder, "\\");
			strcat(App->CLSB_Project->m_Project_Sub_Folder, App->CLSB_Project->m_Project_Name);
			strcat(App->CLSB_Project->m_Project_Sub_Folder, "_Prj");

			SetDlgItemText(hDlg, IDC_STPROJECTNAME, (LPCTSTR)App->CLSB_Project->m_Project_Name);
			SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CLSB_Project->m_Project_Sub_Folder);

			//App->CLSB_Project->Directory_Changed_Flag = 1;

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTCHANGELEVEL)
		{
			strcpy(App->CLSB_Dialogs->btext, "Change Level Name");
			strcpy(App->CLSB_Dialogs->Chr_Text, App->CLSB_Project->m_Level_Name);

			App->CLSB_Dialogs->Dialog_Text(10);
			if (App->CLSB_Dialogs->Canceled == 1)
			{
				return TRUE;
			}

			strcpy(App->CLSB_Project->m_Level_Name, App->CLSB_Dialogs->Chr_Text);
			SetDlgItemText(hDlg, IDC_STLEVELNAME, (LPCTSTR)App->CLSB_Project->m_Level_Name);

			//App->CLSB_Project->Directory_Changed_Flag = 1;

			return TRUE;
		}

		/*if (LOWORD(wParam) == IDC_CKQUICKLOAD)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CKQUICKLOAD);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->SBC_Project->Set_QuickLoad_Flag = 1;
			}
			else
			{
				App->SBC_Project->Set_QuickLoad_Flag = 0;
			}

			return TRUE;
		}*/

		if (LOWORD(wParam) == IDCANCEL)
		{
			//App->SBC_Project->Directory_Changed_Flag = 0;

			App->CLSB_Project_Create->Dialog_cancelled_F = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{

			//App->SBC_Project->Save_Project();
			//App->SBC_Project->Project_Loaded = 1;

			App->CLSB_Project_Create->Dialog_cancelled_F = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}
	return FALSE;
}

// *************************************************************************
// *	  		Create_New_Project:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool SB_Project_Create::Create_New_Project()
{
	if (_mkdir(App->CLSB_Project->m_Project_Sub_Folder) == 0)
	{
		_chdir(App->CLSB_Project->m_Project_Sub_Folder);
	}
	else
	{
		_chdir(App->CLSB_Project->m_Project_Sub_Folder);
	}

	bool test = App->CLSB_Project->Save_Project_Ini();
	if (test == 0)
	{
		return 0;
	}

	App->CLSB_Project->Save_Level_Folder();
	App->CLSB_Project->Save_Main_Asset_Folder();

	_chdir(App->CLSB_Project->m_Level_Folder_Path);

	//if (App->CLSB_Scene_Data->Area_Added == 1)
	{
		App->CLSB_Project->Save_Aera_Folder();
	}

	if (App->CLSB_Scene_Data->Player_Added == 1)
	{
		App->CLSB_Project->Save_Players_Folder();
	}

	//	Save_Cameras_Folder();
	App->CLSB_Project->Save_Objects_Folder();
	//	Save_Display_Folder();

		/*App->CLSB_FileView->Change_Level_Name();
		App->CLSB_FileView->Change_Project_Name();*/

		//	App->Set_Main_TitleBar(App->SBC_FileIO->Project_Path_File_Name);

		//	App->CLSB_Object->Clear_Modified_Objects(); // Clear Altered FileView Items

		//	App->SBC_Project->Directory_Changed_Flag = 0;

		//	strcpy(App->SBC_FileIO->Project_Path_File_Name, m_Ini_Path_File_Name);
		//	App->Set_Main_TitleBar(App->SBC_FileIO->Project_Path_File_Name);
		//	App->SBC_FileIO->RecentFileHistory_Update();


			/*if (Set_QuickLoad_Flag == 1)
			{
				strcpy(App->CL_Prefs->QL_User_File, App->SBC_FileIO->Project_Path_File_Name);
				App->CL_Prefs->Prefs_QuickLoad_Default_f = 0;
				App->CL_Prefs->Write_Preferences();
			}*/


	App->Say("Scene Saved");

	return 1;
}

// *************************************************************************
// *	  		Create_New_Area:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool SB_Project_Create::Create_New_Area()
{
	if (App->CLSB_Doc->mModeTool == ID_TOOLS_TEMPLATE)
	{
		if (App->CL_Brush->Get_Brush_Count() == 0)
		{
			App->CLSB_Doc->AddBrushToWorld();
			App->m_pDoc->SetModifiedFlag();
			App->CLSB_TopTabs->Update_Dlg_Controls();
		}
		else
		{
			App->CLSB_Doc->AddBrushToWorld();
			App->m_pDoc->SetModifiedFlag();
		}
	}

	App->CLSB_Doc->DoGeneralSelect();

	App->CL_World->Reset_Editor();

	App->CLSB_Tabs_Templates_Dlg->Enable_Insert_Button(false);
	App->CLSB_Panels->Set_Aplication_Dialogs_On();

	App->File_Loaded_Flag = 1;

	if (App->BR_True3D_Mode_Active == 1)
	{
		App->CLSB_Mesh_Mgr->Update_World();
	}

	// ---------------------------------------------

	strcpy(App->CL_World->mCurrent_3DT_PathAndFile, App->CLSB_Project->m_Aera_Folder_Path);
	strcat(App->CL_World->mCurrent_3DT_PathAndFile, "\\");
	strcat(App->CL_World->mCurrent_3DT_PathAndFile, App->CLSB_Project->m_Level_Name);
	strcat(App->CL_World->mCurrent_3DT_PathAndFile, ".3dt");

	App->CLSB_File_WE->Save_Document();

	App->CLSB_Mesh_Mgr->WE_Build_Brush_List(0);
	App->CLSB_Bullet->Create_Brush_Trimesh_XX(0);
	App->CLSB_Mesh_Mgr->WE_Convert_All_Texture_Groups();
	App->CLSB_Ogre3D->Convert_ToOgre3D(1, 0);

	return 1;
}

