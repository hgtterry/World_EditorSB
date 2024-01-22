#include "StdAfx.h"
#include "AB_App.h"
#include "resource.h"
#include "SB_Equity.h"

SB_Equity::SB_Equity(void)
{
	mAutoLoad = 0;
	EquitySB_Dialog_Created = 0;
	EquitySB_Dialog_Visible = 0;

	Equity_Render_Mode = Enums::EQ_Mode_None;

	OgreView_3D_hWnd =		nullptr;
	
	First_Run = 1;
	Saved_Camera_Mode = 0;
	Close_Equity_Flag = 0;
}

SB_Equity::~SB_Equity(void)
{
}

// *************************************************************************
// *			Auto_Load_File:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Equity::Auto_Load_File()
{
	if (mAutoLoad == 1)
	{
		char Path[MAX_PATH];
		strcpy(Path, App->WorldEditor_Directory);
		strcat(Path, "Data\\Temp.Wepf");

		strcpy(App->CLSB_Loader->Path_FileName, Path);
		strcpy(App->CLSB_Loader->FileName, "Temp.Wepf");

		bool test = App->CLSB_FileIO->Check_File_Exist(Path);
		if (test == 0)
		{
			App->Message_ToFile("Auto_Load_File No File");
		}
		else
		{
			App->Message_ToFile("Auto_Load_File File Ok");
		}

		App->CLSB_Loader->Read_Project_File(Path);

		test = App->CLSB_Loader->Load_File_Wepf();
		if (test == 0)
		{
			return 0;
		}

		return 1;
	}

	return 1;
}

// *************************************************************************
// *		  Hide_Equity_Dialog:- Terry and Hazel Flanigan 2024	 	   *
// *************************************************************************
void SB_Equity::Hide_Equity_Dialog()
{
	//App->Say("Stop Equity");

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

	/*if (App->CLSB_ViewMgrDlg->Was_BR_True3D_Mode_Active == 1)
	{
		App->CLSB_BR_Render->Start_BR_3D_Mode();
		Debug
	}*/
}

// *************************************************************************
// *		Start_Equity_Dialog_New:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Equity::Start_Equity_Dialog_New()
{
	App->CLSB_ViewMgrDlg->Was_BR_True3D_Mode_Active = App->CLSB_ViewMgrDlg->View_MgrDlg_Active;

	if (EquitySB_Dialog_Created == 0)
	{
		App->Equity_Dlg_hWnd = nullptr;
		mAutoLoad = 0;

		App->Equity_Dlg_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_EQUITYME, App->MainHwnd, (DLGPROC)Equity_Dialog_New_Proc);

		App->CLSB_TopTabs_Equity->Start_Tabs();

		EquitySB_Dialog_Created = 1;

		App->CLSB_Ogre_Setup->RenderHwnd = App->ViewGLhWnd;

		Resize_3DView();

		App->CLSB_FileView->Start_FileView();
		App->CLSB_Panels->Move_FileView_Window();
		App->CLSB_Panels->MovePhysicsView();

		App->CLSB_Properties->Start_GD_Properties();

		App->CLSB_Props_Dialogs->Start_Props_Dialogs();

		Set_Mode_Equity();

	}
}

// **************************************************************************
// *			Equity_Dialog_New_Proc:- Terry and Hazel Flanigan 2022		*
// **************************************************************************
LRESULT CALLBACK SB_Equity::Equity_Dialog_New_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->ViewGLhWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_VIEWER3D, hDlg, NULL);// (DLGPROC)Ogre3D_Proc);
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_TITLENAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 255, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}*/
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_MOVING:
	{
		App->CLSB_Panels->Move_FileView_Window();
		App->CLSB_Panels->MovePhysicsView();
		App->CLSB_Panels->Place_GlobalGroups();

		Root::getSingletonPtr()->renderOneFrame();
		return 0;
	}

	case WM_SIZE:
	{
		App->CLSB_Equity->Resize_3DView();

		App->CLSB_Panels->Move_FileView_Window();
		App->CLSB_Panels->MovePhysicsView();
		App->CLSB_Panels->Place_GlobalGroups();

		Ogre::Root::getSingletonPtr()->renderOneFrame();

	}break;

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == ID_MODE_GROUPS)
		{
			App->CLSB_Model->Render_Type = Enums::Render_Groups;
			App->CLSB_Mesh_Mgr->World_Node->setVisible(false);
			return TRUE;
		}

		if (LOWORD(wParam) == ID_RENDER_BRUSEHS)
		{
			if (App->CLSB_Ogre_Setup->RenderListener->ShowBrushes == 1)
			{
				App->CLSB_Ogre_Setup->RenderListener->ShowBrushes = 0;
			}
			else
			{
				App->CLSB_Ogre_Setup->RenderListener->ShowBrushes = 1;
			}

			return TRUE;
		}
		
		if (LOWORD(wParam) == ID_DEBUG_PREFERENCESWE)
		{
			App->CLSB_Preferences->Start_Preferences_Dlg();
			return TRUE;
		}

		if (LOWORD(wParam) == ID_DEBUG_GENERAL)
		{
			App->CLSB_Ogre_Setup->RenderListener->JustTexture_ID++;

			/*if (App->CLSB_Ogre->RenderListener->ShowBoundingGroup == 1)
			{
				App->CLSB_Ogre->RenderListener->ShowBoundingGroup = 0;
			}
			else
			{
				App->CLSB_Ogre->RenderListener->ShowBoundingGroup = 1;
			}*/

			return TRUE;
		}

		if (LOWORD(wParam) == ID_DEBUG_PROJECTINFO)
		{
			if (App->CLSB_ImGui->Show_Debug_Project == 1)
			{
				App->CLSB_ImGui->Show_Debug_Project = 0;
			}
			else
			{
				App->CLSB_ImGui->Show_Debug_Project = 1;
			}

			return TRUE;
		}

		if (LOWORD(wParam) == ID_DEBUG_SHOWPICKINGDATA)
		{
			if (App->CLSB_ImGui->Show_Face_Selection == 1)
			{
				App->CLSB_ImGui->Show_Face_Selection = 0;
				App->CLSB_FileView->Show_FileView(1);
			}
			else
			{
				App->CLSB_ImGui->Show_Face_Selection = 1;
				App->CLSB_FileView->Show_FileView(0);
			}
			return TRUE;
		}
		
		if (LOWORD(wParam) == ID_DEBUG_ACTORTOWORLD)
		{
			App->CLSB_Loader->Load_ActorWorld();
			return TRUE;
		}

		if (LOWORD(wParam) == ID_DEBUG_SHOWCAMERAPOSITIONS)
		{
			if (App->CLSB_ImGui->Show_Camera_Pos_F == 1)
			{
				App->CLSB_ImGui->Close_Camera_Pos();
			}
			else
			{
				App->CLSB_ImGui->Start_Camera_Pos();
			}
			return TRUE;
		}

		if (LOWORD(wParam) == ID_DEBUG_BOUNDINGBOX)
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				if (App->CLSB_ImGui->Show_BB_Data_F == 1)
				{
					App->CLSB_ImGui->Close_BB_Data();
				}
				else
				{
					App->CLSB_ImGui->Start_BB_Data();
				}
			}
			return TRUE;
		}

		// Physics
		if (LOWORD(wParam) == ID_PHYSICS_PHYSICS)
		{
			if (App->CLSB_Ogre_Setup->OgreListener->GD_Run_Physics == 1)
			{
				App->CLSB_Ogre_Setup->OgreListener->GD_Run_Physics = 0;
			}
			else
			{
				App->CLSB_Ogre_Setup->OgreListener->GD_Run_Physics = 1;
			}
			return TRUE;
		}

		if (LOWORD(wParam) == ID_PHYSICS_DEBUGDRAW)
		{
			int f = App->CLSB_Bullet->Phys_Body->getCollisionFlags();
			
			if (App->CLSB_Ogre_Setup->OgreListener->Dubug_Physics_DrawAll == 1)
			{
				App->CLSB_Ogre_Setup->OgreListener->Dubug_Physics_DrawAll = 0;
				App->CLSB_Bullet->Phys_Body->setCollisionFlags(f | (1 << 5));
			}
			else
			{
				App->CLSB_Ogre_Setup->OgreListener->Dubug_Physics_DrawAll = 1;
				App->CLSB_Bullet->Phys_Body->setCollisionFlags(f& (~(1 << 5)));
			}
			
			return TRUE;
		}

		// File Import
		if (LOWORD(wParam) == ID_IMPORT_GENESIS3DACT)
		{
			//App->CLSB_Loader->RFActor_Loader();
			return TRUE;
		}

		if (LOWORD(wParam) == ID_IMPORT_WORLDEDITORPROJECT)
		{
			/*int Result = App->CLSB_Loader->Open_File_Model("GDSB File   *.Wepf\0*.Wepf\0", "GDSB File", NULL);
			if (Result == 0)
			{
				return 1;
			}

			App->CLSB_Loader->Read_Project_File(App->CLSB_Loader->Path_FileName);
			App->CLSB_Loader->Load_File_Wepf();*/

			return TRUE;
		}

		// File Export/Save
		if (LOWORD(wParam) == ID_FILE_SAVESCENE)
		{
			return TRUE;
		}
		
		// File Clear Model
		if (LOWORD(wParam) == ID_FILE_CLEARMODEL)
		{
			App->CLSB_Model->Clear_Model_And_Reset();
			return TRUE;
		}

		// Camera
		if (LOWORD(wParam) == ID_CAMERAMODE_FREE)
		{
			App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamDetached);
			return TRUE;
		}

		if (LOWORD(wParam) == ID_CAMERAMODE_MODEL)
		{
			App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamModel);
			return TRUE;
		}

		if (LOWORD(wParam) == ID_CAMERA_RESETVIEW)
		{
			App->CLSB_Camera_EQ->Reset_View();
			return TRUE;
		}

		if (LOWORD(wParam) == ID_CAMERA_ZEROVIEW)
		{
			App->CLSB_Camera_EQ->Zero_View();
			return TRUE;
		}

		/*if (LOWORD(wParam) == IDC_TEST)
		{
			App->CLSB_Ogre->OgreListener->StopOgre = 1;
			App->CLSB_Ogre->OgreIsRunning = 0;
			return TRUE;
		}*/

		if (LOWORD(wParam) == IDOK)
		{

			/*App->CLSB_Equity->EquitySB_Dialog_Created = 0;
			EndDialog(hDlg, LOWORD(wParam));*/
			//App->CLSB_Equity->Show_Equity_Dialog(false);
			return TRUE;
		}

		// ---------------- Equity -------------------------
		// -------------------------------------------------

		if (LOWORD(wParam) == ID_FILE_CLEARMODEL_EQUITY)
		{
			App->CLSB_Equity->Clear_Model();
			return TRUE;
		}
		
		if (LOWORD(wParam) == ID_IMPORT_AUTODESK3DS_EQUITY)
		{
			App->CLSB_Assimp->SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
			bool test = App->CLSB_Loader->Assimp_Loader(App->Equity_Dlg_hWnd, "Autodesk 3DS   *.3ds\0*.3ds\0", "Autodesk 3DS");
			if (test == 0){return TRUE;}

			App->CLSB_Model->Render_Type = Enums::Render_Assimp;
			App->CLSB_Grid->Reset_View();
			App->CLSB_Dimensions->Centre_Model_Mid_Assimp();
			App->CLSB_Dimensions->Rotate_Z_Assimp(90);
			App->CLSB_Dimensions->Centre_Model_Mid_Assimp();

			App->CLSB_Equity->Set_Title_Bar(App->CLSB_Loader->Path_FileName);

			return TRUE;
		}

		if (LOWORD(wParam) == ID_OGRE3D_MESHEQUITY)
		{
			bool test = App->CLSB_Loader->Ogre_Loader("Ogre3D   *.mesh\0*.mesh\0", "Ogre3D");
			if (test == 0){return TRUE;}

			App->CLSB_Model->Render_Type = Enums::Render_Assimp;

			App->CLSB_Grid->Reset_View();
			App->CLSB_Dimensions->Centre_Model_Mid_Assimp();

			App->CLSB_Equity->Set_Title_Bar(App->CLSB_Loader->Path_FileName);

			return TRUE;
		}

		if (LOWORD(wParam) == ID_IMPORT_WAVEFRONTOBJ_EQUITY)
		{
			App->CLSB_Assimp->SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
			bool test = App->CLSB_Loader->Assimp_Loader(App->Equity_Dlg_hWnd, "Wavefront OBJ   *.obj\0*.obj\0", "Wavefront OBJ");
			if (test == 0){return TRUE;}

			App->CLSB_Model->Render_Type = Enums::Render_Assimp;
			App->CLSB_Grid->Reset_View();
			App->CLSB_Dimensions->Centre_Model_Mid_Assimp();
		
			App->CLSB_Equity->Set_Title_Bar(App->CLSB_Loader->Path_FileName);

			return TRUE;
		}

		if (LOWORD(wParam) == ID_TEST_TEST)
		{
			App->CLSB_Meshviewer->Start_Mesh_Viewer();

			return TRUE;
		}
		
		if (LOWORD(wParam) == IDCANCEL)
		{
			if (App->CLSB_Equity->Close_Equity_Flag == 1)
			{
				EndDialog(hDlg, LOWORD(wParam));
				App->Get_Current_Document();
				App->m_pDoc->OnCloseDocument();
			}

			ShowWindow(App->ListPanel, false);
			ShowWindow(App->CLSB_Properties->Properties_Dlg_hWnd, false);
			
			if (App->CLSB_Equity->Equity_Render_Mode == Enums::EQ_Mode_Equity)
			{
				App->CLSB_Equity->Hide_Equity_Dialog();
			}
			else
			{
				App->CLSB_GameDirector->Hide_Game_Editor_Dialog();
			}

			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *		OgreView_3D_New_Proc:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
LRESULT CALLBACK SB_Equity::Ogre3D_New_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_MOUSEWHEEL:
	{
		if (App->CLSB_Ogre_Setup->OgreListener->Pl_LeftMouseDown == 0)
		{
			{
		
				int zDelta = (short)HIWORD(wParam);    // wheel rotation

				if (zDelta > 0)
				{
					App->CLSB_Ogre_Setup->OgreListener->Wheel = -1;
				}
				else if (zDelta < 0)
				{
					App->CLSB_Ogre_Setup->OgreListener->Wheel = 1;
				}
				return 1;
			}
		}
	}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{
		App->CLSB_Ogre_Setup->m_imgui.mouseMoved();

		SetFocus(App->ViewGLhWnd);
		break;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		App->CLSB_Ogre_Setup->m_imgui.mousePressed();

		if (!ImGui::GetIO().WantCaptureMouse)
		{
			if (App->CLSB_Ogre_Setup->OgreIsRunning == 1)
			{
				POINT p;
				GetCursorPos(&p);
				//ScreenToClient(App->MainHwnd, &p);
				App->CursorPosX = p.x;
				App->CursorPosY = p.y;
				App->CLSB_Ogre_Setup->OgreListener->Pl_Cent500X = p.x;
				App->CLSB_Ogre_Setup->OgreListener->Pl_Cent500Y = p.y;

				SetCapture(App->ViewGLhWnd);
				SetCursorPos(App->CursorPosX, App->CursorPosY);
				App->CLSB_Ogre_Setup->OgreListener->Pl_RightMouseDown = 1;
				App->CUR = SetCursor(NULL);
				return 1;
			}
			else
			{
				App->CLSB_Ogre_Setup->OgreListener->Pl_LeftMouseDown = 1;
			}

		}

		return 1;
	}
	case WM_RBUTTONUP:
	{
		App->CLSB_Ogre_Setup->m_imgui.mousePressed();

		if (App->CLSB_Ogre_Setup->OgreIsRunning == 1)
		{
			ReleaseCapture();
			App->CLSB_Ogre_Setup->OgreListener->Pl_RightMouseDown = 0;
			SetCursor(App->CUR);

			if (App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode == 1)
			{
				App->CLSB_Picking->Mouse_Pick_Entity();

				char JustName[200];
				int len = strlen(App->CLSB_Picking->TextureName2);
				strcpy(JustName, App->CLSB_Picking->TextureName2);
				JustName[len - 4] = 0;

				App->CLSB_TabsControl->Select_Texture_Tab(0, JustName);

			}

			return 1;
		}

		return 1;
	}

	// Left Mouse Button
	case WM_LBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		App->CLSB_Ogre_Setup->m_imgui.mousePressed();

		if (!ImGui::GetIO().WantCaptureMouse)
		{
			if (App->CLSB_Ogre_Setup->OgreIsRunning == 1)
			{

				POINT p;
				GetCursorPos(&p);
				App->CursorPosX = p.x;
				App->CursorPosY = p.y;
				App->CLSB_Ogre_Setup->OgreListener->Pl_Cent500X = p.x;
				App->CLSB_Ogre_Setup->OgreListener->Pl_Cent500Y = p.y;

				if (App->CLSB_Ogre_Setup->OgreListener->GD_Selection_Mode == 1)
				{
					App->CLSB_Picking->Left_MouseDown = 1;

					App->CLSB_Picking->Mouse_Pick_Entity();

					App->CLSB_Picking->Left_MouseDown = 0;
				}
				
				SetCapture(App->ViewGLhWnd);// Bernie
				SetCursorPos(App->CursorPosX, App->CursorPosY);

				App->CLSB_Ogre_Setup->OgreListener->Pl_LeftMouseDown = 1;

				App->CUR = SetCursor(NULL);

				return 1;
			}
			else
			{
				App->CLSB_Ogre_Setup->OgreListener->Pl_LeftMouseDown = 1;
			}
		}


		return 1;
	}

	case WM_LBUTTONUP:
	{
		App->CLSB_Ogre_Setup->m_imgui.mouseReleased();

		if (App->CLSB_Ogre_Setup->OgreIsRunning == 1)
		{
			ReleaseCapture();
			App->CLSB_Ogre_Setup->OgreListener->Pl_LeftMouseDown = 0;
			SetCursor(App->CUR);
			return 1;
		}

		return 1;
	}
	//case WM_KEYDOWN:
	//	switch (wParam)
	//	{
	//	case 'C':
	//		if (GetAsyncKeyState(VK_CONTROL))
	//		{
	//			//		//		App->CL10_Objects_Com->Copy_Object();
	//			//		//		return 1;
	//		}
	//	case 'V':
	//		if (GetAsyncKeyState(VK_CONTROL))
	//		{
	//			//		//		App->CL10_Objects_Com->Paste_Object();
	//			//		//		return 1;
	//		}
	//		//	return 1;
	//		//	//	// more keys here
	//	}break;
	}

	return DefWindowProc(hDlg, message, wParam, lParam);
}

// *************************************************************************
// *			Resize_3DView:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Equity::Resize_3DView()
{
	if (App->BR_True3D_Mode_Active == 0)
	{
		RECT rcl;
		GetClientRect(App->Equity_Dlg_hWnd, &rcl);

		int X = rcl.right - 10;
		int Y = rcl.bottom - 85;

		SetWindowPos(App->ViewGLhWnd, NULL, 4, 80, X, Y, SWP_NOZORDER);


		if (App->CLSB_Ogre_Setup->OgreIsRunning == 1)
		{
			App->CLSB_Ogre_Setup->mWindow->windowMovedOrResized();
			App->CLSB_Ogre_Setup->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getHeight());
			App->CLSB_Ogre_Setup->mCamera->yaw(Ogre::Radian(0));

			Root::getSingletonPtr()->renderOneFrame();
		}
	}
}

// *************************************************************************
// *	  		Set_Mode_Equity:- Terry and Hazel Flanigan 2023				*
// *************************************************************************
void SB_Equity::Set_Mode_Equity()
{
	Equity_Render_Mode = Enums::EQ_Mode_Equity;

	App->CLSB_TopTabs_Equity->Hide_Tabs();
	ShowWindow(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, SW_SHOW);
	App->CLSB_TopTabs_Equity->Toggle_Tabs_Camera_Flag = 1;

	App->CLSB_ImGui->Show_Physics_Console = 0;

	App->CLSB_TopTabs_Equity->Toggle_Camera_Model_Flag = 1;

	EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_MODEL), 1);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_FIRST_MODEX), 0);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_FREE), 0);
	EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, IDC_UPDATE2), 0);

	if (App->CLSB_Ogre_Setup->OgreIsRunning == 1)
	{
		App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 0;
	}

	RedrawWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *				Go_Equity:- Terry and Hazel Flanigan 2023		  	   *
// *************************************************************************
void SB_Equity::Go_Equity()
{
	if (App->Equity_Dlg_hWnd)
	{
		if (App->CLSB_ImGui->ImGui_Surface_Active == 1)
		{
			App->CLSB_ImGui->Stop_Render();
			EndDialog(App->CLSB_ImGui->ImGui_Dlg_Surface_hWnd, 0);
			App->CLSB_ImGui->ImGui_Surface_Active = 0;
			Debug
		}

		if (App->CLSB_Mesh_Mgr->World_Node && App->CLSB_Mesh_Mgr->World_Ent)
		{
			App->CLSB_Mesh_Mgr->World_Node->setVisible(false);
			//App->CLSB_Scene->V_Object[App->CLSB_Environment->Eviron_Index]->S_Environ[0]->Enabled = 0;
			//App->CLSB_Environment->SetSky(0);
		}

		//App->CLSB_Model->Clear_Model_And_Reset();

		App->CLSB_Grid->Reset_View();

		App->CLSB_TopTabs_Equity->Camera_Set_Model();

		Equity_Render_Mode = Enums::EQ_Mode_Equity;

		App->CLSB_TopTabs_Equity->Hide_Tabs();
		ShowWindow(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, SW_SHOW);
		App->CLSB_TopTabs_Equity->Toggle_Tabs_Camera_Flag = 1;

		App->CLSB_ImGui->Show_Physics_Console = 0;

		App->CLSB_TopTabs_Equity->Toggle_Camera_Model_Flag = 1;

		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_MODEL), 1);
		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_FIRST_MODEX), 0);
		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_FREE), 0);
		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, IDC_UPDATE2), 0);

		if (App->CLSB_Ogre_Setup->OgreIsRunning == 1)
		{
			App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 0;
		}

		RedrawWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		
		App->CLSB_Model->Render_Type = Enums::Render_Assimp;
		App->CLSB_Ogre_Setup->RenderListener->ShowTextured = 1;
		App->CLSB_Model->Model_Loaded = 1;

		ShowWindow(App->ListPanel, false);
		ShowWindow(App->CLSB_Properties->Properties_Dlg_hWnd, false);

		
		ShowWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, false);

		HMENU TestMenu;
		TestMenu = LoadMenu(App->hInst, MAKEINTRESOURCE(IDR_MENU_EQUITY));
		SetMenu(App->Equity_Dlg_hWnd, TestMenu);


		App->CLSB_TopTabs_Equity->Hide_Render_Buttons_EQ(FALSE);

		EquitySB_Dialog_Visible = 1;
		ShowWindow(App->Equity_Dlg_hWnd, SW_SHOW);
		//->CLSB_Environment->SetSky(false);
		App->CLSB_Environment->EnableFog(false);

		App->CLSB_GameDirector->Show_Objects(false);

		App->CLSB_Ogre_Setup->mSceneMgr->setSkyDome(0, "Examples/CloudySky", 10, 10, 1000);
	}

	//App->CLSB_Equity->Show_Equity_Dialog(true);
}

// *************************************************************************
// *			 Set_Camera:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void SB_Equity::Set_Camera()
{
	geVec3d Pos = App->CLSB_Camera_WE->FindCameraEntity()->mOrigin;
	App->CLSB_Ogre_Setup->mCamera->setPosition(Pos.X, Pos.Y, Pos.Z);
	geVec3d Angles;
	App->CLSB_Camera_WE->FindCameraEntity()->GetAngles(&Angles, Level_GetEntityDefs(App->CLSB_Doc->pLevel));

	Ogre::Quaternion Rotation;
	Rotation.w = 1;
	Rotation.x = 0;
	Rotation.y = 0;
	Rotation.z = 0;

	Angles.X = Angles.X + 3.141;

	App->CLSB_Ogre_Setup->OgreListener->mCam->setOrientation(Rotation);
	App->CLSB_Ogre_Setup->OgreListener->mCam->yaw(Ogre::Radian(-Angles.Y));
	App->CLSB_Ogre_Setup->OgreListener->mCam->pitch(Ogre::Radian(-Angles.X));
}

// *************************************************************************
// *			Do_Preview_Selected:- Terry and Hazel Flanigan 2023	  	   *
// *************************************************************************
void SB_Equity::Do_Preview_Selected()
{
	return;
	bool AllGood = 0;

	Equity_Render_Mode = Enums::EQ_Mode_Selected;

	int NumSelBrushes = SelBrushList_GetSize(App->CLSB_Doc->pSelBrushes);

	if (NumSelBrushes == 0)
	{
		App->Say("No Brushes Selected");
		return;
	}

	
	for (int i = 0; i < NumSelBrushes; ++i)
	{
		Brush* pBrush = SelBrushList_GetBrush(App->CLSB_Doc->pSelBrushes, i);
		if (pBrush->Flags & 1 || pBrush->Flags & 1024)
		{
			AllGood = 1;
		}
	}

	if (AllGood == 1)
	{
		App->CLSB_Camera_EQ->Reset_Orientation();

		App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamModel);

		App->CLSB_TopTabs_Equity->Hide_Tabs();
		ShowWindow(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, SW_SHOW);
		App->CLSB_TopTabs_Equity->Toggle_Tabs_Camera_Flag = 1;

		App->CLSB_TopTabs_Equity->Camera_Set_Model();

		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_FREE), 0);
		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_BT_TT_MODEL), 1);
		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Camera_TB_hWnd, IDC_FIRST_MODEX), 0);
		EnableWindow(GetDlgItem(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, IDC_UPDATE2), 0);

		App->CLSB_ImGui->Show_Physics_Console = 0;
		App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 0;

		App->CLSB_Camera_EQ->Reset_View();

		if (App->CLSB_Ogre_Setup->OgreIsRunning == 1)
		{
			App->CLSB_Ogre_Setup->OgreListener->GD_Run_Physics = 0;
		}

		RedrawWindow(App->CLSB_TopTabs_Equity->Tabs_TB_hWnd_Eq, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		App->CLSB_Mesh_Mgr->WE_Build_Brush_List(1);
		App->CLSB_Mesh_Mgr->WE_Convert_All_Texture_Groups();
		App->CLSB_Mesh_Mgr->Set_BBox_All_Selected_Brushes();

		//App->CLSB_Bullet->Create_Brush_Trimesh(0);
		//App->CLSB_Model->Set_BondingBox_Brushes();
		App->CLSB_Dimensions->Centre_Model_Mid_Brushes();

		App->CLSB_Model->Model_Loaded = 1;
		App->CLSB_Ogre_Setup->RenderListener->ShowBrushes = 1;
		App->CLSB_Ogre_Setup->RenderListener->ShowTextured = 1;
		App->CLSB_Model->Render_Type = Enums::LoadedFile_Brushes;

		ShowWindow(App->ListPanel, false);
		ShowWindow(App->CLSB_Properties->Properties_Dlg_hWnd, false);

		//App->CLSB_Equity->Show_Equity_Dialog(true);
	}
	else
	{
		App->Say("Only Cut Brush Selected","Cant not build Geometry from only Cut Brushes");

	}
}

// *************************************************************************
// *			 Set_Title_Bar:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void SB_Equity::Set_Title_Bar(char* Title)
{
	char FullTitle[MAX_PATH];
	strcpy(FullTitle, "Equity ");
	strcat(FullTitle, Title);
	SetWindowText(App->Equity_Dlg_hWnd, FullTitle);
}

// *************************************************************************
// *			 Clear_Model:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void SB_Equity::Clear_Model()
{
	App->CLSB_Assimp->Clear_Data();

	App->CLSB_Model->Render_Type = Enums::Render_Nothing;
	App->CLSB_Grid->Reset_View();
}
