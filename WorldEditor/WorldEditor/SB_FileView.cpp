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
#include "SB_FileView.h"

#define IDM_FILE_NEW 1
#define IDM_FILE_DELETE 2
#define IDM_FILE_RENAME 3
#define IDM_COPY 4
#define IDM_PASTE 5
#define IDM_GOTO 6

SB_FileView::SB_FileView(void)
{
	hImageList = nullptr;
	hBitMap = nullptr;

	Root = nullptr;
	GD_ProjectFolder =	nullptr;
	FV_Players_Folder = nullptr;	// Players Folder FileFView
	FV_Areas_Folder =	nullptr;	// Areas/Rooms Folder FileFView
	FV_LevelFolder =	nullptr;
	FV_Cameras_Folder = nullptr;
	FV_Objects_Folder = nullptr;
	FV_EntitiesFolder = nullptr;
	FV_Evirons_Folder = nullptr;

	Context_Selection = Enums::FileView_None;

	FileView_Folder[0] = 0;
	FileView_File[0] = 0;
}

SB_FileView::~SB_FileView(void)
{
}

// **************************************************************************
// *			Start_FileView:- Terry and Hazel Flanigan 2022				*
// **************************************************************************
void SB_FileView::Start_FileView(void)
{
	App->ListPanel = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_LIST, App->Equity_Dlg_hWnd, (DLGPROC)ListPanel_Proc);
	//Init_Bmps_FileView();
	Init_FileView();
}

// *************************************************************************
// *		ListPanel_Proc_Proc:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
LRESULT CALLBACK SB_FileView::ListPanel_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		//App->SBC_FileView->FileView_Active = 1;
		SendDlgItemMessage(hDlg, IDC_TREE1, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_ENVIRONMENT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		//CheckMenuItem(App->mMenu, ID_WINDOWS_FILEVIEW, MF_BYCOMMAND | MF_CHECKED);
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_SIZE:
	{
		//App->SBC_Panels->Resize_FileView();
	}break;

	case WM_CONTEXTMENU:
	{
		App->CLSB_FileView->Context_Menu(hDlg);
		break;
	}

	case WM_NOTIFY:
	{
		LPNMHDR nmhdr = (LPNMHDR)lParam;
		if (nmhdr->idFrom == IDC_TREE1)
		{
			switch (nmhdr->code)
			{

			case TVN_SELCHANGED:
			{
				App->CLSB_FileView->Get_Selection((LPNMHDR)lParam);
			}

			}
		}

		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_ENVIRONMENT && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return 1;
	}
	case WM_COMMAND:
	{
		/*if (LOWORD(wParam) == IDM_GOTO)
		{
			App->SBC_FileView->Context_Goto(hDlg);
			return TRUE;
		}

		if (LOWORD(wParam) == IDM_COPY)
		{
			App->SBC_FileView->Context_Copy(hDlg);
			return TRUE;
		}*/

		if (LOWORD(wParam) == IDM_FILE_DELETE)
		{
			App->CLSB_FileView->Context_Delete(hDlg);

			return TRUE;
		}

		if (LOWORD(wParam) == IDM_FILE_NEW)
		{
			App->CLSB_FileView->Context_New(hDlg);
			return TRUE;
		}

		if (LOWORD(wParam) == IDM_FILE_RENAME)
		{
			App->CLSB_FileView->Context_Rename(hDlg);
			return TRUE;
		}

		//if (LOWORD(wParam) == IDC_LEVELS)
		//{
		//	//App->SBC_FileView->Level_But_Active = 1;
		//	//App->SBC_FileView->Stock_But_Active = 0;
		//	//App->RedrawWindow_Dlg(hDlg);

		//	//ShowWindow(GetDlgItem(App->ListPanel, IDC_TREE1), 1);

		//	//App->GDCL_FileView->HideRightPanes();
		//	//ShowWindow(App->GD_Properties_Hwnd, 1);*/

		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_STOCK)
		//{
		//	if (App->SBC_Scene->Scene_Loaded == 1)
		//	{
		//		/*App->Cl_FileView->HideRightPanes();
		//		ShowWindow(App->GD_Stock_Hwnd, 1);*/
		//		//App->Cl_Stock->Start_Stock_Dialog();
		//	}
		//	return TRUE;
		//}

		//if (LOWORD(wParam) == IDC_BT_INFO_FILEVIEW)
		//{
		//	App->Cl_Utilities->OpenHTML("Help\\FileView.html");
		//	return TRUE;
		//}

		if (LOWORD(wParam) == IDC_BT_ENVIRONMENT)
		{
			return TRUE;
		}

		
		break;
	}

	case WM_CLOSE:
	{
		/*App->SBC_FileView->FileView_Active = 0;
		ShowWindow(App->ListPanel, 0);

		CheckMenuItem(App->mMenu, ID_WINDOWS_FILEVIEW, MF_BYCOMMAND | MF_UNCHECKED);*/

		return TRUE;
	}

	break;
	}
	return FALSE;
}

// *************************************************************************
// *			Init_FileView:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_FileView::Init_FileView(void)
{
	InitCommonControls();	    // make our tree control to work

	////====================================================//
	hImageList = ImageList_Create(16, 16, FALSE, 8, 0); // Zero Index

	//--------- Grayed Folder Closed Open 0 1
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILEINACTIVE));
	ImageList_Add(hImageList, hBitMap, NULL);
	DeleteObject(hBitMap);

	//--------- Green Folder Closed Open 2 3
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_TREE));
	ImageList_Add(hImageList, hBitMap, NULL);
	DeleteObject(hBitMap);

	//--------- Uselected File Open 4
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILE));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	//--------- Selected File Open 5
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILESELECTED));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	//--------- File changed Open 6
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILECHANGED));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	//--------- File changed Selected Open 7
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FILECHANGEDSELECTED));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	//--------- File changed Selected Open 8 and 9
	hBitMap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_FVFOLDERRED));
	ImageList_Add(hImageList, hBitMap, (HBITMAP)NULL);
	DeleteObject(hBitMap);

	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);

	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	TreeView_DeleteAllItems(Temp);


	TreeView_SetBkColor(Temp, (COLORREF)RGB(255, 255, 255));

	AddRootFolder();
	MoreFoldersD(); //  Folders under root 
	ExpandRoot();
}

// *************************************************************************
// *			ExpandRoot:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void SB_FileView::ExpandRoot(void)
{
	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	HTREEITEM i = TreeView_GetSelection(Temp);

	TreeView_Expand(Temp, GD_ProjectFolder, TVE_EXPAND);
	TreeView_Expand(Temp, FV_LevelFolder, TVE_EXPAND);
	TreeView_Expand(Temp, FV_Players_Folder, TVE_EXPAND);
}

// *************************************************************************
// *			AddRootFolder:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_FileView::AddRootFolder(void)
{
	tvinsert.hParent = Root;			// top most level no need handle
	tvinsert.hInsertAfter = TVI_LAST; // work as root level
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = "Project";// App->SBC_Project->m_Project_Name;
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	GD_ProjectFolder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
}

// *************************************************************************
// *			MoreFoldersD:- Terry and Hazel Flanigan 2022 		 	   *
// *************************************************************************
void SB_FileView::MoreFoldersD(void) // last folder level
{
	//------------------------------------------------------- Level 
	tvinsert.hParent = GD_ProjectFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = "Level"; // App->SBC_Project->m_Level_Name;
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_LevelFolder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	////------------------------------------------------------- Camera
	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = "Camera";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Cameras_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = "Player";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Players_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = "Area";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Areas_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = "Objects";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Objects_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	tvinsert.hParent = FV_LevelFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = "Entities";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_EntitiesFolder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	//--------------------------------------- Evironments Eviron_Entity
	tvinsert.hParent = FV_EntitiesFolder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText = "Evironments";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 1;
	FV_Evirons_Folder = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
}

// *************************************************************************
// *		Get_Selection Terry:- Terry and Hazel Flanigan 2022		 	   *
// *************************************************************************
void SB_FileView::Get_Selection(LPNMHDR lParam)
{

	strcpy(FileView_Folder, "");
	strcpy(FileView_File, "");

	int Index = 0;
	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	HTREEITEM i = TreeView_GetSelection(Temp);

	TVITEM item;
	item.hItem = i;
	item.pszText = FileView_Folder;
	item.cchTextMax = sizeof(FileView_Folder);
	item.mask = TVIF_TEXT | TVIF_PARAM;
	TreeView_GetItem(((LPNMHDR)lParam)->hwndFrom, &item);
	Index = item.lParam;

	HTREEITEM p = TreeView_GetParent(Temp, i);

	TVITEM item1;
	item1.hItem = p;
	item1.pszText = FileView_File;
	item1.cchTextMax = sizeof(FileView_File);
	item1.mask = TVIF_TEXT;
	TreeView_GetItem(((LPNMHDR)lParam)->hwndFrom, &item1);


	//--------------------------------------------------------------------------

	//if (!strcmp(FileView_Folder, App->SBC_Project->m_Level_Name)) // Level Folder
	//{
	//	HideRightPanes();
	//	ShowWindow(App->GD_Properties_Hwnd, 1);

	//	App->SBC_Properties->Edit_Category = Enums::FV_Edit_Level;
	//	App->SBC_Properties->Update_ListView_Level();
	//}

	// ------------------------------------------------------------ Areas
	if (!strcmp(FileView_Folder, "Area")) // Folder
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Areas_Folder;

		return;
	}

	if (!strcmp(FileView_File, "Area"))
	{
		//App->SBC_FileView->Context_Selection = Enums::FileView_Areas_File;
		//App->SBC_Properties->Edit_Category = Enums::Edit_Area;
		//App->SBC_Properties->Current_Selected_Object = Index;

		//HideRightPanes();
		//ShowWindow(App->GD_Properties_Hwnd, 1);

		//App->SBC_Props_Dialog->Hide_Area_Dlg(1);
		//App->SBC_Props_Dialog->Hide_Debug_Dlg(1);
		//App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, 1);
		//App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		//App->SBC_Props_Dialog->Hide_Material_Dlg(1);

		////----------------------------------------------------------------------------
		////App->SBC_Properties->Current_Selected_Object = Index;
		////App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		////App->SBC_Properties->Last_Selected_Object = Index;
		////----------------------------------------------------------------------------

		//App->SBC_Properties->Update_ListView_Area();


		//if (App->SBC_Dimensions->Show_Dimensions == 1)
		//{
		//	App->SBC_Dimensions->Prepare_Dimensions();
		//}

		return;

	}

	// ------------------------------------------------------------ Objects
	if (!strcmp(FileView_Folder, "Objects")) // Folder
	{
		App->CLSB_FileView->Context_Selection = Enums::FileView_Objects_Folder;
		EnableMenuItem(App->CLSB_Game_Editor->GE_Menu, ID_EDIT_DELETEMENU, MF_DISABLED);
		return;
	}
	if (!strcmp(FileView_File, "Objects"))
	{
		App->CLSB_FileView->Context_Selection = Enums::FileView_Objects_File;
		EnableMenuItem(App->CLSB_Game_Editor->GE_Menu, ID_EDIT_DELETEMENU, MF_ENABLED);

		App->CLSB_Props_Dialogs->Hide_Dimensions_Dlg(1, App->CLSB_Game_Editor->V_Object[Index]->Dimensions_Locked);

		/*HideRightPanes();
		ShowWindow(App->SBC_Properties->Properties_Dlg_hWnd, 1);

		App->SBC_Props_Dialog->Hide_Details_Goto_Dlg(1);
		App->SBC_Props_Dialog->Hide_Dimensions_Dlg(1, App->SBC_Scene->V_Object[Index]->Dimensions_Locked);
		App->SBC_Props_Dialog->Hide_Debug_Dlg(1);
		App->SBC_Props_Dialog->Hide_Material_Dlg(1);*/


		App->CLSB_Properties->Edit_Category = Enums::FV_Edit_Object;

		//----------------------------------------------------------------------------
		App->CLSB_Properties->Current_Selected_Object = Index;
		//App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
		//App->SBC_Properties->Last_Selected_Object = Index;
		//----------------------------------------------------------------------------

		//App->SBC_LookUps->Update_Types();

		//App->SBC_Markers->MarkerBB_Addjust(Index);

		App->CLSB_Properties->Update_ListView_Objects();


		/*if (App->SBC_Dimensions->Show_Dimensions == 1)
		{
			App->SBC_Dimensions->Prepare_Dimensions();
		}*/

		return;
	}

	// ------------------------------------------------------------ Players
	if (!strcmp(FileView_Folder, "Player")) // Folder
	{
	//	App->SBC_FileView->Context_Selection = Enums::FileView_Player_Folder;
	//	return;
	}
	if (!strcmp(FileView_File, "Player"))
	{
	//	App->SBC_FileView->Context_Selection = Enums::FileView_Player_File;

	//	HideRightPanes();
		ShowWindow(App->CLSB_Properties->Properties_Dlg_hWnd, 1);
	//	App->SBC_Player->Hide_Player_Dlg(1);

		App->CLSB_Properties->Edit_Category = Enums::Edit_Player;

	//	//----------------------------------------------------------------------------
		App->CLSB_Properties->Current_Selected_Object = Index;
	//	App->SBC_Properties->Reset_Last_Selected_Object(App->SBC_Properties->Last_Selected_Object);
	//	App->SBC_Properties->Last_Selected_Object = Index;
	//	//----------------------------------------------------------------------------

	//	if (App->SBC_Properties->Edit_Physics == 0)
	//	{
			App->CLSB_Properties->Update_ListView_Player();
	//	}
	//	else
	//	{
	//		App->SBC_Properties->Update_ListView_Player_Physics();
	//	}
		return;
	}

	// ------------------------------------------------------------ Cameras
	/*if (!strcmp(FileView_Folder, "Camera"))
	{
		App->SBC_FileView->Context_Selection = Enums::FileView_Cameras_Folder;

		return;
	}

	if (!strcmp(FileView_File, "Camera"))
	{
		App->SBC_FileView->Context_Selection = Enums::FileView_Cameras_File;

		HideRightPanes();
		ShowWindow(App->GD_Properties_Hwnd, 0);
		App->SBC_Com_Camera->Hide_Cam_Dlg(1);

		App->SBC_Properties->Edit_Category = Enums::Edit_Camera;

		App->SBC_Properties->Current_Selected_Object = Index;

		App->SBC_Properties->Update_ListView_Camera();

		return;
	}*/

}

// *************************************************************************
// *				Add_Item:- Terry and Hazel Flanigan 2022		 	   *
// *************************************************************************
HTREEITEM SB_FileView::Add_Item(HTREEITEM Folder, char* SFileName, int Index, bool NewItem)
{
	HWND Temp2 = GetDlgItem(App->ListPanel, IDC_TREE1);

	tvinsert.hParent = Folder;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.pszText = SFileName;

	if (NewItem == 1)
	{
		tvinsert.item.iImage = 6;
		tvinsert.item.iSelectedImage = 7;
		tvinsert.item.lParam = Index;
	}
	else
	{
		tvinsert.item.iImage = 4;
		tvinsert.item.iSelectedImage = 5;
		tvinsert.item.lParam = Index;
	}

	HTREEITEM Temp = (HTREEITEM)SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);

	return Temp;
}

// *************************************************************************
// *			Set_FolderActive:- Terry and Hazel Flanigan 2022	 	   *
// *************************************************************************
void SB_FileView::Set_FolderActive(HTREEITEM Folder)
{
	TVITEM Sitem;

	Sitem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	Sitem.hItem = Folder;
	Sitem.iImage = 3;
	Sitem.iSelectedImage = 3;

	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);
}

// *************************************************************************
// *				SelectItem:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_FileView::SelectItem(HTREEITEM TreeItem)
{

	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);

	//if (Flags[0]->FileView_SceneLoaded == 1)
	{
		//TreeView_Select(Temp, NULL, TVGN_CARET);
		TreeView_Select(Temp, TreeItem, TVGN_CARET);
	}
}

// *************************************************************************
// *			Mark_Altered:- Terry and Hazel Flanigan 2022		 	   *
// *************************************************************************
void SB_FileView::Mark_Altered(HTREEITEM Item)
{
	TVITEM Sitem;
	Sitem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	Sitem.hItem = Item;
	Sitem.iImage = 6;
	Sitem.iSelectedImage = 7;
	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);

	//EnableMenuItem(App->mMenu, ID_FILE_SAVEPROJECTALL, MF_ENABLED);
}

// *************************************************************************
// *		Change_Item_Name:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void SB_FileView::Change_Item_Name(HTREEITEM Folder, char* FolderName)
{
	TVITEM Sitem;
	Sitem.mask = TVIF_TEXT;
	Sitem.hItem = Folder;
	Sitem.pszText = FolderName;
	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);
}

// **************************************************************************
// *			Show_FileView:- Terry and Hazel Flanigan 2022				*
// **************************************************************************
void SB_FileView::Show_FileView(bool show)
{
	if (show == 1)
	{
		ShowWindow(App->ListPanel, SW_SHOW);
	}
	else
	{
		ShowWindow(App->ListPanel, SW_HIDE);
	}
}

// *************************************************************************
// *			Context_Menu:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void SB_FileView::Context_Menu(HWND hDlg)
{
	RECT rcTree;
	HWND hwndTV;
	HTREEITEM htvItem;
	TVHITTESTINFO htInfo = { 0 };
	POINT pt;
	GetCursorPos(&pt);

	long xPos = pt.x;   // x position from message, in screen coordinates
	long yPos = pt.y;   // y position from message, in screen coordinates 

	hwndTV = GetDlgItem(hDlg, IDC_TREE1);         // get the tree view 
	GetWindowRect(hwndTV, &rcTree);              // get its window coordinates
	htInfo.pt.x = xPos - rcTree.left;              // convert to client coordinates
	htInfo.pt.y = yPos - rcTree.top;

	if (htvItem = TreeView_HitTest(hwndTV, &htInfo)) {    // hit test
		TreeView_SelectItem(hwndTV, htvItem);           // success; select the item

		//------------------------------------- Camera
		if (!strcmp(FileView_Folder, "Camera")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED | MF_GRAYED, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Cameras_Folder;
		}

		if (!strcmp(FileView_File, "Camera"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Cameras_File;
		}

		//------------------------------------- Objects
		if (!strcmp(FileView_Folder, "Objects")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Objects_Folder;
		}

		if (!strcmp(FileView_File, "Objects"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING , IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Create Copy");

			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING , IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Objects_File;
		}

		//------------------------------------- Messages
		if (!strcmp(FileView_Folder, "Messages")) // Folder
		{
			hMenu = CreatePopupMenu();
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Messages_Triggers_Folder;
		}

		if (!strcmp(FileView_File, "Messages"))
		{
			hMenu = CreatePopupMenu();

			AppendMenuW(hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(hMenu, MF_STRING, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(hMenu, MF_STRING, IDM_COPY, L"&Create Copy");
			//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(hMenu);
			Context_Selection = Enums::FileView_Messages_Triggers_File;
		}

		//------------------------------------- Sounds
		//if (!strcmp(App->SBC_FileView->FileView_Folder, "Sounds")) // Folder
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Sounds_Folder;
		//}

		//if (!strcmp(App->SBC_FileView->FileView_File, "Sounds"))
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();

		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_GOTO, L"&Goto Camera");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_COPY, L"&Create Copy");
		//	//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_SEPARATOR, 0, NULL);
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Sounds_File;
		//}

		//------------------------------------- Eviron_Entities
		//if (!strcmp(App->SBC_FileView->FileView_Folder, "Evironments")) // Folder
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_EnvironEntity_Folder;
		//}

		/*if (!strcmp(App->SBC_FileView->FileView_File, "Evironments"))
		{
			App->SBC_FileView->hMenu = CreatePopupMenu();

			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_GOTO, L"&Goto Camera");
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(App->SBC_FileView->hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(App->SBC_FileView->hMenu);
			Context_Selection = Enums::FileView_EnvironEntity_File;
		}*/

		//------------------------------------- Areas
		//if (!strcmp(App->SBC_FileView->FileView_Folder, "Area")) // Folder
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Areas_Folder;
		//}

		/*if (!strcmp(App->SBC_FileView->FileView_File, "Area"))
		{
			App->SBC_FileView->hMenu = CreatePopupMenu();

			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(App->SBC_FileView->hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(App->SBC_FileView->hMenu);
			Context_Selection = Enums::FileView_Areas_File;
		}*/

		//------------------------------------- Move_Entities
		//if (!strcmp(App->SBC_FileView->FileView_Folder, "Move_Entities")) // Folder
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Move_Folder;
		//}

		//if (!strcmp(App->SBC_FileView->FileView_File, "Move_Entities"))
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();

		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_GOTO, L"&Goto Camera");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_COPY, L"&Create Copy");
		//	//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_SEPARATOR, 0, NULL);
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Move_File;
		//}

		//------------------------------------- Teleport_Entities
		//if (!strcmp(App->SBC_FileView->FileView_Folder, "Teleporters")) // Folder
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Teleports_Folder;
		//}

		//if (!strcmp(App->SBC_FileView->FileView_File, "Teleporters"))
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();

		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_GOTO, L"&Goto Camera");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_COPY, L"&Create Copy");
		//	//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_SEPARATOR, 0, NULL);
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Teleports_File;
		//}

		//------------------------------------- Collectables_Entities
		//if (!strcmp(App->SBC_FileView->FileView_Folder, "Collectables")) // Folder
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Collectables_Folder;
		//}

		//if (!strcmp(App->SBC_FileView->FileView_File, "Collectables"))
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();

		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_GOTO, L"&Goto Camera");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_COPY, L"&Create Copy");
		//	//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_SEPARATOR, 0, NULL);
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Collectables_File;
		//}

		//------------------------------------- Counters
		//if (!strcmp(App->SBC_FileView->FileView_Folder, "Counters")) // Folder
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Counters_Folder;
		//}

		/*if (!strcmp(App->SBC_FileView->FileView_File, "Counters"))
		{
			App->SBC_FileView->hMenu = CreatePopupMenu();

			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(App->SBC_FileView->hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(App->SBC_FileView->hMenu);
			Context_Selection = Enums::FileView_Counters_File;
		}*/

		//------------------------------------- Text_Messages
		//if (!strcmp(App->SBC_FileView->FileView_Folder, "Text_Message")) // Folder
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_TextMessage_Folder;
		//}

		/*if (!strcmp(App->SBC_FileView->FileView_File, "Text_Message"))
		{
			App->SBC_FileView->hMenu = CreatePopupMenu();

			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_RENAME, L"&Rename");
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_COPY, L"&Copy");
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
			AppendMenuW(App->SBC_FileView->hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_DELETE, L"&Delete");
			TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
			DestroyMenu(App->SBC_FileView->hMenu);
			Context_Selection = Enums::FileView_TextMessage_File;
		}*/

		//------------------------------------- Particles
		//if (!strcmp(App->SBC_FileView->FileView_Folder, "Particles")) // Folder
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Particle_Folder;
		//}

		//if (!strcmp(App->SBC_FileView->FileView_File, "Particles"))
		//{
		//	App->SBC_FileView->hMenu = CreatePopupMenu();

		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_FILE_RENAME, L"&Rename");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_GOTO, L"&Goto Camera");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING, IDM_COPY, L"&Create Copy");
		//	//AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_PASTE, L"&Paste");
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_SEPARATOR, 0, NULL);
		//	AppendMenuW(App->SBC_FileView->hMenu, MF_STRING | MF_GRAYED, IDM_FILE_DELETE, L"&Delete");
		//	TrackPopupMenu(App->SBC_FileView->hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, App->ListPanel, NULL);
		//	DestroyMenu(App->SBC_FileView->hMenu);
		//	Context_Selection = Enums::FileView_Particle_File;
		//}
	}
}

// *************************************************************************
// *		Mark_Altered_Folder:- Terry and Hazel Flanigan 202$		 	   *
// *************************************************************************
void SB_FileView::Mark_Altered_Folder(HTREEITEM Item)
{
	TVITEM Sitem;
	Sitem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	Sitem.hItem = Item;
	Sitem.iImage = 8;
	Sitem.iSelectedImage = 9;
	SendDlgItemMessage(App->ListPanel, IDC_TREE1, TVM_SETITEM, 0, (LPARAM)(const LPTVITEM)&Sitem);

	//EnableMenuItem(App->mMenu, ID_FILE_SAVEPROJECTALL, MF_ENABLED);
}

// *************************************************************************
// *				DeleteItem:- Terry and Hazel Flanigan 2022 			   *
// *************************************************************************
void SB_FileView::DeleteItem()
{
	HWND Temp = GetDlgItem(App->ListPanel, IDC_TREE1);
	HTREEITEM i = TreeView_GetSelection(Temp);
	TreeView_DeleteItem(Temp, i);
}

// *************************************************************************
// *				Context_New:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void SB_FileView::Context_New(HWND hDlg)
{
	/*if (App->SBC_Project->Project_Loaded == 0)
	{
		App->Say("A Project must be created First");

		return;
	}*/

	/*if (Context_Selection == Enums::FileView_Areas_Folder)
	{
		App->SBC_Dialogs->YesNo("Add Area", "Do you want to add a new Area", 1);

		bool Doit = App->SBC_Dialogs->Canceled;
		if (Doit == 0)
		{
			App->SBC_MeshViewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Area;
			App->SBC_MeshViewer->StartMeshViewer();
		}

		return;
	}*/

	/*if (App->SBC_Scene->Area_Added == 0)
	{
		App->Say("An Area must be Added Firest before adding items");

		return;
	}

	if (App->SBC_FileView->Context_Selection == Enums::FileView_Cameras_Folder)
	{
		App->SBC_Dialogs->YesNo("Add Object", "Do you want to add a new Camera", 1);

		bool Doit = App->SBC_Dialogs->Canceled;
		if (Doit == 0)
		{
			App->SBC_Com_Camera->Add_New_Camera();
		}

		return;
	}*/

	if (Context_Selection == Enums::FileView_Objects_Folder)
	{
		App->CLSB_Dialogs->YesNo("Add Object", "Do you want to add a new Object Entity");

		bool Doit = App->CLSB_Dialogs->Canceled;
		if (Doit == 0)
		{
			App->CLSB_Meshviewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Objects;
			App->CLSB_Meshviewer->Start_Mesh_Viewer();
		}

		return;
	}

	//if (App->SBC_FileView->Context_Selection == Enums::FileView_Messages_Triggers_Folder)
	//{
	//	App->SBC_Dialogs->YesNo("Add Message", "Do you want to add a new Message Entity", 1);

	//	bool Doit = App->SBC_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		App->CL_Com_Messages->Add_New_Message();
	//	}

	//	return;
	//}

	//if (App->SBC_FileView->Context_Selection == Enums::FileView_Sounds_Folder)
	//{
	//	App->SBC_Dialogs->YesNo("Add Message", "Do you want to add a new Sound Entity", 1);

	//	bool Doit = App->SBC_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		App->CL_Com_Sounds->Add_New_Sound();
	//	}

	//	return;
	//}

	//if (App->SBC_FileView->Context_Selection == Enums::FileView_EnvironEntity_Folder)
	//{
	//	App->SBC_Dialogs->YesNo("Add Environ Entity", "Do you want to add a new Environ Entity", 1);

	//	bool Doit = App->SBC_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		App->SBC_Com_Environments->Add_New_Environ_Entity(0);
	//	}

	//	return;
	//}


	//if (App->SBC_FileView->Context_Selection == Enums::FileView_Move_Folder)
	//{
	//	App->SBC_Dialogs->YesNo("Add Message", "Do you want to add a new Move Entity", 1);

	//	bool Doit = App->SBC_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		App->SBC_Com_MoveEntity->Add_New_Move_Entity();
	//	}

	//	return;
	//}

	//if (App->SBC_FileView->Context_Selection == Enums::FileView_Teleports_Folder)
	//{
	//	App->SBC_Dialogs->YesNo("Add Message", "Do you want to add a new Teleport Entity", 1);

	//	bool Doit = App->SBC_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		App->CL_Com_Teleporters->Add_New_Teleporter();
	//	}

	//	return;
	//}

	//if (App->SBC_FileView->Context_Selection == Enums::FileView_Collectables_Folder)
	//{

	//	App->SBC_Dialogs->YesNo("Add Object", "Do you want to add a new Collectable", 1);

	//	bool Doit = App->SBC_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		App->SBC_MeshViewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Collectables;
	//		App->SBC_MeshViewer->StartMeshViewer();
	//	}

	//	return;
	//}

	//if (App->SBC_FileView->Context_Selection == Enums::FileView_Counters_Folder)
	//{

	//	App->SBC_Dialogs->YesNo("Add Counter", "Do you want to add a new Counter", 1);

	//	bool Doit = App->SBC_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		App->SBC_Display->Add_New_Counter();
	//	}

	//	return;
	//}

	//if (App->SBC_FileView->Context_Selection == Enums::FileView_Environments_Folder)
	//{

	//	App->SBC_Dialogs->YesNo("Add Environment", "Do you want to add a new Environment", 1);

	//	bool Doit = App->SBC_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		//App->SBC_Com_Environments->Add_New_Environment();
	//	}

	//	return;
	//}

	//if (App->SBC_FileView->Context_Selection == Enums::FileView_Particle_Folder)
	//{

	//	App->SBC_Dialogs->YesNo("Add Particle", "Do you want to add a new Particle", 1);

	//	bool Doit = App->SBC_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		strcpy(App->SBC_Dialogs->Chr_DropText, "GD_Smoke1");
	//		App->SBC_Dialogs->DropList_Data = Enums::DropDialog_Particles;
	//		App->SBC_Dialogs->Dialog_DropGen();

	//		if (App->SBC_Dialogs->Canceled == 0)
	//		{
	//			App->CL_Com_Particles->Add_New_Particle(App->SBC_Dialogs->Chr_DropText);
	//		}

	//	}

	//	return;
	//}

	return;
}

// *************************************************************************
// *			Context_Delete:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_FileView::Context_Delete(HWND hDlg)
{
	// ---------------- Areas
	if (Context_Selection == Enums::FileView_Areas_File)
	{

		/*if (App->SBC_Scene->B_Area[App->SBC_Properties->Current_Selected_Object]->This_Object_UniqueID == 0)
		{
			App->Say("This Area can not be Deleted");
		}*/

		return;
	}

	// ---------------- Cameras
	if (Context_Selection == Enums::FileView_Cameras_File)
	{
		/*if (App->SBC_Scene->Camera_Count == 1)
		{
			App->Say("This Camera can not be Deleted");
		}*/

		return;
	}

	// ---------------- Players
	if (Context_Selection == Enums::FileView_Player_File)
	{
		/*if (App->SBC_Scene->Player_Count == 1)
		{
			App->Say("This Player can not be Deleted");
		}*/

		return;
	}

	// ---------------- Objects
	if (Context_Selection == Enums::FileView_Objects_File)
	{
		App->CLSB_Dialogs->YesNo("Remove Object", "Are you sure");

		bool Doit = App->CLSB_Dialogs->Canceled;
		if (Doit == 0)
		{
			App->CLSB_Object->Delete_Object();
			Mark_Altered_Folder(FV_Objects_Folder);
		}

		return;
	}

	// ---------------- Message Triggers
	if (Context_Selection == Enums::FileView_Messages_Triggers_File)
	{
		/*App->SBC_Dialogs->YesNo("Remove Message", "Are you sure", 1);

		bool Doit = App->SBC_Dialogs->Canceled;
		if (Doit == 0)
		{
			App->CL_Object->Delete_Object();
			App->SBC_FileView->Mark_Altered_Folder(FV_Message_Trigger_Folder);
		}*/

		return;
	}

	// ---------------- Sound Entities
	if (Context_Selection == Enums::FileView_Sounds_File)
	{
		/*App->SBC_Dialogs->YesNo("Remove Sound", "Are you sure", 1);

		bool Doit = App->SBC_Dialogs->Canceled;
		if (Doit == 0)
		{
			App->CL_Object->Delete_Object();
			App->SBC_FileView->Mark_Altered_Folder(FV_Sounds_Folder);
		}*/

		return;
	}

	// ---------------- Move Entities
	if (Context_Selection == Enums::FileView_Move_File)
	{
		/*App->SBC_Dialogs->YesNo("Remove Move Entity", "Are you sure", 1);

		bool Doit = App->SBC_Dialogs->Canceled;
		if (Doit == 0)
		{
			App->CL_Object->Delete_Object();
			App->SBC_FileView->Mark_Altered_Folder(FV_Move_Folder);
		}*/

		return;
	}

	// ---------------- Counters
	if (Context_Selection == Enums::FileView_Counters_File)
	{
		/*if (App->SBC_Scene->B_Counter[App->SBC_Properties->Current_Selected_Object]->Unique_ID == 0)
		{
			App->Say("This Counter can not be Deleted");
			return;
		}

		App->SBC_Dialogs->YesNo("Remove Counter", "Are you sure", 1);

		bool Doit = App->SBC_Dialogs->Canceled;
		if (Doit == 0)
		{
			App->SBC_Display->Delete_Counter();
			Mark_Altered_Folder(App->SBC_FileView->FV_Counters_Folder);
		}*/

		return;
	}

	// ---------------- Environs
	if (Context_Selection == Enums::FileView_EnvironEntity_File)
	{
	//	//int Test = App->SBC_Com_Environments->Get_First_Environ();

	//	if (App->CLSB_Properties->Current_Selected_Object == Test)
	//	{
	//		App->Say("This Environment Entity can not be Deleted");
	//		return;
	//	}

	//	App->CLSB_Dialogs->YesNo("Remove Environment Entity", "Are you sure", 1);

	//	bool Doit = App->CLSB_Dialogs->Canceled;
	//	if (Doit == 0)
	//	{
	//		App->CLSB_Object->Delete_Object();
	//		Mark_Altered_Folder(FV_Evirons_Folder);
	//	}

		return;
	}

	return;
}

// *************************************************************************
// *				Context_Rename:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_FileView::Context_Rename(HWND hDlg)
{
	int Index = App->CLSB_Properties->Current_Selected_Object;

	/*if (Context_Selection == Enums::FileView_Cameras_File)
	{
		App->SBC_Com_Camera->Rename_Camera(Index);
		App->SBC_Properties->Update_ListView_Camera();
		return;
	}

	if (App->SBC_FileView->Context_Selection == Enums::FileView_Counters_File)
	{
		App->SBC_Display->Rename_Counter(Index);
		App->SBC_Properties->Update_ListView_Counters();
		return;
	}

	if (App->SBC_FileView->Context_Selection == Enums::FileView_Collectables_File)
	{
		App->CL_Object->Rename_Object(Index);
		App->SBC_Properties->Update_ListView_Collectables();
		return;
	}

	if (App->SBC_FileView->Context_Selection == Enums::FileView_EnvironEntity_File)
	{
		App->SBC_Com_Environments->Rename_Environ(Index);
		App->SBC_Properties->Update_ListView_Environs();
		return;
	}*/

	App->CLSB_Object->Rename_Object(Index);
	App->CLSB_Properties->Update_ListView_Objects();
}
