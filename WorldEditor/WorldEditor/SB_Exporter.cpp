/*
Copyright (c) World_Viewer 2023 Inflanite Software W.T.Flanigan H.C.Flanigan

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

#include "StdAfx.h"
#include "AB_App.h"
#include "resource.h"
#include "SB_Exporter.h"

SB_Exporter::SB_Exporter(void)
{
	Is_Canceled = 0;
	Export_Selected = 0;

	Selected_Index = 0;

	mJustName[0] = 0;
	mDirectory_Name[0] = 0;
	mFolder_Path[0] = 0;

	Export_Dlg_Hwnd = NULL;
}

SB_Exporter::~SB_Exporter(void)
{
}

// *************************************************************************
// *	  		Start_Export_Dlg:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_Exporter::Start_Export_Dlg()
{
	Is_Canceled = 0;

	DialogBox(App->hInst, (LPCTSTR)IDD_SB_EXPORTOPTIONS, App->MainHwnd, (DLGPROC)Export_Dlg_Proc);
}
// *************************************************************************
// *        Export_Dlg_Proc:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
LRESULT CALLBACK SB_Exporter::Export_Dlg_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{

		SendDlgItemMessage(hDlg, IDC_ST_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_FOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_BROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_CHANGE_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_SUBFOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_SUBFOLDER_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_FOLDER_NAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_FLD, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_FN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_LST_FILEFORMATS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CK_EXPORTALL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_EXPORTSELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_ST_SELECTEDFORMAT, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CLSB_Exporter->Set_Dialog_Data(hDlg);

		App->CLSB_Exporter->Selected_Index = 0;
		App->CLSB_Exporter->Set_Dialog_Data_FromIndex(hDlg);
		
		HWND Temp = GetDlgItem(hDlg, IDC_CK_SUBFOLDER);
		SendMessage(Temp, BM_SETCHECK, BST_CHECKED, 0);
		
		App->CLSB_Exporter->List_File_Formats(hDlg);

		Temp = GetDlgItem(hDlg, IDC_CK_EXPORTALL);
		SendMessage(Temp, BM_SETCHECK, 1, 0);
		App->CLSB_Exporter->Export_Selected = 0;

		App->CLSB_Exporter->Export_Dlg_Hwnd = hDlg;
		
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_SELECTEDFORMAT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(200, 102, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_FOLDER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_ST_SUBFOLDER_NAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		if (GetDlgItem(hDlg, IDC_CK_SUBFOLDER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_FLD) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_FN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_EXPORTALL) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_EXPORTSELECTED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
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


		/*if (some_item->idFrom == IDC_BT_FOLDER_NAME && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		if (some_item->idFrom == IDC_BT_CHANGE_NAME && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_BROWSE && some_item->code == NM_CUSTOMDRAW)
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
	{
		if (LOWORD(wParam) == IDC_CK_EXPORTALL)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_EXPORTALL);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CK_EXPORTSELECTED);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			App->CLSB_Exporter->Export_Selected = 0;

			App->CLSB_Exporter->Set_Dialog_Data_FromIndex(hDlg);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_EXPORTSELECTED)
		{
			int NumSelBrushes = SelBrushList_GetSize(App->CLSB_Doc->pSelBrushes);

			if (NumSelBrushes == 0)
			{
				HWND temp = GetDlgItem(hDlg, IDC_CK_EXPORTSELECTED);
				SendMessage(temp, BM_SETCHECK, 0, 0);
				App->CLSB_Exporter->Export_Selected = 0;

				App->Say("No Brushes Selected");
				return TRUE;
			}

			HWND temp = GetDlgItem(hDlg, IDC_CK_EXPORTSELECTED);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CK_EXPORTALL);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			App->CLSB_Exporter->Export_Selected = 1;

			App->CLSB_Exporter->Set_Dialog_Data_FromIndex(hDlg);
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_SUBFOLDER)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SUBFOLDER);
			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				EnableWindow(GetDlgItem(hDlg, IDC_ST_SUBFOLDER_NAME), 1);
				EnableWindow(GetDlgItem(hDlg, IDC_BT_FOLDER_NAME), 1);
				return 1;
			}
			else
			{
				EnableWindow(GetDlgItem(hDlg, IDC_ST_SUBFOLDER_NAME), 0);
				EnableWindow(GetDlgItem(hDlg, IDC_BT_FOLDER_NAME), 0);
				return 1;
			}

			return 1;
		}

		if (LOWORD(wParam) == IDC_BT_CHANGE_NAME)
		{
			strcpy(App->CLSB_Dialogs->btext, "Change File Name");
			strcpy(App->CLSB_Dialogs->Chr_Text, App->CLSB_Exporter->mJustName);

			App->CLSB_Dialogs->Dialog_Text(Enums::Check_Name_None);

			if (App->CLSB_Dialogs->Canceled == 0)
			{
				strcpy(App->CLSB_Exporter->mJustName, App->CLSB_Dialogs->Chr_Text);
			}

			SetDlgItemText(hDlg, IDC_ST_NAME, App->CLSB_Exporter->mJustName);

			App->CLSB_Exporter->Set_Dialog_Data_FromIndex(hDlg);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_FOLDER_NAME)
		{
			/*strcpy(App->CL_Dialogs->btext, "Change Folder Name");
			strcpy(App->CL_Dialogs->Chr_Text, App->CL_Export_Ogre3D->Directory_Name);

			App->CL_Dialogs->Dialog_Text();

			if (App->CL_Dialogs->Is_Canceled == 0)
			{
				strcpy(App->CLSB_Export_Ogre3D->Directory_Name, App->CL_Dialogs->Chr_Text);
			}

			SetDlgItemText(hDlg, IDC_ST_SUBFOLDER_NAME, App->CL_Export_Ogre3D->Directory_Name);*/

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_BROWSE)
		{
			strcpy(App->CLSB_FileIO->BrowserMessage, "Select Folder To Place Ogre Files");
			int Test = App->CLSB_FileIO->StartBrowser("");
			if (Test == 0) { return 1; }

			SetDlgItemText(hDlg, IDC_ST_FOLDER, App->CLSB_FileIO->szSelectedDir);

			strcpy(App->CLSB_Exporter->mFolder_Path, App->CLSB_FileIO->szSelectedDir);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_LST_FILEFORMATS)
		{
			char buff[256];
			int Index = 0;
			Index = SendDlgItemMessage(hDlg, IDC_LST_FILEFORMATS, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (Index == -1)
			{
				return 1;
			}

			App->CLSB_Exporter->Selected_Index = Index;
			App->CLSB_Exporter->Set_Dialog_Data_FromIndex(hDlg);
			

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CLSB_Exporter->Is_Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CLSB_Exporter->Is_Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	}

	return FALSE;
}

// *************************************************************************
// *			Start_Export:- Terry and Hazel Flanigan 2023 			   *
// *************************************************************************
bool SB_Exporter::Start_Export(bool Use_FileDialog)
{
	App->Enable_Dialogs(0);

	if (Use_FileDialog == 1)
	{
		strcpy(App->CLSB_FileIO->BrowserMessage, "Select Folder To Place Object Files a sub folder will be created");
		int Test = App->CLSB_FileIO->StartBrowser("");

		if (Test == 0)
		{
			App->Enable_Dialogs(1);
			return 1;
		}
	}

	App->CLSB_Exporter->Start_Export_Dlg();

	if (Is_Canceled == 1)
	{
		App->Enable_Dialogs(1);
		return 1;
	}

	App->CLSB_Exporter->Do_Export();

	if (Is_Canceled == 2)
	{
		Start_Export(0);
		return 1;
	}

	App->Enable_Dialogs(1);

	return 1;
}

// *************************************************************************
// *			Do_Export:- Terry and Hazel Flanigan 2023 				   *
// *************************************************************************
bool SB_Exporter::Do_Export()
{
	// ----------------- Ogre3d
	if (App->CLSB_Exporter->Selected_Index == 0)
	{
		bool if_Canceled = App->CLSB_Exporter->Check_File(".mesh");
		if (if_Canceled == 1)
		{
			Is_Canceled = 2;
			return true;
		}

		App->CLSB_PB->Start_ProgressBar();
		App->CLSB_PB->Set_Progress("Starting", 4);
		
		App->CLSB_Exporter->Ogre3D_Model();

		App->CLSB_PB->Stop_Progress_Bar("Ogre3D Mesh file Created successfully");
	}

	// ----------------- Wavefront
	if (App->CLSB_Exporter->Selected_Index == 1)
	{
		bool Test = App->CLSB_Exporter->Check_File(".obj");
		if (Test == 1)
		{
			return true;
		}

		App->CLSB_PB->Start_ProgressBar();
		App->CLSB_PB->Set_Progress("Starting", 4);
		
		App->CLSB_Exporter->Object_Model();

		App->CLSB_PB->Stop_Progress_Bar("Wavefront Object file Created successfully");
	}

	// ----------------- 3ds
	if (App->CLSB_Exporter->Selected_Index == 2)
	{
		bool if_Canceled = App->CLSB_Exporter->Check_File(".3ds");
		if (if_Canceled == 1)
		{
			Is_Canceled = 2;
			return true;
		}

		App->CLSB_PB->Start_ProgressBar();
		App->CLSB_PB->Set_Progress("Starting", 2);

		App->CLSB_Exporter->Autodesk_Model();

		App->CLSB_PB->Stop_Progress_Bar("Autodesk file Created successfully");
	}

	// ----------------- Milkshape
	if (App->CLSB_Exporter->Selected_Index == 3)
	{
		bool Test = App->CLSB_Exporter->Check_File(".ms3d");
		if (Test == 1)
		{
			return true;
		}

		App->CLSB_PB->Start_ProgressBar();
		App->CLSB_PB->Set_Progress("Starting", 4);
		
		App->CLSB_Exporter->Milkshape_Model();

		App->CLSB_PB->Stop_Progress_Bar("Milkshape file Created successfully");
	}

	return 1;
}

// *************************************************************************
// *			Check_File:- Terry and Hazel Flanigan 2023 				   *
// *************************************************************************
bool SB_Exporter::Check_File(char* Extension)
{
	char checkfile[MAX_PATH];
	strcpy(checkfile, App->CLSB_Exporter->mFolder_Path);
	strcat(checkfile, "\\");
	strcat(checkfile, App->CLSB_Exporter->mDirectory_Name);
	strcat(checkfile, "\\");
	strcat(checkfile, App->CLSB_Exporter->mJustName);

	strcat(checkfile, Extension);

	bool test = App->CLSB_FileIO->Check_File_Exist(checkfile);

	if (test == 1)
	{
		App->CLSB_Dialogs->YesNo("File Exsits", "Do you want to update File");

		bool Doit = App->CLSB_Dialogs->Canceled;
		if (Doit == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

// *************************************************************************
// *		Set_Dialog_Data_FromIndex:- Terry and Hazel Flanigan 2023 	   *
// *************************************************************************
void SB_Exporter::Set_Dialog_Data_FromIndex(HWND m_hDlg)
{

	if (Selected_Index == 0)
	{
		strcpy(App->CLSB_Exporter->mDirectory_Name, App->CLSB_Exporter->mJustName);

		if (App->CLSB_Exporter->Export_Selected == 0)
		{
			strcat(App->CLSB_Exporter->mDirectory_Name, "_Ogre_All");
		}
		else
		{
			strcat(App->CLSB_Exporter->mDirectory_Name, "_Ogre_Sel");
		}

		SetDlgItemText(m_hDlg, IDC_ST_SELECTEDFORMAT, "Ogre3D...  ( .mesh )");
	}

	if (Selected_Index == 1)
	{
		strcpy(App->CLSB_Exporter->mDirectory_Name, App->CLSB_Exporter->mJustName);

		if (App->CLSB_Exporter->Export_Selected == 0)
		{
			strcat(App->CLSB_Exporter->mDirectory_Name, "_Wavefront_All");
		}
		else
		{
			strcat(App->CLSB_Exporter->mDirectory_Name, "_Wavefront_Sel");
		}

		SetDlgItemText(m_hDlg, IDC_ST_SELECTEDFORMAT, "Wavefront Object...  ( .obj )");
	}

	if (Selected_Index == 2)
	{
		strcpy(App->CLSB_Exporter->mDirectory_Name, App->CLSB_Exporter->mJustName);

		if (App->CLSB_Exporter->Export_Selected == 0)
		{
			strcat(App->CLSB_Exporter->mDirectory_Name, "_Autodesk_All");
		}
		else
		{
			strcat(App->CLSB_Exporter->mDirectory_Name, "_Autodesk_Sel");
		}

		SetDlgItemText(m_hDlg, IDC_ST_SELECTEDFORMAT, "Autodesk...  ( .3ds )");
	}

	if (Selected_Index == 3)
	{
		strcpy(App->CLSB_Exporter->mDirectory_Name, App->CLSB_Exporter->mJustName);

		if (App->CLSB_Exporter->Export_Selected == 0)
		{
			strcat(App->CLSB_Exporter->mDirectory_Name, "_Milkshape_All");
		}
		else
		{
			strcat(App->CLSB_Exporter->mDirectory_Name, "_Milkshape_Sel");
		}

		SetDlgItemText(m_hDlg, IDC_ST_SELECTEDFORMAT, "Milkshape..  ( .ms3d )");
	}

	SetDlgItemText(m_hDlg, IDC_ST_SUBFOLDER_NAME, App->CLSB_Exporter->mDirectory_Name);
}

// *************************************************************************
// *			Set_Dialog_Data:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
void SB_Exporter::Set_Dialog_Data(HWND m_hDlg)
{
	// Just Name
	char buf[MAX_PATH];
	strcpy(buf, App->CL_World->mCurrent_3DT_File);
	int Len = strlen(buf);
	buf[Len - 4] = 0;
	strcpy(mJustName, buf);
	SetDlgItemText(m_hDlg, IDC_ST_NAME, buf);

	// Folder Path
	strcpy(mFolder_Path, App->CLSB_FileIO->szSelectedDir);
	SetDlgItemText(m_hDlg, IDC_ST_FOLDER, App->CLSB_FileIO->szSelectedDir);

	// Directory Name
	strcpy(mDirectory_Name, App->CLSB_Exporter->mJustName);
	strcat(mDirectory_Name, "_Ogre_All");
	SetDlgItemText(m_hDlg, IDC_ST_SUBFOLDER_NAME, App->CLSB_Exporter->mDirectory_Name);

	SetDlgItemText(m_hDlg, IDC_ST_SELECTEDFORMAT, "Ogre3D...  ( .mesh )");
}

// *************************************************************************
// *			List_File_Formats:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
void SB_Exporter::List_File_Formats(HWND m_hDlg)
{
	char buf[100];

	sprintf(buf,"%s", "Ogre3D...  ( .mesh )");
	SendDlgItemMessage(m_hDlg, IDC_LST_FILEFORMATS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s", "Wavefront Object...  ( .obj )");
	SendDlgItemMessage(m_hDlg, IDC_LST_FILEFORMATS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s", "Autodesk...  ( .3ds )");
	SendDlgItemMessage(m_hDlg, IDC_LST_FILEFORMATS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s", "Milkshape...  ( .ms3d )");
	SendDlgItemMessage(m_hDlg, IDC_LST_FILEFORMATS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	SendDlgItemMessage(m_hDlg, IDC_LST_FILEFORMATS, LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
 
 
// *************************************************************************
// *			Ogre3D_Model:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Exporter::Ogre3D_Model(void)
{
	App->CLSB_PB->Nudge("Build_Brush_List");
	App->CLSB_Mesh_Mgr->WE_Build_Brush_List(Export_Selected);

	App->CLSB_PB->Nudge("Converting to Groups");
	App->CLSB_Mesh_Mgr->WE_Convert_All_Texture_Groups();

	App->CLSB_PB->Nudge("Exporting Ogre3d");
	App->CLSB_Ogre3D->Export_To_Ogre3D(1);
	
	App->CLSB_PB->Nudge("Finished");
}

// *************************************************************************
// *			Object_Model:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Exporter::Object_Model(void)
{
	App->CLSB_PB->Nudge("Build_Brush_List");
	App->CLSB_Mesh_Mgr->WE_Build_Brush_List(Export_Selected);

	App->CLSB_PB->Nudge("Converting to Groups");
	App->CLSB_Mesh_Mgr->WE_Convert_All_Texture_Groups();

	App->CLSB_PB->Nudge("Exporting Wavefront");
	App->CLSB_Export_Object->Create_ObjectFile();

	App->CLSB_PB->Nudge("Finished");
}

// *************************************************************************
// *			3DS_Model:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Exporter::Autodesk_Model(void)
{
	App->CLSB_PB->Nudge("Exporting Autodesk 3ds");
	App->CLSB_Export_3DS->Export_World_GD3D();

	App->CLSB_PB->Nudge("Finished");
}

// *************************************************************************
// *			Milkshape_Model:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Exporter::Milkshape_Model(void)
{
	App->CLSB_PB->Nudge("Build_Brush_List");
	App->CLSB_Mesh_Mgr->WE_Build_Brush_List(Export_Selected);

	App->CLSB_PB->Nudge("Converting to Groups");
	App->CLSB_Mesh_Mgr->WE_Convert_All_Texture_Groups();

	App->CLSB_PB->Nudge("Exporting Milkshape");
	App->CLSB_Export_Milkshape->Export_To_Milk();

	App->CLSB_PB->Nudge("Finished");
}
