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

#include "StdAfx.h"
#include "AB_App.h"
#include "SB_Model.h"

SB_Model::SB_Model(void)
{
	Ogre_Vertice_Count = 0;
	Ogre_Face_Count = 0;

	Brush_Vertice_Count = 0;
	Brush_Face_Count = 0;

	VerticeCount = 0;
	FaceCount = 0;
	GroupCount = 0;
	TextureCount = 0;
	MotionCount = 0;
	BoneCount = 0;
	NormalsCount = 0;

	Model_Loaded = 0;

	XBrushCount = 0;
	BrushCount = 0;

	Render_Type = Enums::LoadedFile_None;

	strcpy(FileName, "No Model Loaded");
	strcpy(Path_FileName, "No Model Loaded");
	strcpy(Model_FolderPath, "No Model Loaded");
	strcpy(Texture_FolderPath, "No Model Loaded");
	JustName[0] = 0;

	int Count = 0;
	while (Count < 4999)
	{
		Group[Count] = nullptr;
		Count++;
	}

	Count = 0;
	while (Count < 11999)
	{
		B_XBrush[Count] = nullptr;
		Count++;
	}

	Count = 0;
	while (Count < 11999)
	{
		B_Brush[Count] = nullptr;
		Count++;
	}
}

SB_Model::~SB_Model(void)
{

}

// *************************************************************************
// *			Reaet_Class:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Model::Reset_Class(void)
{
	Model_Loaded = 0;
	Render_Type = Enums::LoadedFile_None;

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

	FileName[0] = 0;
	Path_FileName[0] = 0;
	Model_FolderPath[0] = 0;
	Texture_FolderPath[0] = 0;
	JustName[0] = 0;
	GroupCount = 0;
	TextureCount = 0;
	VerticeCount = 0;
	FaceCount = 0;
	MotionCount = 0;
	BoneCount = 0;
	NormalsCount = 0;

	//BrushCount = 0;
}

// *************************************************************************
// *		Create_Mesh_Group:- Terry and Hazel Flanigan 2023		  	   *
// *************************************************************************
void SB_Model::Create_Mesh_Group(int Index)
{
	if (Group[Index] != nullptr)
	{
		delete Group[Index];
		Group[Index] = nullptr;
	}

	Group[Index] = new Base_Group();
}

// *************************************************************************
// *		Create_XMesh_Group:- Terry and Hazel Flanigan 2023		  	   *
// *************************************************************************
void SB_Model::Create_XBrush(int Index)
{
	if (B_XBrush[Index] != NULL)
	{
		delete B_XBrush[Index];
		B_XBrush[Index] = NULL;
	}

	B_XBrush[Index] = NULL;
	B_XBrush[Index] = new Base_XBrush();
	if (B_XBrush[Index] == NULL)
	{
		App->Say("Cant create Brush");
	}

	B_XBrush[Index]->Vertice_Count = 0;
	B_XBrush[Index]->Face_Count = 0;

}

// *************************************************************************
// *		Get_Groupt_Count:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
int SB_Model::Get_Groupt_Count()
{
	return GroupCount;
}

// *************************************************************************
// *			Set_Groupt_Count:- Terry and Hazel Flanigan 2023	 	   *
// *************************************************************************
void SB_Model::Set_Groupt_Count(int Count)
{
	GroupCount = Count;
}

// *************************************************************************
// *		Set_Groupt_Count:- Terry and Hazel Flanigan 2023	  		   *
// *************************************************************************
void SB_Model::Set_Texture_Count(int Count)
{
	TextureCount = Count;
}

// *************************************************************************
// *			Set_Groupt_Count:- Terry and Hazel Flanigan 2023	   	   *
// *************************************************************************
void SB_Model::Set_Motion_Count(int Count)
{
	MotionCount = Count;
}

// *************************************************************************
// *			Set_Paths:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Model::Set_Paths(void)
{
	strcpy(FileName, App->CLSB_Loader->FileName);
	strcpy(Path_FileName, App->CLSB_Loader->Path_FileName);

	// Get Texture path assumed at this point to be where model is
	int len1 = strlen(FileName);
	int len2 = strlen(Path_FileName);
	strcpy(Model_FolderPath, Path_FileName);
	Model_FolderPath[len2 - len1] = 0;

	strcpy(Texture_FolderPath, Model_FolderPath); // Back Slash remains

	if (_stricmp(FileName + strlen(FileName) - 5, ".Wepf") == 0)
	{
		char JustFileName[MAX_PATH];
		GetFileTitleA(App->CLSB_Loader->Path_FileName, JustFileName, MAX_PATH);

		strcpy(JustName, JustFileName);
		int Len = strlen(JustFileName);
		JustName[Len - 5] = 0;
	}
	else
	{
		strcpy(JustName, FileName);
		int Len = strlen(JustName);
		JustName[Len - 4] = 0;
	}

	//App->CL_Recent_Files->RecentFileHistory_Update();
}

// *************************************************************************
// *		Create_BondingBox_Assimp:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void SB_Model::Set_BondingBox_Assimp(bool Create)
{
	/*BB_Min.x = App->CLSB_Assimp->Assimp_Group[0]->vertex_Data[0].x;
	BB_Min.y = App->CLSB_Assimp->Assimp_Group[0]->vertex_Data[0].y;
	BB_Min.z = App->CLSB_Assimp->Assimp_Group[0]->vertex_Data[0].z;

	BB_Max.x = App->CLSB_Assimp->Assimp_Group[0]->vertex_Data[0].x;
	BB_Max.y = App->CLSB_Assimp->Assimp_Group[0]->vertex_Data[0].y;
	BB_Max.z = App->CLSB_Assimp->Assimp_Group[0]->vertex_Data[0].z;

	int Count = 0;
	int VertCount = 0;

	while (Count < App->CLSB_Assimp->Total_Assimp_GroupCount)
	{
		VertCount = 0;
		while (VertCount < App->CLSB_Assimp->Assimp_Group[Count]->GroupVertCount)
		{
			if (App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].x < BB_Min.x) BB_Min.x = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].x;
			if (App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].y < BB_Min.y) BB_Min.y = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].y;
			if (App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].z < BB_Min.z) BB_Min.z = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].z;

			if (App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].x > BB_Max.x) BB_Max.x = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].x;
			if (App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].y > BB_Max.y) BB_Max.y = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].y;
			if (App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].z > BB_Max.z) BB_Max.z = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[VertCount].z;
			VertCount++;
		}
		Count++;
	}

	Size.x = (fabs(BB_Max.x - BB_Min.x));
	Size.y = (fabs(BB_Max.y - BB_Min.y));
	Size.z = (fabs(BB_Max.z - BB_Min.z));

	radius = (Size.x > Size.z) ? Size.z / 2.0f : Size.x / 2.0f;

	Centre.x = (BB_Min.x + BB_Max.x) / 2.0f;
	Centre.y = (BB_Min.y + BB_Max.y) / 2.0f;
	Centre.z = (BB_Min.z + BB_Max.z) / 2.0f;*/
}

// *************************************************************************
// *		Create_BondingBox_Model:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Model::Set_BondingBox_Model(bool Create)
{
	BB_Min.x = Group[0]->vertex_Data[0].x;
	BB_Min.y = Group[0]->vertex_Data[0].y;
	BB_Min.z = Group[0]->vertex_Data[0].z;

	BB_Max.x = Group[0]->vertex_Data[0].x;
	BB_Max.y = Group[0]->vertex_Data[0].y;
	BB_Max.z = Group[0]->vertex_Data[0].z;

	int Count = 0;
	int VertCount = 0;

	while (Count < GroupCount)
	{
		VertCount = 0;
		while (VertCount < Group[Count]->GroupVertCount)
		{
			if (Group[Count]->vertex_Data[VertCount].x < BB_Min.x) BB_Min.x = Group[Count]->vertex_Data[VertCount].x;
			if (Group[Count]->vertex_Data[VertCount].y < BB_Min.y) BB_Min.y = Group[Count]->vertex_Data[VertCount].y;
			if (Group[Count]->vertex_Data[VertCount].z < BB_Min.z) BB_Min.z = Group[Count]->vertex_Data[VertCount].z;

			if (Group[Count]->vertex_Data[VertCount].x > BB_Max.x) BB_Max.x = Group[Count]->vertex_Data[VertCount].x;
			if (Group[Count]->vertex_Data[VertCount].y > BB_Max.y) BB_Max.y = Group[Count]->vertex_Data[VertCount].y;
			if (Group[Count]->vertex_Data[VertCount].z > BB_Max.z) BB_Max.z = Group[Count]->vertex_Data[VertCount].z;
			VertCount++;
		}
		Count++;
	}

	Size.x = (fabs(BB_Max.x - BB_Min.x));
	Size.y = (fabs(BB_Max.y - BB_Min.y));
	Size.z = (fabs(BB_Max.z - BB_Min.z));

	radius = (Size.x > Size.z) ? Size.z / 2.0f : Size.x / 2.0f;

	Centre.x = (BB_Min.x + BB_Max.x) / 2.0f;
	Centre.y = (BB_Min.y + BB_Max.y) / 2.0f;
	Centre.z = (BB_Min.z + BB_Max.z) / 2.0f;
}

// *************************************************************************
// *	   Create_BondingBox_Brushes:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void SB_Model::Set_BondingBox_Brushes()
{
	BB_Min.x = B_XBrush[0]->B_Brush[0]->vertex_Data[0].x;
	BB_Min.y = B_XBrush[0]->B_Brush[0]->vertex_Data[0].y;
	BB_Min.z = B_XBrush[0]->B_Brush[0]->vertex_Data[0].z;

	BB_Max.x = B_XBrush[0]->B_Brush[0]->vertex_Data[0].x;
	BB_Max.y = B_XBrush[0]->B_Brush[0]->vertex_Data[0].y;
	BB_Max.z = B_XBrush[0]->B_Brush[0]->vertex_Data[0].z;

	int Count = 0;
	int VertCount = 0;

	while (Count < XBrushCount)
	{
		int BrushLoop = 0;
		int SubBrushCount = B_XBrush[Count]->Brush_Count;

		while (BrushLoop < SubBrushCount)
		{
			VertCount = 0;
			while (VertCount < B_XBrush[Count]->B_Brush[BrushLoop]->Face_Count)
			{
				if (B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].x < BB_Min.x) BB_Min.x = B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].x;
				if (B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].y < BB_Min.y) BB_Min.y = B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].y;
				if (B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].z < BB_Min.z) BB_Min.z = B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].z;

				if (B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].x > BB_Max.x) BB_Max.x = B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].x;
				if (B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].y > BB_Max.y) BB_Max.y = B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].y;
				if (B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].z > BB_Max.z) BB_Max.z = B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[VertCount].z;

				VertCount++;
			}

			BrushLoop++;
		}

		Count++;
	}

	Size.x = (fabs(BB_Max.x - BB_Min.x));
	Size.y = (fabs(BB_Max.y - BB_Min.y));
	Size.z = (fabs(BB_Max.z - BB_Min.z));

	radius = (Size.x > Size.z) ? Size.z / 2.0f : Size.x / 2.0f;

	Centre.x = (BB_Min.x + BB_Max.x) / 2.0f;
	Centre.y = (BB_Min.y + BB_Max.y) / 2.0f;
	Centre.z = (BB_Min.z + BB_Max.z) / 2.0f;
}

// *************************************************************************
// *		Set_BondingBox_Group:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Model::Set_BondingBox_Group(int GroupID)
{
	Group[GroupID]->BB_Min.x = Group[GroupID]->vertex_Data[0].x;
	Group[GroupID]->BB_Min.y = Group[GroupID]->vertex_Data[0].y;
	Group[GroupID]->BB_Min.z = Group[GroupID]->vertex_Data[0].z;

	Group[GroupID]->BB_Max.x = Group[GroupID]->vertex_Data[0].x;
	Group[GroupID]->BB_Max.y = Group[GroupID]->vertex_Data[0].y;
	Group[GroupID]->BB_Max.z = Group[GroupID]->vertex_Data[0].z;

	int Count = GroupID;
	int VertCount = 0;

	while (VertCount < Group[Count]->GroupVertCount)
	{
		if (Group[Count]->vertex_Data[VertCount].x < Group[GroupID]->BB_Min.x) Group[GroupID]->BB_Min.x = Group[Count]->vertex_Data[VertCount].x;
		if (Group[Count]->vertex_Data[VertCount].y < Group[GroupID]->BB_Min.y) Group[GroupID]->BB_Min.y = Group[Count]->vertex_Data[VertCount].y;
		if (Group[Count]->vertex_Data[VertCount].z < Group[GroupID]->BB_Min.z) Group[GroupID]->BB_Min.z = Group[Count]->vertex_Data[VertCount].z;

		if (Group[Count]->vertex_Data[VertCount].x > Group[GroupID]->BB_Max.x) Group[GroupID]->BB_Max.x = Group[Count]->vertex_Data[VertCount].x;
		if (Group[Count]->vertex_Data[VertCount].y > Group[GroupID]->BB_Max.y) Group[GroupID]->BB_Max.y = Group[Count]->vertex_Data[VertCount].y;
		if (Group[Count]->vertex_Data[VertCount].z > Group[GroupID]->BB_Max.z) Group[GroupID]->BB_Max.z = Group[Count]->vertex_Data[VertCount].z;
		VertCount++;
	}
	
	Group[GroupID]->Size.x = (fabs(Group[GroupID]->BB_Max.x - Group[GroupID]->BB_Min.x));
	Group[GroupID]->Size.y = (fabs(Group[GroupID]->BB_Max.y - Group[GroupID]->BB_Min.y));
	Group[GroupID]->Size.z = (fabs(Group[GroupID]->BB_Max.z - Group[GroupID]->BB_Min.z));

	Group[GroupID]->radius = (Group[GroupID]->Size.x > Group[GroupID]->Size.z) ? Group[GroupID]->Size.z / 2.0f : Group[GroupID]->Size.x / 2.0f;

	Group[GroupID]->Centre.x = (Group[GroupID]->BB_Min.x + Group[GroupID]->BB_Max.x) / 2.0f;
	Group[GroupID]->Centre.y = (Group[GroupID]->BB_Min.y + Group[GroupID]->BB_Max.y) / 2.0f;
	Group[GroupID]->Centre.z = (Group[GroupID]->BB_Min.z + Group[GroupID]->BB_Max.z) / 2.0f;

}

// *************************************************************************
// *	Set_BondingBoxes_AllGroups:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Model::Set_BondingBoxes_AllGroups()
{
	int mGroupCount = Get_Groupt_Count();
	int GroupCount = 0;
	while (GroupCount < mGroupCount)
	{
		Set_BondingBox_Group(GroupCount);
		GroupCount++;
	}

}

// *************************************************************************
// *			Clear Model:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Model::Clear_Model_And_Reset(void)
{
	Reset_Class(); // Reset this Class

	//App->CL_FileView->Reset_Class(); // Reset List View

	//App->CL_Groups->Reset_Class();

	//App->CL_Grid->Reset_Class();

	//App->CL_TopBar->Reset_Class();

	//App->CL_Ogre->Ogre_Listener->ImGui_Render_Tab = Enums::ImGui_Render_Model;

	//App->CL_Export_Ogre3D->Export_As_RF = 0;

	SetWindowText(App->Equity_Dlg_hWnd, "Equity_ME");
}

// *************************************************************************
// *			Create_Brush_XX:- Terry and Hazel Flanigan 2023		  	   *
// *************************************************************************
void SB_Model::Create_Brush_XX(int Index)
{
	if (B_Brush[Index] != nullptr)
	{
		delete B_Brush[Index];
		B_Brush[Index] = nullptr;
	}

	B_Brush[Index] = new Base_Brush();

	B_Brush[Index]->Vertice_Count = 0;
	B_Brush[Index]->Face_Count = 0;
}


