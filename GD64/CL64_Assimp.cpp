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
#include "CL64_Assimp.h"

CL64_Assimp::CL64_Assimp(void)
{
	SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
	mTotalVertices = 0;
}

CL64_Assimp::~CL64_Assimp(void)
{
}

// *************************************************************************
// *			  logInfo:- Terry and Hazel Flanigan 2024		  	 	   *
// *************************************************************************
void CL64_Assimp::logInfo(std::string logString)
{
	Assimp::DefaultLogger::get()->info(logString.c_str());
}

// *************************************************************************
// *			  logDebug:- Terry and Hazel Flanigan 2024		  	 	   *
// *************************************************************************
void CL64_Assimp::logDebug(const char* logString)
{
	Assimp::DefaultLogger::get()->debug(logString);
}

// *************************************************************************
// *				LoadFile:- Terry and Hazel Flanigan 2024	  	 	   *
// *************************************************************************
bool CL64_Assimp::LoadFile(const char* pFile)
{
	const aiScene* scene = aiImportFile(pFile, SelectedPreset);

	if (!scene)
	{
		char buf[1024];
		strcpy(buf, aiGetErrorString());
		//App->Say_Win(buf);
		return false;
	}
	else
	{

		GetBasicInfo(scene);

		Create_MeshGroups(scene);

		Get_Group_VertCount(scene);

		StoreMeshData(scene);

		//LoadTextures();

		//App->CL_Model->Set_BondingBox_Model(1); // Create

	}

	aiReleaseImport(scene);

	return 1;
}

// *************************************************************************
// *			GetBasicInfo:- Terry and Hazel Flanigan 2023	  	 	   *
// *************************************************************************
void CL64_Assimp::GetBasicInfo(const aiScene * pScene)
{

	bool test = pScene->HasMeshes();
	if (test == 1)
	{
		//App->CL_Model->Set_Groupt_Count(pScene->mNumMeshes);
	}

	test = pScene->HasMaterials();
	if (test == 1)
	{
		int Mat = 0;

		Mat = pScene->mNumMaterials;

		if (Mat > 0)
		{
			//App->CL_Model->Set_Texture_Count(pScene->mNumMaterials);
		}
		else
		{
			//App->CL_Model->Set_Texture_Count(-1);
		}
	}

	test = pScene->HasAnimations();
	if (test == 1)
	{
		//App->CL_Model->Set_Motion_Count(pScene->mNumAnimations);
	}
}

// *************************************************************************
// *		Create_MeshGroups_ByIndex:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
void CL64_Assimp::Create_MeshGroups(const aiScene* pScene)
{
	//int Count = 0;

	//char GroupName[255];
	//char MaterialName[255];
	//char GroupNum[255];

	//int mGroupCount = App->CL_Model->Get_Groupt_Count();

	//while (Count < mGroupCount)
	//{
	//	aiMesh* mesh = pScene->mMeshes[Count];

	//	App->CL_Model->Create_Mesh_Group(Count);

	//	_itoa(Count, GroupNum, 10);
	//	strcpy(GroupName, "Group_");
	//	strcat(GroupName, GroupNum);
	//	strcpy(App->CL_Model->Group[Count]->GroupName, GroupName);

	//	strcpy(MaterialName, "Material_");
	//	strcat(MaterialName, GroupNum);
	//	strcpy(App->CL_Model->Group[Count]->MaterialName, MaterialName);

	//	////---------------

	//	App->CL_Model->Group[Count]->GroupVertCount = 0;
	//	App->CL_Model->Group[Count]->MaterialIndex = -1;

	//	App->CL_Model->Group[Count]->MaterialIndex = Count;

	//	strcpy(App->CL_Model->Group[Count]->Text_FileName, "No_Texture");

	//	//App->CL_Vm_Model->S_MeshGroup[Count]->HasBones = mesh->HasBones();
	//	//App->CL_Vm_Model->S_MeshGroup[Count]->BoneCount = mesh->mNumBones;

	//	/*if (App->S_MeshGroup[Count]->HasBones == 1)
	//	{
	//	mHasBones = 1;
	//	}*/

	//	// Get Texture Path/Name
	//	strcpy(App->CL_Model->Group[Count]->Texture_FolderPath, App->CL_Model->Texture_FolderPath); // Back Slash remains

	//	aiString texPath;
	//	aiMaterial* mtl = pScene->mMaterials[mesh->mMaterialIndex];

	//	strcpy(App->CL_Model->Group[Count]->MaterialName, mtl->GetName().C_Str());

	//	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath))
	//	{
	//		strcpy(App->CL_Model->Group[Count]->Text_FileName, texPath.C_Str());
	//		strcpy(App->CL_Model->Group[Count]->Equity_Text_FileName, texPath.C_Str());
	//	}
	//	else
	//	{
	//		strcpy(App->CL_Model->Group[Count]->Text_FileName, "No_Texture");
	//		App->CL_Model->Group[Count]->MaterialIndex = -1;
	//	}

	//	// FileView
	//	App->CL_Model->Group[Count]->ListView_Item = App->CL_FileView->Add_Group(App->CL_Model->Group[Count]->GroupName, Count);

	//	Count++;
	//}

	//App->CL_FileView->Set_FolderActive(App->CL_FileView->GD_GroupsFolder);

}

// *************************************************************************
// *		Get_Group_VertCount:- Terry and Hazel Flanigan 2024		   	   *
// *************************************************************************
void CL64_Assimp::Get_Group_VertCount(const aiScene* pScene)
{
	int Count = 0;
	unsigned int t = 0;
	unsigned int i = 0;
	int VC = 0;
	int mTotalVertices = 0;
	int mTotalFaces = 0;
	
	/*int mGroupCount = App->CL_Model->Get_Groupt_Count();

	while (Count < mGroupCount)
	{
		aiMesh* mesh = pScene->mMeshes[Count];

		VC = 0;
		t = 0;

		mTotalFaces = mTotalFaces + mesh->mNumFaces;

		App->CL_Model->Group[Count]->GroupFaceCount = mesh->mNumFaces;

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

		App->CL_Model->Group[Count]->vertex_Data.resize(VC);
		App->CL_Model->Group[Count]->Normal_Data.resize(VC);
		App->CL_Model->Group[Count]->MapCord_Data.resize(VC);
		App->CL_Model->Group[Count]->Face_Data.resize(VC);

		mTotalVertices = mTotalVertices + mesh->mNumVertices;
		Count++;
	}

	App->CL_Model->VerticeCount = mTotalVertices;
	App->CL_Model->FaceCount = mTotalFaces;*/

}

// *************************************************************************
// *			StoreMeshData:- Terry and Hazel Flanigan 2024		   	   *
// *************************************************************************
void CL64_Assimp::StoreMeshData(const aiScene* pScene)
{
	int FaceNum = 0;
	int FaceIndexNum = 0;
	mTotalVertices = 0;
	int GroupCount = 0;
	unsigned int t = 0;
	unsigned int i = 0;
	int VC = 0;

	//int mGroupCount = App->CL_Model->Get_Groupt_Count();

	//while (GroupCount < mGroupCount)
	//{
	//	aiMesh* mesh = pScene->mMeshes[GroupCount];

	//	VC = 0;
	//	t = 0;

	//	App->CL_Model->Group[GroupCount]->GroupFaceCount = mesh->mNumFaces;
	//	App->CL_Model->Group[GroupCount]->FaceIndex_Data.resize(mesh->mNumFaces * 3);
	//	App->CL_Model->Group[GroupCount]->Face_Data.resize(mesh->mNumFaces);

	//	while (t < mesh->mNumFaces)
	//	{
	//		aiFace* face = &mesh->mFaces[t];

	//		i = 0;
	//		while (i < face->mNumIndices) // Triangulated Each face has 3 Indices
	//		{
	//			int vertexIndex = face->mIndices[i];

	//			App->CL_Model->Group[GroupCount]->vertex_Data[vertexIndex].x = mesh->mVertices[vertexIndex].x;
	//			App->CL_Model->Group[GroupCount]->vertex_Data[vertexIndex].y = mesh->mVertices[vertexIndex].y;
	//			App->CL_Model->Group[GroupCount]->vertex_Data[vertexIndex].z = mesh->mVertices[vertexIndex].z;


	//			if (mesh->HasNormals())
	//			{
	//				App->CL_Model->Group[GroupCount]->Normal_Data[vertexIndex].x = mesh->mNormals[vertexIndex].x;
	//				App->CL_Model->Group[GroupCount]->Normal_Data[vertexIndex].y = mesh->mNormals[vertexIndex].y;
	//				App->CL_Model->Group[GroupCount]->Normal_Data[vertexIndex].z = mesh->mNormals[vertexIndex].z;
	//			}

	//			//		App->CL_Model_Data->S_MeshGroup[Count]->FaceIndices[VC] = FaceNum;

	//			if (mesh->HasTextureCoords(0))
	//			{
	//				App->CL_Model->Group[GroupCount]->MapCord_Data[vertexIndex].u = mesh->mTextureCoords[0][vertexIndex].x;
	//				App->CL_Model->Group[GroupCount]->MapCord_Data[vertexIndex].v = 1 - mesh->mTextureCoords[0][vertexIndex].y;
	//			}

	//			VC++;
	//			i++;
	//			FaceNum++;
	//		}

	//		App->CL_Model->Group[GroupCount]->Face_Data[t].a = face->mIndices[0];
	//		App->CL_Model->Group[GroupCount]->Face_Data[t].b = face->mIndices[1];
	//		App->CL_Model->Group[GroupCount]->Face_Data[t].c = face->mIndices[2];

	//		App->CL_Model->Group[GroupCount]->FaceIndex_Data[t].Index = FaceIndexNum;

	//		FaceIndexNum++;

	//		t++;
	//	}

	//	App->CL_Model->Group[GroupCount]->GroupVertCount = mesh->mNumVertices;

	//	mTotalVertices = mTotalVertices + mesh->mNumVertices;
	//	GroupCount++;
	//}
}
