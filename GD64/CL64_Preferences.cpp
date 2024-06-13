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
#include "CL64_Preferences.h"

CL64_Preferences::CL64_Preferences(void)
{
	Start_FullScreen = 0;
	Start_Full_3DWin = 0;

	WriteData = nullptr;
}

CL64_Preferences::~CL64_Preferences(void)
{
}

// *************************************************************************
// *	  		Clean_Up:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Preferences::Clean_Up(void)
{
	
}

// *************************************************************************
// *	  	Start_Preferences_Dlg:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Preferences::Start_Preferences_Dlg()
{

	DialogBox(App->hInst, (LPCTSTR)IDD_PREFERENCES, App->MainHwnd, (DLGPROC)Preferences_Dlg_Proc);

}

// *************************************************************************
// *        Preferences_Dlg_Proc:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
LRESULT CALLBACK CL64_Preferences::Preferences_Dlg_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_ST_STARTUP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CK_SU_FULL3DVIEW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CK_SU_FULLSCREEN, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		
		// Start in Full Screen App
		if (App->CL_Preferences->Start_FullScreen == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_FULLSCREEN);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_FULLSCREEN);
			SendMessage(temp, BM_SETCHECK, 0, 0);
		}

		// Start in Full 3D Screen
		if (App->CL_Preferences->Start_Full_3DWin == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_FULL3DVIEW);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_FULL3DVIEW);
			SendMessage(temp, BM_SETCHECK, 0, 0);
		}


		return TRUE;

	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_CK_SU_FULLSCREEN) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CK_SU_FULL3DVIEW) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		if (GetDlgItem(hDlg, IDC_ST_STARTUP) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_CK_SU_FULLSCREEN)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_FULLSCREEN);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Preferences->Start_FullScreen = 1;
				return 1;
			}
			else
			{
				App->CL_Preferences->Start_FullScreen = 0;
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CK_SU_FULL3DVIEW)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CK_SU_FULL3DVIEW);

			int test = SendMessage(temp, BM_GETCHECK, 0, 0);
			if (test == BST_CHECKED)
			{
				App->CL_Preferences->Start_Full_3DWin = 1;
				return 1;
			}
			else
			{
				App->CL_Preferences->Start_Full_3DWin = 0;
				return 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CL_Preferences->Write_Preferences();
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}

	return FALSE;
}

// *************************************************************************
// *			Read_Preferences:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Preferences::Read_Preferences()
{
	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->GD_Directory_FullPath);
	strcat(Preferences_Path, "\\");
	strcat(Preferences_Path, "Data");
	strcat(Preferences_Path, "\\");
	strcat(Preferences_Path, "Preferences.ini");

	App->CL_Ini_File->SetPathName(Preferences_Path);

	Start_FullScreen = App->CL_Ini_File->GetInt("Startup", "Full_Screen", 0, 10);
	Start_Full_3DWin = App->CL_Ini_File->GetInt("Startup", "Full_3DWin", 0, 10);

}

// *************************************************************************
// *		Write_Preferences:- Terry and Hazel Flanigan 2024 			   *
// *************************************************************************
bool CL64_Preferences::Write_Preferences()
{
	WriteData = nullptr;

	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->GD_Directory_FullPath);
	strcat(Preferences_Path, "\\");
	strcat(Preferences_Path, "Data");
	strcat(Preferences_Path, "\\");
	strcat(Preferences_Path, "Preferences.ini");

	//App->Say(Preferences_Path);

	WriteData = fopen(Preferences_Path, "wt");
	if (!WriteData)
	{
		App->Say("Cant Open Preference File");
		return 0;
	}

	fprintf(WriteData, "%s\n", "[Startup]");
	fprintf(WriteData, "%s%i\n", "Full_Screen=", Start_FullScreen);
	fprintf(WriteData, "%s%i\n", "Full_3DWin=", Start_Full_3DWin);

	fclose(WriteData);

	return 1;
}
