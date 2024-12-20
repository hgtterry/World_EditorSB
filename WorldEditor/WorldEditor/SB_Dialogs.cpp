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
#include "SB_Dialogs.h"

SB_Dialogs::SB_Dialogs(void)
{
	MessageString[0] = 0;
	MessageString2[0] = 0;

	F_ListData_Dlg_Active = 0;
	Select_Face_Dlg_Active = 0;

	Mouse_Normal = 1;
	Mouse_Slow = 0;
	Mouse_VerySlow = 0;
	Mouse_Fast = 0;

	Canceled = 0;
	TrueFlase = 0;
	What_Check_Name = Enums::Check_Name_None;

	YesNoCancel_Result = 0;

	mWhatList = 0;

	btext[0] = 0;
	Chr_Text[0] = 0;

	Chr_DropText[0] = 0;
	DropList_Data = 0;
	Selected_Face_Index = 1;
}

SB_Dialogs::~SB_Dialogs(void)
{
}

// *************************************************************************
// *	  			Dialog_Text:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
bool SB_Dialogs::Dialog_Text(int What_Check)
{
	Canceled = 0;
	What_Check_Name = What_Check;

	if (App->CLSB_Equity->EquitySB_Dialog_Visible == 0)
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_SB_TEXT_DIALOG, App->MainHwnd, (DLGPROC)Dialog_Text_Proc);
	}
	else
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_SB_TEXT_DIALOG, App->Equity_Dlg_hWnd, (DLGPROC)Dialog_Text_Proc);
	}

	return 1;
}

// **************************************************************************
// *				Dialog_Text_Proc:- Terry and Hazel Flanigan 2022		*
// **************************************************************************
LRESULT CALLBACK SB_Dialogs::Dialog_Text_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{

		//App->SetTitleBar(hDlg);

		HFONT Font;
		Font = CreateFont(-20, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
		SendDlgItemMessage(hDlg, IDC_TITLENAME, WM_SETFONT, (WPARAM)Font, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_EDITTEXT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_TITLENAME, (LPCTSTR)App->CLSB_Dialogs->btext);

		SetDlgItemText(hDlg, IDC_EDITTEXT, (LPCTSTR)App->CLSB_Dialogs->Chr_Text);


		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_TITLENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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
		if (LOWORD(wParam) == IDOK)
		{
			char buff[255];
			GetDlgItemText(hDlg, IDC_EDITTEXT, (LPTSTR)buff, 255);

			strcpy(App->CLSB_Dialogs->Chr_Text, buff);

			if (App->CLSB_Dialogs->What_Check_Name == Enums::Check_Name_Brushes)
			{
				bool test = App->CL_Brush->Check_if_Name_Exist(buff);
				if (test == 1)
				{
					App->Say("Brush Name Exist");
					return TRUE;
				}
			}

			if (App->CLSB_Dialogs->What_Check_Name == Enums::Check_Name_Brushes)
			{
				/*bool test = App->CL_Brush->Check_if_Name_Exist(buff);
				if (test == 1)
				{
					App->Say("Brush Name Exist");
					return TRUE;
				}*/
			}

			App->CLSB_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CLSB_Dialogs->Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *	  			YesNo:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Dialogs::YesNo(char* Text, char* Text2)
{
	Canceled = 0;
	MessageString[0] = 0;
	MessageString2[0] = 0;

	strcpy(MessageString, Text);
	strcpy(MessageString2, Text2);

	if (App->CLSB_Equity->EquitySB_Dialog_Visible == 0)
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_SB_YESNO, App->MainHwnd, (DLGPROC)YesNo_Proc);
	}
	else
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_SB_YESNO, App->Equity_Dlg_hWnd, (DLGPROC)YesNo_Proc);
	}
}

// *************************************************************************
// *		  YesNo_Proc_Proc:- Terry and Hazel Flanigan 2023	  		   *
// *************************************************************************
LRESULT CALLBACK SB_Dialogs::YesNo_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//App->SetTitleBar(hDlg);

		SendDlgItemMessage(hDlg, IDC_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTEXT, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_BANNER, App->CLSB_Dialogs->MessageString);
		SetDlgItemText(hDlg, IDC_STTEXT, App->CLSB_Dialogs->MessageString2);
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		if (GetDlgItem(hDlg, IDC_STTEXT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		return FALSE;
	}
	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

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

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == IDOK)
		{
			App->CLSB_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CLSB_Dialogs->Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}
	return FALSE;
}

// *************************************************************************
// *	  		YesNoCancel:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Dialogs::YesNoCancel(char* Text, char* Text2)
{
	App->CLSB_Dialogs->YesNoCancel_Result = 0;

	MessageString[0] = 0;
	MessageString2[0] = 0;

	strcpy(MessageString, Text);
	strcpy(MessageString2, Text2);

	DialogBox(App->hInst, (LPCTSTR)IDD_SB_YESNOCANCEL, App->MainHwnd, (DLGPROC)YesNoCancel_Proc);
}

// *************************************************************************
// *		  YesNoCancel_Proc:- Terry and Hazel Flanigan 2023	  		   *
// *************************************************************************
LRESULT CALLBACK SB_Dialogs::YesNoCancel_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_NONO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_BANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STTEXT, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_BANNER, App->CLSB_Dialogs->MessageString);
		SetDlgItemText(hDlg, IDC_STTEXT, App->CLSB_Dialogs->MessageString2);
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_BANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		if (GetDlgItem(hDlg, IDC_STTEXT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		return FALSE;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

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

		if (some_item->idFrom == IDC_NONO && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == IDOK) // Yes
		{
			App->CLSB_Dialogs->YesNoCancel_Result = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_NONO) // No
		{
			App->CLSB_Dialogs->YesNoCancel_Result = 2;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	
		if (LOWORD(wParam) == IDCANCEL) // Cancel
		{
			App->CLSB_Dialogs->YesNoCancel_Result = 3;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}
	return FALSE;
}

// *************************************************************************
// *	  		Start_ListData:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Dialogs::Start_ListData(int WhatList)
{
	mWhatList = WhatList;

	if (F_ListData_Dlg_Active == 1)
	{
		return 1;
	}

	App->Get_Current_Document();

	if (App->CLSB_Equity->EquitySB_Dialog_Visible == 0)
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_SCENEDATA, App->MainHwnd, (DLGPROC)ListData_Proc);
	}
	else
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_SCENEDATA, App->Equity_Dlg_hWnd, (DLGPROC)ListData_Proc);
	}

	return 1;
}
// *************************************************************************
// *			ListData_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_Dialogs::ListData_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_LISTGROUP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		if (App->CLSB_Dialogs->mWhatList == 0)
		{
			SetWindowText(hDlg, "World Information");
			App->CLSB_Dialogs->List_SceneData(hDlg);
		}

		if (App->CLSB_Dialogs->mWhatList == 1)
		{
			App->CLSB_Dialogs->Read_ErrorLog(hDlg);
		}

		if (App->CLSB_Dialogs->mWhatList == 2)
		{
			App->CLSB_Dialogs->ListGroups(hDlg);
		}

		App->CLSB_Dialogs->F_ListData_Dlg_Active = 1;

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

		if (some_item->idFrom == IDOK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			App->CLSB_Dialogs->F_ListData_Dlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CLSB_Dialogs->F_ListData_Dlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	return FALSE;
}

// *************************************************************************
// *	  	List_SceneData:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Dialogs::List_SceneData(HWND hDlg)
{


	char buf[MAX_PATH];

	sprintf(buf, "%s", " -------------------------------- World --------------------------------");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s", "    ");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %i", "File Loaded", App->File_Loaded_Flag);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %i", "Brush Count",App->CL_Brush->Get_Brush_Count());
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s", "    ");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	
	sprintf(buf, "%s", " -------------------------------- 3DT File --------------------------------");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s", "    ");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%s", "Title = ", App->CL_World->mCurrent_Title);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%s", "File = ", App->CL_World->mCurrent_3DT_File);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%s", "Path = ", App->CL_World->mCurrent_3DT_Path);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%s", "Path and File = ", App->CL_World->mCurrent_3DT_PathAndFile);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s", "    ");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	// ----------------------------------------- TXL_FilePath
	sprintf(buf, "%s", " -------------------------------- TXL File --------------------------------");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s", "    ");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "File = %s", App->CL_World->mCurrent_TXL_FileName);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "Path and File = %s", App->CL_World->mCurrent_TXL_FilePath);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);


	sprintf(buf, "%s%i", "Brush Count = ", App->CL_World->Get_Brush_Count());
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	int Stack_Memory = App->Get_Stack();
	sprintf(buf, "%s%i", "Stack Bytes= ", Stack_Memory);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	float megs = ((float)Stack_Memory / 1024) / 1024;
	sprintf(buf, "%s%f", "Stack megs= ", megs);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	float used = ((float)(2097152 - Stack_Memory) / 1024);
	sprintf(buf, "%s%f", "Stack Used K= ", used);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	if (App->m_pDoc && (App->m_pDoc->IsModified() == TRUE))
	{
		sprintf(buf, "%s", "File Modified Yes");
		SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else
	{
		sprintf(buf, "%s", "File Modified No");
		SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
}

// *************************************************************************
// *	  	List_BoundingBox:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Dialogs::List_BoundingBox(HWND hDlg)
{

	char buf[255];

	sprintf(buf, "%s", "3DT File--------------------------------");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%s", "File = ", App->CL_World->mCurrent_3DT_File);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%s", "Path = ", App->CL_World->mCurrent_3DT_Path);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%s", "Path and File = ", App->CL_World->mCurrent_3DT_PathAndFile);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	//--------------------------------
	sprintf(buf, "%s", "    ");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s", "    ");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	// ----------------------------------------- TXL_FilePath
	sprintf(buf, "%s", "TXL File --------------------------------");
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "File = %s", App->CL_World->mCurrent_TXL_FileName);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "Path and File = %s", App->CL_World->mCurrent_TXL_FilePath);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);


	sprintf(buf, "%s%i", "Brush Count = ", App->CL_World->Get_Brush_Count());
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	int Stack_Memory = App->Get_Stack();
	sprintf(buf, "%s%i", "Stack Bytes= ", Stack_Memory);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	float megs = ((float)Stack_Memory / 1024) / 1024;
	sprintf(buf, "%s%f", "Stack megs= ", megs);
	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	float used = ((float)(2097152 - Stack_Memory) / 1024);
	sprintf(buf, "%s%f", "Stack Used K= ", used);

	SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

}

// *************************************************************************
// *		 	Read_ErrorLog:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Dialogs::Read_ErrorLog(HWND hDlg)
{
	if (App->Debug_App == 1)
	{
		FILE* fp = NULL;

		char buffer[MAX_PATH];

		char Path[MAX_PATH];
		strcpy(Path, App->WorldEditor_Directory);
		strcat(Path, "Error.log");

		fp = fopen(Path, "r");
		if (!fp)
		{
			App->Say("Cant Find File", Path);
			return;
		}

		while (!feof(fp))
		{
			buffer[0] = 0;
			fgets(buffer, MAX_PATH, fp);
			SendDlgItemMessage(hDlg, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buffer);
		}

		fclose(fp);

		unlink(Path);
	}
}

// *************************************************************************
// *	  	Start_Speed_Camera:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Dialogs::Start_Speed_Camera()
{
	DialogBox(App->hInst, (LPCTSTR)IDD_MOUSESENSITIVITY, App->Equity_Dlg_hWnd, (DLGPROC)Speed_Camera_Proc);
}

// *************************************************************************
// *		Speed_Camera_Proc:- Terry and Hazel Flanigan 2023	  		   *
// *************************************************************************
LRESULT CALLBACK SB_Dialogs::Speed_Camera_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_STBANNER, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_CKNORMAL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKSLOW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKVERYSLOW, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKFAST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STBANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

		if (some_item->idFrom == IDC_CKNORMAL && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Dialogs->Mouse_Normal);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CKSLOW && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Dialogs->Mouse_Slow);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CKVERYSLOW && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Dialogs->Mouse_VerySlow);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CKFAST && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Dialogs->Mouse_Fast);
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

		if (LOWORD(wParam) == IDC_CKNORMAL)
		{

			App->CLSB_Ogre_Setup->OgreListener->mMoveSensitivity = 50;
			App->CLSB_Ogre_Setup->OgreListener->mMoveSensitivityMouse = 50;

			App->CLSB_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CLSB_Dialogs->Mouse_Normal = 1;

			InvalidateRect(hDlg, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKSLOW)
		{

			App->CLSB_Ogre_Setup->OgreListener->mMoveSensitivity = 5;
			App->CLSB_Ogre_Setup->OgreListener->mMoveSensitivityMouse = 5;

			App->CLSB_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CLSB_Dialogs->Mouse_Slow = 1;

			InvalidateRect(hDlg, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKVERYSLOW)
		{

			App->CLSB_Ogre_Setup->OgreListener->mMoveSensitivity = 1;
			App->CLSB_Ogre_Setup->OgreListener->mMoveSensitivityMouse = 1;


			App->CLSB_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CLSB_Dialogs->Mouse_VerySlow = 1;

			InvalidateRect(hDlg, NULL, false);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKFAST)
		{

			App->CLSB_Ogre_Setup->OgreListener->mMoveSensitivity = 250;
			App->CLSB_Ogre_Setup->OgreListener->mMoveSensitivityMouse = 250;

			App->CLSB_Dialogs->UnCheck_All_SpeedMouseOption();
			App->CLSB_Dialogs->Mouse_Fast = 1;

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
	return FALSE;
}

// *************************************************************************
// *	UnCheck_All_SpeedMouseOption:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void SB_Dialogs::UnCheck_All_SpeedMouseOption()
{
	Mouse_Normal = 0;
	Mouse_Slow = 0;
	Mouse_VerySlow = 0;
	Mouse_Fast = 0;
}

// *************************************************************************
// *	  	Start_Dialog_DropGen:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
bool SB_Dialogs::Start_Dialog_DropGen()
{
	CreateDialog(App->hInst, (LPCTSTR)IDD_EQ_DROPGEN, App->Equity_Dlg_hWnd, (DLGPROC)Dialog_DropGen_Proc);

	return 1;
}
// *************************************************************************
// *		Dialog_DropGen_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_Dialogs::Dialog_DropGen_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{

		//App->SetTitleBar(hDlg);

		SendDlgItemMessage(hDlg, IDC_TITLE, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_EQ_UPDATE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_LISTSELECTION, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STSELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_TITLE, (LPCTSTR)App->CLSB_Dialogs->btext);
		SetDlgItemText(hDlg, IDC_STSELECTED, (LPCTSTR)App->CLSB_Dialogs->Chr_DropText);

		HWND tempList = GetDlgItem(hDlg, IDC_LISTSELECTION);

		if (App->CLSB_Dialogs->DropList_Data == 0)
		{

			//App->CLSB_Ogre->RenderListener->GroupNumber = 0;
			//App->CLSB_Ogre->RenderListener->ShowBoundingGroup = 1;
			
			App->CLSB_Dialogs->ListGroups(tempList);

			//HWND tempList2 = GetDlgItem(hDlg, IDC_LISTGROUPDETAIL);
			//App->CLSB_Dialogs->UpdateGroupDetails(tempList2);

			return TRUE;
		}

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{

		if (GetDlgItem(hDlg, IDC_TITLE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STSELECTED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
		}

		return FALSE;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

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

		if (some_item->idFrom == IDC_BT_EQ_UPDATE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

	
		return CDRF_DODEFAULT;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}
	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_LISTSELECTION)
		{
			char buff[256];
			int Index = 0;
			Index = SendDlgItemMessage(hDlg, IDC_LISTSELECTION, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (Index == -1)
			{
				return 1;
			}

			SendDlgItemMessage(hDlg, IDC_LISTSELECTION, LB_GETTEXT, (WPARAM)Index, (LPARAM)buff);

			SetDlgItemText(hDlg, IDC_STSELECTED, buff);

			if (App->CLSB_Dialogs->DropList_Data == 0)
			{
				App->CLSB_Ogre_Setup->RenderListener->GroupNumber = Index;
			}

			HWND tempList2 = GetDlgItem(hDlg, IDC_LISTGROUPDETAIL);
			App->CLSB_Dialogs->UpdateGroupDetails(tempList2);

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_EQ_UPDATE)
		{
			App->CLSB_Model->Set_BondingBoxes_AllGroups();

			HWND tempList2 = GetDlgItem(hDlg, IDC_LISTGROUPDETAIL);
			App->CLSB_Dialogs->UpdateGroupDetails(tempList2);

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDOK)
		{
			char buff[256];
			GetDlgItemText(hDlg, IDC_STSELECTED, (LPTSTR)buff, 256);
			strcpy(App->CLSB_Dialogs->Chr_DropText, buff);

			App->CLSB_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CLSB_Dialogs->Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	return FALSE;
}

// *************************************************************************
// *			ListGroups:- Terry and Hazel Flanigan 2023		 		   *
// *************************************************************************
void SB_Dialogs::ListGroups(HWND List)
{
	char buf[MAX_PATH];
	//int BrushCount = App->CLSB_Model->BrushCount;

	//sprintf(buf, "Brush Count = %i", BrushCount);
	//SendDlgItemMessage(List, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	int Count = 0;
	/*while (Count < BrushCount)
	{
		sprintf(buf, "%i %s %i %i", Count, App->CLSB_Model->B_Brush[Count]->BrushName, App->CLSB_Model->B_Brush[Count]->Brush_Count, App->CLSB_Model->B_Brush[Count]->SubBrush_Count);
		SendDlgItemMessage(List, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		Count++;
	}*/

	sprintf(buf, " -----------------------");
	SendDlgItemMessage(List, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	
	sprintf(buf,"%s %i","Brush Count = ", App->CLSB_Model->BrushCount);
	SendDlgItemMessage(List, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	
	SendDlgItemMessage(List, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)" ");

	Count = 0;
	while (Count < App->CLSB_Model->BrushCount)
	{
		sprintf(buf, "Faces %i Vertices %i", App->CLSB_Model->B_Brush[Count]->Face_Count,App->CLSB_Model->B_Brush[Count]->Vertice_Count);
		SendDlgItemMessage(List, IDC_LISTGROUP, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		/*int loop = 0;
		while (loop < App->CLSB_Model->B_XBrush[Count]->Brush_Count)
		{

			loop++;
		}*/

		Count++;
	}
	
}

// *************************************************************************
// *	  					 TrueFlase Terry Bernie						   *
// *************************************************************************
void SB_Dialogs::Dialog_TrueFlase()
{
	if (App->CLSB_Equity->EquitySB_Dialog_Visible == 0)
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_SB_TRUEFALSE, App->MainHwnd, (DLGPROC)Dialog_TrueFlase_Proc);
	}
	else
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_SB_TRUEFALSE, App->Equity_Dlg_hWnd, (DLGPROC)Dialog_TrueFlase_Proc);
	}
}
// *************************************************************************
// *						Dialog_TrueFlase_Proc	  					   *
// *************************************************************************
LRESULT CALLBACK SB_Dialogs::Dialog_TrueFlase_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_TITLENAME, WM_SETFONT, (WPARAM)App->Font_Arial20, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CHECKNO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CHECKYES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SetDlgItemText(hDlg, IDC_TITLENAME, (LPCTSTR)App->CLSB_Dialogs->btext);


		if (App->CLSB_Dialogs->TrueFlase == 1)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CHECKYES);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			temp = GetDlgItem(hDlg, IDC_CHECKNO);
			SendMessage(temp, BM_SETCHECK, 0, 0);
		}
		else
		{
			HWND temp = GetDlgItem(hDlg, IDC_CHECKYES);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			temp = GetDlgItem(hDlg, IDC_CHECKNO);
			SendMessage(temp, BM_SETCHECK, 1, 0);
		}
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_TITLENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		if (GetDlgItem(hDlg, IDC_CHECKYES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		if (GetDlgItem(hDlg, IDC_CHECKNO) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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
		if (LOWORD(wParam) == IDC_CHECKYES)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CHECKYES);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			temp = GetDlgItem(hDlg, IDC_CHECKNO);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			App->CLSB_Dialogs->TrueFlase = 1;
		}

		if (LOWORD(wParam) == IDC_CHECKNO)
		{
			HWND temp = GetDlgItem(hDlg, IDC_CHECKNO);
			SendMessage(temp, BM_SETCHECK, 1, 0);
			temp = GetDlgItem(hDlg, IDC_CHECKYES);
			SendMessage(temp, BM_SETCHECK, 0, 0);
			App->CLSB_Dialogs->TrueFlase = 0;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CLSB_Dialogs->Canceled = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CLSB_Dialogs->Canceled = 1;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}
	return FALSE;
}

// *************************************************************************
// *		UpdateGroupDetails:- Terry and Hazel Flanigan 2023		 	   *
// *************************************************************************
void SB_Dialogs::UpdateGroupDetails(HWND List)
{
	SendMessage(List, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	char buf[MAX_PATH];

	int Num = App->CLSB_Ogre_Setup->RenderListener->GroupNumber;

	sprintf(buf, "Centre X= %f Y= %f Z = %f", App->CLSB_Model->Group[Num]->Centre.x, App->CLSB_Model->Group[Num]->Centre.y, App->CLSB_Model->Group[Num]->Centre.z);
	SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)buf);

	sprintf(buf, "Size X= %f Y= %f Z = %f", App->CLSB_Model->Group[Num]->Size.x, App->CLSB_Model->Group[Num]->Size.y, App->CLSB_Model->Group[Num]->Size.z);
	SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)buf);
}

// *************************************************************************
// *	  	Start_Select_Face_Dlg:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Dialogs::Start_Select_Face_Dlg()
{
	if (Select_Face_Dlg_Active == 0)
	{
		CreateDialog(App->hInst, (LPCTSTR)IDD_SB_SELECT_FACE, App->Equity_Dlg_hWnd, (DLGPROC)Select_Face_Proc);
		Select_Face_Dlg_Active = 1;
	}
}

// *************************************************************************
// *		Select_Face_Proc:- Terry and Hazel Flanigan 2023	  		   *
// *************************************************************************
LRESULT CALLBACK SB_Dialogs::Select_Face_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		
		SendDlgItemMessage(hDlg, IDC_CB_FACENUMBER, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LIST_POINTS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_RENDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		App->CLSB_Dialogs->Fill_Face_Combo(hDlg);
		App->CLSB_Picking->Select_Face_In_Brush(App->CLSB_Picking->Real_Face_Index+1);
		App->CLSB_Dialogs->Show_Face_Data(App->CLSB_Picking->Real_Face_Index+1, App->CLSB_Picking->Selected_Face, hDlg);
		App->CLSB_Ogre_Setup->RenderListener->Show_Selected_Face = 1;

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_STBANNER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
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

		if (some_item->idFrom == IDC_BT_RENDER && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Ogre_Setup->RenderListener->Show_Selected_Face);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_CB_FACENUMBER)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_SELECTED_BRUSH);
				App->CLSB_Dialogs->Selected_Face_Index = SendMessage(temp, CB_GETCURSEL, 0, 0);

				App->CLSB_Picking->Select_Face_In_Brush(App->CLSB_Dialogs->Selected_Face_Index +1);
				App->CLSB_Dialogs->Show_Face_Data(App->CLSB_Dialogs->Selected_Face_Index, App->CLSB_Picking->Selected_Face, hDlg);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_RENDER)
		{
			if (App->CLSB_Ogre_Setup->RenderListener->Show_Selected_Face == 1)
			{
				App->CLSB_Ogre_Setup->RenderListener->Show_Selected_Face = 0;
			}
			else
			{
				App->CLSB_Ogre_Setup->RenderListener->Show_Selected_Face = 1;
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{
			App->CLSB_Dialogs->Select_Face_Dlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CLSB_Dialogs->Select_Face_Dlg_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	return FALSE;
}

// *************************************************************************
// *	  	Fill_Face_Combo:- Terry and Hazel Flanigan 2023	    	   *
// *************************************************************************
void SB_Dialogs::Fill_Face_Combo(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_CB_FACENUMBER, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	char buf[MAX_PATH];
	int Count = 0;
	while (Count < App->CLSB_Picking->Real_Face_Count)
	{
		SendDlgItemMessage(hDlg, IDC_CB_FACENUMBER, CB_ADDSTRING, (WPARAM)0, (LPARAM)itoa(Count+1,buf,10));
		Count++;
		
	}

	SendDlgItemMessage(hDlg, IDC_CB_FACENUMBER, CB_SETCURSEL, (WPARAM)App->CLSB_Picking->Real_Face_Index+1, (LPARAM)0);
}

typedef struct TexInfoTag
{
	geVec3d VecNormal;
	geFloat xScale, yScale;
	int xShift, yShift;
	geFloat	Rotate;			// texture rotation angle in degrees
	TexInfo_Vectors TVecs;
	int Dib;				// index into the wad
	char Name[16];
	geBoolean DirtyFlag;
	geVec3d Pos;
	int txSize, tySize;		// texture size (not currently used)
	geXForm3d XfmFaceAngle;	// face rotation angle
} TexInfo;

typedef struct FaceTag
{
	int			NumPoints;
	int			Flags;
	Plane		Face_Plane;
	int			LightIntensity;
	geFloat		Reflectivity;
	geFloat		Translucency;
	geFloat		MipMapBias;
	geFloat		LightXScale, LightYScale;
	TexInfo		Tex;
	geVec3d* Points;
} Face;

// *************************************************************************
// *		  Show_Face_Data:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Dialogs::Show_Face_Data(int Index, const Face* f, HWND hDlg)
{

	SendDlgItemMessage(hDlg, IDC_LIST_POINTS, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	char buf[MAX_PATH];
	int		i, xShift, yShift, Rotate;
	geFloat xScale, yScale, rot;

	sprintf(buf, "%s", " -------------------------------------------");
	SendDlgItemMessage(hDlg, IDC_LIST_POINTS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %i", "Face Index", Index + 1);
	SendDlgItemMessage(hDlg, IDC_LIST_POINTS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%i", "Flags ", f->Flags);
	SendDlgItemMessage(hDlg, IDC_LIST_POINTS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%d", "NumPoints", f->NumPoints);
	SendDlgItemMessage(hDlg, IDC_LIST_POINTS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	for (i = 0; i < f->NumPoints; i++)
	{
		sprintf(buf, "XYZ %f %f %f", f->Points[i].X, f->Points[i].Y, f->Points[i].Z);
		SendDlgItemMessage(hDlg, IDC_LIST_POINTS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}


	const TexInfo_Vectors* TVecs = Face_GetTextureVecs(f);
	geVec3d uVec, vVec;
	geFloat U, V;

	int txSize, tySize;

	Face_GetTextureSize(f, &txSize, &tySize);

	// make sure that the texture size is set correctly (division!)
	if (txSize == 0)
		txSize = 32;
	if (tySize == 0)
		tySize = 32;

	geVec3d_Scale(&TVecs->uVec, 1.f / (geFloat)txSize, &uVec);
	geVec3d_Scale(&TVecs->vVec, -1.f / (geFloat)tySize, &vVec);

	const geVec3d* verts = Face_GetPoints(f);

	int j = 0;
	for (j = 0; j < f->NumPoints; j++)
	{
		U = geVec3d_DotProduct(&(verts[j]), &uVec);
		V = geVec3d_DotProduct(&(verts[j]), &vVec);
		U += (TVecs->uOffset / txSize);
		V -= (TVecs->vOffset / tySize);

		sprintf(buf, "UV %.3f %.3f", U, V);
		SendDlgItemMessage(hDlg, IDC_LIST_POINTS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	char buf2[MAX_PATH];
	strcpy(buf, Face_GetTextureName(f));
	sprintf(buf2, "%s%s", "Texture: = ", buf);
	SendDlgItemMessage(hDlg, IDC_LIST_POINTS, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf2);

	return 1;
}


	
