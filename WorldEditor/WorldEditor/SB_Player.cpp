/*
Copyright (c) 2021 Scene Builder and Equity -- Inflanite Software W.T.Flanigan H.C.Flanigan

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
#include "resource.h"
#include "AB_App.h"
#include "SB_Player.h"

SB_Player::SB_Player()
{
	Player_Props_HWND = nullptr;

	ShowDebug = 0;
	Show_Physics_Debug = 1;

	Toggle_Objects_Flag = 1;
	Toggle_Physics_Flag = 0;

	Col_Object_Index = 0;
	Last_ColisionIndex = 0;

	Col_numManifolds = 0;
	Col_Player_Index = 0;

	Round = 0;
	Distance = 0;
	Life_Time = 0;

	is_Animated = 0;

	Last_Message_Index = 0;
	mJump = 0;
	Current_Position.ZERO;
	
	Is_On_Ground = 0;
	AddGravity = 0;
}


SB_Player::~SB_Player()
{
}

// *************************************************************************
// *	  					Reset_Class Bernie							   *
// *************************************************************************
void SB_Player::Reset_Class(void)
{
	int Count = 0;

	//while (Count < App->SBC_Scene->Player_Count) // Remove Ogre Objects
	//{
	//	if (App->SBC_Scene->B_Player[Count])
	//	{
	//		if (App->SBC_Scene->B_Player[Count]->Player_Ent && App->SBC_Scene->B_Player[Count]->Player_Node)
	//		{
	//			App->CL_Ogre->mSceneMgr->destroySceneNode(App->SBC_Scene->B_Player[Count]->Player_Node);
	//			App->CL_Ogre->mSceneMgr->destroyEntity(App->SBC_Scene->B_Player[Count]->Player_Ent);
	//			App->CL_Ogre->mSceneMgr->destroyCamera(App->SBC_Scene->B_Player[Count]->CameraPitch);
	//			App->SBC_Scene->B_Player[Count]->Player_Ent = nullptr;
	//			App->SBC_Scene->B_Player[Count]->Player_Node = nullptr;
	//			App->SBC_Scene->B_Player[Count]->CameraPitch = nullptr;
	//		}

	//		delete App->SBC_Scene->B_Player[Count];
	//		App->SBC_Scene->B_Player[Count] = nullptr;
	//	}

	//	Count++;
	//}

	//App->SBC_Scene->Player_Count = 0;
	//App->SBC_Scene->Player_Added = 0;

}


// *************************************************************************
// *	  	Create_Player_Object:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Player::Create_Player_Object(void)
{
	int Index = App->CLSB_Scene_Data->Player_Count;

	App->CLSB_Scene_Data->B_Player[Index] = new Base_Player();

	Initialize();

	App->CLSB_Scene_Data->B_Player[Index]->CameraPitch = App->CLSB_Ogre_Setup->mSceneMgr->createCamera("PlayerPitch");

	App->CLSB_Scene_Data->Player_Count++;

}

// *************************************************************************
// *	  		Initialize:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Player::Initialize()
{
	Ogre::Vector3 Pos;

	int Index = App->CLSB_Scene_Data->Player_Count;

	Base_Player* pBase = App->CLSB_Scene_Data->B_Player[Index];

	// ------------------- Ogre
	if (pBase->Player_Ent && pBase->Player_Node)
	{
		App->CLSB_Ogre_Setup->mSceneMgr->destroySceneNode(pBase->Player_Node);
		App->CLSB_Ogre_Setup->mSceneMgr->destroyEntity(pBase->Player_Ent);
		App->CLSB_Ogre_Setup->mSceneMgr->destroyCamera(pBase->CameraPitch);
		pBase->Player_Ent = nullptr;
		pBase->Player_Node = nullptr;
		pBase->CameraPitch = nullptr;
	}

	pBase->Player_Ent = App->CLSB_Ogre_Setup->mSceneMgr->createEntity("Player_1", "axes.mesh", App->CLSB_Ogre_Setup->App_Resource_Group);
	pBase->Player_Node = App->CLSB_Ogre_Setup->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	pBase->Player_Node->attachObject(pBase->Player_Ent);
	pBase->Player_Node->setVisible(false);

	Pos.x = pBase->StartPos.x;
	Pos.y = pBase->StartPos.y;
	Pos.z = pBase->StartPos.z;

	pBase->Player_Node->setPosition(Pos.x, Pos.y, Pos.z);

	App->CLSB_Scene_Data->Player_Added = 1;

}

// *************************************************************************
// *	  			Start_Player_PropsPanel Terry Bernie				   *
// *************************************************************************
bool SB_Player::Start_Player_PropsPanel()
{

	//Player_Props_HWND = CreateDialog(App->hInst, (LPCTSTR)IDD_PROPS_PLAYER, App->SBC_Properties->Properties_Dlg_hWnd, (DLGPROC)Player_PropsPanel_Proc);
	return 1;
}
// *************************************************************************
// *		  	Player_PropsPanel_Proc  Terry Bernie					   *
// *************************************************************************
LRESULT CALLBACK SB_Player::Player_PropsPanel_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		/*SendDlgItemMessage(hDlg, IDC_BTSAVE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTOBJECT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTPHYSICS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_PHYSICSDEBUG, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_GOTO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDC_BTPL_LOCATIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_COLLISIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));*/

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		//return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;
		

		//if (some_item->idFrom == IDC_BTSAVE && some_item->code == NM_CUSTOMDRAW)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Normal(item);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_BTLOOKAT && some_item->code == NM_CUSTOMDRAW)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Normal(item);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_BTPL_LOCATIONS && some_item->code == NM_CUSTOMDRAW)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Normal(item);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_BT_GOTO && some_item->code == NM_CUSTOMDRAW)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Normal(item);
		//	return CDRF_DODEFAULT;
		//}

		//// ------------------------------------------ 
		//if (some_item->idFrom == IDC_BTOBJECT && some_item->code == NM_CUSTOMDRAW)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Toggle(item, App->SBC_Player->Toggle_Objects_Flag);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_BTPHYSICS && some_item->code == NM_CUSTOMDRAW)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Toggle(item, App->SBC_Player->Toggle_Physics_Flag);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_PHYSICSDEBUG && some_item->code == NM_CUSTOMDRAW)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Toggle(item, App->SBC_Player->Show_Physics_Debug);
		//	return CDRF_DODEFAULT;
		//}

		//if (some_item->idFrom == IDC_BT_COLLISIONS && some_item->code == NM_CUSTOMDRAW)
		//{
		//	LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		//	App->Custom_Button_Toggle(item, App->CL_Vm_ImGui->Show_Collision_Debug);
		//	return CDRF_DODEFAULT;
		//}
		
		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:

		//if (LOWORD(wParam) == IDC_BTPL_LOCATIONS)
		//{
		//	App->SBC_Locations->Start_Locations_Dlg();
		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_BT_COLLISIONS)
		//{
		//	if (App->CL_Vm_ImGui->Show_Collision_Debug == 1)
		//	{
		//		App->CL_Vm_ImGui->Show_Collision_Debug = 0;
		//	}
		//	else
		//	{
		//		App->CL_Vm_ImGui->Show_Collision_Debug = 1;
		//	}

		//	return 1;
		//}
		//
		//if (LOWORD(wParam) == IDC_BT_GOTO)
		//{

		//	//App->CL_Ogre->mCamera->setPosition(Ogre::Vector3(App->SBC_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin()));
		//	
		//	float x = App->SBC_Scene->B_Player[0]->StartPos.x;
		//	float y = App->SBC_Scene->B_Player[0]->StartPos.y;
		//	float z = App->SBC_Scene->B_Player[0]->StartPos.z;

		//	App->SBC_Scene->B_Player[0]->Phys_Body->getWorldTransform().setOrigin(btVector3(x,y,z));
		//	App->SBC_Scene->B_Player[0]->Phys_Body->getWorldTransform().setRotation(App->SBC_Scene->B_Player[0]->Physics_Rotation);
		//	

		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_BTSAVE)
		//{
		//	if (App->SBC_Scene->Scene_Loaded == 1)
		//	{

		//		App->SBC_Scene->B_Player[0]->StartPos = Ogre::Vector3(App->SBC_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin());
		//		App->SBC_Scene->B_Player[0]->Physics_Rotation = App->SBC_Scene->B_Player[0]->Phys_Body->getWorldTransform().getRotation();
		//		
		//		App->SBC_Scene->B_Locations[0]->Physics_Position = App->SBC_Scene->B_Player[0]->Phys_Body->getWorldTransform().getOrigin();
		//		App->SBC_Scene->B_Locations[0]->Physics_Rotation = App->SBC_Scene->B_Player[0]->Phys_Body->getWorldTransform().getRotation();

		//		App->SBC_Scene->B_Player[0]->Altered = 1;
		//		App->SBC_Scene->Scene_Modified = 1;
		//		App->SBC_FileView->Mark_Altered(App->SBC_Scene->B_Player[0]->FileViewItem);

		//		App->SBC_Properties->Update_ListView_Player();

		//		App->Say("Player Saved");
		//	}
		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_BTOBJECT)
		//{
		//	if (App->SBC_Scene->Scene_Loaded == 1)
		//	{
		//		App->SBC_Properties->Edit_Physics = 0;
		//		App->SBC_Properties->Update_ListView_Player();

		//		App->SBC_Player->Toggle_Objects_Flag = 1;
		//		App->SBC_Player->Toggle_Physics_Flag = 0;
		//		RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		//	}
		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_BTPHYSICS)
		//{
		//	if (App->SBC_Scene->Scene_Loaded == 1)
		//	{
		//		App->SBC_Properties->Edit_Physics = 1;
		//		App->SBC_Properties->Update_ListView_Player_Physics();

		//		App->SBC_Player->Toggle_Objects_Flag = 0;
		//		App->SBC_Player->Toggle_Physics_Flag = 1;
		//		RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		//	}
		//	return 1;
		//}

		//if (LOWORD(wParam) == IDC_PHYSICSDEBUG)
		//{
		//	int f = App->SBC_Scene->B_Player[0]->Phys_Body->getCollisionFlags();

		//	if (App->SBC_Player->Show_Physics_Debug == 1)
		//	{
		//		App->SBC_Player->Show_Physics_Debug = 0;
		//		App->SBC_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f | (1 << 5));

		//		App->CL_Ogre->BulletListener->Render_Debug_Flag = 0;
		//		App->CL_Ogre->RenderFrame();
		//		App->CL_Ogre->BulletListener->Render_Debug_Flag = 1;
		//	}
		//	else
		//	{
		//		App->SBC_Player->Show_Physics_Debug = 1;
		//		App->SBC_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f & (~(1 << 5)));
		//	}
		//	
		//	return 1;
		//}


		break;
	}
	return FALSE;
}

// *************************************************************************
// *					Hide_Player_Dlg Terry Bernie 					   *
// *************************************************************************
void SB_Player::Hide_Player_Dlg(bool Show)
{
	ShowWindow(Player_Props_HWND, Show);
}

// *************************************************************************
// *	  					Adjust_CapsuleTerry Bernie					   *
// *************************************************************************
void SB_Player::Adjust_Capsule(void)
{
	//App->SBC_Scene->B_Player[0]->Phys_Shape = new btCapsuleShape(btScalar(App->SBC_Scene->B_Player[0]->Capsule_Radius), btScalar(App->SBC_Scene->B_Player[0]->Capsule_Height));
	//App->SBC_Scene->B_Player[0]->Phys_Body->setCollisionShape(App->SBC_Scene->B_Player[0]->Phys_Shape);

}

// *************************************************************************
// *	  					Rotate Terry Bernie							   *
// *************************************************************************
bool SB_Player::OnGround() const
{
	//return App->SBC_Scene->B_Player[0]->mOnGround;
	return 0;
}


