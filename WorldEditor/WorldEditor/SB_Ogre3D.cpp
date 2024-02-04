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
#include "SB_Ogre3D.h"

SB_Ogre3D::SB_Ogre3D(void)
{
	mDirectory_Name[0] = 0;
	mSelected_Directory[0] = 0;

	mWorld_File_Path[0] = 0;
	mWorld_File_PathAndFile[0] = 0;

	mExport_Just_Name[0] = 0;

	mExport_Path[0] = 0;
	mExport_PathAndFile_Material[0] = 0;
	mExport_PathAndFile_Mesh[0] = 0;

	strcpy(mWorld_Mesh_JustName, "World");

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;

	Export_Manual = NULL;
	World_Manual = NULL;

	NoTexture = 0;
	NoMaterialFileFound = 0;

	NameIndex = 0;

	OgreModel_Ent = NULL;
	OgreModel_Node = NULL;

	TempResourceGroupXX = "TemporyResourceGroup";
}

SB_Ogre3D::~SB_Ogre3D(void)
{
	
}

// *************************************************************************
// *		Set_Export_Paths:- Terry and Hazel Flanigan 2023		 	   *
// *************************************************************************
void SB_Ogre3D::Set_Export_Paths(void)
{
	char ExportFolder[MAX_PATH];

	strcpy(mSelected_Directory, App->CLSB_Exporter->mFolder_Path);
	strcpy(mDirectory_Name, App->CLSB_Exporter->mDirectory_Name);
	strcpy(mExport_Just_Name, App->CLSB_Exporter->mJustName);

	strcpy(mExport_Path, mSelected_Directory);
	strcat(mExport_Path, "\\");
	strcat(mExport_Path, mDirectory_Name);
	strcat(mExport_Path, "\\");

	strcpy(mExport_PathAndFile_Mesh, mExport_Path);
	strcat(mExport_PathAndFile_Mesh, mExport_Just_Name);
	strcat(mExport_PathAndFile_Mesh, ".mesh");
	
	strcpy(mExport_PathAndFile_Material, mExport_Path);
	strcat(mExport_PathAndFile_Material, mExport_Just_Name);
	strcat(mExport_PathAndFile_Material, ".material");

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;

}

// *************************************************************************
// *	  		Export_To_Ogre3D:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_Ogre3D::Export_To_Ogre3D(bool Create)
{
	Set_Export_Paths();

	CreateDirectory(mExport_Path, NULL);

	if (Create == 1)
	{
		Export_Manual = App->CLSB_Ogre_Setup->mSceneMgr->createManualObject("OgreManual2");
		Export_Manual->setRenderQueueGroup(2);
	}

	int A = 0;
	int B = 0;
	int C = 0;

	Export_Manual->setDynamic(false);
	Export_Manual->setCastShadows(false);

	Export_Manual->estimateVertexCount(App->CLSB_Model->VerticeCount);
	Export_Manual->estimateIndexCount(App->CLSB_Model->FaceCount);

	char MaterialNumber[255];
	char MatName[255];

	int GroupCountTotal = App->CLSB_Model->Get_Groupt_Count();
	int Count = 0;
	int FaceCount = 0;
	int FaceIndex = 0;

	while (Count < GroupCountTotal)
	{
		_itoa(Count, MaterialNumber, 10);
		strcpy(MatName, mExport_Just_Name);
		strcat(MatName, "_Material_");
		strcat(MatName, MaterialNumber);

		Export_Manual->begin(MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST);

		FaceCount = 0;
		FaceIndex = 0;

		while (FaceCount < App->CLSB_Model->Group[Count]->GroupFaceCount)
		{
			A = App->CLSB_Model->Group[Count]->Face_Data[FaceCount].a;
			B = App->CLSB_Model->Group[Count]->Face_Data[FaceCount].b;
			C = App->CLSB_Model->Group[Count]->Face_Data[FaceCount].c;

			// --------------------------------------------------

			Get_Data(Count, A);

			Export_Manual->position(Ogre::Vector3(x, y, z));
			Export_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			Export_Manual->normal(Ogre::Vector3(nx, ny, nz));
			Export_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, B);

			Export_Manual->position(Ogre::Vector3(x, y, z));
			Export_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			Export_Manual->normal(Ogre::Vector3(nx, ny, nz));
			Export_Manual->index(FaceIndex);
			FaceIndex++;

			Get_Data(Count, C);

			Export_Manual->position(Ogre::Vector3(x, y, z));
			Export_Manual->textureCoord(Ogre::Vector2(u, 1 - v));
			Export_Manual->normal(Ogre::Vector3(nx, ny, nz));
			Export_Manual->index(FaceIndex);

			FaceIndex++;
			FaceCount++;
		}

		Export_Manual->end();

		Count++;
	}


	if (Export_Manual->getNumSections() == 0)
	{
		App->Say("Can not create Ogre Sections");
		return;
	}

	Ogre::MeshPtr mesh = Export_Manual->convertToMesh("TestMesh");

	mesh->setAutoBuildEdgeLists(true);
	mesh->buildEdgeList();

	App->CLSB_Ogre_Setup->mSceneMgr->destroyManualObject(Export_Manual);

	Ogre::MeshSerializer* ms = new Ogre::MeshSerializer();
	ms->exportMesh(mesh.get(), mExport_PathAndFile_Mesh);
	delete(ms);

	DecompileTextures_TXL(mExport_Path);

	CreateMaterialFile(mExport_PathAndFile_Material);

}

// *************************************************************************
// *		Set_World_Paths:- Terry and Hazel Flanigan 2023			 	   *
// *************************************************************************
void SB_Ogre3D::Set_World_Paths(void)
{
	char Num[100];
	itoa(NameIndex, Num, 10);

	strcpy(mWorld_Mesh_JustName, "World");
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

	strcpy(mExport_Just_Name, mWorld_Mesh_JustName);

	NameIndex++;

	x, y, z = 0;
	nx, ny, nz = 0;
	u, v = 0;
}

// *************************************************************************
// *	  		Convert_ToOgre3D:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_Ogre3D::Convert_ToOgre3D(bool Create, bool Use_Project_Path)
{
	if (Use_Project_Path == 0)
	{
		Set_World_Paths();
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
			World_Manual->textureCoord(Ogre::Vector2(u,1 - v));
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
void SB_Ogre3D::Get_Data(int Index, int FaceIndex)
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
bool SB_Ogre3D::DecompileTextures_TXL(char* PathAndFile)
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
void SB_Ogre3D::CreateMaterialFile(char* MatFileName)
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

// *************************************************************************
// *			Clean_Direcory:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Ogre3D::Clean_Direcory()
{
	if (App->CLSB_Game_Editor->Project_Loaded == 0)
	{
		return 0;
	}

	char pSearchPath[MAX_PATH];

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	strcpy(pSearchPath, mWorld_File_Path);
	strcat(pSearchPath, "\\");
	strcat(pSearchPath, "*.*");

	hFind = FindFirstFile(pSearchPath, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				if (_stricmp(FindFileData.cFileName, "WorldFoler.txt") == 0)
				{

				}
				else
				{
					char file[MAX_PATH];
					strcpy(file, mWorld_File_Path);
					strcat(file, "\\");
					strcat(file, FindFileData.cFileName);
					remove(file);
				}
			}

		} while (::FindNextFile(hFind, &FindFileData));

		FindClose(hFind);

		return 0;
	}

	return 1;
}

// *************************************************************************
// *					Load_OgreModel Terry Bernie 					   *
// *************************************************************************
bool SB_Ogre3D::Load_OgreModel(void)
{
	//Ogre::ResourceGroupManager::getSingleton().createResourceGroup(TempResourceGroup);

	NoTexture = 0;
	NoMaterialFileFound = 0;

	AddToScene();

	Create_MeshGroups();
	Extract_Mesh_Two();

	App->CLSB_Model->Set_BondingBox_Assimp(1);

	//Get_SkeletonInstance();

	//Get_BoneNames();

	//Get_Motions();

	Get_Textures();

	/*bool SkellAnimation = OgreModel_Ent->hasSkeleton();
	Ogre::SkeletonInstance* skeletonInstance = OgreModel_Ent->getSkeleton();

	if (skeletonInstance && SkellAnimation == 1)
	{
		if (App->CL_Model->MotionCount > 0)
		{
			Ogre::Animation* animation = skeletonInstance->getAnimation(0);
			strcpy(App->CL_Motions->Selected_Motion_Name, animation->getName().c_str());

			strcpy(App->CL_Motions->Decode_MotionByName, App->CL_Motions->Selected_Motion_Name);
			App->CL_Ogre->Ogre_Listener->Animate_State = OgreModel_Ent->getAnimationState(App->CL_Motions->Selected_Motion_Name);
		}
	}*/

	return 1;
}

// *************************************************************************
// *	  					Get_Textures Terry Bernie					   *
// *************************************************************************
void SB_Ogre3D::Get_Textures(void)
{
	App->CLSB_Textures->Load_Textures_Ogre3D();

}

// *************************************************************************
// *					AddToScene Terry Bernie 						   *
// *************************************************************************
void SB_Ogre3D::AddToScene(void)
{

	if (OgreModel_Ent && OgreModel_Node)
	{
		OgreModel_Node->detachAllObjects();
		App->CLSB_Ogre_Setup->mSceneMgr->destroySceneNode(OgreModel_Node);
		App->CLSB_Ogre_Setup->mSceneMgr->destroyEntity(OgreModel_Ent);
		OgreModel_Ent = nullptr;
		OgreModel_Node = nullptr;
	}

	Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(TempResourceGroupXX);
	
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(App->CLSB_Model->Texture_FolderPath,
		"FileSystem",
		TempResourceGroupXX);

	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(TempResourceGroupXX);
	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(TempResourceGroupXX);

	OgreModel_Ent = App->CLSB_Ogre_Setup->mSceneMgr->createEntity("UserMesh", App->CLSB_Model->FileName, TempResourceGroupXX);
	OgreModel_Node = App->CLSB_Ogre_Setup->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	OgreModel_Node->attachObject(OgreModel_Ent);

	OgreModel_Node->setVisible(false);
	//OgreModel_Node->setPosition(268, 140, -240);
	OgreModel_Node->setPosition(0, 0, 0);
	OgreModel_Node->setScale(1, 1, 1);

	//App->CL_Grid->Grid_SetVisible(1);
}

// *************************************************************************
// *						Create_MeshGroups Terry Bernie		  	 	   *
// *************************************************************************
void SB_Ogre3D::Create_MeshGroups()
{
	char GroupName[255];
	char GroupNum[255];

	int SubMeshCount = OgreModel_Ent->getNumSubEntities();

	int Count = 0;
	while (Count < SubMeshCount)
	{
		App->CLSB_Assimp->Create_Assimp_Mesh_Group(Count);
		
		App->CLSB_Assimp->Assimp_Group[Count]->GroupVertCount = 0;
		App->CLSB_Assimp->Assimp_Group[Count]->MaterialIndex = -1;

		strcpy(App->CLSB_Assimp->Assimp_Group[Count]->Text_FileName, "No_Texture");

		itoa(Count, GroupNum, 10);
		strcpy(GroupName, "Group_");
		strcat(GroupName, GroupNum);

		strcpy(App->CLSB_Assimp->Assimp_Group[Count]->GroupName, GroupName);

		//---------------
		Ogre::SubMesh const* subMesh = OgreModel_Ent->getSubEntity(Count)->getSubMesh();


		//---------------
		char JustName[MAX_PATH];
		strcpy(JustName, subMesh->getMaterialName().c_str());

		char TestName;
		//App->CL_FileIO->CheckPath(JustName, JustName, &TestName);
		strcpy(App->CLSB_Assimp->Assimp_Group[Count]->MaterialName, &TestName);
		//---------------

		App->CLSB_Assimp->Assimp_Group[Count]->GroupVertCount = subMesh->vertexData->vertexCount;
		App->CLSB_Assimp->Assimp_Group[Count]->IndicesCount = subMesh->vertexData->vertexCount;

		//---------------
		//App->CLSB_Assimp->Assimp_Group[Count]->ListView_Item = App->CL_FileView->Add_Group(GroupName, Count);

		Count++;
	}

	//App->CL_Model->TextureCount = SubMeshCount;
	App->CLSB_Assimp->Total_Assimp_GroupCount = SubMeshCount;
}

// *************************************************************************
// *						Extract_Mesh_Two				   		 	   *
// *************************************************************************
bool SB_Ogre3D::Extract_Mesh_Two()
{
	int FaceCount = 0;
	int FaceNum = 0;
	int FaceIndexNum = 0;
	int mFaceIndex = 0;
	int xx = 0;
	size_t vertex_count, index_count;
	Vector3* vertices;
	Vector3* normals;
	unsigned long* indices;

	Ogre::int16* BoneIndices;	// Bone Index

	int SubMeshCount = OgreModel_Ent->getNumSubEntities();

	unsigned int Vertloop = 0;
	unsigned int Faceloop = 0;
	int Count = 0;

	while (Count < SubMeshCount)
	{
		Get_SubPoseMeshInstance(OgreModel_Ent->getMesh(), vertex_count, vertices, index_count, indices, Count, BoneIndices);

		int mUVTest = NewGet_SubPoseTextureUV(OgreModel_Ent->getMesh(), Count);

		NewGet_SubPoseNormals(OgreModel_Ent->getMesh(), vertex_count, normals, Count);


		App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data.resize(index_count);
		App->CLSB_Assimp->Assimp_Group[Count]->Normal_Data.resize(index_count);
		App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data.resize(index_count);
		App->CLSB_Assimp->Assimp_Group[Count]->Face_Data.resize(index_count);
		App->CLSB_Assimp->Assimp_Group[Count]->FaceIndex_Data.resize(index_count);

		App->CLSB_Assimp->Assimp_Group[Count]->BoneIndex_Data.resize(index_count);

		FaceIndexNum = 0;
		int Faceit = 0;
		FaceCount = 0;
		Vertloop = 0;
		xx = 0;
		while (Vertloop < vertex_count) // Process Vertices
		{
			App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[Vertloop].x = vertices[Vertloop].x;
			App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[Vertloop].y = vertices[Vertloop].y;
			App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[Vertloop].z = vertices[Vertloop].z;

			App->CLSB_Assimp->Assimp_Group[Count]->BoneIndex_Data[Vertloop].Index = BoneIndices[Vertloop]; // Bone Index 

			if (mUVTest)
			{
				App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[Vertloop].u = MeshTextureCoords[Vertloop].x;
				App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[Vertloop].v = 1 - MeshTextureCoords[Vertloop].y;
			}

			App->CLSB_Assimp->Assimp_Group[Count]->Normal_Data[Vertloop].x = normals[Vertloop].x;
			App->CLSB_Assimp->Assimp_Group[Count]->Normal_Data[Vertloop].y = normals[Vertloop].y;
			App->CLSB_Assimp->Assimp_Group[Count]->Normal_Data[Vertloop].z = normals[Vertloop].z;

			Vertloop++;
		}

		FaceIndexNum = 0;
		Faceloop = 0;
		while (Faceloop < index_count) // Process Faces
		{
			App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[FaceIndexNum].a = indices[Faceloop];
			Faceloop++;
			App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[FaceIndexNum].b = indices[Faceloop];
			Faceloop++;
			App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[FaceIndexNum].c = indices[Faceloop];
			Faceloop++;

			FaceIndexNum++;

			App->CLSB_Assimp->Assimp_Group[Count]->FaceIndex_Data[xx].Index = mFaceIndex;

			xx++;
			mFaceIndex++;
		}

		App->CLSB_Assimp->Assimp_Group[Count]->GroupFaceCount = FaceIndexNum;
		App->CLSB_Assimp->Assimp_Group[Count]->GroupVertCount = Vertloop;
		App->CLSB_Assimp->Assimp_Group[Count]->IndicesCount = Vertloop;

		App->CLSB_Assimp->Total_Assimp_VerticeCount = App->CLSB_Assimp->Total_Assimp_VerticeCount + Vertloop;
		App->CLSB_Assimp->Total_Assimp_FaceCount = App->CLSB_Assimp->Total_Assimp_FaceCount + FaceIndexNum;


		//GetBoneAssignment(OgreModel_Ent->getMesh(), Count, 0);

		Count++;
	}

	return 1;
}

// *************************************************************************
// *					Get_SubPoseMeshInstance Terry Bernie			   *
// *************************************************************************
void SB_Ogre3D::Get_SubPoseMeshInstance(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count,
	unsigned long*& indices,
	int SubMesh,
	Ogre::int16*& BoneIndices)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;


	const Vector3& position = Vector3::ZERO;
	const Quaternion& orient = Quaternion::IDENTITY;
	const Vector3& scale = Vector3::UNIT_SCALE;

	vertex_count = index_count = 0;
	Ogre::SubMesh* submesh = mesh->getSubMesh(SubMesh);

	vertex_count = submesh->vertexData->vertexCount;
	Ogre::SubMesh::VertexBoneAssignmentList plist = submesh->getBoneAssignments();

	index_count = submesh->indexData->indexCount;

	// Allocate space for the vertices and indices
	vertices = new Ogre::Vector3[vertex_count];
	BoneIndices = new Ogre::int16[vertex_count]; // Create Space for Bone Indexs 
	indices = new unsigned long[index_count];

	//-------------------- Get Data
	Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

	if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
	{
		if (submesh->useSharedVertices)
		{
			added_shared = true;
			shared_offset = current_offset;
		}

		const Ogre::VertexElement* posElem =
			vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

		Ogre::HardwareVertexBufferSharedPtr vbuf =
			vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		unsigned char* vertex =
			static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

		float* pReal;

		if (OgreModel_Ent->hasSkeleton() == 1)
		{
			Ogre::SubMesh::BoneAssignmentIterator itor = submesh->getBoneAssignmentIterator();

			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				Ogre::VertexBoneAssignment_s bb = itor.getNext();
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
				BoneIndices[j] = bb.boneIndex;
			}
		}
		else
		{
			for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);
				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
				vertices[current_offset + j] = (orient * (pt * scale)) + position;
			}
		}

		vbuf->unlock();
		next_offset += vertex_data->vertexCount;
	}

	Ogre::IndexData* index_data = submesh->indexData;
	size_t numTris = index_data->indexCount / 3;
	Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

	bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

	unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

	size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;

	if (use32bitindexes)
	{
		for (size_t k = 0; k < numTris * 3; ++k)
		{
			indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
		}
	}
	else
	{
		for (size_t k = 0; k < numTris * 3; ++k)
		{
			indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
				static_cast<unsigned long>(offset);
		}
	}

	ibuf->unlock();
	current_offset = next_offset;
}

// *************************************************************************
// *					NewGet_SubPoseTextureUV Terry Bernie			   *
// *************************************************************************
bool SB_Ogre3D::NewGet_SubPoseTextureUV(Ogre::MeshPtr mesh, int SubMesh)
{
	Ogre::SubMesh* submesh = mesh->getSubMesh(SubMesh);
	int m_iCoordSet = 0;

	//// Get vertex UV coordinates
	//std::vector<Vector2> MeshTextureCoords;
	{
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
		// Get last set of texture coordinates
		int i = 0;
		const VertexElement* texcoordElem;
		const VertexElement* pCurrentElement = NULL;
		do
		{
			texcoordElem = pCurrentElement;
			pCurrentElement = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES, i++);
		} while (pCurrentElement);
		m_iCoordSet = i - 2;
		if (!texcoordElem)
		{
			App->Say("no TextCords");
			return false;
		}
		HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(texcoordElem->getSource());
		unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

		float* pReal;

		MeshTextureCoords.resize(vertex_data->vertexCount);

		for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
		{
			texcoordElem->baseVertexPointerToElement(vertex, &pReal);
			MeshTextureCoords[j] = Vector2(pReal[0], pReal[1]);
		}

		vbuf->unlock();
	}

	return true;
}

// *************************************************************************
// *					NewGet_SubPoseNormals Terry Bernie				   *
// *************************************************************************
bool SB_Ogre3D::NewGet_SubPoseNormals(Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& Normals, int SubMesh)
{

	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	const Vector3& position = Vector3::ZERO;
	const Quaternion& orient = Quaternion::IDENTITY;
	const Vector3& scale = Vector3::UNIT_SCALE;

	vertex_count = 0;

	Ogre::SubMesh* submesh = mesh->getSubMesh(SubMesh);

	vertex_count = submesh->vertexData->vertexCount;

	Normals = new Ogre::Vector3[vertex_count];

	//-------------------- Get Data
	Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

	if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
	{
		if (submesh->useSharedVertices)
		{
			added_shared = true;
			shared_offset = current_offset;
		}

		const Ogre::VertexElement* posElem =
			vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);

		Ogre::HardwareVertexBufferSharedPtr vbuf =
			vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

		unsigned char* vertex =
			static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

		float* pReal;

		for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
		{
			posElem->baseVertexPointerToElement(vertex, &pReal);
			Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
			Normals[current_offset + j] = (orient * (pt * scale)) + position;
		}

		vbuf->unlock();
	}
	return 1;
}
