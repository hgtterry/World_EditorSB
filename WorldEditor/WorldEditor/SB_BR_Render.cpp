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
#include "SB_BR_Render.h"


SB_BR_Render::SB_BR_Render()
{
	MeshView_3D_hWnd = nullptr;

	// ------------------------------------------------ 
	GridManual = nullptr;
	GridNode = nullptr;

	Scale_X = 4;
	Scale_Y = 4;
	Scale_Z = 4;

	Division_X = 2;
	Division_Y = 2;
	Division_Z = 2;

	YAxis_min = -8;
	YAxis_max = 8;

	ZAxis_min = -8;
	ZAxis_max = 8;

	XAxis_min = -8;
	XAxis_max = 8;

	HairExtend = 8;

	ColourHairZ = ColourValue(1, 0, 0, 1);
	ColourHairX = ColourValue(0, 0, 1, 1);
	ColourHairY = ColourValue(0, 1, 0, 1);
	ColourMain = ColourValue(0.7, 0.7, 0, 0.6);
	ColourDivision = ColourValue(1, 1, 1, 0.4);

	ShowDivisions = 1;

	// ------------------------------------------------ 
	RB_View_Window = NULL;
	RB_SceneMgr = NULL;
	mCameraMeshView = NULL;
	CamNode = NULL;

	RB_Render_Started = 0;
}

SB_BR_Render::~SB_BR_Render()
{
}

// *************************************************************************
// *			Start_RB_Window:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_BR_Render::Start_RB_Window()
{
	Surface_Hwnd = NULL;
	Surface_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_RB_RENDER_WIN, MeshView_3D_hWnd, (DLGPROC)RB_Window_Proc);
	bool test = Set_Render_Window();
	if (test == 0)
	{
		App->Say("Failed to Start Right Bottom Womdoe");
		return;
	}

	Resize_3DView();
}

// *************************************************************************
// *		  RB_Window_Proc:- Terry and Hazel Flanigan 2023	  		   *
// *************************************************************************
LRESULT CALLBACK SB_BR_Render::RB_Window_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}
	case WM_NOTIFY:
	{
		return CDRF_DODEFAULT;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}
	case WM_COMMAND:

		break;

	}
	return FALSE;
}

// *************************************************************************
// *			Set_Render_Window:- Terry and Hazel Flanigan 2023          *
// *************************************************************************
bool SB_BR_Render::Set_Render_Window(void)
{
	
	Ogre::NameValuePairList options;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)Surface_Hwnd);

	RB_View_Window = App->CLSB_Ogre->mRoot->createRenderWindow("RB_ViewWin", 1024, 768, false, &options);

	RB_SceneMgr = App->CLSB_Ogre->mRoot->createSceneManager("DefaultSceneManager", "MeshViewGD");

	mCameraMeshView = RB_SceneMgr->createCamera("CameraMV");
	mCameraMeshView->setPosition(Ogre::Vector3(0, 0, 0));
	mCameraMeshView->setNearClipDistance(0.1);
	mCameraMeshView->setFarClipDistance(8000);

	Ogre::Viewport* vp = RB_View_Window->addViewport(mCameraMeshView);
	mCameraMeshView->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));

	CamNode = RB_SceneMgr->getRootSceneNode()->createChildSceneNode("Camera_Node");
	CamNode->attachObject(mCameraMeshView);

	////-------------------------------------------- 
	RB_SceneMgr->setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	// add a bright light above the scene
	Light* light = RB_SceneMgr->createLight();
	light->setType(Light::LT_POINT);
	light->setPosition(-10, 40, 20);
	light->setSpecularColour(ColourValue::White);

	/*Ogre::Vector3 Centre = MvEnt->getBoundingBox().getCenter();
	Ogre::Real Radius = MvEnt->getBoundingRadius();*/

	Grid_Update(1);

	App->CLSB_Ogre->RB_m_imgui.Init(RB_SceneMgr, Surface_Hwnd);

	RB_RenderListener = new SB_BR_Listener();

	App->CLSB_Ogre->mRoot->addFrameListener(RB_RenderListener);


	mCameraMeshView->setPosition(Ogre::Vector3(0, 0, 0));
	//mCameraMeshView->lookAt(Ogre::Vector3(0, 30, 0));

	Resize_3DView();
	
	//App->CLSB_BR_Render->Switch_Proc();

	int test = 0;
	test = SetWindowLong(Surface_Hwnd, GWL_WNDPROC, (LONG)Ogre3D_Proc);

	if (!test)
	{
		App->Say("Cant Start Ogre Proc");
		return 0;
	}
	else
	{
		App->Beep_Win();
	}

	

	RB_Render_Started = 1;

	return 1;
}

// *************************************************************************
// *	  		Switch_Proc:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_BR_Render::Switch_Proc()
{
	

	int test = 0;
	test = SetWindowLong(Surface_Hwnd, GWL_WNDPROC, (LONG)Ogre3D_Proc);

	if (!test)
	{
		App->Say("Cant Start Ogre Proc");
		//return;
	}
	else
	{
		
	}
}

// *************************************************************************
// *		       Ogre3D_Proc:- Terry and Hazel Flanigan 2023 	    	   *
// *************************************************************************
LRESULT CALLBACK SB_BR_Render::Ogre3D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_MOUSEWHEEL:
	{
		if (App->CLSB_BR_Render->RB_RenderListener->Pl_LeftMouseDown == 0)
		{
			{

				int zDelta = (short)HIWORD(wParam);    // wheel rotation

				if (zDelta > 0)
				{
					App->CLSB_BR_Render->RB_RenderListener->Wheel_Move = -1;
				}
				else if (zDelta < 0)
				{
					App->CLSB_BR_Render->RB_RenderListener->Wheel_Move = 1;
				}
				return 1;
			}
		}
	}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{
		App->CLSB_Ogre->m_imgui.mouseMoved();

		SetFocus(App->CLSB_BR_Render->Surface_Hwnd);
		break;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		App->CLSB_Ogre->m_imgui.mousePressed();

		if (!ImGui::GetIO().WantCaptureMouse)
		{
			if (App->CLSB_Ogre->OgreIsRunning == 1)
			{
				POINT p;
				GetCursorPos(&p);
				//ScreenToClient(App->MainHwnd, &p);
				App->CursorPosX = p.x;
				App->CursorPosY = p.y;
				App->CLSB_BR_Render->RB_RenderListener->Pl_Cent500X = p.x;
				App->CLSB_BR_Render->RB_RenderListener->Pl_Cent500Y = p.y;

				SetCapture(App->CLSB_BR_Render->Surface_Hwnd);
				SetCursorPos(App->CursorPosX, App->CursorPosY);
				App->CLSB_BR_Render->RB_RenderListener->Pl_RightMouseDown = 1;
				App->CUR = SetCursor(NULL);
				return 1;
			}
			else
			{
				App->CLSB_BR_Render->RB_RenderListener->Pl_LeftMouseDown = 1;
			}

		}

		return 1;
	}
	case WM_RBUTTONUP:
	{
		App->CLSB_Ogre->m_imgui.mousePressed();

		if (App->CLSB_Ogre->OgreIsRunning == 1)
		{
			ReleaseCapture();
			App->CLSB_BR_Render->RB_RenderListener->Pl_RightMouseDown = 0;
			SetCursor(App->CUR);

			/*if (App->CLSB_Ogre->OgreListener->GD_Selection_Mode == 1)
			{
				App->CLSB_Picking->Mouse_Pick_Entity();

				char JustName[200];
				int len = strlen(App->CLSB_Picking->TextureName2);
				strcpy(JustName, App->CLSB_Picking->TextureName2);
				JustName[len - 4] = 0;

				App->CL_TabsControl->Select_Texture_Tab(0, JustName);

			}*/

			return 1;
		}

		return 1;
	}

	// Left Mouse Button
	case WM_LBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		App->CLSB_Ogre->m_imgui.mousePressed();

		if (!ImGui::GetIO().WantCaptureMouse)
		{
			if (App->CLSB_Ogre->OgreIsRunning == 1)
			{

				POINT p;
				GetCursorPos(&p);
				App->CursorPosX = p.x;
				App->CursorPosY = p.y;
				App->CLSB_BR_Render->RB_RenderListener->Pl_Cent500X = p.x;
				App->CLSB_BR_Render->RB_RenderListener->Pl_Cent500Y = p.y;

				//if (App->CLSB_Ogre->OgreListener->GD_Selection_Mode == 1)
				{
					App->CLSB_Picking->Left_MouseDown = 1;

					App->CLSB_Picking->Mouse_Pick_Entity();

					App->CLSB_Picking->Left_MouseDown = 0;
				}

				SetCapture(App->CLSB_BR_Render->Surface_Hwnd);// Bernie
				SetCursorPos(App->CursorPosX, App->CursorPosY);

				App->CLSB_BR_Render->RB_RenderListener->Pl_LeftMouseDown = 1;

				App->CUR = SetCursor(NULL);

				return 1;
			}
			else
			{
				App->CLSB_BR_Render->RB_RenderListener->Pl_LeftMouseDown = 1;
			}
		}


		return 1;
	}

	case WM_LBUTTONUP:
	{
		App->CLSB_Ogre->m_imgui.mouseReleased();

		if (App->CLSB_Ogre->OgreIsRunning == 1)
		{
			ReleaseCapture();
			App->CLSB_BR_Render->RB_RenderListener->Pl_LeftMouseDown = 0;
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
void SB_BR_Render::Resize_3DView()
{
	RECT rect;
	GetWindowRect(MeshView_3D_hWnd, &rect);
	
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	
	SetWindowPos(Surface_Hwnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);


	if (App->CLSB_Ogre->OgreIsRunning == 1)
	{
		RB_View_Window->windowMovedOrResized();
		mCameraMeshView->setAspectRatio((Ogre::Real)RB_View_Window->getWidth() / (Ogre::Real)RB_View_Window->getHeight());
		mCameraMeshView->yaw(Ogre::Radian(0));

		Root::getSingletonPtr()->renderOneFrame();
	}
}

// *************************************************************************
// *	  		Grid_Update:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void SB_BR_Render::Grid_Update(bool Create)
{
	int i = 0;
	Real r;

	if (Create == 1)
	{
		GridManual = RB_SceneMgr->createManualObject("GridManual");
		GridManual->setRenderQueueGroup(1);
	}

	GridManual->clear();
	GridManual->begin("BaseWhiteAlphaBlended", RenderOperation::OT_LINE_LIST);

	//if (ShowGridFlag == 1)
	{
		for (int x = XAxis_min; x <= XAxis_max; ++x)
		{
			GridManual->position(x, 0, ZAxis_min);
			GridManual->colour(ColourMain);
			GridManual->position(x, 0, ZAxis_max);
			GridManual->colour(ColourMain);
			if (x < XAxis_max && ShowDivisions == 1)
			{
				for (int d = 0; d < Division_X; ++d)
				{
					r = x + (1.0 / Real(Division_X)) * Real(d);
					GridManual->position(r, 0, ZAxis_min);
					GridManual->colour(ColourDivision);
					GridManual->position(r, 0, ZAxis_max);
					GridManual->colour(ColourDivision);
				}
			}
		}

		for (int z = ZAxis_min; z <= ZAxis_max; ++z)
		{
			GridManual->position(Ogre::Vector3(XAxis_min, 0, z));
			GridManual->colour(ColourMain);
			GridManual->position(Ogre::Vector3(XAxis_max, 0, z));
			GridManual->colour(ColourMain);
			if (z < ZAxis_max && ShowDivisions == 1)
			{
				for (int d = 1; d < Division_Y; ++d)
				{
					r = z + (1.0 / Real(Division_Y)) * Real(d);
					GridManual->position(XAxis_min, 0, r);
					GridManual->colour(ColourDivision);
					GridManual->position(XAxis_max, 0, r);
					GridManual->colour(ColourDivision);
				}
			}
		}
	}

	GridManual->end();

	if (Create == 1)
	{
		GridNode = RB_SceneMgr->getRootSceneNode()->createChildSceneNode();
		GridNode->attachObject(GridManual);
	}

	GridNode->setPosition(0, 0, 0);
	GridNode->setVisible(true);
	GridNode->setScale(Scale_X, Scale_Y, Scale_Z);
}

// *************************************************************************
// *	  		Update_Scene:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void SB_BR_Render::Update_Scene()
{
	if (RB_Render_Started == 0)
	{
		App->CLSB_BR_Render->Start_RB_Window();
	}

	App->CLSB_BR_Render->Resize_3DView();

	App->CLSB_Mesh_Mgr->WE_Build_Brush_List(0);
	App->CLSB_Bullet->Create_Brush_Trimesh_XX(0);
	App->CLSB_Mesh_Mgr->WE_Convert_All_Texture_Groups();
	App->CLSB_Export_Ogre3D->Convert_ToOgre3D(1);
	App->CLSB_Ogre->OgreListener->CameraMode = Enums::CamDetached;
}


