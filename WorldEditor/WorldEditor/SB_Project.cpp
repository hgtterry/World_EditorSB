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

	App->CLSB_Scene_Data->Clear_Level();
	App->CLSB_Scene_Data->Create_Resources_Group();

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

		App->CLSB_Scene_Data->Area_Added = 1;
	}

	// ------------------------------------- Player
	if (Options->Has_Player > 0)
	{
	//	bool test = Load_Project_Player();
	//	App->SBC_DCC->Player_CanJump = App->CL_Prefs->Prefs_PlayerCanJump_Flag;

		App->CLSB_Player->Create_Player_Object();
		App->CLSB_Properties->Update_ListView_Player();
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
		Load_Project_Objects();
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
// *	  	Load_Project_Objects:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool SB_Project::Load_Project_Objects()
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
		(void) sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		V_Object->Mesh_Pos = Ogre::Vector3(x, y, z);

		// ------------- Scale
		App->CLSB_Ini->GetString(mSection, "Mesh_Scale", chr_Tag1, MAX_PATH);
		(void) sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		V_Object->Mesh_Scale = Ogre::Vector3(x, y, z);

		// ------------- Rotation
		App->CLSB_Ini->GetString(mSection, "Mesh_Rot", chr_Tag1, MAX_PATH);
		(void) sscanf(chr_Tag1, "%f,%f,%f", &x, &y, &z);
		V_Object->Mesh_Rot = Ogre::Vector3(x, y, z);

		// ------------- Mesh_Quat
		App->CLSB_Ini->GetString(mSection, "Mesh_Quat", chr_Tag1, MAX_PATH);
		(void) sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

		V_Object->Mesh_Quat.w = w;
		V_Object->Mesh_Quat.x = x;
		V_Object->Mesh_Quat.y = y;
		V_Object->Mesh_Quat.z = z;

		// ------------- Physics_Quat
		App->CLSB_Ini->GetString(mSection, "Physics_Quat", chr_Tag1, MAX_PATH);
		(void) sscanf(chr_Tag1, "%f,%f,%f,%f", &w, &x, &y, &z);

		V_Object->Physics_Quat.w = w;
		V_Object->Physics_Quat.x = x;
		V_Object->Physics_Quat.y = y;
		V_Object->Physics_Quat.z = z;

		V_Object->Dimensions_Locked = App->CLSB_Ini->GetInt(mSection, "Dimensions_Lock", 0);

		Count++;

	}

	App->CLSB_Scene_Data->Object_Count = Count;

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

	App->CLSB_Scene_Data->Add_Resource_Location_Project(m_Main_Assets_Path);

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

	//if (App->CLSB_Scene_Data->Area_Added == 1)
	{
		Save_Aera_Folder();
	}

	if (App->CLSB_Scene_Data->Player_Added == 1)
	{
		Save_Players_Folder();
	}

//	Save_Cameras_Folder();
	Save_Objects_Folder();
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
	fprintf(WriteFile, "%s%i\n", "Areas_Count=", App->CLSB_Scene_Data->Area_Count);
	fprintf(WriteFile, "%s%i\n", "Areas_ID_Count=", App->CLSB_Scene_Data->UniqueID_Area_Count);

	fprintf(WriteFile, "%s%i\n", "Players_Count=", App->CLSB_Scene_Data->Player_Count);
	fprintf(WriteFile, "%s%i\n", "Cameras_Count=", App->CLSB_Scene_Data->Camera_Count);
	fprintf(WriteFile, "%s%i\n", "Objects_Count=", App->CLSB_Scene_Data->Object_Count);
	fprintf(WriteFile, "%s%i\n", "Objects_ID_Count=", App->CLSB_Scene_Data->UniqueID_Object_Counter);


	int Adjusted = App->CLSB_Scene_Data->Get_Adjusted_Counters_Count();

	fprintf(WriteFile, "%s%i\n", "Counters_Count=", Adjusted);
	fprintf(WriteFile, "%s%i\n", "Counters_ID_Count=", App->CLSB_Scene_Data->UniqueID_Counters_Count);

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

	App->CLSB_Project_Create->Create_New_Area();
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
	strcat(File, "Areas.adat");

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
	fprintf(WriteFile, "%s%i\n", "Areas_Count=", App->CLSB_Scene_Data->Area_Count);

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	int Count = 0;
	while (Count < App->CLSB_Scene_Data->Area_Count)
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

// *************************************************************************
// *	  	Save_Objects_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool SB_Project::Save_Objects_Folder()
{
	m_Objects_Folder_Path[0] = 0;

	strcpy(m_Objects_Folder_Path, m_Level_Folder_Path);
	strcat(m_Objects_Folder_Path, "\\");
	strcat(m_Objects_Folder_Path, "Objects");

	_mkdir(m_Objects_Folder_Path);
	_chdir(m_Objects_Folder_Path);

	Save_Objects_Data();

	_chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  		Save_Objects_Data:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool SB_Project::Save_Objects_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	strcpy(File, m_Objects_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Objects.odat");

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

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	int new_Count = 0;

	int Count = 0;
	while (Count < App->CLSB_Scene_Data->Object_Count)
	{
		if (App->CLSB_Game_Editor->V_Object[Count]->Deleted == 0)
		{
			strcpy(buff, "[Object_");
			_itoa(new_Count, Cbuff, 10);
			strcat(buff, Cbuff);
			strcat(buff, "]");

			fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

			fprintf(WriteFile, "%s%s\n", "Mesh_Name=", App->CLSB_Game_Editor->V_Object[Count]->Mesh_Name); // Change

			fprintf(WriteFile, "%s%s\n", "Mesh_File=", App->CLSB_Game_Editor->V_Object[Count]->Mesh_FileName);
			fprintf(WriteFile, "%s%s\n", "Mesh_Resource_Path=", App->CLSB_Game_Editor->V_Object[Count]->Mesh_Resource_Path);
			fprintf(WriteFile, "%s%s\n", "Material_File=", App->CLSB_Game_Editor->V_Object[Count]->Material_File);
			fprintf(WriteFile, "%s%i\n", "Object_ID=", App->CLSB_Game_Editor->V_Object[Count]->This_Object_UniqueID);
			fprintf(WriteFile, "%s%i\n", "Object_Type=", App->CLSB_Game_Editor->V_Object[Count]->Type);
			fprintf(WriteFile, "%s%i\n", "Object_Shape=", App->CLSB_Game_Editor->V_Object[Count]->Shape);
			fprintf(WriteFile, "%s%i\n", "Object_Usage=", App->CLSB_Game_Editor->V_Object[Count]->Usage);

			// Position
			x = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Pos.x;
			y = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Pos.y;
			z = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Pos.z;
			fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Pos=", x, y, z);

			// Mesh_Scale
			x = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Scale.x;
			y = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Scale.y;
			z = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Scale.z;
			fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Scale=", x, y, z);

			// Mesh_Rot
			x = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Rot.x;
			y = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Rot.y;
			z = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Rot.z;
			fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Rot=", x, y, z);

			// Mesh_Quat
			w = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Quat.w;
			x = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Quat.x;
			y = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Quat.y;
			z = App->CLSB_Game_Editor->V_Object[Count]->Mesh_Quat.z;
			fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Mesh_Quat=", w, x, y, z);

			// Physics_Quat
			w = App->CLSB_Game_Editor->V_Object[Count]->Physics_Quat.w;
			x = App->CLSB_Game_Editor->V_Object[Count]->Physics_Quat.x;
			y = App->CLSB_Game_Editor->V_Object[Count]->Physics_Quat.y;
			z = App->CLSB_Game_Editor->V_Object[Count]->Physics_Quat.z;
			fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Physics_Quat=", w, x, y, z);

			fprintf(WriteFile, "%s%i\n", "Dimensions_Lock=", App->CLSB_Game_Editor->V_Object[Count]->Dimensions_Locked);

			//---------------------------------------------------------------------------------- Message Entity
			if (App->CLSB_Game_Editor->V_Object[Count]->Usage == Enums::Usage_Message)
			{
				//fprintf(WriteFile, "%s\n", "-- Message");
				//fprintf(WriteFile, "%s%s\n", "Message_Text=", App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Message_Text);

				//x = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Message_PosX;
				//y = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Message_PosY;
				//fprintf(WriteFile, "%s%f,%f\n", "Message_Pos=", x, y);

				//// Message Counter
				//fprintf(WriteFile, "%s%i\n", "Message_Counter_ID=", App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Counter_ID);
				//fprintf(WriteFile, "%s%i\n", "Message_Trigger_Value=", App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Trigger_Value);
				//fprintf(WriteFile, "%s%i\n", "Message_Counter_Disabled=", App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Counter_Disabled);

				//fprintf(WriteFile, "%s%i\n", "Message_CentreX=", App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->PosXCentre_Flag);
				//fprintf(WriteFile, "%s%i\n", "Message_CentreY=", App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->PosYCentre_Flag);

				//x = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Text_Colour.x;
				//y = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Text_Colour.y;
				//z = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Text_Colour.z;
				//w = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->Text_Colour.w;
				//fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Message_Text_Colour=", x, y, z, w);

				//x = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->BackGround_Colour.x;
				//y = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->BackGround_Colour.y;
				//z = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->BackGround_Colour.z;
				//w = App->CLSB_Game_Editor->V_Object[Count]->S_Message[0]->BackGround_Colour.w;
				//fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Message_BackGround_Colour=", x, y, z, w);

				//fprintf(WriteFile, "%s%i\n", "Message_Show_BackGround=", App->SBC_Scene->V_Object[Count]->S_Message[0]->Show_BackGround);

			}

			//---------------------------------------------------------------------------------- Sound Entity
			if (App->CLSB_Game_Editor->V_Object[Count]->Usage == Enums::Usage_Sound)
			{
				/*fprintf(WriteFile, "%s%s\n", "Sound_File=", App->SBC_Scene->V_Object[Count]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Sound_Volume=", App->SBC_Scene->V_Object[Count]->SndVolume);*/
			}

			//---------------------------------------------------------------------------------- Colectable Entity
			if (App->CLSB_Game_Editor->V_Object[Count]->Usage == Enums::Usage_Colectable)
			{
				/*fprintf(WriteFile, "%s\n", "-- Colectable");
				fprintf(WriteFile, "%s%s\n", "Col_Sound_File=", App->CLSB_Game_Editor->V_Object[Count]->S_Collectable[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Col_Sound_Volume=", App->CLSB_Game_Editor->V_Object[Count]->S_Collectable[0]->SndVolume);
				fprintf(WriteFile, "%s%i\n", "Col_Play=", App->CLSB_Game_Editor->V_Object[Count]->S_Collectable[0]->Play);
				fprintf(WriteFile, "%s%s\n", "Col_Counter_Name=", App->CLSB_Game_Editor->V_Object[Count]->S_Collectable[0]->Counter_Name);
				fprintf(WriteFile, "%s%i\n", "Col_Counter_ID=", App->CLSB_Game_Editor->V_Object[Count]->S_Collectable[0]->Counter_ID);
				fprintf(WriteFile, "%s%i\n", "Col_Maths=", App->CLSB_Game_Editor->V_Object[Count]->S_Collectable[0]->Maths);
				fprintf(WriteFile, "%s%i\n", "Col_Value=", App->CLSB_Game_Editor->V_Object[Count]->S_Collectable[0]->Value);
				fprintf(WriteFile, "%s%i\n", "Col_Disabled=", App->CLSB_Game_Editor->V_Object[Count]->S_Collectable[0]->Counter_Disabled);*/
			}

			//---------------------------------------------------------------------------------- Move Entity
			if (App->CLSB_Game_Editor->V_Object[Count]->Usage == Enums::Usage_Move)
			{
				//fprintf(WriteFile, "%s\n", "-- Move Entity");
				//fprintf(WriteFile, "%s%f\n", "Move_Distance=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Move_Distance);
				//fprintf(WriteFile, "%s%i\n", "Move_IsNegative=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->IsNegative);

				//fprintf(WriteFile, "%s%f\n", "Move_NewPos=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Newpos);
				//fprintf(WriteFile, "%s%i\n", "Move_ObjectID=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Object_To_Move_Index);
				//fprintf(WriteFile, "%s%s\n", "Move_ObjectName=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Object_Name);
				//fprintf(WriteFile, "%s%i\n", "Move_Re_Trigger=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Re_Trigger);
				//fprintf(WriteFile, "%s%f\n", "Move_Speed=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Speed);
				//fprintf(WriteFile, "%s%i\n", "Move_Triggered=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Triggered);
				//fprintf(WriteFile, "%s%i\n", "Move_WhatDirection=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->WhatDirection);

				//// Move Sound
				//fprintf(WriteFile, "%s%s\n", "Move_Sound=", App->CLSB_Game_Editor->V_Object[Count]->Sound_File);
				//fprintf(WriteFile, "%s%i\n", "Move_Play_Sound=", App->CLSB_Game_Editor->V_Object[Count]->Play_Sound);
				//fprintf(WriteFile, "%s%f\n", "Move_Volume=", App->CLSB_Game_Editor->V_Object[Count]->SndVolume);

				//// Move Counter
				//fprintf(WriteFile, "%s%i\n", "Move_Counter_ID=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Counter_ID);
				//fprintf(WriteFile, "%s%i\n", "Move_Trigger_Value=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Trigger_Value);
				//fprintf(WriteFile, "%s%i\n", "Move_Counter_Disabled=", App->CLSB_Game_Editor->V_Object[Count]->S_MoveType[0]->Counter_Disabled);
			}

			//---------------------------------------------------------------------------------- Teleport Entity
			if (App->CLSB_Game_Editor->V_Object[Count]->Usage == Enums::Usage_Teleport)
			{
				/*fprintf(WriteFile, "%s\n", "-- Teleport");
				fprintf(WriteFile, "%s%s\n", "Tele_Goto=", App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Name);

				fprintf(WriteFile, "%s%i\n", "Tele_ID=", App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Location_ID);

				fprintf(WriteFile, "%s%s\n", "Tele_Sound=", App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Sound_File);
				fprintf(WriteFile, "%s%f\n", "Tele_Volume=", App->SBC_Scene->V_Object[Count]->S_Teleport[0]->SndVolume);
				fprintf(WriteFile, "%s%i\n", "Tele_Play=", App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Play);

				x = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Player_Position.x;
				y = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Player_Position.y;
				z = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Player_Position.z;
				fprintf(WriteFile, "%s%f,%f,%f\n", "Tele_Mesh_Position=", x, y, z);

				x = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Physics_Position.getX();
				y = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Physics_Position.getY();
				z = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Physics_Position.getZ();
				fprintf(WriteFile, "%s%f,%f,%f\n", "Tele_Physics_Position=", x, y, z);

				w = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Physics_Rotation.getW();
				x = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Physics_Rotation.getX();
				y = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Physics_Rotation.getY();
				z = App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Physics_Rotation.getZ();
				fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Tele_Physics_Rotation=", w, x, y, z);*/

				// Teleport Counter
				//fprintf(WriteFile, "%s%i\n", "Tele_Counter_ID=", App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Counter_ID);
				//fprintf(WriteFile, "%s%i\n", "Tele_Trigger_Value=", App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Trigger_Value);
				//fprintf(WriteFile, "%s%i\n", "Tele_Counter_Disabled=", App->SBC_Scene->V_Object[Count]->S_Teleport[0]->Counter_Disabled);

				////--------------- Environment
				//fprintf(WriteFile, "%s\n", "------------------------------------------------------------------------------ Teleporter Environ");
				//fprintf(WriteFile, "%s%i\n", "Environ_Enabled=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Environ_Enabled);
				//fprintf(WriteFile, "%s%s\n", "Environment_Name=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Environment_Name);
				//fprintf(WriteFile, "%s%i\n", "Environment_ID=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Environment_ID);

				////--------------- Sound
				//fprintf(WriteFile, "%s%s\n", "Sound_File=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Sound_File);
				//fprintf(WriteFile, "%s%f\n", "Snd_Volume=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->SndVolume);

				//fprintf(WriteFile, "%s%i\n", "Sound_Play=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Play);
				//fprintf(WriteFile, "%s%i\n", "Sound_Loop=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Loop);

				////--------------- Light

				//x = App->SBC_Scene->V_Object[Count]->S_Environ[0]->AmbientColour.x;
				//y = App->SBC_Scene->V_Object[Count]->S_Environ[0]->AmbientColour.y;
				//z = App->SBC_Scene->V_Object[Count]->S_Environ[0]->AmbientColour.z;
				//fprintf(WriteFile, "%s%f,%f,%f\n", "Ambient_Colour=", x, y, z);

				//x = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Light_Position.x;
				//y = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Light_Position.y;
				//z = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Light_Position.z;
				//fprintf(WriteFile, "%s%f,%f,%f\n", "Light_Position=", x, y, z);

				////--------------- Sky
				//fprintf(WriteFile, "%s%i\n", "Sky_Enable=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Enabled);
				//fprintf(WriteFile, "%s%i\n", "Sky_Type=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->type);
				//fprintf(WriteFile, "%s%s\n", "Sky_Material=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Material);
				//fprintf(WriteFile, "%s%f\n", "Sky_Curvature=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Curvature);
				//fprintf(WriteFile, "%s%f\n", "Sky_Tiling=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Tiling);
				//fprintf(WriteFile, "%s%f\n", "Sky_Distance=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Distance);

				////--------------- Fog
				//fprintf(WriteFile, "%s%i\n", "Fog_On=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_On);
				//fprintf(WriteFile, "%s%i\n", "Fog_Mode=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Mode);

				//x = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Colour.x;
				//y = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Colour.y;
				//z = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Colour.z;
				//fprintf(WriteFile, "%s%f,%f,%f\n", "Fog_Colour=", x, y, z);

				//fprintf(WriteFile, "%s%f\n", "Fog_Start=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Start);
				//fprintf(WriteFile, "%s%f\n", "Fog_End=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_End);
				//fprintf(WriteFile, "%s%f\n", "Fog_Density=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Density);

			}

			//---------------------------------------------------------------------------------- Environ Entity
			if (App->CLSB_Game_Editor->V_Object[Count]->Usage == Enums::Usage_EnvironEntity)
			{
				//fprintf(WriteFile, "%s\n", "--------------------------------------------------------------------------------- EnvironEntity");
				//fprintf(WriteFile, "%s%s\n", "Environment_Name=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Environment_Name);
				//fprintf(WriteFile, "%s%i\n", "Environment_ID=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Environment_ID);

				////--------------- Sound
				//fprintf(WriteFile, "%s%s\n", "Sound_File=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Sound_File);
				//fprintf(WriteFile, "%s%f\n", "Snd_Volume=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->SndVolume);

				//fprintf(WriteFile, "%s%i\n", "Sound_Play=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Play);
				//fprintf(WriteFile, "%s%i\n", "Sound_Loop=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Loop);

				////--------------- Light

				//x = App->SBC_Scene->V_Object[Count]->S_Environ[0]->AmbientColour.x;
				//y = App->SBC_Scene->V_Object[Count]->S_Environ[0]->AmbientColour.y;
				//z = App->SBC_Scene->V_Object[Count]->S_Environ[0]->AmbientColour.z;
				//fprintf(WriteFile, "%s%f,%f,%f\n", "Ambient_Colour=", x, y, z);

				//x = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Light_Position.x;
				//y = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Light_Position.y;
				//z = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Light_Position.z;
				//fprintf(WriteFile, "%s%f,%f,%f\n", "Light_Position=", x, y, z);

				////--------------- Sky
				//fprintf(WriteFile, "%s%i\n", "Sky_Enable=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Enabled);
				//fprintf(WriteFile, "%s%i\n", "Sky_Type=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->type);
				//fprintf(WriteFile, "%s%s\n", "Sky_Material=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Material);
				//fprintf(WriteFile, "%s%f\n", "Sky_Curvature=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Curvature);
				//fprintf(WriteFile, "%s%f\n", "Sky_Tiling=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Tiling);
				//fprintf(WriteFile, "%s%f\n", "Sky_Distance=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Distance);

				////--------------- Fog
				//fprintf(WriteFile, "%s%i\n", "Fog_On=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_On);
				//fprintf(WriteFile, "%s%i\n", "Fog_Mode=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Mode);

				//x = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Colour.x;
				//y = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Colour.y;
				//z = App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Colour.z;
				//fprintf(WriteFile, "%s%f,%f,%f\n", "Fog_Colour=", x, y, z);

				//fprintf(WriteFile, "%s%f\n", "Fog_Start=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Start);
				//fprintf(WriteFile, "%s%f\n", "Fog_End=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_End);
				//fprintf(WriteFile, "%s%f\n", "Fog_Density=", App->SBC_Scene->V_Object[Count]->S_Environ[0]->Fog_Density);
			}

			//---------------------------------------------------------------------------------- Particle
			if (App->CLSB_Game_Editor->V_Object[Count]->Usage == Enums::Usage_Particle)
			{
				/*fprintf(WriteFile, "%s\n", "-- Particle");
				fprintf(WriteFile, "%s%s\n", "Particle_Script=", App->SBC_Scene->V_Object[Count]->S_Particle[0]->ParticleScript);
				fprintf(WriteFile, "%s%f\n", "Particle_SpeedFactor=", App->SBC_Scene->V_Object[Count]->S_Particle[0]->SpeedFactor);*/

			}

			fprintf(WriteFile, "%s\n", " ");
			fprintf(WriteFile, "%s\n", "*************************************************************************************");

			new_Count++;
		}

		Count++;
	}

	fprintf(WriteFile, "%s\n", "[Counters]");
	fprintf(WriteFile, "%s%i\n", "Objects_Count=", new_Count);

	fclose(WriteFile);

	return 1;
}

// *************************************************************************
// *	  	Save_Players_Folder:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool SB_Project::Save_Players_Folder()
{
	m_Players_Folder_Path[0] = 0;

	strcpy(m_Players_Folder_Path, m_Level_Folder_Path);
	strcat(m_Players_Folder_Path, "\\");
	strcat(m_Players_Folder_Path, "Players");


	_mkdir(m_Players_Folder_Path);

	_chdir(m_Players_Folder_Path);

	Save_Player_Data();

	_chdir(m_Level_Folder_Path); // Return to Level Folder
	return 1;
}

// *************************************************************************
// *	  	Save_Player_Data:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool SB_Project::Save_Player_Data()
{
	Ogre::Vector3 Pos;
	char File[1024];

	float W = 0;
	float X = 0;
	float Y = 0;
	float Z = 0;

	strcpy(File, m_Players_Folder_Path);
	strcat(File, "\\");
	strcat(File, "Players.pdat");

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
	fprintf(WriteFile, "%s%i\n", "Player_Count=", App->CLSB_Scene_Data->Player_Count);

	fprintf(WriteFile, "%s\n", " ");

	char Cbuff[255];
	char buff[255];
	int Count = 0;
	while (Count < App->CLSB_Scene_Data->Player_Count)
	{
		strcpy(buff, "[Player_");
		_itoa(Count, Cbuff, 10);
		strcat(buff, Cbuff);
		strcat(buff, "]");

		fprintf(WriteFile, "%s\n", buff); // Header also Player name until changed by user

		fprintf(WriteFile, "%s%s\n", "Player_Name=", App->CLSB_Scene_Data->B_Player[Count]->Player_Name);

		Pos.x = App->CLSB_Scene_Data->B_Player[Count]->StartPos.x;
		Pos.y = App->CLSB_Scene_Data->B_Player[Count]->StartPos.y;
		Pos.z = App->CLSB_Scene_Data->B_Player[Count]->StartPos.z;
		fprintf(WriteFile, "%s%f,%f,%f\n", "Start_Position=", Pos.x, Pos.y, Pos.z);

		W = App->CLSB_Scene_Data->B_Player[Count]->Physics_Rotation.getW();
		X = App->CLSB_Scene_Data->B_Player[Count]->Physics_Rotation.getX();
		Y = App->CLSB_Scene_Data->B_Player[Count]->Physics_Rotation.getY();
		Z = App->CLSB_Scene_Data->B_Player[Count]->Physics_Rotation.getZ();

		fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Start_Rotation=", W, X, Y, Z);

		fprintf(WriteFile, "%s%s\n", "Shape=", "Capsule");
		fprintf(WriteFile, "%s%f\n", "Mass=", App->CLSB_Scene_Data->B_Player[Count]->Capsule_Mass);
		fprintf(WriteFile, "%s%f\n", "Radius=", App->CLSB_Scene_Data->B_Player[Count]->Capsule_Radius);
		fprintf(WriteFile, "%s%f\n", "Height=", App->CLSB_Scene_Data->B_Player[Count]->Capsule_Height);
		fprintf(WriteFile, "%s%f\n", "Ground_Speed=", App->CLSB_Scene_Data->B_Player[Count]->Ground_speed);
		fprintf(WriteFile, "%s%f\n", "Cam_Height=", App->CLSB_Scene_Data->B_Player[Count]->PlayerHeight);
		fprintf(WriteFile, "%s%f\n", "Turn_Rate=", App->CLSB_Scene_Data->B_Player[Count]->TurnRate);
		fprintf(WriteFile, "%s%f\n", "Limit_Look_Up=", App->CLSB_Scene_Data->B_Player[Count]->Limit_Look_Up);
		fprintf(WriteFile, "%s%f\n", "Limit_Look_Down=", App->CLSB_Scene_Data->B_Player[Count]->Limit_Look_Down);
		fprintf(WriteFile, "%s%f\n", "Player_Height=", App->CLSB_Scene_Data->B_Player[Count]->PlayerHeight);

		Count++;
	}

	// ---------------------------------------- Player Locations

	float w = 0;
	float x = 0;
	float y = 0;
	float z = 0;

	fprintf(WriteFile, "%s\n", " ");
	fprintf(WriteFile, "%s\n", "[Locations]");
	
	//int RealCount = App->Cl_LookUps->Player_Location_GetCount(); // Get The real Count Minus Deleted Files

	//fprintf(WriteFile, "%s%i\n", "Locations_Count=", RealCount);

	//int Location = 0; // Correct for Deleted Files

	//Count = 0;
	//while (Count < App->CLSB_Scene_Data->Player_Location_Count)
	//{
	//	if (App->SBC_Scene->B_Locations[Count]->Deleted == 0)
	//	{
	//		fprintf(WriteFile, "%s\n", " ");

	//		char Cbuff[255];
	//		char buff[255];
	//		strcpy(buff, "[Location_");
	//		_itoa(Location, Cbuff, 10);
	//		strcat(buff, Cbuff);
	//		strcat(buff, "]");
	//		fprintf(WriteFile, "%s\n", buff);

	//		fprintf(WriteFile, "%s%i\n", "Locatoin_ID=", App->SBC_Scene->B_Locations[Count]->This_Object_UniqueID);
	//		fprintf(WriteFile, "%s%s\n", "Name=", App->SBC_Scene->B_Locations[Count]->Name);

	//		x = App->SBC_Scene->B_Locations[Count]->Current_Position.x;
	//		y = App->SBC_Scene->B_Locations[Count]->Current_Position.y;
	//		z = App->SBC_Scene->B_Locations[Count]->Current_Position.z;
	//		fprintf(WriteFile, "%s%f,%f,%f\n", "Mesh_Position=", x, y, z);

	//		x = App->SBC_Scene->B_Locations[Count]->Physics_Position.getX();
	//		y = App->SBC_Scene->B_Locations[Count]->Physics_Position.getY();
	//		z = App->SBC_Scene->B_Locations[Count]->Physics_Position.getZ();
	//		fprintf(WriteFile, "%s%f,%f,%f\n", "Physics_Position=", x, y, z);

	//		w = App->SBC_Scene->B_Locations[Count]->Physics_Rotation.getW();
	//		x = App->SBC_Scene->B_Locations[Count]->Physics_Rotation.getX();
	//		y = App->SBC_Scene->B_Locations[Count]->Physics_Rotation.getY();
	//		z = App->SBC_Scene->B_Locations[Count]->Physics_Rotation.getZ();
	//		fprintf(WriteFile, "%s%f,%f,%f,%f\n", "Physics_Rotation=", w, x, y, z);
	//		Location++;
	//	}

	//	Count++;
	//}

	fclose(WriteFile);

	return 1;
}

