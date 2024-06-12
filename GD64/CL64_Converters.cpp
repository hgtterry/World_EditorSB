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
#include "CL64_Converters.h"

CL64_Converters::CL64_Converters(void)
{
	Export_Manual = nullptr;
	World_Manual =	nullptr;

	World_Node =	nullptr;
	World_Ent =		nullptr;

	mWorld_Mesh_JustName[0] = 0;
	mWorld_File_PathAndFile[0] = 0;
	mWorld_File_Path[0] = 0;
	mExport_Just_Name[0] = 0;

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;
}

CL64_Converters::~CL64_Converters(void)
{
}

// *************************************************************************
// *			Set_Paths:- Terry and Hazel Flanigan 2024			 	   *
// *************************************************************************
void CL64_Converters::Set_Paths(void)
{
	/*char Num[100];
	itoa(NameIndex, Num, 10);*/

	/*strcpy(mWorld_Mesh_JustName, "World");
	strcat(mWorld_Mesh_JustName, Num);

	strcpy(mWorld_File_PathAndFile, App->WorldEditor_Directory);
	strcat(mWorld_File_PathAndFile, "\\");
	strcat(mWorld_File_PathAndFile, "Data");
	strcat(mWorld_File_PathAndFile, "\\");
	strcat(mWorld_File_PathAndFile, "World_Test");

	strcpy(mWorld_File_Path, mWorld_File_PathAndFile);

	strcat(mWorld_File_PathAndFile, "\\");
	strcat(mWorld_File_PathAndFile, mWorld_Mesh_JustName);
	strcat(mWorld_File_PathAndFile, ".mesh");

	strcpy(mExport_Just_Name, mWorld_Mesh_JustName);*/

	strcpy(mWorld_File_Path, App->CL_Model->Model_FolderPath);

	strcpy(mWorld_File_PathAndFile, App->CL_Model->Model_FolderPath);
	strcat(mWorld_File_PathAndFile, App->CL_Model->JustName);
	strcat(mWorld_File_PathAndFile, ".mesh");

	strcpy(mWorld_Mesh_JustName, App->CL_Model->JustName);
	strcpy(mExport_Just_Name, App->CL_Model->JustName);
	
	//App->Say(mWorld_File_PathAndFile);

	/*NameIndex++;*/

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;
}

// *************************************************************************
// *	  		Convert_ToOgre3D:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Converters::Convert_ToOgre3D(bool Create)
{

	Set_Paths();

	App->CL_Model->Ogre_Face_Count = 0;

	if (Create == 1)
	{
		World_Manual = App->CL_Ogre->mSceneMgr->createManualObject("OgreManual2");
		World_Manual->setRenderQueueGroup(2);
	}

	int A = 0;
	int B = 0;
	int C = 0;

	World_Manual->setDynamic(false);
	World_Manual->setCastShadows(false);

	World_Manual->estimateVertexCount(App->CL_Model->VerticeCount);
	World_Manual->estimateIndexCount(App->CL_Model->FaceCount);

	char MaterialNumber[255];
	char MatName[255];

	int GroupCountTotal = App->CL_Model->GroupCount;
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

		CreateMaterial(MatName);
		//App->Say(MatName);

		World_Manual->begin(MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST, App->CL_Ogre->App_Resource_Group);

		FaceCount = 0;
		FaceIndex = 0;

		TotalFaces = TotalFaces + App->CL_Model->Group[Count]->GroupFaceCount;

		while (FaceCount < App->CL_Model->Group[Count]->GroupFaceCount)
		{
			A = App->CL_Model->Group[Count]->Face_Data[FaceCount].a;
			B = App->CL_Model->Group[Count]->Face_Data[FaceCount].b;
			C = App->CL_Model->Group[Count]->Face_Data[FaceCount].c;

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

	App->CL_Model->Ogre_Face_Count = TotalFaces;

	if (World_Manual->getNumSections() == 0)
	{
		App->Say("Can not create Ogre Sections");
	}

	Ogre::MeshPtr mesh = World_Manual->convertToMesh("TestMesh");

	mesh->setAutoBuildEdgeLists(true);
	mesh->buildEdgeList();
	
	App->CL_Ogre->mSceneMgr->destroyManualObject(World_Manual);

	Ogre::MeshSerializer* ms = new Ogre::MeshSerializer();
	ms->exportMesh(mesh.get(), mWorld_File_PathAndFile);
	delete(ms);

	/*char OutputFolder[MAX_PATH];
	strcpy(OutputFolder, mWorld_File_Path);
	strcat(OutputFolder, "\\");*/

	//DecompileTextures_TXL(OutputFolder);

	char Material_PathAndFile[MAX_PATH];
	strcpy(Material_PathAndFile, mWorld_File_Path);
	//strcat(Material_PathAndFile, "\\");
	strcat(Material_PathAndFile, mWorld_Mesh_JustName);
	strcat(Material_PathAndFile, ".material");

	//App->Say(Material_PathAndFile);

	CreateMaterialFile(Material_PathAndFile);

	//char Name[MAX_PATH];
	//strcpy(Name, mWorld_Mesh_JustName);
	//strcat(Name, ".mesh");

	//if (World_Ent)
	//{
	//	World_Node->detachAllObjects();

	//	App->CL_Ogre->mSceneMgr->destroySceneNode(World_Node);
	//	App->CL_Ogre->mSceneMgr->destroyEntity(World_Ent);

	//	World_Node = NULL;
	//	World_Ent = NULL;

	//	//Ogre::ResourcePtr ptr = Ogre::MeshManager::getSingleton().getByName(Name,App->CLSB_Ogre->World_Resource_Group);
	//	//ptr->unload();

	//	//Ogre::MeshManager::getSingleton().remove(Name);

	//	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(App->CL_Ogre->World_Resource_Group);
	//	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App->CL_Ogre->World_Resource_Group);

	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CL_Ogre->World_Resource_Group);
	//	Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(App->CL_Ogre->World_Resource_Group);
	//	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(App->CL_Ogre->World_Resource_Group);

	//}
	//else
	//{
	//	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mWorld_File_Path, "FileSystem", App->CL_Ogre->World_Resource_Group);
	//}

	//World_Ent = App->CL_Ogre->mSceneMgr->createEntity(Name);
	//World_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();

	//World_Node->attachObject(World_Ent);

	//World_Node->setPosition(0, 0, 0);
	//World_Node->setVisible(true);
	//World_Node->setScale(1, 1, 1);

	//remove(mWorld_File_PathAndFile);
	//remove(Material_PathAndFile);

	App->Say("Converted");
}

// *************************************************************************
// *			CreateMaterial:- Terry and Hazel Flanigan 2024  	   	   *
// *************************************************************************
void CL64_Converters::CreateMaterial(char* MatName)
{

	MaterialManager* omatMgr = MaterialManager::getSingletonPtr();

	auto status = omatMgr->createOrRetrieve(MatName, App->CL_Ogre->App_Resource_Group);
	auto omat = static_pointer_cast<Material>(status.first);

}

// *************************************************************************
// *		CreateMaterialFile:- Terry and Hazel Flanigan 2024		   	   *
// *************************************************************************
void CL64_Converters::CreateMaterialFile(char* MatFileName)
{
	char MatName[255];
	char File[255];
	char MaterialNumber[255];

	Ogre::String OMatFileName = MatFileName;
	Ogre::String OFile;
	Ogre::String OMatName;

	int numMaterials = App->CL_Model->GroupCount;

	Ogre::MaterialManager& matMgrSgl = Ogre::MaterialManager::getSingleton();

	Ogre::MaterialSerializer matSer;

	for (int i = 0; i < numMaterials; ++i)
	{
		_itoa(i, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		strcpy(File, App->CL_Model->Group[i]->Text_FileName);

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

// *************************************************************************
// *			Get_Data:- Terry and Hazel Flanigan 2024  			   	   *
// *************************************************************************
void CL64_Converters::Get_Data(int Index, int FaceIndex)
{
	x = App->CL_Model->Group[Index]->vertex_Data[FaceIndex].x;
	y = App->CL_Model->Group[Index]->vertex_Data[FaceIndex].y;
	z = App->CL_Model->Group[Index]->vertex_Data[FaceIndex].z;

	u = App->CL_Model->Group[Index]->MapCord_Data[FaceIndex].u;
	v = App->CL_Model->Group[Index]->MapCord_Data[FaceIndex].v;

	nx = App->CL_Model->Group[Index]->Normal_Data[FaceIndex].x;
	ny = App->CL_Model->Group[Index]->Normal_Data[FaceIndex].y;
	nz = App->CL_Model->Group[Index]->Normal_Data[FaceIndex].z;
}

