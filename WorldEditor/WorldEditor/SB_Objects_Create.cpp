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
#include "SB_Objects_Create.h"

SB_Objects_Create::SB_Objects_Create(void)
{
	m_ResourcePath[0] = 0;
}

SB_Objects_Create::~SB_Objects_Create(void)
{
}

// *************************************************************************
//			Add_Objects_From_File:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool SB_Objects_Create::Add_Objects_From_File() // From File
{

	int Object_Count = App->CLSB_Scene->Object_Count;
	int Count = 0;

	while (Count < Object_Count)
	{
		{
			App->CLSB_Objects_Create->Add_New_Object(Count, 0);
			App->CLSB_GameDirector->V_Object[Count]->Altered = 0;
			App->CLSB_GameDirector->V_Object[Count]->Folder = Enums::Folder_Objects;
			App->CLSB_GameDirector->V_Object[Count]->FileViewItem = App->CLSB_FileView->Add_Item(App->CLSB_FileView->FV_Objects_Folder, App->CLSB_GameDirector->V_Object[Count]->Mesh_Name, Count, false);
		}

		Count++;
	}

	if (Object_Count > 0)
	{
		App->CLSB_FileView->Set_FolderActive(App->CLSB_FileView->FV_Objects_Folder);
		//ShowWindow(App->SBC_Properties->Properties_Dlg_hWnd, 1);
		App->CLSB_FileView->SelectItem(App->CLSB_GameDirector->V_Object[0]->FileViewItem);
	}

	return 1;
}

// *************************************************************************
//		Add_Objects_From_MeshViewer:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void SB_Objects_Create::Add_Objects_From_MeshViewer()
{

	//if (App->SBC_MeshViewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Collectables) // Collectables
	//{
	//	App->CL_Com_Collectables->Add_New_Collectable();
	//	return;
	//}

	int Index = App->CLSB_Scene->Object_Count;

	App->CLSB_GameDirector->V_Object[Index] = new Base_Object();

	Base_Object* Object = App->CLSB_GameDirector->V_Object[Index];
	Object->This_Object_UniqueID = App->CLSB_Scene->UniqueID_Object_Counter; // Unique ID


	strcpy(Object->Mesh_Name, App->CLSB_Meshviewer->Object_Name);
	strcpy(Object->Mesh_FileName, App->CLSB_Meshviewer->Selected_MeshFile);
	strcpy(Object->Mesh_Resource_Path, m_ResourcePath);
	strcpy(Object->Material_File, App->CLSB_Meshviewer->m_Material_File);

	Object->Type = App->CLSB_Meshviewer->Physics_Type;
	Object->Shape = App->CLSB_Meshviewer->Physics_Shape;


	App->CLSB_Objects_Create->Dispatch_MeshViewer();

	App->CLSB_FileView->SelectItem(App->CLSB_GameDirector->V_Object[Index]->FileViewItem);


	App->CLSB_Scene->UniqueID_Object_Counter++; // Unique ID
	App->CLSB_Scene->Object_Count++;  // Must be last line

	App->CLSB_Scene->Scene_Modified = 1;
}

// *************************************************************************
//			Dispatch_MeshViewer:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void SB_Objects_Create::Dispatch_MeshViewer()
{
	int Index = App->CLSB_Scene->Object_Count;

	//if (App->CLSB_Meshviewer->Mesh_Viewer_Mode == Enums::Mesh_Viewer_Area) // Area
	//{
	//	App->SBC_Com_Area->Add_Aera_To_Project(0, App->App->CLSB_Meshviewer->Selected_MeshFile, m_ResourcePath);
	//	App->Say("Dispatch_MeshViewer");
	//}
	//else
	{
		Add_New_Object(Index, 1);
		App->CLSB_GameDirector->V_Object[Index]->Altered = 1;
		App->CLSB_GameDirector->V_Object[Index]->Folder = Enums::Folder_Objects;
		App->CLSB_GameDirector->V_Object[Index]->FileViewItem = App->CLSB_FileView->Add_Item(App->CLSB_FileView->FV_Objects_Folder,
		App->CLSB_GameDirector->V_Object[Index]->Mesh_Name, Index, true);

	}
}

// *************************************************************************
//				Add_New_Object:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
bool SB_Objects_Create::Add_New_Object(int Index, bool From_MeshViewer)
{
	char Mesh_File[255];
	char ConNum[256];
	char Ogre_Name[256];

	Base_Object* Object = App->CLSB_GameDirector->V_Object[Index];


	strcpy_s(Ogre_Name, "GDEnt_");
	_itoa(Index, ConNum, 10);
	strcat(Ogre_Name, ConNum);

	strcpy(Mesh_File, Object->Mesh_FileName);

	Object->Object_Ent = App->CLSB_Ogre_Setup->mSceneMgr->createEntity(Ogre_Name, Mesh_File, App->CLSB_Scene->Project_Resource_Group);
	Object->Object_Node = App->CLSB_Ogre_Setup->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Object->Object_Node->attachObject(Object->Object_Ent);

	Object->Object_Node->setVisible(true);

	Object->Object_Node->setOrientation(Object->Mesh_Quat);


	// Get Material Name
	Ogre::String text = Object->Object_Ent->getMesh()->getSubMesh(0)->getMaterialName().c_str();
	Ogre::MaterialPtr  Mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName(text));
	strcpy(Object->Material_File, Mat->getOrigin().c_str());
	Object->UsageEX = 777;

	// If from MeshViewer Get Placement Method
	/*if (From_MeshViewer == 1 && App->SBC_MeshViewer->Placement_Camera == 1)
	{
		Ogre::Vector3 Pos = App->CL_Object->GetPlacement();
		Object->Mesh_Pos = Pos;
		Object->Object_Node->setPosition(Pos);
	}
	else
	{*/
	Object->Object_Node->setPosition(0, 0, 0);// Object->Mesh_Pos);
	Object->Object_Node->setVisible(true);
	//}


		App->CLSB_GameDirector->Project_Loaded = 1;


	//---------------------- Static
	if (Object->Type == Enums::Bullet_Type_Static)
	{
		if (Object->Shape == Enums::Shape_Box)
		{
			Add_Physics_Box(0, Index);
		}

		/*if (Object->Shape == Enums::Sphere)
		{
			Add_Physics_Sphere(false, Index);
		}

		if (Object->Shape == Enums::Capsule)
		{
			Add_Physics_Capsule(false, Index);
		}

		if (Object->Shape == Enums::Cylinder)
		{
			Add_Physics_Cylinder(false, Index);
		}

		if (Object->Shape == Enums::Cone)
		{
			Add_Physics_Cone(false, Index);
		}*/
	}

	//---------------------- Dynamic
	if (Object->Type == Enums::Bullet_Type_Dynamic)
	{
		/*if (Object->Shape == Enums::Shape_Box)
		{
			Add_Physics_Box(true, Index);
		}*/

		if (Object->Shape == Enums::Shape_Sphere)
		{
			Add_Physics_Sphere(true, Index);
		}

		/*if (Object->Shape == Enums::Capsule)
		{
			Add_Physics_Capsule(true, Index);
		}

		if (Object->Shape == Enums::Cylinder)
		{
			Add_Physics_Cylinder(true, Index);
		}

		if (Object->Shape == Enums::Cone)
		{
			Add_Physics_Cone(true, Index);
		}*/
	}

	//---------------------- Tri_Mesh
	//if (Object->Type == Enums::Bullet_Type_TriMesh)
	//{
	//	create_New_Trimesh(Index);
	//}


	//if (Object->Usage == Enums::Usage_Room) // Rooms
	//{
	//	App->SBC_Scene->Area_Added = 1;
	//}

	//ShowWindow(App->GD_Properties_Hwnd, 1);

	return 1;
}

// *************************************************************************
//			Add_Physics_Sphere:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_Objects_Create::Add_Physics_Sphere(bool Dynamic, int Index)
{
	Base_Object* Object = App->CLSB_GameDirector->V_Object[Index];

	if (Dynamic == 1)
	{
		Object->Type = Enums::Bullet_Type_Dynamic;
		Object->Shape = Enums::Shape_Sphere;

	}
	else
	{
		Object->Type = Enums::Bullet_Type_Static;
		Object->Shape = Enums::Shape_Sphere;
	}

	AxisAlignedBox worldAAB = Object->Object_Ent->getBoundingBox();
	worldAAB.transformAffine(Object->Object_Node->_getFullTransform());
	Ogre::Vector3 Centre = worldAAB.getCenter();

	Object->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0.0f, 0.0f, 0.0f, 1));

	btScalar mass;

	if (Dynamic == 1)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 0.0f;
	}


	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);

	startTransform.setOrigin(initialPosition);

	float Radius = App->CLSB_Object->GetMesh_BB_Radius(Object->Object_Node);
	Object->Physics_Size = Ogre::Vector3(Radius, 0, 0);

	btCollisionShape* newRigidShape = new btSphereShape(Radius);
	newRigidShape->calculateLocalInertia(mass, localInertia);

	App->CLSB_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Object->Phys_Body = new btRigidBody(rbInfo);
	Object->Phys_Body->setRestitution(1.0);
	Object->Phys_Body->setFriction(2.5);
	Object->Phys_Body->setUserPointer(Object->Object_Node);
	Object->Phys_Body->setWorldTransform(startTransform);

	if (Dynamic == 1)
	{
		Object->Usage = Enums::Usage_Dynamic;
		Object->Phys_Body->setUserIndex(Enums::Usage_Dynamic);
		Object->Phys_Body->setUserIndex2(Index);
	}
	else
	{
		Object->Usage = Enums::Usage_Static;
		Object->Phys_Body->setUserIndex(Enums::Usage_Static);
		Object->Phys_Body->setUserIndex2(Index);
	}

	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CLSB_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CLSB_GameDirector->V_Object[Index]->Physics_Valid = 1;

	App->CLSB_Physics->Set_Physics(Index);
}

// *************************************************************************
//				Add_Physics_Box:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void SB_Objects_Create::Add_Physics_Box(bool Dynamic, int Index)
{
	if (App->CLSB_GameDirector->V_Object[Index])
	{
		
	}
	else
	{
		App->Say("Object Index Out of Bounds");
		return;
	}

	Base_Object* Object = App->CLSB_GameDirector->V_Object[Index];

	if (Dynamic == 1)
	{
		Object->Type = Enums::Bullet_Type_Dynamic;
		Object->Shape = Enums::Shape_Box;

	}
	else
	{
		Object->Type = Enums::Bullet_Type_Static;
		Object->Shape = Enums::Shape_Box;
	}

	AxisAlignedBox worldAAB = Object->Object_Ent->getBoundingBox();
	worldAAB.transformAffine(Object->Object_Node->_getFullTransform());
	Ogre::Vector3 Centre = worldAAB.getCenter();

	Object->Physics_Pos = Ogre::Vector3(Centre.x, Centre.y, Centre.z);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 0, 1));

	btScalar mass;
	if (Dynamic == 1)
	{
		mass = 1.0f;
	}
	else
	{
		mass = 0.0f;
	}


	btVector3 localInertia(0, 0, 0);
	btVector3 initialPosition(Centre.x, Centre.y, Centre.z);
	startTransform.setOrigin(initialPosition);

	Ogre::Vector3 Size = App->CLSB_Object->GetMesh_BB_Size(Object->Object_Node);
	float sx = Size.x / 2;
	float sy = Size.y / 2;
	float sz = Size.z / 2;
	
	Object->Physics_Size = Ogre::Vector3(sx, sy, sz);

	btCollisionShape* newRigidShape = new btBoxShape(btVector3(sx, sy, sz));
	newRigidShape->calculateLocalInertia(mass, localInertia);

	//App->CLSB_Bullet->collisionShapes.push_back(newRigidShape);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, newRigidShape, localInertia);

	Object->Phys_Body = new btRigidBody(rbInfo);
	Object->Phys_Body->setRestitution(1.0);
	Object->Phys_Body->setFriction(1.5);
	Object->Phys_Body->setUserPointer(Object->Object_Node);
	Object->Phys_Body->setWorldTransform(startTransform);

	if (Dynamic == 1)
	{
		Object->Usage = Enums::Usage_Dynamic;
		Object->Phys_Body->setUserIndex(Enums::Usage_Dynamic);
		Object->Phys_Body->setUserIndex2(Index);
	}
	else
	{
		Object->Usage = Enums::Usage_Static;
		Object->Phys_Body->setUserIndex(Enums::Usage_Static);
		Object->Phys_Body->setUserIndex2(Index);
	}

	
	int f = Object->Phys_Body->getCollisionFlags();
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	Object->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CLSB_Bullet->dynamicsWorld->addRigidBody(Object->Phys_Body);

	App->CLSB_GameDirector->V_Object[Index]->Physics_Valid = 1;
	return;
	App->CLSB_Physics->Set_Physics(Index);
}

