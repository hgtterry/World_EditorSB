/*
Copyright (c) 2021 Scene Builder and Equity -- Inflanite Software W.T.Flanigan H.C.Flanigan

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

:- Terry and Hazel Flanigan 2024

*/

#include "stdafx.h"
#include "AB_App.h"
#include "resource.h"
#include "SB_MeshViewer.h"

SB_MeshViewer::SB_MeshViewer()
{
	MeshView_3D_hWnd = nullptr;

	ListHwnd = nullptr;
	CB_hWnd = nullptr;

	MvEnt = NULL;
	MvNode = NULL;
	Phys_Body = NULL;

	Mesh_Viewer_Mode = 0; // 0 = Defaulet Objects 1 = Collectables

	// ------------------------------------------------ 
	Physics_Type = Enums::Bullet_Type_None;
	Physics_Shape = Enums::Shape_None;
	Last_Selected_Physics_Shape = Enums::Shape_None;

	SelectDynamic = 0;
	SelectStatic = 0;
	SelectTriMesh = 0;

	Selected_Shape_Box = 0;
	Selected_Shape_Sphere = 0;
	Selected_Shape_Capsule = 0;
	Selected_Shape_Cylinder = 0;
	Selected_Shape_Cone = 0;

	Show_Just_This_Mesh = 1;

	Mesh_Render_Running = 0;
	Mesh_Render_Running_New = 0;

	CursorPosX = 500;
	CursorPosY = 300;

	Placement_Camera = 1;

	View_Centred_Flag = 0;
	View_Zoomed_Flag = 1;
	// ------------------------------------------------ 

	Last_MeshFile[0] = 0;
	m_Material_File[0] = 0;

	strcpy(mResource_Folder, App->WorldEditor_Directory);
	strcat(mResource_Folder, "\\Media_New\\Structure\\");
	strcpy(Selected_MeshFile, "Barrel_1.mesh");

	strcpy(m_Current_Folder, "Structure");

	MV_Resource_Group = "MV_Resource_Group";
}


SB_MeshViewer::~SB_MeshViewer()
{
}

// *************************************************************************
// *					Enable_ShapeButtons Terry Bernie				   *
// *************************************************************************
void SB_MeshViewer::Enable_TypeButtons(bool state)
{
	ShowWindow(GetDlgItem(MainDlgHwnd, IDC_MVSTATIC), state);
	ShowWindow(GetDlgItem(MainDlgHwnd, IDC_DYNAMIC), state);
	ShowWindow(GetDlgItem(MainDlgHwnd, IDC_TRIMESH), state);
	ShowWindow(GetDlgItem(MainDlgHwnd, IDC_JUSTMESH), state);
	ShowWindow(GetDlgItem(MainDlgHwnd, IDC_TEST), state);
}
// *************************************************************************
// *					Enable_ShapeButtons Terry Bernie				   *
// *************************************************************************
void SB_MeshViewer::Enable_ShapeButtons(bool state)
{
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_BOX), state);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_SPHERE), state);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_CAPSULE), state);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_CYLINDER), state);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_CONE), state);

	Selected_Shape_Box = 0;
	Selected_Shape_Sphere = 0;
	Selected_Shape_Capsule = 0;
	Selected_Shape_Cylinder = 0;
	Selected_Shape_Cone = 0;
}

// *************************************************************************
// *		Start Mesh Viewer:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool SB_MeshViewer::Start_Mesh_Viewer()
{
	if (Mesh_Render_Running_New == 0)
	{
		MvEnt = NULL;
		MvNode = NULL;
		Last_MeshFile[0] = 0;

		if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Area)
		{
			strcpy(mResource_Folder, App->WorldEditor_Directory);
			strcat(mResource_Folder, "\\Media_New\\Areas\\");
			strcpy(Selected_MeshFile, "Test1.mesh");
		}
		else
		{
			strcpy(mResource_Folder, App->WorldEditor_Directory);
			strcat(mResource_Folder, "\\Media_New\\Structure\\");
			strcpy(Selected_MeshFile, "Barrel_1.mesh");
		}

		if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Collectables)
		{
			strcpy(mResource_Folder, App->WorldEditor_Directory);
			strcat(mResource_Folder, "\\Media_New\\Collectables\\");
			strcpy(Selected_MeshFile, "Blueball.mesh");
		}

		Create_Resources_Group();
		Add_Resources();

		MainDlgHwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_MESHVIEWER, App->Equity_Dlg_hWnd, (DLGPROC)MeshViewer_Proc);

		Start_Render();

		// Collectables
		if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Collectables)
		{
			App->CLSB_Meshviewer->Set_For_Collectables();
		}

		// Areas
		if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Area)
		{
			App->CLSB_Meshviewer->Set_For_Areas(MainDlgHwnd);
		}

		// Default
		if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Objects)
		{
			App->CLSB_Meshviewer->Set_For_Objects(MainDlgHwnd);
		}
		// 
		//Set_Debug_Shapes();
		//App->CLSB_Meshviewer->Set_OgreWindow();

		Mesh_Render_Running_New = 1;
	}

	return 1;
}

// *************************************************************************
// *						MeshViewer_Proc Terry Bernie				   *
// *************************************************************************
LRESULT CALLBACK SB_MeshViewer::MeshViewer_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{

		SendDlgItemMessage(hDlg, IDC_BT_FOLDERBROWSE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BOX, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_SELECTEDNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_MVSTATIC, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_DYNAMIC, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TRIMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_JUSTMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_TEST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_SPHERE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CAPSULE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CYLINDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CONE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_LISTFILES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_OBJECTNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STNAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_SHAPE2, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_TYPE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STFOLDER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STPLACEMENT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKPLACECAMERA, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CKPLACECENTER, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_PROPERTIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_FOLDERS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BTMV_CENTRE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTMV_ZOOMED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BT_JUSTMESH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDCANCEL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SetWindowText(hDlg, App->CLSB_Meshviewer->mResource_Folder);

		HWND temp = GetDlgItem(hDlg, IDC_CKPLACECAMERA);
		SendMessage(temp, BM_SETCHECK, 1, 0);

		App->CLSB_Meshviewer->ListHwnd = GetDlgItem(hDlg, IDC_LISTFILES);


		//App->CLSB_Meshviewer->MeshView_3D_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_VIEWER3D_MV, hDlg, NULL);// (DLGPROC)MeshView_3D_Proc);
		//App->CLSB_Meshviewer->Set_OgreWindow();

		
		HWND CB_hWnd = GetDlgItem(hDlg, IDC_CB_FOLDERS);

		App->CLSB_Meshviewer->Get_Media_Folders_Actors(CB_hWnd); // Populate Combo

		
		App->CLSB_Meshviewer->SelectStatic = 0;
		App->CLSB_Meshviewer->SelectDynamic = 0;
		App->CLSB_Meshviewer->SelectTriMesh = 0;
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_STNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_SHAPE2) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_TYPE) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STFOLDER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STPLACEMENT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CKPLACECAMERA) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_CKPLACECENTER) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_SELECTEDNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->Brush_White;
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

		if (some_item->idFrom == IDC_BT_JUSTMESH && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->Show_Just_This_Mesh);
			
			return CDRF_DODEFAULT;
		}
		
		if (some_item->idFrom == IDC_BT_FOLDERBROWSE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_MVSTATIC && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->SelectStatic);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_DYNAMIC && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->SelectDynamic);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_TRIMESH && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->SelectTriMesh);
			return CDRF_DODEFAULT;
		}

		//// ---------------------------------------------------------------------
		if (some_item->idFrom == IDC_BOX && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BOX));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->Selected_Shape_Box);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_SPHERE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_SPHERE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->Selected_Shape_Sphere);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CAPSULE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_CAPSULE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->Selected_Shape_Capsule);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CYLINDER && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_CYLINDER));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->Selected_Shape_Cylinder);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_CONE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_CONE));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->Selected_Shape_Cone);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_PROPERTIES && some_item->code == NM_CUSTOMDRAW)
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

		if (some_item->idFrom == IDC_BTMV_CENTRE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->View_Centred_Flag);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTMV_ZOOMED && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Toggle(item, App->CLSB_Meshviewer->View_Zoomed_Flag);
			return CDRF_DODEFAULT;
		}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		if (LOWORD(wParam) == IDC_BT_JUSTMESH)
		{
			if (App->CLSB_Meshviewer->Show_Just_This_Mesh == 1)
			{
				App->CLSB_Meshviewer->Show_Just_This_Mesh = 0;
				App->CLSB_Meshviewer->Show_Exsisting_Objects(1);
			}
			else
			{
				App->CLSB_Meshviewer->Show_Just_This_Mesh = 1;
				App->CLSB_Meshviewer->Show_Exsisting_Objects(0);
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDC_BTMV_CENTRE)
		{
			/*App->CLSB_Meshviewer->GridNode->resetOrientation();
			App->CLSB_Meshviewer->Reset_Camera();

			App->CLSB_Meshviewer->View_Zoomed_Flag = 0;
			App->CLSB_Meshviewer->View_Centred_Flag = 1;*/

			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTMV_ZOOMED)
		{
			/*App->CLSB_Meshviewer->View_Zoomed_Flag = 1;
			App->CLSB_Meshviewer->View_Centred_Flag = 0;
			
			Ogre::Vector3 Centre = App->CLSB_Meshviewer->MvEnt->getBoundingBox().getCenter();
			Ogre::Real Radius = App->CLSB_Meshviewer->MvEnt->getBoundingRadius();

			App->CLSB_Meshviewer->mCameraMeshView->setPosition(0, Centre.y, -Radius * (Real(2.5)));
			App->CLSB_Meshviewer->mCameraMeshView->lookAt(0, Centre.y, 0);*/

			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKPLACECAMERA)
		{
			/*HWND temp = GetDlgItem(hDlg, IDC_CKPLACECAMERA);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CKPLACECENTER);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			App->CLSB_Meshviewer->Placement_Camera = 1;*/

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CKPLACECENTER)
		{

			/*HWND temp = GetDlgItem(hDlg, IDC_CKPLACECENTER);
			SendMessage(temp, BM_SETCHECK, 1, 0);

			temp = GetDlgItem(hDlg, IDC_CKPLACECAMERA);
			SendMessage(temp, BM_SETCHECK, 0, 0);

			App->CLSB_Meshviewer->Placement_Camera = 0;*/

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_PROPERTIES)
		{

			//App->SBC_MeshViewer->Show_Mesh_Properties();
			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CB_FOLDERS)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{

				HWND temp = GetDlgItem(hDlg, IDC_CB_FOLDERS);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				SendMessage(temp, CB_GETLBTEXT, Index, (LPARAM)App->CLSB_Meshviewer->m_Current_Folder);

				SendMessage(App->CLSB_Meshviewer->ListHwnd, LB_RESETCONTENT, 0, 0);

				strcpy(App->CLSB_Meshviewer->mResource_Folder, App->WorldEditor_Directory);
				strcat(App->CLSB_Meshviewer->mResource_Folder, "\\Media_New\\");
				strcat(App->CLSB_Meshviewer->mResource_Folder, App->CLSB_Meshviewer->m_Current_Folder);
				strcat(App->CLSB_Meshviewer->mResource_Folder, "\\");

				//SetDlgItemText(hDlg, IDC_ST_CURRENTFOLDER, App->CLSB_Meshviewer->mResource_Folder);
				//SetWindowText(hDlg, App->CLSB_Meshviewer->mResource_Folder);

				App->CLSB_Meshviewer->Add_Resources();
				App->CLSB_Meshviewer->Get_Files(hDlg);

			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_LISTFILES)
		{
			char buff[256];
			int Index = 0;
			Index = SendDlgItemMessage(hDlg, IDC_LISTFILES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (Index == -1)
			{
				return 1;
			}

			SendDlgItemMessage(hDlg, IDC_LISTFILES, LB_GETTEXT, (WPARAM)Index, (LPARAM)buff);
			SetDlgItemText(hDlg, IDC_SELECTEDNAME, buff);

			strcpy(App->CLSB_Meshviewer->Selected_MeshFile, buff);

			App->CLSB_Meshviewer->Add_Resources();

			App->CLSB_Meshviewer->Update_Mesh(App->CLSB_Meshviewer->Selected_MeshFile);

			//App->CLSB_Meshviewer->GridNode->resetOrientation();
			//App->CLSB_Meshviewer->Reset_Camera();

			return TRUE;

		}

		// ---------------------------------------------------------------------
		if (LOWORD(wParam) == IDC_BT_FOLDERBROWSE)
		{
			/*strcpy(App->Com_CDialogs->BrowserMessage, "Select Folder");
			int Test = App->Com_CDialogs->StartBrowser(App->SBC_MeshViewer->mResource_Folder, App->Fdlg);

			if (Test == 0) { return true; }


			SetDlgItemText(hDlg, IDC_ST_CURRENTFOLDER, App->Com_CDialogs->szSelectedDir);
			SetWindowText(hDlg, App->Com_CDialogs->szSelectedDir);

			strcpy(App->SBC_MeshViewer->mResource_Folder, App->Com_CDialogs->szSelectedDir);

			App->SBC_MeshViewer->Add_Resources();
			App->SBC_MeshViewer->Get_Files();*/
			return TRUE;
		}
		//if (LOWORD(wParam) == ID_TOOLS_MVRESOURCEVIEWER)
		//{
		//	//App->SBC_Resources->Start_Resources(App->SBC_MeshViewer->MainDlgHwnd);
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == ID_FOLDERS_POO)
		//{

		//	//App->SBC_MeshViewer->Start_Folders();

		//	return TRUE;
		//}

		if (LOWORD(wParam) == IDC_TRIMESH)
		{
			App->CLSB_Meshviewer->Physics_Type = Enums::Bullet_Type_TriMesh;
			App->CLSB_Meshviewer->SelectStatic = 0;
			App->CLSB_Meshviewer->SelectDynamic = 0;
			App->CLSB_Meshviewer->SelectTriMesh = 1;

			App->CLSB_Meshviewer->Enable_ShapeButtons(false);

			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();

			App->CLSB_Meshviewer->Physics_Type = Enums::Shape_TriMesh;

			App->CLSB_Meshviewer->Show_Physics_Trimesh();

			return 1;
		}

		if (LOWORD(wParam) == IDC_MVSTATIC)
		{
			if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Collectables)
			{
				return 1;
			}

			App->CLSB_Meshviewer->Physics_Type = Enums::Bullet_Type_Static;
			
			App->CLSB_Meshviewer->SelectStatic = 1;
			App->CLSB_Meshviewer->SelectDynamic = 0;
			App->CLSB_Meshviewer->SelectTriMesh = 0;
			
			App->CLSB_Meshviewer->Enable_ShapeButtons(true);

			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();

			App->CLSB_Meshviewer->Reselect_Shape();
			return 1;
		}

		if (LOWORD(wParam) == IDC_DYNAMIC)
		{
			App->CLSB_Meshviewer->Physics_Type = Enums::Bullet_Type_Dynamic;
			
			App->CLSB_Meshviewer->SelectDynamic = 1;
			App->CLSB_Meshviewer->SelectStatic = 0;
			App->CLSB_Meshviewer->SelectTriMesh = 0;

			App->CLSB_Meshviewer->Enable_ShapeButtons(true);

			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();

			App->CLSB_Meshviewer->Reselect_Shape();
			return 1;
		}

		// --------------------------------------------------------------------- Shapes
		if (LOWORD(wParam) == IDC_BOX)
		{
			if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Collectables)
			{
				return 1;
			}

			App->CLSB_Meshviewer->Reset_Shape_Flags();
			App->CLSB_Meshviewer->Selected_Shape_Box = 1;
			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();

			App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Box;
			App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Box;

			App->CLSB_Meshviewer->Show_Physics_Box();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_SPHERE)
		{
			App->CLSB_Meshviewer->Reset_Shape_Flags();
			App->CLSB_Meshviewer->Selected_Shape_Sphere = 1;
			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();

			App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Sphere;
			App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Sphere;

			App->CLSB_Meshviewer->Show_Physics_Sphere();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CAPSULE)
		{
			App->CLSB_Meshviewer->Reset_Shape_Flags();
			App->CLSB_Meshviewer->Selected_Shape_Capsule = 1;
			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();

			App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Capsule;
			App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Capsule;

			App->CLSB_Meshviewer->Show_Physics_Capsule();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CYLINDER)
		{
			App->CLSB_Meshviewer->Reset_Shape_Flags();
			App->CLSB_Meshviewer->Selected_Shape_Cylinder = 1;
			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();

			App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Cylinder;
			App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Cylinder;

			App->CLSB_Meshviewer->Show_Physics_Cylinder();

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_CONE)
		{
			App->CLSB_Meshviewer->Reset_Shape_Flags();
			App->CLSB_Meshviewer->Selected_Shape_Cone = 1;
			App->CLSB_Meshviewer->RedrawWindow_Dlg_Buttons();
			App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Cone;
			App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Cone;

			App->CLSB_Meshviewer->Show_Physics_Cone();
			return TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{

			if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Area || App->CLSB_Meshviewer->Physics_Type == Enums::Bullet_Type_TriMesh)
			{
				
			}
			else if (App->CLSB_Meshviewer->Physics_Type == Enums::Bullet_Type_None || App->CLSB_Meshviewer->Physics_Shape == Enums::Shape_None)
			{
				App->Say("No Type or Shape Selected");
				return TRUE;
			}

			App->CLSB_Ogre_Setup->Pause_Render = 1;

			char buff[255];
			GetDlgItemText(hDlg, IDC_OBJECTNAME, (LPTSTR)buff, 256);
			strcpy(App->CLSB_Meshviewer->Object_Name, buff);

			if (App->CLSB_Meshviewer->Phys_Body)
			{
				App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(App->CLSB_Meshviewer->Phys_Body);
				App->CLSB_Meshviewer->Phys_Body = nullptr;
			}

			App->CLSB_Meshviewer->Stop_Render();
			App->CLSB_Meshviewer->Mesh_Render_Running = 0;

			
			//App->CLSB_Meshviewer->Delete_Resources_Group();

			if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Area) // Area
			{
				App->CLSB_Meshviewer->Copy_Assets();
				//App->SBC_Com_Area->Add_New_Area();
			}
			else // Normal Object
			{
				App->CLSB_Meshviewer->Copy_Assets();
				App->CLSB_Objects_Create->Add_Objects_From_MeshViewer();
			}

			//App->CLSB_Meshviewer->Set_Debug_Shapes();
			
			App->CLSB_Ogre_Setup->Pause_Render = 0;
			App->CLSB_Meshviewer->Mesh_Render_Running_New = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}


		if (LOWORD(wParam) == IDCANCEL)
		{
			
			if (App->CLSB_Meshviewer->Phys_Body)
			{
				App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(App->CLSB_Meshviewer->Phys_Body);
				App->CLSB_Meshviewer->Phys_Body = nullptr;
			}

			App->CLSB_Meshviewer->Stop_Render();
			App->CLSB_Meshviewer->Mesh_Render_Running = 0;

			App->CLSB_Meshviewer->Mesh_Render_Running_New = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}

	
	return FALSE;
}

// *************************************************************************
// *			Start_Render:- Terry and Hazel Flanigan 2022	     	   *
// *************************************************************************
void SB_MeshViewer::Start_Render(void)
{
	//Set_Render_Mode();

	/*RECT rect;
	GetWindowRect(MeshView_3D_hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	SetWindowPos(App->ViewGLhWnd, HWND_TOP, 0, 0, width, height, NULL);

	SetParent(App->ViewGLhWnd, MeshView_3D_hWnd);

	App->CLSB_Ogre_Setup->mWindow->resize(width, height);

	App->CLSB_Ogre_Setup->mWindow->windowMovedOrResized();
	App->CLSB_Ogre_Setup->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getHeight());

	Root::getSingletonPtr()->renderOneFrame();*/


	MvEnt = NULL;
	MvNode = NULL;
	Phys_Body = NULL;

	MvEnt = App->CLSB_Ogre_Setup->mSceneMgr->createEntity("MVTest2", Selected_MeshFile, MV_Resource_Group);
	MvNode = App->CLSB_Ogre_Setup->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	MvNode->attachObject(MvEnt);
	MvNode->setVisible(true);

	// Debug Physics Shape
	btDebug_Manual = App->CLSB_Ogre_Setup->mSceneMgr->createManualObject("MVManual");
	btDebug_Manual->setRenderQueueGroup(RENDER_QUEUE_MAX);
	btDebug_Manual->setDynamic(true);
	btDebug_Manual->estimateVertexCount(2000);
	btDebug_Manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1, 1);
	btDebug_Manual->end();
	btDebug_Node = App->CLSB_Ogre_Setup->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	btDebug_Node->attachObject(btDebug_Manual);

	Saved_btDebug_Manual = App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual;

	//Mesh_Render_Running = 1;

}

// *************************************************************************
// *			Stop_Render:- Terry and Hazel Flanigan 2023	      	   *
// *************************************************************************
void SB_MeshViewer::Stop_Render(void)
{
	/*SetParent(App->ViewGLhWnd, App->Equity_Dlg_hWnd);
	SetWindowPos(App->ViewGLhWnd, HWND_TOP, 235, 11, 542, 455, SWP_NOZORDER);

	App->CLSB_Equity->Resize_3DView();

	App->CLSB_Ogre_Setup->mWindow->windowMovedOrResized();
	App->CLSB_Ogre_Setup->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getHeight());
	App->CLSB_Ogre_Setup->mCamera->yaw(Radian(0));
	Root::getSingletonPtr()->renderOneFrame();*/

	App->CLSB_Equity->Equity_Render_Mode = Enums::EQ_Mode_GameDirector;

	if (MvEnt && MvNode)
	{
		MvNode->detachAllObjects();
		App->CLSB_Ogre_Setup->mSceneMgr->destroySceneNode(MvNode);
		App->CLSB_Ogre_Setup->mSceneMgr->destroyEntity(MvEnt);
		MvEnt = NULL;
		MvNode = NULL;
	}

	if (btDebug_Manual)
	{
		btDebug_Node->detachAllObjects();
		App->CLSB_Ogre_Setup->mSceneMgr->destroySceneNode(btDebug_Node);
		App->CLSB_Ogre_Setup->mSceneMgr->destroyManualObject("MVManual");
		btDebug_Manual = NULL;
		btDebug_Node = NULL;
	}

	App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual = Saved_btDebug_Manual;

	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 0;

	if (Phys_Body)
	{
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(Phys_Body);
		Phys_Body = nullptr;
	}

	Mesh_Render_Running = 0;
}

// *************************************************************************
// *		MeshView_3D_Proc:- Terry and Hazel Flanigan 2022 			   *
// *************************************************************************
LRESULT CALLBACK SB_MeshViewer::MeshView_3D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		if (App->CLSB_Meshviewer->Mesh_Render_Running == 0)
		{
			return (LONG)App->BlackBrush;
		}
	}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{

		SetFocus(App->CLSB_Meshviewer->MeshView_3D_hWnd);

		break;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		//if (App->CLSB_Meshviewer->Mesh_Render_Running == 1)
		//{
		//	SetCapture(App->CLSB_Meshviewer->MeshView_3D_hWnd);// Bernie
		//	SetCursorPos(App->CLSB_Meshviewer->CursorPosX, App->CLSB_Meshviewer->CursorPosY);
		//	App->CLSB_Meshviewer->RenderListener->Pl_RightMouseDown = 1;
		//	App->CUR = SetCursor(NULL);
		//	return 1;
		//}

		return 1;
	}
	case WM_RBUTTONUP:
	{
		if (App->CLSB_Meshviewer->Mesh_Render_Running == 1)
		{
			/*ReleaseCapture();
			App->CLSB_Meshviewer->RenderListener->Pl_RightMouseDown = 0;
			SetCursor(App->CUR);*/
			return 1;
		}

		return 1;
	}

	// Left Mouse Button
	case WM_LBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		if (App->CLSB_Meshviewer->Mesh_Render_Running == 1)
		{
			//SetCapture(App->CLSB_Meshviewer->MeshView_3D_hWnd);// Bernie
			//SetCursorPos(App->CLSB_Meshviewer->CursorPosX, App->CLSB_Meshviewer->CursorPosY);

			//App->CLSB_Meshviewer->RenderListener->Pl_LeftMouseDown = 1;

			//App->CUR = SetCursor(NULL);

			return 1;
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		if (App->CLSB_Meshviewer->Mesh_Render_Running == 1)
		{
			/*ReleaseCapture();
			App->CLSB_Meshviewer->RenderListener->Pl_LeftMouseDown = 0;
			SetCursor(App->CUR);*/
			return 1;
		}

		return 1;
	}

	}
	return FALSE;
}

// *************************************************************************
// *	  			Reselect_Shape:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_MeshViewer::Reselect_Shape()
{
	App->CLSB_Meshviewer->Physics_Shape = App->CLSB_Meshviewer->Last_Selected_Physics_Shape;
	
	App->CLSB_Meshviewer->Reset_Shape_Flags();

	if (App->CLSB_Meshviewer->Physics_Shape == Enums::Shape_Box)
	{
		App->CLSB_Meshviewer->Selected_Shape_Box = 1;
		
		App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Box;
		App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Box;

		App->CLSB_Meshviewer->Show_Physics_Box();
	}

	if (App->CLSB_Meshviewer->Physics_Shape == Enums::Shape_Sphere)
	{
		App->CLSB_Meshviewer->Selected_Shape_Sphere = 1;
		
		App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Sphere;
		App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Sphere;

		App->CLSB_Meshviewer->Show_Physics_Sphere();
	}

	if (App->CLSB_Meshviewer->Physics_Shape == Enums::Shape_Capsule)
	{
		App->CLSB_Meshviewer->Selected_Shape_Capsule = 1;
		
		App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Capsule;
		App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Capsule;

		App->CLSB_Meshviewer->Show_Physics_Capsule();
	}

	if (App->CLSB_Meshviewer->Physics_Shape == Enums::Shape_Cylinder)
	{
		App->CLSB_Meshviewer->Selected_Shape_Cylinder = 1;

		App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Cylinder;
		App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Cylinder;

		App->CLSB_Meshviewer->Show_Physics_Cylinder();
	}

	if (App->CLSB_Meshviewer->Physics_Shape == Enums::Shape_Cone)
	{
		App->CLSB_Meshviewer->Selected_Shape_Cone = 1;
		
		App->CLSB_Meshviewer->Physics_Shape = Enums::Shape_Cone;
		App->CLSB_Meshviewer->Last_Selected_Physics_Shape = Enums::Shape_Cone;

		App->CLSB_Meshviewer->Show_Physics_Cone();
	}

	RedrawWindow_Dlg_Buttons();
}

// *************************************************************************
// *		RedrawWindow_Dlg_Buttons:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void SB_MeshViewer::RedrawWindow_Dlg_Buttons()
{
	HWND Temp = NULL;

	// Shapes
	Temp = GetDlgItem(MainDlgHwnd, IDC_BOX);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	Temp = GetDlgItem(MainDlgHwnd, IDC_SPHERE);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	Temp = GetDlgItem(MainDlgHwnd, IDC_CAPSULE);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	Temp = GetDlgItem(MainDlgHwnd, IDC_CYLINDER);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	Temp = GetDlgItem(MainDlgHwnd, IDC_CONE);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);


	// Type
	Temp = GetDlgItem(MainDlgHwnd, IDC_MVSTATIC);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	Temp = GetDlgItem(MainDlgHwnd, IDC_DYNAMIC);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	Temp = GetDlgItem(MainDlgHwnd, IDC_TRIMESH);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	// View
	Temp = GetDlgItem(MainDlgHwnd, IDC_BT_PROPERTIES);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	Temp = GetDlgItem(MainDlgHwnd, IDC_BTMV_ZOOMED);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	Temp = GetDlgItem(MainDlgHwnd, IDC_BTMV_CENTRE);
	RedrawWindow(Temp, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

}

// *************************************************************************
// *	  			Copy_Assets:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_MeshViewer::Copy_Assets()
{
	App->CLSB_Scene->Add_Resource_Location_Project(mResource_Folder);

	// ------------------ Copy Mesh
	strcpy(SourceFile, mResource_Folder);
	strcat(SourceFile, Selected_MeshFile);

	strcpy(DestinationFile, App->CLSB_Project->m_Main_Assets_Path);
	strcat(DestinationFile, Selected_MeshFile);

	CopyFile(SourceFile, DestinationFile, false);

	// ------------------ Copy Material File
	strcpy(SourceFile, mResource_Folder);
	strcat(SourceFile, m_Material_File);

	strcpy(DestinationFile, App->CLSB_Project->m_Main_Assets_Path);
	strcat(DestinationFile, m_Material_File);

	CopyFile(SourceFile, DestinationFile, false);

	// ------------------ Copy Textures
	int Count = 0;
	while (Count < Texure_Count)
	{
		strcpy(SourceFile, App->CLSB_Meshviewer->mResource_Folder);
		strcat(SourceFile, v_Texture_Names[Count].c_str());

		strcpy(DestinationFile, App->CLSB_Project->m_Main_Assets_Path);
		strcat(DestinationFile, v_Texture_Names[Count].c_str());

		CopyFile(SourceFile, DestinationFile, false);

		Count++;
	}
}

// *************************************************************************
// *	  		Get_Mesh_Assets:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_MeshViewer::Get_Mesh_Assets()
{
	App->CLSB_Meshviewer->m_Material_File[0] = 0;
	v_Texture_Names.resize(0);
	Texure_Count = 0;

	int SubMeshCount = App->CLSB_Meshviewer->MvEnt->getNumSubEntities();
	char pScriptName[255];
	char pMaterialFile[255];
	Ogre::String st;
	Ogre::MaterialPtr MP;

	MP.setNull();
	bool loaded = 0;
	
	// ---------------------------------------------------------- Material File
	Ogre::SubMesh const *subMesh = App->CLSB_Meshviewer->MvEnt->getSubEntity(0)->getSubMesh();
	Ogre::String MatName = subMesh->getMaterialName();
	strcpy(pScriptName, MatName.c_str());

	loaded = Ogre::MaterialManager::getSingleton().resourceExists(MatName);

	if (loaded == 1)
	{
		MP = Ogre::MaterialManager::getSingleton().getByName(MatName, App->CLSB_Meshviewer->MV_Resource_Group);
		st = MP->getOrigin();
		strcpy(pMaterialFile, st.c_str());

		strcpy(App->CLSB_Meshviewer->m_Material_File, pMaterialFile);
	}
	else
	{
		//strcpy(test, "Not Loaded:- ");
	}

	// ---------------------------------------------------------- Textures
	Ogre::ResourcePtr TP;
	Ogre::ResourceManager::ResourceMapIterator TextureIterator = Ogre::TextureManager::getSingleton().getResourceIterator();

	while (TextureIterator.hasMoreElements())
	{
		//strcpy(pScriptName,(static_cast<Ogre::MaterialPtr>(TextureIterator.peekNextValue()))->getName().c_str());

		if (TextureIterator.peekNextValue()->getGroup() == App->CLSB_Meshviewer->MV_Resource_Group)
		{

			strcpy(pScriptName, TextureIterator.peekNextValue()->getName().c_str());
			TP = Ogre::TextureManager::getSingleton().getByName(pScriptName);

			if (TP->isLoaded() == 1)
			{
				v_Texture_Names.push_back(pScriptName);
				Texure_Count = v_Texture_Names.size();
			}
			else
			{
				v_Texture_Names.push_back(pScriptName);
				Texure_Count = v_Texture_Names.size();
				//App->Say(pScriptName);
				//strcpy(test, "Not Loaded:- ");	
			}

		}

		TextureIterator.moveNext();
	}
}


// *************************************************************************
// *					Set_ResourceMesh_File  Terry Flanigan			   *
// *************************************************************************
void SB_MeshViewer::Set_ResourceMesh_File(HWND hDlg)
{
	char buff[MAX_PATH];

	strcpy(App->CLSB_Meshviewer->mResource_Folder, App->WorldEditor_Directory);
	strcat(App->CLSB_Meshviewer->mResource_Folder, "\\Media_New\\");
	strcat(App->CLSB_Meshviewer->mResource_Folder, App->CLSB_Meshviewer->m_Current_Folder);
	strcat(App->CLSB_Meshviewer->mResource_Folder, "\\");

	//SetDlgItemText(hDlg, IDC_ST_CURRENTFOLDER, App->CLSB_Meshviewer->mResource_Folder);
	//SetWindowText(hDlg, App->CLSB_Meshviewer->mResource_Folder);
	
	App->CLSB_Meshviewer->Add_Resources();

	App->CLSB_Meshviewer->Get_Files(hDlg);
	

	HWND temp = GetDlgItem(hDlg, IDC_CB_FOLDERS);
	SendMessage(temp, CB_SELECTSTRING, -1, LPARAM(App->CLSB_Meshviewer->m_Current_Folder));
	

	SendDlgItemMessage(hDlg, IDC_LISTFILES, LB_GETTEXT, (WPARAM)0, (LPARAM)buff);
	SetDlgItemText(hDlg, IDC_SELECTEDNAME, buff);

	strcpy(App->CLSB_Meshviewer->Selected_MeshFile, buff);
}

// *************************************************************************
// *			Update_Mesh:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void SB_MeshViewer::Update_Mesh(char* MeshFile)
{
	if (MvEnt && MvNode)
	{
		MvNode->detachAllObjects();
		App->CLSB_Ogre_Setup->mSceneMgr->destroySceneNode(MvNode);
		App->CLSB_Ogre_Setup->mSceneMgr->destroyEntity(MvEnt);
		MvEnt = NULL;
		MvNode = NULL;
	}

	MvEnt = App->CLSB_Ogre_Setup->mSceneMgr->createEntity("MV",MeshFile, App->CLSB_Meshviewer->MV_Resource_Group);
	MvNode = App->CLSB_Ogre_Setup->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	MvNode->attachObject(MvEnt);
	MvNode->setPosition(0, 0, 0);
	MvNode->resetOrientation();

	if (App->CLSB_Meshviewer->View_Zoomed_Flag == 1)
	{
		Ogre::Vector3 Centre = MvEnt->getBoundingBox().getCenter();
		Ogre::Real Radius = MvEnt->getBoundingRadius();
	}

	Get_Mesh_Assets();

	if (Physics_Shape == Enums::Shape_Box)
	{
		Show_Physics_Box();
	}

	if (Physics_Shape == Enums::Shape_Sphere)
	{
		Show_Physics_Sphere();
	}

	if (Physics_Shape == Enums::Shape_Capsule)
	{
		Show_Physics_Capsule();
	}

	if (Physics_Shape == Enums::Shape_Cylinder)
	{
		Show_Physics_Cylinder();
	}

	if (App->CLSB_Meshviewer->Physics_Shape == Enums::Shape_Cone)
	{
		Show_Physics_Cone();
	}

	if (App->CLSB_Meshviewer->Physics_Type == Enums::Shape_TriMesh)
	{
		Show_Physics_Trimesh();
	}

}

// *************************************************************************
// *			OgreWindow:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
bool SB_MeshViewer::Set_OgreWindow(void)
{
	MvEnt = NULL;
	MvNode = NULL;
	Phys_Body = NULL;

	//Ogre::NameValuePairList options;

	//options["externalWindowHandle"] =
	//	Ogre::StringConverter::toString((size_t)MeshView_3D_hWnd);

	//MeshView_Window = App->CLSB_Ogre_Setup->mRoot->createRenderWindow("MeshViewWin", 1024, 768, false, &options);

	//mSceneMgrMeshView = App->CLSB_Ogre_Setup->mRoot->createSceneManager("DefaultSceneManager", "MeshViewGD");

	////mCameraMeshView = mSceneMgrMeshView->createCamera("CameraMV");
	////mCameraMeshView->setPosition(Ogre::Vector3(0, 0, 0));
	////mCameraMeshView->setNearClipDistance(0.1);
	////mCameraMeshView->setFarClipDistance(8000);

	////Ogre::Viewport* vp = MeshView_Window->addViewport(mCameraMeshView);
	////mCameraMeshView->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	////vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));

	//CamNode = mSceneMgrMeshView->getRootSceneNode()->createChildSceneNode("Camera_Node");
	//CamNode->attachObject(mCameraMeshView);

	//////-------------------------------------------- 
	//
	//MvEnt = mSceneMgrMeshView->createEntity("MVTest2", Selected_MeshFile, MV_Resource_Group);
	//MvNode = mSceneMgrMeshView->getRootSceneNode()->createChildSceneNode();
	//MvNode->attachObject(MvEnt);
	//MvNode->setVisible(true);

	//mSceneMgrMeshView->setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	//// add a bright light above the scene
	//Light* light = mSceneMgrMeshView->createLight();
	//light->setType(Light::LT_POINT);
	//light->setPosition(-10, 40, 20);
	//light->setSpecularColour(ColourValue::White);

	//Ogre::Vector3 Centre = MvEnt->getBoundingBox().getCenter();
	//Ogre::Real Radius = MvEnt->getBoundingRadius();

	//Grid_Update(1);
	//
	////RenderListener = new SB_MeshView_Listener();
	//
	////App->CLSB_Ogre_Setup->mRoot->addFrameListener(RenderListener);

	//Reset_Camera();

	//// Debug Physics Shape
	//btDebug_Manual = mSceneMgrMeshView->createManualObject("MVManual");
	//btDebug_Manual->setRenderQueueGroup(RENDER_QUEUE_MAX);
	//btDebug_Manual->setDynamic(true);
	//btDebug_Manual->estimateVertexCount(2000);
	//btDebug_Manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	//btDebug_Manual->position(0, 0, 0);
	//btDebug_Manual->colour(1,1,1,1);
	//btDebug_Manual->end();
	//btDebug_Node = mSceneMgrMeshView->getRootSceneNode()->createChildSceneNode();
	//btDebug_Node->attachObject(btDebug_Manual);

	//Saved_btDebug_Manual = App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual;

	//Mesh_Render_Running = 1;
	return 1;
}

// *************************************************************************
// *		Close_MeshWindow:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_MeshViewer::Close_OgreWindow(void)
{
	//App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual = Saved_btDebug_Manual;

	//App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 0;

	//if (Phys_Body)
	//{
	//	App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(Phys_Body);
	//	Phys_Body = nullptr;
	//}

	////App->CLSB_Ogre_Setup->mRoot->removeFrameListener(RenderListener);

	//App->CLSB_Ogre_Setup->mRoot->detachRenderTarget("MeshViewWin");
	//MeshView_Window->destroy();
	////App->CLSB_Ogre_Setup->mRoot->destroySceneManager(mSceneMgrMeshView);

	//Mesh_Render_Running = 0;
}

// *************************************************************************
// *	  	Reset_Shape_Flags:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_MeshViewer::Reset_Shape_Flags()
{
	Selected_Shape_Box = 0;
	Selected_Shape_Sphere = 0;
	Selected_Shape_Capsule = 0;
	Selected_Shape_Cylinder = 0;
	Selected_Shape_Cone = 0;
}

// *************************************************************************
// *			Get_Files:- Terry and Hazel Flanigan 2022			 	   *
// *************************************************************************
bool SB_MeshViewer::Get_Files(HWND hDlg)
{
	SendMessage(ListHwnd, LB_RESETCONTENT, 0, 0);

	char Path[1024];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(Path, mResource_Folder);
	strcat(Path, "*.*");
	
	hFind = FindFirstFile(Path, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (FindFileData.dwFileAttributes)
			{
				if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				{

				}
				else
				{
					if (_stricmp(FindFileData.cFileName + strlen(FindFileData.cFileName) - 5, ".mesh") == 0)
					{
						SendMessage(ListHwnd, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)FindFileData.cFileName);
					}
				}

			}
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}

	SendMessage(ListHwnd,LB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	char buff[256];
	SendDlgItemMessage(hDlg, IDC_LISTFILES, LB_GETTEXT, (WPARAM)0, (LPARAM)buff);
	SetDlgItemText(hDlg, IDC_SELECTEDNAME, buff);

	strcpy(App->CLSB_Meshviewer->Selected_MeshFile, buff);
	//App->CLSB_Meshviewer->Update_Mesh(App->CLSB_Meshviewer->Selected_MeshFile);

	return 0;
}

// *************************************************************************
// *		Create_Resources_Group:- Terry and Hazel Flanigan 2022	  	   *
// *************************************************************************
bool SB_MeshViewer::Create_Resources_Group()
{
	bool Test = Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(mResource_Folder, MV_Resource_Group);
	if (Test == 0)
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(MV_Resource_Group);
	}
	//else
	//{
		//Ogre::ResourceGroupManager::getSingleton().deleteResource(mResource_Folder, "FileSystem", MV_Resource_Group);
	//}
	
	return 1;
}

// *************************************************************************
// *						Add_Resources	Terry Bernie 			 	   *
// *************************************************************************
bool SB_MeshViewer::Add_Resources()
{
	bool Test = Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(mResource_Folder, MV_Resource_Group);

	if (Test == 0)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mResource_Folder, "FileSystem", MV_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(MV_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(MV_Resource_Group);
	}
	
	return 1;
}

// *************************************************************************
// *					Delete_Resources_Group	Terry Bernie 		 	   *
// *************************************************************************
bool SB_MeshViewer::Delete_Resources_Group()
{
	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(MV_Resource_Group);
	return 1;
}

// *************************************************************************
// *					Get_Media_FoldersActors Terry Berni			 	   *
// *************************************************************************
bool SB_MeshViewer::Get_Media_Folders_Actors(HWND DropHwnd)
{
	char Path[1024];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(Path, App->WorldEditor_Directory);
	strcat(Path, "\\Media_New\\*.*");

	hFind = FindFirstFile(Path, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				{

				}
				else
				{
					SendMessage(DropHwnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)FindFileData.cFileName);
				}

			}
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	SendMessage(DropHwnd, CB_SETCURSEL, 0, 0);
	return 0;
}

// *************************************************************************
// *				GetMeshFiles   Terry Bernie							   *
// *************************************************************************
bool SB_MeshViewer::GetMeshFiles(char* Location, bool ResetList)
{
	if (ResetList == true)
	{
		SendMessage(ListHwnd, LB_RESETCONTENT, 0, 0);
	}

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	char SearchName[255];
	strcpy(SearchName, Location);

	hFind = FindFirstFile(SearchName, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	SendMessage(ListHwnd, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)ffd.cFileName);

	while (FindNextFile(hFind, &ffd) != 0)
	{
		SendMessage(ListHwnd, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)ffd.cFileName);
	}

	SendMessage(ListHwnd, LB_SETCURSEL, 0, 0);

	char buff[256];
	int Index = 0;
	Index = SendMessage(ListHwnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

	if (Index == -1)
	{
		return 0;
	}

	SendMessage(ListHwnd, LB_GETTEXT, (WPARAM)0, (LPARAM)buff);

	strcpy(Selected_MeshFile, buff);
	Update_Mesh(Selected_MeshFile);

	return 1;
}

// *************************************************************************
// *	  	Show_Mesh_Properties:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_MeshViewer::Show_Mesh_Properties()
{
	//DialogBox(App->hInst,(LPCTSTR)IDD_LISTDATA, MainDlgHwnd,(DLGPROC)Mesh_Properties_Proc);
}

// *************************************************************************
// *		Properties_ListBox_Proc:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
LRESULT CALLBACK SB_MeshViewer::Mesh_Properties_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		/*SendDlgItemMessage(hDlg, IDC_LISTGROUP, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		HWND List = GetDlgItem(hDlg, IDC_LISTGROUP);
		ListView_DeleteAllItems(List);

		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)App->SBC_MeshViewer->mResource_Folder);
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"");
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)App->SBC_MeshViewer->Selected_MeshFile);
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)App->SBC_MeshViewer->m_Material_File);
		SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)"");*/

		/*int Count = 0;
		while (Count < App->SBC_MeshViewer->Texure_Count)
		{
			SendMessage(List, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)App->SBC_MeshViewer->v_Texture_Names[Count].c_str());
			
			Count++;
		}*/

		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CTLCOLORSTATIC:
	{
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

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
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

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *	  	Set_Debug_Shapes:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_MeshViewer::Set_Debug_Shapes()
{
	int Count = 0;

	while (Count < App->CLSB_Scene->Player_Count)
	{
		if (App->CLSB_Scene->B_Player[Count]->Physics_Debug_On == 1)
		{
			int f = App->CLSB_Scene->B_Player[Count]->Phys_Body->getCollisionFlags();
			App->CLSB_Scene->B_Player[Count]->Phys_Body->setCollisionFlags(f ^ btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
		}

		Count++;
	}

	/*Count = 0;
	while (Count < App->SBC_Scene->Object_Count)
	{
		if (App->SBC_Scene->V_Object[Count]->Physics_Debug_On == 1)
		{
			int f = App->SBC_Scene->V_Object[Count]->Phys_Body->getCollisionFlags();
			App->SBC_Scene->V_Object[Count]->Phys_Body->setCollisionFlags(f ^ btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
		}

		Count++;
	}*/

	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 0;
	App->CLSB_Ogre_Setup->RenderFrame();
	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 1;
}

// *************************************************************************
// *	  	Show_Exsisting_Objects:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_MeshViewer::Show_Exsisting_Objects(bool flag)
{
	int Count = 0;

	while (Count < App->CLSB_Scene->Object_Count)
	{
		if (App->CLSB_GameDirector->V_Object[Count]->Object_Node)
		{
			App->CLSB_GameDirector->V_Object[Count]->Object_Node->setVisible(flag);
		}

		Count++;
	}

}

// *************************************************************************
// *			Show_Physics_None:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_MeshViewer::Show_Physics_None()
{
	btDebug_Manual->beginUpdate(0);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->end();

	if (Phys_Body)
	{
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(Phys_Body);
		Phys_Body = nullptr;
	}
}

// *************************************************************************
// *			Show_Physics_Box:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_MeshViewer::Show_Physics_Box()
{
	btDebug_Manual->beginUpdate(0);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1,1,1);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1,1,1);
	btDebug_Manual->end();

	if (Phys_Body)
	{
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(Phys_Body);
		Phys_Body = nullptr;
	}

	AxisAlignedBox worldAAB = MvEnt->getBoundingBox();
	worldAAB.transformAffine(MvNode->_getFullTransform());
	Ogre::Vector3 Centre = worldAAB.getCenter();

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 0, 1));

	btScalar mass;
	mass = 0.0f;
	
	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);
	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = GetMesh_BB_Size(MvNode);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	btCollisionShape* newRigidShape = new btBoxShape(btVector3(sx, sy, sz));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//App->CLSB_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Phys_Body = new btRigidBody(rbInfo);
	Phys_Body->setRestitution(1.0);
	Phys_Body->setFriction(1.5);
	Phys_Body->setUserPointer(MvNode);
	Phys_Body->setWorldTransform(startTransform);

	App->CLSB_Bullet->dynamicsWorld->addRigidBody(Phys_Body);

	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 1;
	App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual = btDebug_Manual;
	
	Physics_Rotation();

}

// *************************************************************************
// *		Show_Physics_Capsule:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_MeshViewer::Show_Physics_Capsule()
{
	btDebug_Manual->beginUpdate(0);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->end();

	if (Phys_Body)
	{
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(Phys_Body);
		Phys_Body = nullptr;
	}

	AxisAlignedBox worldAAB = MvEnt->getBoundingBox();
	worldAAB.transformAffine(MvNode->_getFullTransform());
	Ogre::Vector3 Centre = worldAAB.getCenter();

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	mass = 0.0f;
	
	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = GetMesh_BB_Size(MvNode);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	float Radius = GetMesh_BB_Radius(MvNode);
	
	btCollisionShape* newRigidShape = new btCapsuleShape(Radius, sy);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//App->CLSB_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Phys_Body = new btRigidBody(rbInfo);
	Phys_Body->setRestitution(1.0);
	Phys_Body->setFriction(1.5);
	Phys_Body->setUserPointer(MvNode);
	Phys_Body->setWorldTransform(startTransform);

	App->CLSB_Bullet->dynamicsWorld->addRigidBody(Phys_Body);

	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 1;
	App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual = btDebug_Manual;

	Physics_Rotation();

}

// *************************************************************************
// *		Show_Physics_Cone:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_MeshViewer::Show_Physics_Cone()
{
	btDebug_Manual->beginUpdate(0);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->end();

	if (Phys_Body)
	{
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(Phys_Body);
		Phys_Body = nullptr;
	}

	AxisAlignedBox worldAAB = MvEnt->getBoundingBox();
	worldAAB.transformAffine(MvNode->_getFullTransform());
	Ogre::Vector3 Centre = worldAAB.getCenter();

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	mass = 0.0f;
	
	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = GetMesh_BB_Size(MvNode);
	float sx = Size.x / 2;
	float sy = Size.y;// / 2;
	float sz = Size.z / 2;

	float Radius = GetMesh_BB_Radius(MvNode);
	
	btCollisionShape* newRigidShape = new btConeShape(Radius, sy);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CLSB_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Phys_Body = new btRigidBody(rbInfo);
	Phys_Body->setRestitution(1.0);
	Phys_Body->setFriction(1.5);
	Phys_Body->setUserPointer(MvNode);
	Phys_Body->setWorldTransform(startTransform);

	App->CLSB_Bullet->dynamicsWorld->addRigidBody(Phys_Body);

	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 1;
	App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual = btDebug_Manual;

	Physics_Rotation();
}

// *************************************************************************
// *		Show_Physics_Sphere:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_MeshViewer::Show_Physics_Sphere()
{
	btDebug_Manual->beginUpdate(0);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->end();

	if (Phys_Body)
	{
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(Phys_Body);
		Phys_Body = nullptr;
	}

	AxisAlignedBox worldAAB = MvEnt->getBoundingBox();
	worldAAB.transformAffine(MvNode->_getFullTransform());
	Ogre::Vector3 Centre = worldAAB.getCenter();

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	mass = 0.0f;
	

	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	float Radius = GetMesh_BB_Radius(MvNode);
	
	btCollisionShape* newRigidShape = new btSphereShape(Radius);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//App->CLSB_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Phys_Body = new btRigidBody(rbInfo);
	Phys_Body->setRestitution(1.0);
	Phys_Body->setFriction(1.5);
	Phys_Body->setUserPointer(MvNode);
	Phys_Body->setWorldTransform(startTransform);

	App->CLSB_Bullet->dynamicsWorld->addRigidBody(Phys_Body);

	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 1;
	App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual = btDebug_Manual;

	Physics_Rotation();
}

// *************************************************************************
// *		Show_Physics_Cylinder:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_MeshViewer::Show_Physics_Cylinder()
{
	btDebug_Manual->beginUpdate(0);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->end();

	if (Phys_Body)
	{
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(Phys_Body);
		Phys_Body = nullptr;
	}

	AxisAlignedBox worldAAB = MvEnt->getBoundingBox();
	worldAAB.transformAffine(MvNode->_getFullTransform());
	Ogre::Vector3 Centre = worldAAB.getCenter();
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;
	mass = 0.0f;
	

	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = GetMesh_BB_Size(MvNode);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;

	float Radius = GetMesh_BB_Radius(MvNode);
	
	btCollisionShape* newRigidShape = new btCylinderShape(btVector3(sx, sy, sz));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CLSB_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Phys_Body = new btRigidBody(rbInfo);
	Phys_Body->setRestitution(1.0);
	Phys_Body->setFriction(1.5);
	Phys_Body->setUserPointer(MvNode);
	Phys_Body->setWorldTransform(startTransform);

	App->CLSB_Bullet->dynamicsWorld->addRigidBody(Phys_Body);

	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 1;
	App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual = btDebug_Manual;
	
	Physics_Rotation();
}

// *************************************************************************
// *		Show_Physics_Trimesh:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_MeshViewer::Show_Physics_Trimesh()
{
	btDebug_Manual->beginUpdate(0);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1);
	btDebug_Manual->end();

	if (Phys_Body)
	{
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(Phys_Body);
		Phys_Body = nullptr;
	}

	// Get the mesh from the entity
	Ogre::MeshPtr myMesh = MvEnt->getMesh();
	Ogre::Mesh::SubMeshIterator SubMeshIter = myMesh->getSubMeshIterator();

	// Create the triangle mesh
	btTriangleMesh* triMesh = NULL;
	btVector3 vert0, vert1, vert2;
	int i = 0;

	while (SubMeshIter.hasMoreElements())
	{
		i = 0;
		Ogre::SubMesh* subMesh = SubMeshIter.getNext();
		Ogre::IndexData* indexData = subMesh->indexData;
		Ogre::VertexData* vertexData = subMesh->vertexData;

		const Ogre::VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		
		Ogre::HardwareVertexBufferSharedPtr vBuffer = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

		Ogre::HardwareIndexBufferSharedPtr iBuffer = indexData->indexBuffer;

		// Set Sizes
		std::vector<Ogre::Vector3> vertices;
		vertices.reserve(vertexData->vertexCount);
		std::vector<unsigned long> indices;
		indices.reserve(indexData->indexCount);

		// Lock the Vertex Buffer (READ ONLY)
		unsigned char* vertex = static_cast<unsigned char*> (vBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		float* pReal = NULL;

		for (size_t j = 0; j < vertexData->vertexCount; ++j, vertex += vBuffer->getVertexSize()) {
			posElem->baseVertexPointerToElement(vertex, &pReal);
			Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

			vertices.push_back(pt);
		}
		vBuffer->unlock();
		
		bool use32bitindexes = (iBuffer->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		// Lock the Index Buffer (READ ONLY)
		unsigned long* pLong = static_cast<unsigned long*> (iBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*> (pLong);

		if (use32bitindexes) {
			for (size_t k = 0; k < indexData->indexCount; ++k) {
				//
				indices.push_back(pLong[k]);
			}
		}
		else {
			for (size_t k = 0; k < indexData->indexCount; ++k) {
				//
				indices.push_back(static_cast<unsigned long> (pShort[k]));
			}
		}
		iBuffer->unlock();

		// Add Vertices
		if (triMesh == nullptr)
		{
			triMesh = new btTriangleMesh(use32bitindexes);
		}

		for (size_t y = 0; y < indexData->indexCount / 3; y++) {
			// Set each vertex
			vert0.setValue(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z);
			vert1.setValue(vertices[indices[i + 1]].x, vertices[indices[i + 1]].y, vertices[indices[i + 1]].z);
			vert2.setValue(vertices[indices[i + 2]].x, vertices[indices[i + 2]].y, vertices[indices[i + 2]].z);

			// Add the triangle into the triangle mesh
			triMesh->addTriangle(vert0, vert1, vert2);

			i += 3;
		}

	}

	const bool useQuantizedAABB = true;
	btBvhTriangleMeshShape* mShape = new btBvhTriangleMeshShape(triMesh, false, true);
	//mShape->buildOptimizedBvh();

	float x = MvNode->getPosition().x;
	float y = MvNode->getPosition().y;
	float z = MvNode->getPosition().z;

	btVector3 inertia(0, 0, 0);
	mShape->calculateLocalInertia(0.0, inertia);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));
	btVector3 initialPosition(x, y, z);
	startTransform.setOrigin(initialPosition);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);


	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI
	(
		0,  // mass
		myMotionState,// initial position
		mShape,      // collision shape of body
		inertia   // local inertia
	);

	Phys_Body = new btRigidBody(rigidBodyCI);
	Phys_Body->clearForces();
	Phys_Body->setLinearVelocity(btVector3(0, 0, 0));
	Phys_Body->setAngularVelocity(btVector3(0, 0, 0));
	Phys_Body->setWorldTransform(startTransform);

	App->CLSB_Bullet->dynamicsWorld->addRigidBody(Phys_Body);

	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 1;
	App->CLSB_Ogre_Setup->BulletListener->btDebug_Manual = btDebug_Manual;

	Physics_Rotation();
}

// *************************************************************************
// *	  	Set_Shape_Buttons:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_MeshViewer::Set_Shape_Buttons()
{

	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_MVSTATIC), 0);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_DYNAMIC), 0);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_TRIMESH), 0);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_JUSTMESH), 0);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_TEST), 0);

	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_BOX), 0);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_SPHERE), 0);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_CAPSULE), 0);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_CYLINDER), 0);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_CONE), 0);

	Selected_Shape_Box = 0;
	Selected_Shape_Sphere = 0;
	Selected_Shape_Capsule = 0;
	Selected_Shape_Cylinder = 0;
	Selected_Shape_Cone = 0;
}

// *************************************************************************
// *			Set_For_Objects:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void SB_MeshViewer::Set_For_Objects(HWND hDlg)
{
	Set_ResourceMesh_File(hDlg);
	
	Get_Files(hDlg);

	Enable_ShapeButtons(true);
	Enable_TypeButtons(true);

	Selected_Shape_Box = 1;
	SelectStatic = 1;

	Physics_Type = Enums::Bullet_Type_Static;
	Physics_Shape = Enums::Shape_Box;
	Show_Physics_Box();


	char ATest[256];
	char ConNum[256];

	strcpy_s(ATest, "Object_");
	_itoa(App->CLSB_Scene->Object_Count, ConNum, 10);
	strcat(ATest, ConNum);

	SetDlgItemText(hDlg, IDC_OBJECTNAME, ATest);
	strcpy(Object_Name, ATest);

	Enable_TypeButtons(1);
	RedrawWindow_Dlg_Buttons();
}

// *************************************************************************
// *			Set_For_Areas:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_MeshViewer::Set_For_Areas(HWND hDlg)
{
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_MVSTATIC), false);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_DYNAMIC), false);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_TRIMESH), true);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_JUSTMESH), false);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_TEST), false);

	SelectTriMesh = 1;

	Enable_ShapeButtons(0);

	/*char ConNum[256];
	char ATest[256];

	strcpy_s(ATest, "Area_");
	_itoa(App->SBC_Scene->Area_Count, ConNum, 10);
	strcat(ATest, ConNum);

	SetDlgItemText(hDlg, IDC_OBJECTNAME, ATest);
	strcpy(App->SBC_MeshViewer->Object_Name, ATest);
	strcpy(App->SBC_MeshViewer->m_Current_Folder, "Areas");
	HWND temp = GetDlgItem(hDlg, IDC_CB_FOLDERS);
	SendMessage(temp, CB_SELECTSTRING, -1, LPARAM(App->SBC_MeshViewer->m_Current_Folder));

	SendMessage(App->SBC_MeshViewer->ListHwnd, LB_RESETCONTENT, 0, 0);

	strcpy(App->SBC_MeshViewer->mResource_Folder, App->EquityDirecory_FullPath);
	strcat(App->SBC_MeshViewer->mResource_Folder, "\\Media_New\\");
	strcat(App->SBC_MeshViewer->mResource_Folder, App->SBC_MeshViewer->m_Current_Folder);
	strcat(App->SBC_MeshViewer->mResource_Folder, "\\");

	SetDlgItemText(hDlg, IDC_ST_CURRENTFOLDER, App->SBC_MeshViewer->mResource_Folder);
	SetWindowText(hDlg, App->SBC_MeshViewer->mResource_Folder);

	App->SBC_MeshViewer->Add_Resources();
	App->SBC_MeshViewer->Get_Files();*/
}

// *************************************************************************
// *	  	Set_For_Collectables:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_MeshViewer::Set_For_Collectables()
{
	/*Set_Shape_Buttons();

	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_MVSTATIC), 1);
	EnableWindow(GetDlgItem(MainDlgHwnd, IDC_BOX), 1);

	App->SBC_MeshViewer->Physics_Type = Enums::Bullet_Type_Static;
	App->SBC_MeshViewer->Physics_Shape = Enums::Shape_Box;;
	App->SBC_MeshViewer->SelectStatic = 1;
	App->SBC_MeshViewer->SelectDynamic = 0;
	App->SBC_MeshViewer->SelectTriMesh = 0;

	App->RedrawWindow_Dlg(MainDlgHwnd);

	Selected_Shape_Box = 1;

	char ConNum[256];
	char ATest[256];

	strcpy_s(ATest, "Collectable_");
	_itoa(App->SBC_Scene->Object_Count, ConNum, 10);
	strcat(ATest, ConNum);

	SetDlgItemText(MainDlgHwnd, IDC_OBJECTNAME, ATest);
	strcpy(App->SBC_MeshViewer->Object_Name, ATest);
	strcpy(App->SBC_MeshViewer->m_Current_Folder, "Collectables");
	HWND temp = GetDlgItem(MainDlgHwnd, IDC_CB_FOLDERS);
	SendMessage(temp, CB_SELECTSTRING, -1, LPARAM(App->SBC_MeshViewer->m_Current_Folder));

	SendMessage(App->SBC_MeshViewer->ListHwnd, LB_RESETCONTENT, 0, 0);

	strcpy(App->SBC_MeshViewer->mResource_Folder, App->EquityDirecory_FullPath);
	strcat(App->SBC_MeshViewer->mResource_Folder, "\\Media_New\\");
	strcat(App->SBC_MeshViewer->mResource_Folder, App->SBC_MeshViewer->m_Current_Folder);
	strcat(App->SBC_MeshViewer->mResource_Folder, "\\");

	SetDlgItemText(MainDlgHwnd, IDC_ST_CURRENTFOLDER, App->SBC_MeshViewer->mResource_Folder);
	SetWindowText(MainDlgHwnd, App->SBC_MeshViewer->mResource_Folder);

	App->SBC_MeshViewer->Add_Resources();
	App->SBC_MeshViewer->Get_Files();*/

}

// *************************************************************************
// *	  		GetMesh_BB_Size:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
Ogre::Vector3 SB_MeshViewer::GetMesh_BB_Size(SceneNode* mNode)
{
	AxisAlignedBox aab = mNode->getAttachedObject(0)->getBoundingBox();
	Ogre::Vector3 min = aab.getMinimum() * mNode->getScale();
	Ogre::Vector3 max = aab.getMaximum() * mNode->getScale();
	Ogre::Vector3 center = aab.getCenter() * mNode->getScale();
	Ogre::Vector3 size(fabs(max.x - min.x), fabs(max.y - min.y), fabs(max.z - min.z));
	float radius = (size.x > size.z) ? size.z / 2.0f : size.x / 2.0f;

	return size;
}

// *************************************************************************
//		Get_BoundingBox_World_Centre:- Terry and Hazel Flanigan 2022	   *
// *************************************************************************
Ogre::Vector3 SB_MeshViewer::Get_BoundingBox_World_Centre()
{
	//if (App->SBC_Scene->V_Object[Object_Index]->Shape == Enums::Shape_TriMesh)
	//{
		//Ogre::Vector3 Pos = App->SBC_Scene->V_Object[Object_Index]->Object_Node->getPosition();
		//return Pos;
	//}
	//else
	{
		AxisAlignedBox worldAAB = MvEnt->getBoundingBox();
		worldAAB.transformAffine(MvNode->_getFullTransform());
		Ogre::Vector3 Centre = worldAAB.getCenter();

		return Centre;
	}
}

// *************************************************************************
// *	  		GetMesh_BB_Radius:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
float SB_MeshViewer::GetMesh_BB_Radius(SceneNode* mNode)
{
	AxisAlignedBox aab = mNode->getAttachedObject(0)->getBoundingBox();
	Ogre::Vector3 min = aab.getMinimum() * mNode->getScale();
	Ogre::Vector3 max = aab.getMaximum() * mNode->getScale();
	Ogre::Vector3 center = aab.getCenter() * mNode->getScale();
	Ogre::Vector3 size(fabs(max.x - min.x), fabs(max.y - min.y), fabs(max.z - min.z));
	float radius = (size.x > size.z) ? size.z / 2.0f : size.x / 2.0f;

	return radius;
}

// *************************************************************************
// *			Physics_Rotation:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_MeshViewer::Physics_Rotation(void)
{
	if (App->CLSB_Meshviewer->Phys_Body)
	{
		Ogre::Quaternion Physics_Quat;
		Physics_Quat.w = App->CLSB_Meshviewer->MvNode->getOrientation().w;
		Physics_Quat.x = App->CLSB_Meshviewer->MvNode->getOrientation().x;
		Physics_Quat.y = App->CLSB_Meshviewer->MvNode->getOrientation().y;
		Physics_Quat.z = App->CLSB_Meshviewer->MvNode->getOrientation().z;

		App->CLSB_Meshviewer->Phys_Body->getWorldTransform().setRotation(btQuaternion(Physics_Quat.x, Physics_Quat.y, Physics_Quat.z, Physics_Quat.w));

		AxisAlignedBox worldAAB = App->CLSB_Meshviewer->MvEnt->getBoundingBox();
		worldAAB.transformAffine(App->CLSB_Meshviewer->MvNode->_getFullTransform());

		if (Physics_Type == Enums::Shape_TriMesh)
		{
		}
		else
		{
			Ogre::Vector3 Centre = worldAAB.getCenter();
			App->CLSB_Meshviewer->Phys_Body->getWorldTransform().setOrigin(btVector3(Centre.x, Centre.y, Centre.z));
		}
	}
}

