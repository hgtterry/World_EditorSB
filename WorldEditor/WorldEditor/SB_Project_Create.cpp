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

	Level_SetWadPath(App->CLSB_Doc->pLevel, "Default.txl");
	
	App->CLSB_File_WE->Save_Document();

	char SourceFile[MAX_PATH];
	char DestinationFile[MAX_PATH];

	strcpy(SourceFile,App->WorldEditor_Directory);
	strcat(SourceFile,"Data\\Default.txl");

	strcpy(DestinationFile, App->CLSB_Project->m_Aera_Folder_Path);
	strcat(DestinationFile, "\\");
	strcat(DestinationFile, "Default.txl");

	CopyFile(SourceFile, DestinationFile, false);

	App->CLSB_Mesh_Mgr->WE_Build_Brush_List(0);
	App->CLSB_Bullet->Create_Brush_Trimesh_XX(0);
	App->CLSB_Mesh_Mgr->WE_Convert_All_Texture_Groups();
	Create_Ogre_Model(1, 0);

	return 1;
}

// *************************************************************************
// *	  		Create_Ogre_Model:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_Project_Create::Create_Ogre_Model(bool Create, bool Use_Project_Path)
{
	if (Use_Project_Path == 0)
	{
		//Set_World_Paths();
	}

	App->CLSB_Model->Ogre_Face_Count = 0;

	if (Create == 1)
	{
		World_Manual = App->CLSB_Ogre_Setup->mSceneMgr->createManualObject("OgreManual2");
		World_Manual->setRenderQueueGroup(2);
	}

	int A = 0;
	int B = 0;
	int C = 0;

	World_Manual->setDynamic(false);
	World_Manual->setCastShadows(false);

	World_Manual->estimateVertexCount(App->CLSB_Model->VerticeCount);
	World_Manual->estimateIndexCount(App->CLSB_Model->FaceCount);

	char MaterialNumber[255];
	char MatName[255];

	int GroupCountTotal = App->CLSB_Model->Get_Groupt_Count();
	int Count = 0;
	int FaceCount = 0;
	int FaceIndex = 0;
	int TotalFaces = 0;

	while (Count < GroupCountTotal)
	{
		_itoa(Count, MaterialNumber, 10);
		strcpy(MatName, mWorld_Mesh_JustName);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		World_Manual->begin(MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST);

		FaceCount = 0;
		FaceIndex = 0;

		TotalFaces = TotalFaces + App->CLSB_Model->Group[Count]->GroupFaceCount;

		while (FaceCount < App->CLSB_Model->Group[Count]->GroupFaceCount)
		{
			A = App->CLSB_Model->Group[Count]->Face_Data[FaceCount].a;
			B = App->CLSB_Model->Group[Count]->Face_Data[FaceCount].b;
			C = App->CLSB_Model->Group[Count]->Face_Data[FaceCount].c;

			// --------------------------------------------------

			Get_Data(Count, A);

			World_Manual->position(Ogre::Vector3(x, y, z));
			World_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			World_Manual->normal(Ogre::Vector3(nx, ny, nz));
			World_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, B);

			World_Manual->position(Ogre::Vector3(x, y, z));
			World_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			World_Manual->normal(Ogre::Vector3(nx, ny, nz));
			World_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, C);

			World_Manual->position(Ogre::Vector3(x, y, z));
			World_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			World_Manual->normal(Ogre::Vector3(nx, ny, nz));
			World_Manual->index(FaceIndex);

			FaceIndex++;
			FaceCount++;
		}

		World_Manual->end();

		Count++;
	}

	App->CLSB_Model->Ogre_Face_Count = TotalFaces;

	if (World_Manual->getNumSections() == 0)
	{
		App->Say("Can not create Ogre Sections");
	}

	Ogre::MeshPtr mesh = World_Manual->convertToMesh("TestMesh");

	mesh->setAutoBuildEdgeLists(true);
	mesh->buildEdgeList();

	App->CLSB_Ogre_Setup->mSceneMgr->destroyManualObject(World_Manual);

	Ogre::MeshSerializer* ms = new Ogre::MeshSerializer();
	ms->exportMesh(mesh.get(), mWorld_File_PathAndFile);
	delete(ms);

	char OutputFolder[MAX_PATH];
	strcpy(OutputFolder, mWorld_File_Path);
	strcat(OutputFolder, "\\");

	DecompileTextures_TXL(OutputFolder);

	char Material_PathAndFile[MAX_PATH];
	strcpy(Material_PathAndFile, mWorld_File_Path);
	strcat(Material_PathAndFile, "\\");
	strcat(Material_PathAndFile, mWorld_Mesh_JustName);
	strcat(Material_PathAndFile, ".material");

	CreateMaterialFile(Material_PathAndFile);

	char Name[MAX_PATH];
	strcpy(Name, mWorld_Mesh_JustName);
	strcat(Name, ".mesh");

	if (App->CLSB_Mesh_Mgr->World_Ent)
	{
		App->CLSB_Mesh_Mgr->World_Node->detachAllObjects();

		App->CLSB_Ogre_Setup->mSceneMgr->destroySceneNode(App->CLSB_Mesh_Mgr->World_Node);
		App->CLSB_Ogre_Setup->mSceneMgr->destroyEntity(App->CLSB_Mesh_Mgr->World_Ent);

		App->CLSB_Mesh_Mgr->World_Node = NULL;
		App->CLSB_Mesh_Mgr->World_Ent = NULL;

		//Ogre::ResourcePtr ptr = Ogre::MeshManager::getSingleton().getByName(Name,App->CLSB_Ogre->World_Resource_Group);
		//ptr->unload();

		//Ogre::MeshManager::getSingleton().remove(Name);

		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CLSB_Ogre_Setup->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CLSB_Ogre_Setup->World_Resource_Group);

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CLSB_Ogre_Setup->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(App->CLSB_Ogre_Setup->World_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CLSB_Ogre_Setup->World_Resource_Group);

	}
	else
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CLSB_Ogre_Setup->World_Resource_Group);
	}


	App->CLSB_Mesh_Mgr->World_Ent = App->CLSB_Ogre_Setup->mSceneMgr->createEntity(Name);
	App->CLSB_Mesh_Mgr->World_Node = App->CLSB_Ogre_Setup->mSceneMgr->getRootSceneNode()->createChildSceneNode();

	App->CLSB_Mesh_Mgr->World_Node->attachObject(App->CLSB_Mesh_Mgr->World_Ent);

	App->CLSB_Mesh_Mgr->World_Node->setPosition(0, 0, 0);
	App->CLSB_Mesh_Mgr->World_Node->setVisible(true);
	App->CLSB_Mesh_Mgr->World_Node->setScale(1, 1, 1);

	if (Use_Project_Path == 0)
	{
		remove(mWorld_File_PathAndFile);
		remove(Material_PathAndFile);
	}
}

// *************************************************************************
// *			Get_Data:- Terry and Hazel Flanigan 2023  			   	   *
// *************************************************************************
void SB_Project_Create::Get_Data(int Index, int FaceIndex)
{
	x = App->CLSB_Model->Group[Index]->vertex_Data[FaceIndex].x;
	y = App->CLSB_Model->Group[Index]->vertex_Data[FaceIndex].y;
	z = App->CLSB_Model->Group[Index]->vertex_Data[FaceIndex].z;

	u = App->CLSB_Model->Group[Index]->MapCord_Data[FaceIndex].u;
	v = App->CLSB_Model->Group[Index]->MapCord_Data[FaceIndex].v;

	nx = App->CLSB_Model->Group[Index]->Normal_Data[FaceIndex].x;
	ny = App->CLSB_Model->Group[Index]->Normal_Data[FaceIndex].y;
	nz = App->CLSB_Model->Group[Index]->Normal_Data[FaceIndex].z;
}

// *************************************************************************
// *		DecompileTextures_TXL:- Terry and Hazel Flanigan 2023  	   	   *
// *************************************************************************
bool SB_Project_Create::DecompileTextures_TXL(char* PathAndFile)
{

	char buf[MAX_PATH];

	int GroupCount = 0;
	int GroupCountTotal = App->CLSB_Model->Get_Groupt_Count();

	while (GroupCount < GroupCountTotal)
	{
		strcpy(buf, App->CLSB_Model->Group[GroupCount]->Text_FileName);
		int Len = strlen(buf);
		buf[Len - 4] = 0;

		App->CLSB_Textures->Extract_TXL_Texture(buf, PathAndFile);

		GroupCount++;
	}


	return 1;
}

// *************************************************************************
// *		CreateMaterialFile:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
void SB_Project_Create::CreateMaterialFile(char* MatFileName)
{
	char MatName[255];
	char File[255];
	char MaterialNumber[255];

	Ogre::String OMatFileName = MatFileName;
	Ogre::String OFile;
	Ogre::String OMatName;

	int numMaterials = App->CLSB_Model->GroupCount;

	Ogre::MaterialManager& matMgrSgl = Ogre::MaterialManager::getSingleton();

	Ogre::MaterialSerializer matSer;

	for (int i = 0; i < numMaterials; ++i)
	{
		_itoa(i, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		strcpy(File, App->CLSB_Model->Group[i]->Text_FileName);

		OMatName = MatName;
		OFile = File;

		Ogre::MaterialPtr ogremat = matMgrSgl.create(OMatName,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);


		if (0 < strlen(File))
		{
			ogremat->getTechnique(0)->getPass(0)->createTextureUnitState(File);

			if (_stricmp(File + strlen(File) - 4, ".TGA") == 0)
			{
				ogremat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER, 128);
			}
		}

		matSer.queueForExport(ogremat);
	}

	matSer.exportQueued(OMatFileName);
}

