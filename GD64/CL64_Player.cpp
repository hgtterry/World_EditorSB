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
#include "CL64_Player.h"

CL64_Player::CL64_Player(void)
{
	Player_Count = 0;
}

CL64_Player::~CL64_Player(void)
{
}

// *************************************************************************
// *	  	Create_Player_Object:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_Player::Create_Player_Object(void)
{
	int Index = Player_Count;

	App->CL_Scene->B_Player[Index] = new Base_Player();

	Initialize();

	App->CL_Scene->B_Player[Index]->CameraPitch = App->CL_Ogre->mSceneMgr->createCamera("PlayerPitch");
	
	Player_Count++;

}

// *************************************************************************
// *	  			Initialize:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Player::Initialize()
{
	Ogre::Vector3 Pos;

	int Index = Player_Count;

	Base_Player* pBase = App->CL_Scene->B_Player[Index];
	
	// ------------------- Ogre
	/*if (pBase->Player_Ent && pBase->Player_Node)
	{
		App->CL_Ogre->mSceneMgr->destroySceneNode(pBase->Player_Node);
		App->CL_Ogre->mSceneMgr->destroyEntity(pBase->Player_Ent);
		App->CL_Ogre->mSceneMgr->destroyCamera(pBase->CameraPitch);
		pBase->Player_Ent = nullptr;
		pBase->Player_Node = nullptr;
		pBase->CameraPitch = nullptr;
	}*/
	
	/*pBase->Player_Ent = App->CL_Ogre->mSceneMgr->createEntity("Player_1", "axes.mesh", App->CL_Ogre->App_Resource_Group);
	pBase->Player_Node = App->CL_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	pBase->Player_Node->attachObject(pBase->Player_Ent);
	pBase->Player_Node->setVisible(false);*/

	Pos.x = 0; //pBase->StartPos.x;
	Pos.y = 0; //pBase->StartPos.y;
	Pos.z = 0;// pBase->StartPos.z;


	
	//pBase->Player_Node->setPosition(Pos.x, Pos.y, Pos.z);
	
	// ------------------------ Bulet
	btVector3 pos = btVector3(Pos.x, Pos.y, Pos.z);
	
	btVector3 inertia = btVector3(0, 0, 0);
	btQuaternion rot = btQuaternion(0, 0, 0, 1);
	btDefaultMotionState* state = new btDefaultMotionState(btTransform(rot, pos));
	
	pBase->Phys_Shape = new btCapsuleShape(btScalar(pBase->Capsule_Radius), btScalar(pBase->Capsule_Height));
	pBase->Phys_Body = new btRigidBody(pBase->Capsule_Mass, state, pBase->Phys_Shape, inertia);
	//pBase->Phys_Body->setActivationState(DISABLE_DEACTIVATION);
	pBase->Phys_Body->setSleepingThresholds(0.0, 0.0);
	pBase->Phys_Body->setAngularFactor(0.0);

	pBase->Phys_Body->setUserPointer(pBase->Player_Node);

	pBase->Phys_Body->setUserIndex(Enums::Usage_Player);


	int f = pBase->Phys_Body->getCollisionFlags();
	//pBase->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->CL_Scene->B_Player[0]->Physics_Rotation);
	App->CL_Bullet->dynamicsWorld->addRigidBody(pBase->Phys_Body);

	// Save for later
	/*Current_Position = pBase->Player_Node->getPosition();
	Physics_Position = pBase->Phys_Body->getWorldTransform().getOrigin();
	Physics_Rotation = pBase->Phys_Body->getWorldTransform().getRotation();*/



	/*App->SBC_DCC = new DynamicCharacterController(pBase->Phys_Body, NULL);
	App->SBC_DCC->mShapeRadius = pBase->Capsule_Radius;
	App->SBC_DCC->mShapeHalfHeight = pBase->Capsule_Height / 2;

	App->SBC_DCC->setMovementDirection(btVector3(0, 0, 1));
	App->SBC_DCC->updateAction(App->SBC_Bullet->dynamicsWorld, 1);

	App->SBC_Scene->Player_Added = 1;*/

}
