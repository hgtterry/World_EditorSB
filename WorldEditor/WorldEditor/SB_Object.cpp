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
#include "SB_Object.h"

SB_Object::SB_Object()
{

}

SB_Object::~SB_Object()
{
}

// *************************************************************************
//		Get_BoundingBox_World_Centre:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
Ogre::Vector3 SB_Object::Get_BoundingBox_World_Centre(int Object_Index)
{
	/*if (App->SBC_Scene->V_Object[Object_Index]->Shape == Enums::Shape_TriMesh)
	{
		Ogre::Vector3 Pos = App->SBC_Scene->V_Object[Object_Index]->Object_Node->getPosition();
		return Pos;
	}
	else*/
	{
		AxisAlignedBox worldAAB = App->CLSB_Game_Editor->V_Object[Object_Index]->Object_Ent->getBoundingBox();
		worldAAB.transformAffine(App->CLSB_Game_Editor->V_Object[Object_Index]->Object_Node->_getFullTransform());
		Ogre::Vector3 Centre = worldAAB.getCenter();

		return Centre;
	}
}

// *************************************************************************
// *	  		GetMesh_BB_Radius:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
float SB_Object::GetMesh_BB_Radius(SceneNode* mNode)
{
	AxisAlignedBox aab = mNode->getAttachedObject(0)->getBoundingBox();
	Ogre::Vector3 min = aab.getMinimum() * mNode->getScale();
	Ogre::Vector3 max = aab.getMaximum() * mNode->getScale();
	Ogre::Vector3 center = aab.getCenter() * mNode->getScale();
	Ogre::Vector3 size(fabs(max.x - min.x), fabs(max.y - min.y), fabs(max.z - min.z));
	float radius = (size.x > size.z) ? size.z / 2.0f : size.x / 2.0f;

	return radius;
}

// *************************************************************************
// *	  		GetMesh_BB_Size:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
Ogre::Vector3 SB_Object::GetMesh_BB_Size(SceneNode* mNode)
{
	AxisAlignedBox aab = mNode->getAttachedObject(0)->getBoundingBox();
	Ogre::Vector3 min = aab.getMinimum() * mNode->getScale();
	Ogre::Vector3 max = aab.getMaximum() * mNode->getScale();
	Ogre::Vector3 center = aab.getCenter() * mNode->getScale();
	Ogre::Vector3 size(fabs(max.x - min.x), fabs(max.y - min.y), fabs(max.z - min.z));
	float radius = (size.x > size.z) ? size.z / 2.0f : size.x / 2.0f;

	return size;
}

// *************************************************************************
//			Object_Camera_Goto:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void SB_Object::Object_Camera_Goto(int Object_Index)
{
	App->CLSB_Ogre_Setup->OgreListener->GD_CameraMode = Enums::CamDetached;

	Ogre::Vector3 WS;
	Ogre::Vector3 WS2;
	Ogre::Vector3 Centre;
	Ogre::Vector3 CentreNode;
	Ogre::Vector3 Size;

	int Index = Object_Index;

	if (App->CLSB_Properties->Edit_Category == Enums::Edit_Area)
	{
		//Centre = App->CLSB_Scene->B_Area[Index]->Area_Node->getAttachedObject(0)->getBoundingBox().getCenter();
		//WS = App->CLSB_Scene->B_Area[Index]->Area_Node->convertLocalToWorldPosition(Centre);
	}
	else
	{
		CentreNode = App->CLSB_Game_Editor->V_Object[Index]->Object_Node->getPosition();

		Centre = App->CLSB_Game_Editor->V_Object[Index]->Object_Node->getAttachedObject(0)->getBoundingBox().getCenter();
		Size = App->CLSB_Game_Editor->V_Object[Index]->Object_Node->getAttachedObject(0)->getBoundingBox().getMaximum();

		//Centre.z = Centre.z + Size.z;

		WS = App->CLSB_Game_Editor->V_Object[Index]->Object_Node->convertLocalToWorldPosition(Size);
		WS2 = App->CLSB_Game_Editor->V_Object[Index]->Object_Node->convertLocalToWorldPosition(Centre);
		//WS.z = WS.z - 40;// Size.z;
	}

	App->CLSB_Ogre_Setup->mCamera->setPosition(WS);
	App->CLSB_Ogre_Setup->mCamera->lookAt(WS2);
}

// *************************************************************************
// *	  		GetPlacement:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
Ogre::Vector3 SB_Object::GetPlacement(int Distance)
{

	Ogre::Vector3 Placement;
	Ogre::Vector3 CamPos;
	Ogre::Quaternion CamRot;
	Ogre::Radian mYaw;
	Ogre::Vector3 mDirection;


	CamPos = App->CLSB_Ogre_Setup->mCamera->getPosition();
	mYaw = App->CLSB_Ogre_Setup->mCamera->getOrientation().getYaw();

	App->CLSB_Ogre_Setup->PlacementCam->setPosition(CamPos);

	CamRot = Ogre::Quaternion::IDENTITY;
	App->CLSB_Ogre_Setup->PlacementCam->setOrientation(CamRot);
	App->CLSB_Ogre_Setup->PlacementCam->yaw(mYaw);

	Ogre::Vector3 TranslateVector = Ogre::Vector3::ZERO;

	//float Radius = OgreNode->getAttachedObject(0)->getBoundingRadius();
	//Radius = (Radius * 7) + 2;


	TranslateVector.z = Distance; // Distance from Camera/Player

	CamPos = App->CLSB_Ogre_Setup->mCamera->getPosition();

	App->CLSB_Ogre_Setup->PlacementCam->moveRelative(TranslateVector);

	Placement = App->CLSB_Ogre_Setup->PlacementCam->getPosition();
	Placement.y = CamPos.y - 3; // = Placement.y - (float)13.5 / 2;

	return Placement;
}

// **************************************************************************
// *	  		Delete_Object:- Terry and Hazel Flanigan 2022				*
// **************************************************************************
void SB_Object::Delete_Object()
{
	int MeshIndex = App->CLSB_Properties->Current_Selected_Object;
	btRigidBody* body = App->CLSB_Game_Editor->V_Object[MeshIndex]->Phys_Body;

	if (body)
	{
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(body);
	}

	App->CLSB_FileView->DeleteItem();

	App->CLSB_Game_Editor->V_Object[MeshIndex]->Deleted = 1;
	App->CLSB_Game_Editor->V_Object[MeshIndex]->Object_Node->setVisible(false);

	App->CLSB_Scene->Scene_Modified = 1;
}

// *************************************************************************
// *			Rename_Object:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_Object::Rename_Object(int Index)
{
	Base_Object* Object = App->CLSB_Game_Editor->V_Object[Index];

	strcpy(App->CLSB_Dialogs->btext, "Change Object Name");
	strcpy(App->CLSB_Dialogs->Chr_Text, Object->Mesh_Name);

	App->CLSB_Dialogs->Dialog_Text(Enums::Check_Names_Objects);

	if (App->CLSB_Dialogs->Canceled == 1)
	{
		return;
	}

	strcpy(Object->Mesh_Name, App->CLSB_Dialogs->Chr_Text);

	Object->Altered = 1;
	App->CLSB_Scene->Scene_Modified = 1;
	App->CLSB_FileView->Mark_Altered(Object->FileViewItem);

	App->CLSB_FileView->Change_Item_Name(Object->FileViewItem, Object->Mesh_Name);
}
