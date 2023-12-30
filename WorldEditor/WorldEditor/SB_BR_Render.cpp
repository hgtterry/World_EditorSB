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
	
}

SB_BR_Render::~SB_BR_Render()
{
}

// *************************************************************************
// *			Start_BR_3D_Mode:- Terry and Hazel Flanigan 2023	      	   *
// *************************************************************************
void SB_BR_Render::Start_BR_3D_Mode(void)
{
	App->CLSB_Model->Set_Equity();
	App->CLSB_Camera_EQ->Reset_Orientation();
	App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamDetached);
	App->CLSB_TopTabs_Equity->Camera_Set_Free();

	App->CLSB_Model->Model_Loaded = 1;
	App->CLSB_Ogre->RenderListener->ShowBrushes = 1;
	App->CLSB_Ogre->RenderListener->ShowTextured = 1;
	App->CLSB_Model->Render_Type = Enums::LoadedFile_None;
	App->CLSB_Camera_EQ->Zero_View();

	App->CLSB_Equity->EquitySB_Dialog_Visible = 1;
	
	App->CLSB_BR_Render->Go_BR_3D_Mode();
	App->CLSB_Mesh_Mgr->Update_World();
}

// *************************************************************************
// *			Go_BR_3D_Mode:- Terry and Hazel Flanigan 2022	    	  	   *
// *************************************************************************
void SB_BR_Render::Go_BR_3D_Mode(void)
{
	App->Block_RB_Actions = 1;
	App->BR_True3D_Mode_Active = 1;

	RECT rect;
	GetWindowRect(App->WE_3DView_Hwnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	SetWindowPos(App->ViewGLhWnd, HWND_TOP, rect.left, rect.top - 27, width, height-15, NULL);

	SetParent(App->ViewGLhWnd, App->MainHwnd);

	App->CLSB_Ogre->mWindow->resize(width, height - 15);

	App->CLSB_Ogre->mWindow->windowMovedOrResized();
	App->CLSB_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre->mWindow->getHeight());

	App->CLSB_ImGui->Show_Physics_Console = 0;

	Root::getSingletonPtr()->renderOneFrame();

}

// *************************************************************************
// *			BR_Resize:- Terry and Hazel Flanigan 2022	    	  	   *
// *************************************************************************
void SB_BR_Render::BR_3D_Resize(void)
{

	RECT rect;
	GetWindowRect(App->WE_3DView_Hwnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	SetWindowPos(App->ViewGLhWnd, HWND_TOP, rect.left, rect.top - 27, width, height - 15, NULL);

	App->CLSB_Ogre->mWindow->resize(width, height - 15);

	App->CLSB_Ogre->mWindow->windowMovedOrResized();
	App->CLSB_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre->mWindow->getHeight());

	Root::getSingletonPtr()->renderOneFrame();
}

// *************************************************************************
// *			Exit_BR_Mode:- Terry and Hazel Flanigan 2023	      	   *
// *************************************************************************
void SB_BR_Render::Exit_BR_3D_Mode(void)
{
	if(App->BR_True3D_Mode_Active == 1)
	{
		App->BR_True3D_Mode_Active = 0;

		SetParent(App->ViewGLhWnd, App->Equity_Dlg_hWnd);
		SetWindowPos(App->ViewGLhWnd, HWND_TOP, 235, 11, 542, 455, SWP_NOZORDER);
		
		App->CLSB_Equity->Resize_3DView();
		
		App->CLSB_Ogre->mWindow->windowMovedOrResized();
		App->CLSB_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre->mWindow->getHeight());
		App->CLSB_Ogre->mCamera->yaw(Radian(0));
		Root::getSingletonPtr()->renderOneFrame();

		App->CLSB_Scene->FullScreenMode_Flag = 0;
		App->CLSB_ImGui->Show_Physics_Console = 1;
		
		App->Block_RB_Actions = 0;
		App->CLSB_Equity->EquitySB_Dialog_Visible = 0;
	}
}
