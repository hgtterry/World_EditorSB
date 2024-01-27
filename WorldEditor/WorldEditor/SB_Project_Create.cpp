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
}

SB_Project_Create::~SB_Project_Create()
{
}

// *************************************************************************
// *	  		Start_New_Project:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_Project_Create::Start_New_Project()
{
	App->CLSB_Scene_Data->Clear_Level();
	App->CLSB_Scene_Data->Create_Resources_Group();

	Start_Save_Project_Dialog();

	//App->CLSB_Dialogs->YesNo("Add Area", "Do you want to add a new Area");

	//bool Doit = App->CLSB_Dialogs->Canceled;
	//if (Doit == 0)
	//{
	//	App->CLSB_Meshviewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Area;
	//	//App->CLSB_Meshviewer->StartMeshViewer();
	//}
	//else
	//{
	//	//First_Area_Build_Project(1);
	//}
}

// *************************************************************************
// *	  	Add_First_New_Area:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_Project_Create::Add_First_New_Area()
{
	First_Area_Build_Project(0);
}

// *************************************************************************
//		First_Area_Build_Project:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool SB_Project_Create::First_Area_Build_Project(bool NoArea)
{
	
	//char Temp[MAX_PATH];
	//strcpy(Temp, App->SBC_Scene->Project_Resource_Group.c_str());

	//// ------------------------ Add Area
	//if (NoArea == 0)
	//{
	//	App->SBC_Scene->B_Area[0] = new Base_Area();
	//	App->SBC_Com_Area->Add_Aera_To_Project(0, App->SBC_MeshViewer->Selected_MeshFile, Temp);
	//	strcpy(App->SBC_Scene->B_Area[0]->Material_File, App->SBC_MeshViewer->m_Material_File);
	//	App->SBC_Scene->Area_Count++;
	//	App->SBC_Scene->Area_Added = 1;

	//	HTREEITEM Temp2 = App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Areas_Folder, "Area_1", 0, true);
	//	App->SBC_Scene->B_Area[0]->FileViewItem = Temp2;
	//	App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Areas_Folder);
	//}

	//// ------------------------ Add Default Camera
	//App->SBC_Com_Camera->Add_New_Camera();
	//App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Cameras_Folder);
	//App->CL_Ogre->OgreListener->GD_CameraMode = Enums::CamDetached;

	//// ------------------------ Add Default Player
	//App->SBC_Player->Create_Player_Object();
	//strcpy(App->SBC_Scene->B_Player[0]->Player_Name, "Player_1");

	//HTREEITEM Temp1 =App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Players_Folder, "Player_1", 0, true);
	//App->SBC_Scene->B_Player[0]->FileViewItem = Temp1;
	//App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Players_Folder);

	//// ------------------------ Add Counter
	//App->SBC_Display->Add_New_Counter();

	//// ------------------------ Add Location
	//App->SBC_Locations->Create_Location_Entity("Start_Position");

	//// ------------------------ Add Environ

	//App->SBC_Com_Environments->Add_New_Environ_Entity(1);
	//int mIndex = App->SBC_Com_Environments->Get_First_Environ();
	//App->SBC_Com_Environments->Set_First_Environment(mIndex);

	//// ------------------------ Set Scene
	//App->SBC_Grid->Grid_SetVisible(1);
	//App->SBC_FileView->Redraw_FileView();

	//App->SBC_FileView->SelectItem(App->SBC_Scene->B_Area[0]->FileViewItem);

	//App->SBC_Physics->Reset_Physics();
	//App->SBC_Physics->Enable_Physics(1);
	//App->SBC_Scene->Scene_Loaded = 1;
	////App->SBC_Scene->Area_Added = 1;
	//App->CL_Ogre->Block_RenderingQueued = 0;

	////------------------------------------------------------------------------------ WHY
	//int f = App->SBC_Scene->B_Player[0]->Phys_Body->getCollisionFlags();
	//App->SBC_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	//App->CL_Ogre->Block_RenderingQueued = 0;

	return 1;
}

// *************************************************************************
// *	  Start_Save_Project_Dialog:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_Project_Create::Start_Save_Project_Dialog()
{
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
		SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CLSB_Project->m_Project_Sub_Folder);

		SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->CLSB_Project->m_Project_Sub_Folder);

		SetDlgItemText(hDlg, IDC_STBANNER, (LPCTSTR)"Save Project As");

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

		/*if (LOWORD(wParam) == IDC_BTCHANGE)
		{
			strcpy(App->SBC_Dialogs->btext, "Change Project Name");
			strcpy(App->SBC_Dialogs->Chr_Text, App->SBC_Project->m_Project_Name);

			App->SBC_Dialogs->Dialog_Text();

			if (App->SBC_Dialogs->Canceled == 1)
			{
				return TRUE;
			}

			int len1 = strlen(App->SBC_Project->m_Project_Sub_Folder);
			int len2 = strlen(App->SBC_Project->m_Project_Name);
			App->SBC_Project->m_Project_Sub_Folder[len1 - (len2 + 5)] = 0;


			strcpy(App->SBC_Project->m_Project_Name, App->SBC_Dialogs->Chr_Text);

			strcpy(App->SBC_Project->m_Project_Sub_Folder, App->SBC_Project->m_Project_Sub_Folder);
			strcat(App->SBC_Project->m_Project_Sub_Folder, "\\");
			strcat(App->SBC_Project->m_Project_Sub_Folder, App->SBC_Project->m_Project_Name);
			strcat(App->SBC_Project->m_Project_Sub_Folder, "_Prj");

			SetDlgItemText(hDlg, IDC_STPROJECTNAME, (LPCTSTR)App->SBC_Project->m_Project_Name);
			SetDlgItemText(hDlg, IDC_STPJFOLDERPATH, (LPCTSTR)App->SBC_Project->m_Project_Sub_Folder);

			App->SBC_Project->Directory_Changed_Flag = 1;

			return TRUE;
		}*/

		/*if (LOWORD(wParam) == IDC_BTCHANGELEVEL)
		{
			strcpy(App->SBC_Dialogs->btext, "Change Level Name");
			strcpy(App->SBC_Dialogs->Chr_Text, App->SBC_Project->m_Level_Name);

			App->SBC_Dialogs->Dialog_Text();
			if (App->SBC_Dialogs->Canceled == 1)
			{
				return TRUE;
			}

			strcpy(App->SBC_Project->m_Level_Name, App->SBC_Dialogs->Chr_Text);
			SetDlgItemText(hDlg, IDC_STLEVELNAME, (LPCTSTR)App->SBC_Project->m_Level_Name);

			App->SBC_Project->Directory_Changed_Flag = 1;

			return TRUE;
		}*/

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

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		/*if (LOWORD(wParam) == IDOK)
		{

			App->SBC_Project->Save_Project();
			App->SBC_Project->Project_Loaded = 1;

			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}*/

		break;

	}
	return FALSE;
}
