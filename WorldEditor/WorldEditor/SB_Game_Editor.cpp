/*
Copyright (c) 2022 - 2024 Reality Factory Scene Builder -- HGT Software W.T.Flanigan H.C.Flanigan

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
#include "SB_Game_Editor.h"

SB_Game_Editor::SB_Game_Editor(void)
{
	Project_Loaded = 0;
}

SB_Game_Editor::~SB_Game_Editor(void)
{
}

//// *************************************************************************
//// *			Go_GameDirector:- Terry and Hazel Flanigan 2023		 	   *
//// *************************************************************************
//void SB_GameDirector::Go_GameDirector()
//{
//	if (App->Equity_Dlg_hWnd)
//	{
//		App->Get_Current_Document();
//
//		App->CLSB_Equity->Equity_Render_Mode = Enums::EQ_Mode_GameDirector;
//
//		App->CLSB_ImGui->Show_Physics_Console = 1;
//
//		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_MODEL), 0);
//		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_FIRST_MODEX), 1);
//		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_FREE), 1);
//		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, IDC_UPDATE2), 1);
//
//		App->CLSB_Ogre->BulletListener->Render_Debug_Flag = 1;
//
//		RedrawWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
//
//		Ogre::Vector3 OldCamPos;
//
//		//------------------------------------------------------
//		int BCount = App->CL_World->Get_Brush_Count();
//		if (BCount == 0)
//		{
//			App->Say("No Scene to Preview");
//			return;
//		}
//
//		if (App->CLSB_Scene->Scene_Loaded == 0)
//		{
//			App->CLSB_TopTabs->Update_Dlg_Controls();
//
//			OldCamPos = App->CLSB_Camera_EQ->Saved_Pos;
//			Ogre::Quaternion OldCamRot = App->CLSB_Camera_EQ->Saved_Rotation;
//			App->CLSB_Ogre->mCamera->setPosition(Ogre::Vector3(OldCamPos.x, OldCamPos.y, OldCamPos.z));
//			App->CLSB_Ogre->mCamera->setOrientation(OldCamRot);
//
//			App->CLSB_Model->Set_Equity();
//			App->CLSB_Camera_EQ->Reset_Orientation();
//			App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamDetached);
//			App->CLSB_TopTabs_Equity->Camera_Set_Free();
//
//			App->CLSB_Model->Model_Loaded = 1;
//			App->CLSB_Ogre->RenderListener->ShowBrushes = 1;
//			App->CLSB_Ogre->RenderListener->ShowTextured = 1;
//			
//			App->CLSB_Model->Render_Type = Enums::Render_Ogre;
//
//			ShowWindow(App->ListPanel, true);
//			ShowWindow(App->CLSB_Properties->Properties_Dlg_hWnd, true);
//
//			SetWindowText(App->Equity_Dlg_hWnd, "GameDirector");
//
//			ShowWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, true);
//
//			HMENU TestMenu;
//			TestMenu = LoadMenu(App->hInst, MAKEINTRESOURCE(IDR_MENUSB));
//			SetMenu(App->Equity_Dlg_hWnd, TestMenu);
//
//			ShowWindow(App->Equity_Dlg_hWnd, SW_SHOW);
//			App->CLSB_Equity->EquitySB_Dialog_Visible = 1;
//			//App->CLSB_Equity->Show_Equity_Dialog(true);
//
//			App->CLSB_TopTabs->Update_Dlg_Controls();
//			App->CLSB_Equity->Resize_3DView();
//			App->CLSB_Panels->MovePhysicsView();
//
//			if (App->CLSB_Equity->First_Run == 1)
//			{
//				App->CLSB_Camera_EQ->Zero_View();
//				App->CLSB_Equity->First_Run = 0;
//			}
//
//			App->CLSB_Mesh_Mgr->Update_World();
//
//			App->CLSB_Project->Load_Project();
//
//			App->CLSB_Environment->Add_New_Environ_Entity(true);
//			App->CLSB_Environment->Set_First_Environment(App->CLSB_Scene->Object_Count - 1);
//		}
//		else
//		{
//			App->CLSB_Mesh_Mgr->Update_World();
//
//			//App->CLSB_Model->Set_BondingBox_Brushes();
//
//			if (App->CLSB_Equity->Saved_Camera_Mode == 0)
//			{
//				App->CLSB_Camera_EQ->Reset_Orientation();
//				App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamDetached);
//				App->CLSB_TopTabs_Equity->Camera_Set_Free();
//			}
//			else
//			{
//				App->CLSB_Camera_EQ->Reset_Orientation();
//				App->CLSB_Ogre->OgreListener->CameraMode = Enums::CamFirst;
//				App->CLSB_TopTabs_Equity->Camera_Set_First();
//			}
//
//			OldCamPos = App->CLSB_Camera_EQ->Saved_Pos;
//			Ogre::Quaternion OldCamRot = App->CLSB_Camera_EQ->Saved_Rotation;
//			App->CLSB_Ogre->mCamera->setPosition(Ogre::Vector3(OldCamPos.x, OldCamPos.y, OldCamPos.z));
//			App->CLSB_Ogre->mCamera->setOrientation(OldCamRot);
//
//			App->CLSB_Model->Model_Loaded = 1;
//			App->CLSB_Ogre->RenderListener->ShowBrushes = 1;
//			App->CLSB_Ogre->RenderListener->ShowTextured = 1;
//			//App->CLSB_Model->Model_Type = Enums::LoadedFile_Brushes;
//
//			App->CLSB_Model->Render_Type = Enums::Render_Ogre;
//
//			ShowWindow(App->ListPanel, true);
//			ShowWindow(App->CLSB_Properties->Properties_Dlg_hWnd, true);
//
//			ShowWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, true);
//
//			HMENU TestMenu;
//			TestMenu = LoadMenu(App->hInst, MAKEINTRESOURCE(IDR_MENUSB));
//			SetMenu(App->Equity_Dlg_hWnd, TestMenu);
//			SetWindowText(App->Equity_Dlg_hWnd, "GameDirector");
//
//			App->CLSB_Equity->EquitySB_Dialog_Visible = 1;
//			ShowWindow(App->Equity_Dlg_hWnd, SW_SHOW);
//
//			//App->CLSB_Equity->Show_Equity_Dialog(true);
//
//			App->CLSB_TopTabs->Update_Dlg_Controls();
//
//			App->CLSB_Environment->Set_First_Environment(App->CLSB_Scene->Object_Count - 1);
//			App->CLSB_GameDirector->Show_Objects(true);
//		}
//	}
//}

// *************************************************************************
// *			Go_Game_Editor:- Terry and Hazel Flanigan 2023		 	   *
// *************************************************************************
void SB_Game_Editor::Go_Game_Editor()
{
	if (App->Equity_Dlg_hWnd)
	{
		App->Get_Current_Document();

		if (App->CLSB_ImGui->ImGui_Surface_Active == 1)
		{
			App->CLSB_ImGui->Stop_Render();
			EndDialog(App->CLSB_ImGui->ImGui_Dlg_Surface_hWnd,0);
			App->CLSB_ImGui->ImGui_Surface_Active = 0;
			//Debug
		}

		App->CLSB_Equity->Equity_Render_Mode = Enums::EQ_Mode_GameDirector;

		App->CLSB_ImGui->Show_Physics_Console = 1;

		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_MODEL), 0);
		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_FIRST_MODEX), 1);
		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_FREE), 1);
		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, IDC_UPDATE2), 1);

		App->CLSB_Ogre->BulletListener->Render_Debug_Flag = 1;

		RedrawWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		//------------------------------------------------------
		int BCount = App->CL_World->Get_Brush_Count();
		if (BCount == 0)
		{
			App->Say("No Scene to Preview");
			return;
		}

		
		App->CLSB_TopTabs->Update_Dlg_Controls();

		App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamDetached);
		App->CLSB_TopTabs_Equity->Camera_Set_Free();

		App->CLSB_Model->Model_Loaded = 1;
		App->CLSB_Ogre->RenderListener->ShowBrushes = 1;
		App->CLSB_Ogre->RenderListener->ShowTextured = 1;

		App->CLSB_Model->Render_Type = Enums::Render_Ogre;

		ShowWindow(App->ListPanel, true);
		ShowWindow(App->CLSB_Properties->Properties_Dlg_hWnd, true);

		SetWindowText(App->Equity_Dlg_hWnd, "Game Editor");

		ShowWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, true);

		HMENU TestMenu;
		TestMenu = LoadMenu(App->hInst, MAKEINTRESOURCE(IDR_MENUGAMEEDITOR));
		SetMenu(App->Equity_Dlg_hWnd, TestMenu);


		//  Set Position
		geVec3d Pos = App->CLSB_Camera_WE->FindCameraEntity()->mOrigin;
		App->CLSB_Ogre->mCamera->setPosition(Pos.X, Pos.Y, Pos.Z);
		geVec3d Angles;
		App->CLSB_Camera_WE->FindCameraEntity()->GetAngles(&Angles, Level_GetEntityDefs(App->CLSB_Doc->pLevel));


		Ogre::Quaternion Rotation;
		//Rotation.IDENTITY;
		Rotation.FromAngleAxis(Ogre::Degree(Angles.X), Rotation * Ogre::Vector3::UNIT_X);
		Rotation.FromAngleAxis(Ogre::Radian(60), Rotation * Ogre::Vector3::UNIT_Y);
		Rotation.FromAngleAxis(Ogre::Degree(0), Rotation * Ogre::Vector3::UNIT_Z);

		App->CLSB_Ogre->mCamera->setOrientation(Rotation);
		// ------------------

		ShowWindow(App->Equity_Dlg_hWnd, SW_SHOW);
		App->CLSB_Equity->EquitySB_Dialog_Visible = 1;

		App->CLSB_TopTabs->Update_Dlg_Controls();
		App->CLSB_Equity->Resize_3DView();
		App->CLSB_Panels->MovePhysicsView();

		if (App->CLSB_Equity->First_Run == 1)
		{
			//App->CLSB_Camera_EQ->Zero_View();
			App->CLSB_Equity->First_Run = 0;
		}

		App->CLSB_Mesh_Mgr->Update_World();

		if (App->CLSB_GameDirector->Project_Loaded == 0)
		{
			App->CLSB_Project->Load_Project();
			App->CLSB_Environment->Add_New_Environ_Entity(true);
			App->CLSB_Environment->Set_First_Environment(App->CLSB_Scene->Object_Count - 1);
		}


	}
}

// *************************************************************************
// *	  Hide_Game_Editor_Dialog:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void SB_Game_Editor::Hide_Game_Editor_Dialog()
{
	//App->Say("Stop GD");

	if (App->CLSB_Equity->Equity_Render_Mode == Enums::EQ_Mode_GameDirector);
	{
		App->CLSB_Camera_EQ->Save_Camera_Pos();
		App->CLSB_Equity->Saved_Camera_Mode = App->CLSB_TopTabs_Equity->Toggle_Camera_First_Flag;
	}

	App->CLSB_Equity->EquitySB_Dialog_Visible = 0;
	App->CLSB_TopTabs->Update_Dlg_Controls();

	ShowWindow(App->Equity_Dlg_hWnd, SW_HIDE);

	App->CLSB_Tabs_True3D_Dlg->GameEditor_Active_Flag = 0;
	RedrawWindow(App->CLSB_ViewMgrDlg->MgrDlg_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	if (App->CLSB_Mesh_Mgr->Brush_Viewer_Dialog_Active)
	{
		App->CLSB_Mesh_Mgr->Brush_Viewer_Dialog_Active = 0;
		App->CLSB_TopTabs_Equity->Toggle_MeshManager_Flag = 0;
		EndDialog(App->CLSB_Mesh_Mgr->Mesh_Viewer_HWND, 0);
	}

	if (App->CLSB_ViewMgrDlg->Was_BR_True3D_Mode_Active == 1)
	{
		App->CLSB_BR_Render->Start_BR_3D_Mode();
		App->CLSB_ViewMgrDlg->Was_BR_True3D_Mode_Active = 0;
	}
}

// *************************************************************************
//			Show_Objects:- Terry and Hazel Flanigan 2024        		   *
// *************************************************************************
void SB_Game_Editor::Show_Objects(bool Flag)
{
	int Object_Count = App->CLSB_Scene->Object_Count-1;
	int Count = 0;

	while (Count < Object_Count)
	{
		if (V_Object[Count]->Object_Node)
		{
			V_Object[Count]->Object_Node->setVisible(Flag);
		}

		Count++;
	}
}



