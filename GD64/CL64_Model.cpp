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
#include "CL64_Model.h"

CL64_Model::CL64_Model()
{
	// Internal
	GroupCount = 0;
	TextureCount = 0;
	MotionCount = 0;
	VerticeCount = 0;
	FaceCount = 0;
	Model_Loaded = 0;

	// Ogre
	Ogre_Face_Count = 0;

	Model_Type = Enums::LoadedFile_None;

	strcpy(Texture_FolderPath, "No Model Loaded");
	strcpy(FileName, "No Model Loaded");
	strcpy(Path_FileName, "No Model Loaded");
	strcpy(Model_FolderPath, "No Model Loaded");
	strcpy(JustName, "No Model Loaded");
	
	int Count = 0;
	while (Count < 99)
	{
		Group[Count] = nullptr;
		Count++;
	}
}

CL64_Model::~CL64_Model()
{
}

// *************************************************************************
// *			Reaet_Class:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Model::Reset_Class(void)
{
	Model_Loaded = 0;
	Model_Type = Enums::LoadedFile_None;

	int Count = 0;
	int Index = GroupCount;

	//--------------------- Clear Groups
	while (Count < Index)
	{
		if (Group[Count] != nullptr)
		{
			Group[Count]->vertex_Data.clear();
			Group[Count]->vertex_Data.resize(0);
			Group[Count]->Face_Data.resize(0);
			Group[Count]->Normal_Data.resize(0);
			Group[Count]->MapCord_Data.resize(0);


			if (Group[Count]->Base_Bitmap)
			{
				DeleteObject(Group[Count]->Base_Bitmap);
			}

			delete Group[Count];
			Group[Count] = nullptr;
		}
		Count++;
	}

	//--------------------- Clear Bounding box data
	/*if (S_BoundingBox[0] != nullptr)
	{
		delete S_BoundingBox[0];
		S_BoundingBox[0] = nullptr;
	}*/

	/*FileName[0] = 0;
	Path_FileName[0] = 0;
	Model_FolderPath[0] = 0;*/
	Texture_FolderPath[0] = 0;
	/*JustName[0] = 0;*/
	GroupCount = 0;
	TextureCount = 0;
	VerticeCount = 0;
	FaceCount = 0;
	MotionCount = 0;

	/*ItsAnOgreModel = 0;*/
}

// *************************************************************************
// *		Create_Mesh_Group:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void CL64_Model::Create_Mesh_Group(int Index)
{
	if (Group[Index] != nullptr)
	{
		delete Group[Index];
		Group[Index] = nullptr;
	}

	App->CL_Model->Group[Index] = new Base_Group();
}

// *************************************************************************
// *			Clear Model:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Model::Clear_Model_And_Reset(void)
{
	Reset_Class(); // Reset this Class

	//App->CL_FileView->Reset_Class(); // Reset List View

	//App->CL_Groups->Reset_Class();

	//App->CL_Grid->Reset_Class();

	//App->CL_TopBar->Reset_Class();

	//App->CL_Ogre->Ogre_Listener->ImGui_Render_Tab = Enums::ImGui_Render_Model;

	//App->CL_Export_Ogre3D->Export_As_RF = 0;

	SetWindowText(App->MainHwnd, "Equity_ME");
}

// *************************************************************************
// *			Set_Paths:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Model::Set_Paths(void)
{
	strcpy(FileName, App->CL_File_IO->Model_FileName);
	strcpy(Path_FileName, App->CL_File_IO->Model_Path_FileName);

	// Get Texture path assumed at this point to be where model is
	int len1 = strlen(FileName);
	int len2 = strlen(Path_FileName);
	strcpy(Model_FolderPath, Path_FileName);
	Model_FolderPath[len2 - len1] = 0;

	strcpy(Texture_FolderPath, Model_FolderPath); // Back Slash remains


	if (_stricmp(FileName + strlen(FileName) - 5, ".ms3d") == 0)
	{
		strcpy(JustName, FileName);
		int Len = strlen(JustName);
		JustName[Len - 5] = 0;
	}
	else if (_stricmp(FileName + strlen(FileName) - 5, ".G3ds") == 0)
	{
		strcpy(JustName, FileName);
		int Len = strlen(JustName);
		JustName[Len - 5] = 0;
	}
	else if (_stricmp(FileName + strlen(FileName) - 5, ".Wepf") == 0)
	{
		/*char JustFileName[MAX_PATH];
		GetFileTitleA(App->CL_Equity_SB->Pref_WE_Path_FileName, JustFileName, MAX_PATH);

		strcpy(JustName, JustFileName);
		int Len = strlen(JustFileName);
		JustName[Len - 5] = 0;*/
	}
	else
	{
		strcpy(JustName, FileName);
		int Len = strlen(JustName);
		JustName[Len - 4] = 0;
	}
}