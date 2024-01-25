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
#include "SB_Project.h"

SB_Project::SB_Project(void)
{
	strcpy(m_Main_Assets_Path, "None");

	strcpy(m_Project_Sub_Folder, App->WorldEditor_Directory);
	strcat(m_Project_Sub_Folder, "\\");
	strcat(m_Project_Sub_Folder, "Projects");
	strcat(m_Project_Sub_Folder, "\\");
	strcat(m_Project_Sub_Folder, "First_Project");
	strcat(m_Project_Sub_Folder, "_Prj");

	strcpy(m_Project_Name, "First_Project");
	strcpy(m_Level_Name, "First_Level");

	m_Level_Folder_Path[0] = 0;
	m_Players_Folder_Path[0] = 0;
	m_Objects_Folder_Path[0] = 0;
	m_Cameras_Folder_Path[0] = 0;
	m_Enviromnet_Folder_Path[0] = 0;

	m_Ini_Path_File_Name[0] = 0;

	m_Level_File_Name[0] = 0;

	//WriteFile = NULL;

	//Project_Loaded = 0;
	Directory_Changed_Flag = 0;
	//Set_QuickLoad_Flag = 1;

	m_Ini_Path_File_Name[0] = 0;

	WriteFile = nullptr;
}

SB_Project::~SB_Project(void)
{
}

// *************************************************************************
// *	  		Load_Project:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
bool SB_Project::Load_Project()
{
	char Default_Project[MAX_PATH];
	strcpy(Default_Project, App->WorldEditor_Directory);
	strcat(Default_Project, "Levels\\First_Project_Prj\\Project.SBProj");

	strcpy(App->CLSB_FileIO->Project_File_Name,"Project.SBProj");
	strcpy(App->CLSB_FileIO->Project_Path_File_Name, Default_Project);
	
	m_Ini_Path_File_Name[0] = 0;

	App->CLSB_Scene->Clear_Level();
	App->CLSB_Scene->Create_Resources_Group();

	Set_Paths();

	// ------------------------------------------------------------------- 
	Load_Options* Options = new Load_Options;

	Options->Has_Area = 0;
	Options->Has_Player = 0;
	Options->Has_Camera = 0;
	Options->Has_Objects = 0;
	Options->Has_Counters = 0;

	int Int1 = 0;
	char chr_Tag1[1024];
	char chr_Tag2[1024];

	chr_Tag1[0] = 0;
	chr_Tag2[0] = 0;

	App->CLSB_Ini->SetPathName(m_Ini_Path_File_Name);

	App->CLSB_Ini->GetString("Version_Data", "Version", chr_Tag1, 1024);
	
	//App->Cl_Ini->GetString("Files", "Level_Name", m_Level_Name, MAX_PATH);
	//App->Cl_Ini->GetString("Files", "Project_Name", m_Project_Name, MAX_PATH);
	//App->Cl_Ini->GetString("Files", "Game_Name", App->SBC_Build->GameName, MAX_PATH, "YourGameName");


	Options->Has_Area = App->CLSB_Ini->GetInt("Options", "Areas_Count", 0, 10);
	Options->Has_Player = App->CLSB_Ini->GetInt("Options", "Players_Count", 0, 10);
	Options->Has_Camera = App->CLSB_Ini->GetInt("Options", "Cameras_Count", 0, 10);
	Options->Has_Objects = App->CLSB_Ini->GetInt("Options", "Objects_Count", 0, 10);
	Options->Has_Counters = App->CLSB_Ini->GetInt("Options", "Counters_Count", 0, 10);

	//App->SBC_Scene->UniqueID_Object_Counter = App->Cl_Ini->GetInt("Options", "Objects_ID_Count", 0, 10);
	//App->SBC_Scene->UniqueID_Counters_Count = App->Cl_Ini->GetInt("Options", "Counters_ID_Count", 0, 10);
	//App->SBC_Scene->UniqueID_Area_Count = App->Cl_Ini->GetInt("Options", "Areas_ID_Count", 0, 10);

	//App->SBC_Build->GameOptions->Show_FPS = App->Cl_Ini->GetInt("Config", "Show_FPS", 0, 10);
	//App->SBC_Build->GameOptions->FullScreen = App->Cl_Ini->GetInt("Config", "Game_FullScreen", 1, 10);
	//App->SBC_Build->GameOptions->Zipped_Assets_Flag = App->Cl_Ini->GetInt("Config", "Zipped_Assets", 1, 10);
	//App->SBC_Build->GameOptions->Front_Dialog_Flag = App->Cl_Ini->GetInt("Config", "Use_Front_Dlg", 1, 10);

	////-------------------------------------- Set Resource Path

	Load_Get_Resource_Path();

	// ------------------------------------- Aera
	if (Options->Has_Area > 0)
	{
		//bool test = Load_Project_Aera();
		//App->CLSB_Scene->Build_World(0);
		
		/*App->CLSB_Mesh_Mgr->WE_Build_Brush_List(0);
		App->CLSB_Bullet->Create_Brush_Trimesh_XX(0);

		App->CLSB_Mesh_Mgr->WE_Convert_All_Texture_Groups();
		Debug*/

		//App->CLSB_Model->Set_BondingBox_Brushes();

		App->CLSB_Scene->Area_Added = 1;
	}

	// ------------------------------------- Player
	if (Options->Has_Player > 0)
	{
	//	bool test = Load_Project_Player();
	//	App->SBC_DCC->Player_CanJump = App->CL_Prefs->Prefs_PlayerCanJump_Flag;

		App->CLSB_Player->Create_Player_Object();
		App->CLSB_Properties->Update_ListView_Player();

		App->CLSB_Scene->B_Player[0]->FileViewItem = App->CLSB_FileView->Add_Item(App->CLSB_FileView->FV_Players_Folder, "Player_1", 0, false);
		App->CLSB_FileView->Set_FolderActive(App->CLSB_FileView->FV_Players_Folder);
		App->CLSB_FileView->SelectItem(App->CLSB_FileView->FV_Players_Folder);
		App->CLSB_FileView->ExpandRoot();
	}
	
	//// ------------------------------------- Camera
	//if (Options->Has_Camera > 0)
	//{
	//	Load_Project_Camera();
	//	App->SBC_Scene->Camera_Added = 1;
	//	App->SBC_Com_Camera->Set_Camera(0);
	//	App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Cameras_Folder);
	//}

	// ------------------------------------- Objects
	if (Options->Has_Objects > 0)
	{
		V_Load_Project_Objects();
		App->CLSB_Objects_Create->Add_Objects_From_File();
	}

	//// ------------------------------------- Counters
	//if (Options->Has_Counters > 0)
	//{
	//	Load_Project_Counters();
	//	App->SBC_Display->Add_Counters_From_File();
	//}


	//App->CL_Ogre->OgreListener->GD_CameraMode = Enums::CamDetached;

	//App->SBC_FileView->Change_Level_Name();
	//App->SBC_FileView->Change_Project_Name();
	//App->SBC_FileView->Redraw_FileView();

	//App->SBC_Scene->Scene_Modified = 0;

	//int Test = App->SBC_Com_Environments->Get_First_Environ();
	//if (Test == -1)
	//{
	//	App->SBC_Com_Environments->Add_New_Environ_Entity(1);
	//	int mIndex = App->SBC_Com_Environments->Get_First_Environ();
	//	App->SBC_Com_Environments->Set_First_Environment(mIndex);

	//	App->SBC_Scene->Scene_Modified = 1;
	//}
	//else
	//{
	//	App->SBC_Com_Environments->Set_First_Environment(Test);
	//}

	App->CLSB_Game_Editor->Project_Loaded = 1;
	//App->SBC_Project->Project_Loaded = 1;

	//delete Options;

	//App->Set_Main_TitleBar(App->SBC_FileIO->Project_Path_File_Name);
	//App->SBC_FileIO->RecentFileHistory_Update();
	//App->CL_Prefs->Update_User_File(App->SBC_FileIO->Project_Path_File_Name);

	//App->SBC_FileView->SelectItem(App->SBC_FileView->FV_LevelFolder);

	//EnableMenuItem(App->mMenu, ID_FILE_SAVEPROJECTALL, MF_GRAYED);

	return 1;
}

// *************************************************************************
// *	  	V_Load_Project_Objects:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool SB_Project::V_Load_Project_Objects()
{
	int Int_Tag = 0;
	char Object_Ini_Path[MAX_PATH];
	char chr_Tag1[MAX_PATH];
	int Object_Count = 0;

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	strcpy(Object_Ini_Path, App->WorldEditor_Directory);
	strcat(Object_Ini_Path, "\\Levels\\First_Project_Prj\\First_Level\\Objects\\");

	//strcat(Object_Ini_Path, m_Level_Name);
	//strcat(Object_Ini_Path, "\\");

	//strcat(Object_Ini_Path, "Objects");
	//strcat(Object_Ini_Path, "\\");

	////---------------------------------------------------

	strcat(Object_Ini_Path, "Objects.efd");

	//App->Say(Object_Ini_Path);

	App->CLSB_Ini->SetPathName(Object_Ini_Path);

	Object_Count = App->CLSB_Ini->GetInt("Counters", "Objects_Count", 0);

	//App->Say_Int(Object_Count);

	int Count = 0;
	while (Count < Object_Count)
	{
		char mNumChr[MAX_PATH] = { 0 };
		char mSection[MAX_PATH] = { 0 };

		strcpy(mSection, "Object_");
		_itoa(Count, mNumChr, 10);
		strcat(mSection, mNumChr);

		App->CLSB_Game_Editor->V_Object.push_back(new Base_Object());

		Base_Object* V_Object = App->CLSB_Game_Editor->V_Object[Count];

		App->CLSB_Ini->GetString(mSection, "Mesh_Name", V_Object->Mesh_Name, MAX_PATH);
		App->CLSB_Ini->GetString(mSection, "Mesh_File", V_Object->Mesh_FileName, MAX_PATH);
		App->CLSB_Ini->GetString(mSection, "Mesh_Resource_Path", V_Object->Mesh_Resource_Path, MAX_PATH);
		App->CLSB_Ini->GetString(mSection, "Material_File", V_Object->Material_File, MAX_PATH);

		// ------------- Types
		V_Object->This_Object_UniqueID = App->CLSB_Ini->GetInt(mSection, "Object_ID", 0);
		V_Object->Type = App->CLSB_Ini->GetInt(mSection, "Object_Type", 0);
		V_Object->Shape = App->CLSB_Ini->GetInt(mSection, "Object_Shape", 0);
		V_Object->Usage = App->CLSB_Ini->GetInt(mSection, "Object_Usage", 0);

		// ------------- Pos
		App->CLSB_Ini->GetString(mSection, "Mesh_Pos", chr_Tag1, MAX_PATH);
		sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		V_Object->Mesh_Pos = Ogre::Vector3(x, y, z);

		// ------------- Scale
		App->CLSB_Ini->GetString(mSection, "Mesh_Scale", chr_Tag1, MAX_PATH);
		sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		V_Object->Mesh_Scale = Ogre::Vector3(x, y, z);

		// ------------- Rotation
		App->CLSB_Ini->GetString(mSection, "Mesh_Rot", chr_Tag1, MAX_PATH);
		sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		V_Object->Mesh_Rot = Ogre::Vector3(x, y, z);

		// ------------- Mesh_Quat
		App->CLSB_Ini->GetString(mSection, "Mesh_Quat", chr_Tag1, MAX_PATH);
		sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

		V_Object->Mesh_Quat.w = w;
		V_Object->Mesh_Quat.x = x;
		V_Object->Mesh_Quat.y = y;
		V_Object->Mesh_Quat.z = z;

		// ------------- Physics_Quat
		App->CLSB_Ini->GetString(mSection, "Physics_Quat", chr_Tag1, MAX_PATH);
		sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

		V_Object->Physics_Quat.w = w;
		V_Object->Physics_Quat.x = x;
		V_Object->Physics_Quat.y = y;
		V_Object->Physics_Quat.z = z;

		V_Object->Dimensions_Locked = App->CLSB_Ini->GetInt(mSection, "Dimensions_Lock", 0);

		Count++;

	}

	App->CLSB_Scene->Object_Count = Count;

	return 1;
}

// *************************************************************************
// *	  		Set_Paths:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void SB_Project::Set_Paths()
{

	strcpy(m_Level_File_Name, App->CLSB_FileIO->Project_File_Name);
	strcpy(m_Project_Sub_Folder, App->CLSB_FileIO->Project_Path_File_Name);
	strcpy(m_Ini_Path_File_Name, App->CLSB_FileIO->Project_Path_File_Name);

	strcpy(m_Level_Folder_Path, App->CLSB_FileIO->Project_Path_File_Name);

	// Get path no file 
	int len1 = strlen(m_Level_File_Name);
	int len2 = strlen(m_Project_Sub_Folder);
	strcpy(m_Project_Sub_Folder, m_Project_Sub_Folder);
	m_Project_Sub_Folder[len2 - (len1 + 1)] = 0;

	//GetFileTitleA(App->CL_Equity_SB->Pref_WE_Path_FileName, JustFileName, MAX_PATH);
}

// *************************************************************************
// *	  	Load_Get_Resource_Path:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool SB_Project::Load_Get_Resource_Path()
{
	
	m_Main_Assets_Path[0] = 0;

	strcpy(m_Main_Assets_Path, m_Project_Sub_Folder);
	strcat(m_Main_Assets_Path, "\\");

	strcat(m_Main_Assets_Path, m_Level_Name);
	strcat(m_Main_Assets_Path, "\\");

	strcat(m_Main_Assets_Path, "Assets");
	strcat(m_Main_Assets_Path, "\\");

	App->CLSB_Scene->Add_Resource_Location_Project(m_Main_Assets_Path);

	return 1;
}

// *************************************************************************
// *	  		Save_Project:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool SB_Project::Save_Project()
{
	if (_mkdir(m_Project_Sub_Folder) == 0)
	{
		_chdir(m_Project_Sub_Folder);
	}
	else
	{
		_chdir(m_Project_Sub_Folder);
	}

	bool test = Save_Project_Ini();
	if (test == 0)
	{
		return 0;
	}

	Save_Level_Folder();
	Save_Main_Asset_Folder();

	_chdir(m_Level_Folder_Path);

	if (App->CLSB_Scene->Area_Added == 1)
	{
		Save_Aera_Folder();
	}

	if (App->CLSB_Scene->Player_Added == 1)
	{
//		Save_Players_Folder();
	}

//	Save_Cameras_Folder();
//	Save_Objects_Folder();
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
// *	  		Save_Project_Ini:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool SB_Project::Save_Project_Ini()
{
	m_Ini_Path_File_Name[0] = 0;

	strcpy(m_Ini_Path_File_Name, m_Project_Sub_Folder);
	strcat(m_Ini_Path_File_Name, "\\");
	strcat(m_Ini_Path_File_Name, "Project.SBProj");

	int test = App->CLSB_FileIO->SearchFolders(m_Project_Sub_Folder, "\\Project.SBProj");
	if (test == 1)
	{
		App->CLSB_Dialogs->YesNo("File Exsits", "Do you want to update File");

		bool Doit = App->CLSB_Dialogs->Canceled;
		if (Doit == 1)
		{
			return 0;
		}
	}

	WriteFile = nullptr;

	WriteFile = fopen(m_Ini_Path_File_Name, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.2");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Files]");
	fprintf(WriteFile, "%s%s\n", "Project_Name=", App->CLSB_Project->m_Project_Name);
	fprintf(WriteFile, "%s%s\n", "Level_Name=", App->CLSB_Project->m_Level_Name);
	fprintf(WriteFile, "%s%s\n", "Game_Name=", App->CLSB_Build->GameName);

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Options]");
	fprintf(WriteFile, "%s%i\n", "Areas_Count=", App->CLSB_Scene->Area_Count);
	fprintf(WriteFile, "%s%i\n", "Areas_ID_Count=", App->CLSB_Scene->UniqueID_Area_Count);

	fprintf(WriteFile, "%s%i\n", "Players_Count=", App->CLSB_Scene->Player_Count);
	fprintf(WriteFile, "%s%i\n", "Cameras_Count=", App->CLSB_Scene->Camera_Count);
	fprintf(WriteFile, "%s%i\n", "Objects_Count=", App->CLSB_Scene->Object_Count);
	fprintf(WriteFile, "%s%i\n", "Objects_ID_Count=", App->CLSB_Scene->UniqueID_Object_Counter);


	int Adjusted = App->CLSB_Scene->Get_Adjusted_Counters_Count();

	fprintf(WriteFile, "%s%i\n", "Counters_Count=", Adjusted);
	fprintf(WriteFile, "%s%i\n", "Counters_ID_Count=", App->CLSB_Scene->UniqueID_Counters_Count);

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Config]");
	fprintf(WriteFile, "%s%i\n", "Show_FPS=", App->CLSB_Build->GameOptions->Show_FPS);
	fprintf(WriteFile, "%s%i\n", "Game_FullScreen=", App->CLSB_Build->GameOptions->FullScreen);
	fprintf(WriteFile, "%s%i\n", "Zipped_Assets=", App->CLSB_Build->GameOptions->Zipped_Assets_Flag);
	fprintf(WriteFile, "%s%i\n", "Use_Front_Dlg=", App->CLSB_Build->GameOptions->Front_Dialog_Flag);

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Save_Level_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool SB_Project::Save_Level_Folder()
{
	strcpy(m_Level_Folder_Path, m_Project_Sub_Folder);
	strcat(m_Level_Folder_Path, "\\");
	strcat(m_Level_Folder_Path, m_Level_Name);

	// First Level Folder
	if (_mkdir(m_Level_Folder_Path) == 0)
	{
		_chdir(m_Level_Folder_Path);
	}
	else
	{
		_chdir(m_Level_Folder_Path);
	}

	return 1;
}

// *************************************************************************
// *	  	Save_Main_Asset_Folder:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool SB_Project::Save_Main_Asset_Folder()
{
	char LastFolder[MAX_PATH];

	if (Directory_Changed_Flag == 1)
	{
		strcpy(LastFolder, m_Main_Assets_Path);
	}

	m_Main_Assets_Path[0] = 0;

	strcpy(m_Main_Assets_Path, m_Level_Folder_Path);
	strcat(m_Main_Assets_Path, "\\");
	strcat(m_Main_Assets_Path, "Assets");
	strcat(m_Main_Assets_Path, "\\");

	if (_mkdir(m_Main_Assets_Path) == 0)
	{
		_chdir(m_Main_Assets_Path);
	}
	else
	{
		_chdir(m_Main_Assets_Path);
	}

	if (Directory_Changed_Flag == 1)
	{
		Copy_Assets(LastFolder, m_Main_Assets_Path);
	}

	Directory_Changed_Flag = 0;

	_chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  		Copy_Assets:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool SB_Project::Copy_Assets(char* SourceFolder, char* DestinationFolder)
{
	char SourceFile[MAX_PATH];
	char DestinationFile[MAX_PATH];

	char Path[MAX_PATH];
	strcpy(Path, SourceFolder);
	strcat(Path, "*.*");

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(Path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {

			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{

				strcpy(SourceFile, SourceFolder);
				strcat(SourceFile, fd.cFileName);

				strcpy(DestinationFile, DestinationFolder);
				strcat(DestinationFile, fd.cFileName);

				CopyFile(SourceFile, DestinationFile, false);
			}

		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	return 1;
}

// *************************************************************************
// *	  	Save_Aera_Folder:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
bool SB_Project::Save_Aera_Folder()
{
	m_Aera_Folder_Path[0] = 0;

	strcpy(m_Aera_Folder_Path, m_Level_Folder_Path);
	strcat(m_Aera_Folder_Path, "\\");
	strcat(m_Aera_Folder_Path, "Areas");


	if (_mkdir(m_Aera_Folder_Path) == 0)
	{
		_chdir(m_Aera_Folder_Path);
	}
	else
	{
		_chdir(m_Aera_Folder_Path);
	}

	Save_Aeras_Data();

	_chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  		Save_Aeras_Data:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool SB_Project::Save_Aeras_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	strcpy(File, m_Aera_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Areas.aer");

	WriteFile = nullptr;

	WriteFile = fopen(File, "wt");

	if (!WriteFile)
	{
		App->Say("Cant Create File");
		App->Say(File);
		return 0;
	}

	fprintf(WriteFile, "%s\n", "[Version_Data]");
	fprintf(WriteFile, "%s%s\n", "Version=", "V1.2");

	fprintf(WriteFile, "%s\n", " ");

	fprintf(WriteFile, "%s\n", "[Counters]");
	fprintf(WriteFile, "%s%i\n", "Areas_Count=", App->CLSB_Scene->Area_Count);

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	int Count = 0;
	while (Count < App->CLSB_Scene->Area_Count)
	{
		strcpy(buff, "[Area_");
		_itoa(Count, Cbuff, 10);
		strcat(buff, Cbuff);
		strcat(buff, "]");

		fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

		//fprintf(WriteFile, "%s%s\n", "Area_Name=", App->CLSB_Scene->B_Area[Count]->Area_Name); // Change

		//fprintf(WriteFile, "%s%s\n", "Area_File=", App->CLSB_Scene->B_Area[Count]->Area_FileName);
		//fprintf(WriteFile, "%s%s\n", "Area_Path_File=", App->CLSB_Scene->B_Area[Count]->Area_Path_And_FileName);
		//fprintf(WriteFile, "%s%s\n", "Area_Resource_Path=", App->CLSB_Scene->B_Area[Count]->Area_Resource_Path);
		//fprintf(WriteFile, "%s%s\n", "Material_File=", App->CLSB_Scene->B_Area[Count]->Material_File);
		//fprintf(WriteFile, "%s%i\n", "Area_Object_ID=", App->CLSB_Scene->B_Area[Count]->This_Object_UniqueID);

		// ------------ Position
//		x = App->CLSB_Scene->B_Area[Count]->Area_Node->getPosition().x;
//		y = App->CLSB_Scene->B_Area[Count]->Area_Node->getPosition().y;
//		z = App->CLSB_Scene->B_Area[Count]->Area_Node->getPosition().z;
		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Pos=", x, y, z);

		// ------------ Scale
//		x = App->CLSB_Scene->B_Area[Count]->Mesh_Scale.x;
//		y = App->CLSB_Scene->B_Area[Count]->Mesh_Scale.y;
//		z = App->CLSB_Scene->B_Area[Count]->Mesh_Scale.z;
		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Scale=", x, y, z);

		// ------------ Mesh_Rot
//		x = App->CLSB_Scene->B_Area[Count]->Mesh_Rot.x;
//		y = App->CLSB_Scene->B_Area[Count]->Mesh_Rot.y;
//		z = App->CLSB_Scene->B_Area[Count]->Mesh_Rot.z;
		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Rot=", x, y, z);

		// ------------ Mesh_Quat
//		w = App->CLSB_Scene->B_Area[Count]->Mesh_Quat.w;
//		x = App->CLSB_Scene->B_Area[Count]->Mesh_Quat.x;
//		y = App->CLSB_Scene->B_Area[Count]->Mesh_Quat.y;
//		z = App->CLSB_Scene->B_Area[Count]->Mesh_Quat.z;
		fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Mesh_Quat=", w, x, y, z);


		fprintf(WriteFile, "%s\n", " ");

		Count++;
	}

	fclose(WriteFile);

	return 1;
}
