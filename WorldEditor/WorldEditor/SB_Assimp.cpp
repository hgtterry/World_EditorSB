/*
Copyright (c) 2022 Equity_ME Model Editor -- Inflanite Software W.T.Flanigan H.C.Flanigan

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
#include "SB_Assimp.h"


SB_Assimp::SB_Assimp()
{
	SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
	mTotalVertices = 0;
	DoTextures = 0;

	Total_Assimp_GroupCount = 0;
	Total_Assimp_VerticeCount = 0;
	Total_Assimp_FaceCount = 0;

	int Count = 0;
	while (Count < 499)
	{
		Assimp_Group[Count] = nullptr;
		Count++;
	}
}


SB_Assimp::~SB_Assimp()
{
}

// *************************************************************************
// *				logInfo:- Terry and Hazel Flanigan 2023		  	 	   *
// *************************************************************************
void SB_Assimp::logInfo(std::string logString)
{
	Assimp::DefaultLogger::get()->info(logString.c_str());
}

// *************************************************************************
// *				logDebug:- Terry and Hazel Flanigan 2023	  	 	   *
// *************************************************************************
void SB_Assimp::logDebug(const char* logString)
{
	Assimp::DefaultLogger::get()->debug(logString);
}

// *************************************************************************
// *			LoadFile:- Terry and Hazel Flanigan 2023		  	 	   *
// *************************************************************************
bool SB_Assimp::LoadFile(const char* pFile)
{

	//SelectedPreset = GetUserPresets(); // See what user wants

	const aiScene* scene = aiImportFile(pFile, SelectedPreset);
	
	if (!scene)
	{
		char buf[1024];
		strcpy(buf, aiGetErrorString());
		App->Say(buf);
		return false;
	}
	else
	{
		
		GetBasicInfo(scene);
		
		Create_MeshGroups(scene);
		
		Get_Group_VertCount(scene);
		
		StoreMeshData(scene);
		
		if (DoTextures == 1)
		{
			//LoadTextures();
		}

		//App->CLSB_Model->Set_BondingBox_Model(1); // Create Main Bounding Box

		// Create Groups Bounding Boxes
		//App->CLSB_Model->Set_BondingBoxes_AllGroups();
	}

	
	aiReleaseImport(scene);

	return 1;
}

// *************************************************************************
// *			GetBasicInfo:- Terry and Hazel Flanigan 2023	  	 	   *
// *************************************************************************
void SB_Assimp::GetBasicInfo(const aiScene* pScene)
{

	bool test = pScene->HasMeshes();
	if (test == 1)
	{
		//App->CLSB_Model->Set_Groupt_Count(pScene->mNumMeshes);
		Total_Assimp_GroupCount = pScene->mNumMeshes;
	}

	test = pScene->HasMaterials();
	if (test == 1)
	{
		int Mat = 0;

		Mat = pScene->mNumMaterials;

		if (Mat > 0)
		{
			App->CLSB_Model->Set_Texture_Count(pScene->mNumMaterials);
		}
		else
		{
			App->CLSB_Model->Set_Texture_Count(-1);
		}
	}

	test = pScene->HasAnimations();
	if (test == 1)
	{
		App->CLSB_Model->Set_Motion_Count(pScene->mNumAnimations);
	}
}

// *************************************************************************
// *	Create_MeshGroups_ByIndex:- Terry and Hazel Flanigan 2023	 	   *
// *************************************************************************
void SB_Assimp::Create_MeshGroups(const aiScene* pScene)
{
	int Count = 0;

	char GroupName[255];
	char MaterialName[255];
	char GroupNum[255];

	//int mGroupCount = App->CLSB_Model->Get_Groupt_Count();
	int mGroupCount = Total_Assimp_GroupCount;

	while (Count < mGroupCount)
	{
		aiMesh* mesh = pScene->mMeshes[Count];
		Create_Assimp_Mesh_Group(Count);

		//App->Say(mesh->mName.C_Str());

		_itoa(Count, GroupNum, 10);
		strcpy(GroupName, "Group_");
		strcat(GroupName, GroupNum);
		strcpy(Assimp_Group[Count]->GroupName, GroupName);

		strcpy(MaterialName, "Material_");
		strcat(MaterialName, GroupNum);
		strcpy(Assimp_Group[Count]->MaterialName, MaterialName);

		////---------------

		Assimp_Group[Count]->GroupVertCount = 0;
		Assimp_Group[Count]->MaterialIndex = -1;

		Assimp_Group[Count]->MaterialIndex = Count;

		strcpy(Assimp_Group[Count]->Text_FileName, "No_Texture");

		//strcpy(App->CLSB_Model->Group[Count]->Texture_FolderPath, App->CLSB_Model->Texture_FolderPath); // Back Slash remains

		aiString texPath;
		aiMaterial* mtl = pScene->mMaterials[mesh->mMaterialIndex];

		strcpy(Assimp_Group[Count]->MaterialName, mtl->GetName().C_Str());

		if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath))
		{
			strcpy(Assimp_Group[Count]->Text_FileName, texPath.C_Str());
			strcpy(Assimp_Group[Count]->Equity_Text_FileName, texPath.C_Str());
		}
		else
		{
			strcpy(Assimp_Group[Count]->Text_FileName, "No_Texture");
			Assimp_Group[Count]->MaterialIndex = -1;
		}

		Count++;
	}
}

// *************************************************************************
// *		Get_Group_VertCount:- Terry and Hazel Flanigan 2023		  	   *
// *************************************************************************
void SB_Assimp::Get_Group_VertCount(const aiScene* pScene)
{
	int Count = 0;
	unsigned int t = 0;
	unsigned int i = 0;
	int VC = 0;
	int mTotalVertices = 0;
	int mTotalFaces = 0;

	//int mGroupCount = App->CLSB_Model->Get_Groupt_Count();
	int mGroupCount = Total_Assimp_GroupCount;

	while (Count < mGroupCount)
	{
		aiMesh* mesh = pScene->mMeshes[Count];

		VC = 0;
		t = 0;

		mTotalFaces = mTotalFaces + mesh->mNumFaces;

		Assimp_Group[Count]->GroupFaceCount = mesh->mNumFaces;

		while (t < mesh->mNumFaces)
		{
			aiFace* face = &mesh->mFaces[t];

			i = 0;
			while (i < face->mNumIndices)
			{
				VC++;
				i++;
			}

			t++;
		}

		Assimp_Group[Count]->vertex_Data.resize(VC);
		Assimp_Group[Count]->Normal_Data.resize(VC);
		Assimp_Group[Count]->MapCord_Data.resize(VC);
		Assimp_Group[Count]->Face_Data.resize(VC);

		mTotalVertices = mTotalVertices + mesh->mNumVertices;
		Count++;
	}

	App->CLSB_Model->VerticeCount = mTotalVertices;
	App->CLSB_Model->FaceCount = mTotalFaces;

	Total_Assimp_VerticeCount = mTotalVertices;
	Total_Assimp_FaceCount = mTotalFaces;

}

// *************************************************************************
// *			StoreMeshData:- Terry and Hazel Flanigan 2023		   	   *
// *************************************************************************
void SB_Assimp::StoreMeshData(const aiScene* pScene)
{
	int FaceNum = 0;
	int FaceIndexNum = 0;
	mTotalVertices = 0;
	int GroupCount = 0;
	unsigned int t = 0;
	unsigned int i = 0;
	int VC = 0;

	//int mGroupCount = App->CLSB_Model->Get_Groupt_Count();
	int mGroupCount = Total_Assimp_GroupCount;

	while (GroupCount < mGroupCount)
	{
		aiMesh* mesh = pScene->mMeshes[GroupCount];

		VC = 0;
		t = 0;

		Assimp_Group[GroupCount]->GroupFaceCount = mesh->mNumFaces;
		Assimp_Group[GroupCount]->FaceIndex_Data.resize(mesh->mNumFaces * 3);

		Assimp_Group[GroupCount]->Face_Data.resize(mesh->mNumFaces);

		while (t < mesh->mNumFaces)
		{
			aiFace* face = &mesh->mFaces[t];

			i = 0;
			while (i<face->mNumIndices) // Triangulated Each face has 3 Indices
			{
				int vertexIndex = face->mIndices[i];

				Assimp_Group[GroupCount]->vertex_Data[vertexIndex].x = mesh->mVertices[vertexIndex].x;
				Assimp_Group[GroupCount]->vertex_Data[vertexIndex].y = mesh->mVertices[vertexIndex].y;
				Assimp_Group[GroupCount]->vertex_Data[vertexIndex].z = mesh->mVertices[vertexIndex].z;


				if (mesh->HasNormals())
				{
					Assimp_Group[GroupCount]->Normal_Data[vertexIndex].x = mesh->mNormals[vertexIndex].x;
					Assimp_Group[GroupCount]->Normal_Data[vertexIndex].y = mesh->mNormals[vertexIndex].y;
					Assimp_Group[GroupCount]->Normal_Data[vertexIndex].z = mesh->mNormals[vertexIndex].z;
				}

				//		App->CL_Scene_Data->S_MeshGroup[Count]->FaceIndices[VC] = FaceNum;

				if (mesh->HasTextureCoords(0))
				{
					Assimp_Group[GroupCount]->MapCord_Data[vertexIndex].u = mesh->mTextureCoords[0][vertexIndex].x;
					Assimp_Group[GroupCount]->MapCord_Data[vertexIndex].v = 1 - mesh->mTextureCoords[0][vertexIndex].y;
				}

				VC++;
				i++;
				FaceNum++;
			}

			Assimp_Group[GroupCount]->Face_Data[t].a = face->mIndices[0];
			Assimp_Group[GroupCount]->Face_Data[t].b = face->mIndices[1];
			Assimp_Group[GroupCount]->Face_Data[t].c = face->mIndices[2];

			Assimp_Group[GroupCount]->FaceIndex_Data[t].Index = FaceIndexNum;

			FaceIndexNum++;

			t++;
		}

		Assimp_Group[GroupCount]->GroupVertCount = mesh->mNumVertices;

		mTotalVertices = mTotalVertices + mesh->mNumVertices;
		GroupCount++;
	}
}

// *************************************************************************
// *			LoadTextures:- Terry and Hazel Flanigan 2023	 	 	   *
// *************************************************************************
void SB_Assimp::LoadTextures()
{
	App->CLSB_Textures->Load_Textures_Assimp();
}

// *************************************************************************
// *		Create_Mesh_Group:- Terry and Hazel Flanigan 2023		  	   *
// *************************************************************************
void SB_Assimp::Create_Assimp_Mesh_Group(int Index)
{
	if (Assimp_Group[Index] != nullptr)
	{
		delete Assimp_Group[Index];
		Assimp_Group[Index] = nullptr;
	}

	Assimp_Group[Index] = new Base_Assimp();
}
