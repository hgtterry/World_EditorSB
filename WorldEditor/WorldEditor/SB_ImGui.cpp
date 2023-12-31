/*
Copyright (c) 2023 Equity_ME Model Editor -- Inflanite Software W.T.Flanigan H.C.Flanigan

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
#include "resource.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "SB_ImGui.h"

#define	M_PI		((geFloat)3.14159265358979323846f)
#define Units_RadiansToDegrees(r) ((((geFloat)(r)) * 180.0f) / M_PI)

SB_ImGui::SB_ImGui()
{
	ImGui::CreateContext();
	ImGui_Set_Colours();
	Load_Font();

	PosX = 500;
	PosY = 500;

	Model_Data_PosX = 500;
	Model_Data_PosY = 500;

	Show_FPS = 1;
	StartPos = 0;
	
	Updating_F = 0;
	Show_Face_Selection = 0;
	Show_Model_Data_F = 0;
	Show_Camera_Pos_F = 0;
	Show_BB_Data_F = 0;

	PosX_Selected = 1;
	PosY_Selected = 0;
	PosZ_Selected = 0;

	// -------------- Physics Console
	Disable_Physics_Console = 0;
	Show_Physics_Console = 1;
	Physics_PosX = 500;
	Physics_PosY = 500;
	Physics_Console_StartPos = 0;

	Resize_ImGui_Surface_F = 0;

	Cam_Dlg_Data_PosX = 0;
	Cam_Dlg_Data_PosX = 0;

	// -------------- Float
	Show_Dialog_Float = 0;
	Float_StartPos = 0;
	Float_PosX = 0;
	Float_PosY = 0;
	Float_Step = 0.05f;
	Float_Canceld = 0;
	Float_Exit = 0;
	strcpy(Float_Banner, "Banner");
	m_Dialog_Float_Copy = 0;
	m_Dialog_Float = 10.222;

	Show_Textures_F = 0;
	Show_Brush_Data_F = 0;
	Show_Real_Brush_F = 0;
	Show_Global_Data_F = 0;
	Show_Assimp_Data_F = 0;
	Show_WE_Data_F = 0;

	Pick_Face = 1;

	m_pDoc = nullptr;
	pCameraEntity = nullptr;
	Select_Face_F = 0;

	ImGui_Surface_Active = 0;
	ImGui_Dlg_Surface_hWnd = NULL;
}


SB_ImGui::~SB_ImGui()
{
}

// *************************************************************************
// *	  	Start_ImGui_Surface:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::Start_ImGui_Surface()
{	
	if (ImGui_Surface_Active == 0)
	{
		ImGui_Surface_Active = 1;
		ImGui_Dlg_Surface_hWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_IMGUI_SURFACE2, App->MainHwnd, (DLGPROC)ImGui_Surface_Proc);
		Start_Render();
		Render_Surface_Resize();
		Show_WE_Data_F = 1;
		SetWindowPos(ImGui_Dlg_Surface_hWnd, NULL, 0, 0, 300+20, 300+40, SWP_NOMOVE | SWP_NOZORDER);

	}
}

// *************************************************************************
// *		  ImGui_Surface_Proc:- Terry and Hazel Flanigan 2023	  	   *
// *************************************************************************
LRESULT CALLBACK SB_ImGui::ImGui_Surface_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
		LPNMHDR some_item = (LPNMHDR)lParam;

		/*if (some_item->idFrom == IDOK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}*/

		return CDRF_DODEFAULT;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{
		App->CLSB_Ogre_Setup->m_imgui.mouseMoved();

		SetFocus(App->CLSB_ImGui->ImGui_Dlg_Surface_hWnd);
		break;
	}

	case WM_SIZE:
	{
		App->CLSB_ImGui->Render_Surface_Resize();
		Ogre::Root::getSingletonPtr()->renderOneFrame();

	}break;

	case WM_COMMAND:

		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CLSB_ImGui->Stop_Render();
			App->CLSB_ImGui->ImGui_Surface_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;

	}
	return FALSE;
}

// *************************************************************************
// *			Start_Render:- Terry and Hazel Flanigan 2022	     	   *
// *************************************************************************
void SB_ImGui::Start_Render(void)
{
	Set_Render_Mode();

	RECT rect;
	GetWindowRect(ImGui_Dlg_Surface_hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	SetWindowPos(App->ViewGLhWnd, HWND_TOP, 0, 0, width, height, NULL);

	SetParent(App->ViewGLhWnd, ImGui_Dlg_Surface_hWnd);

	App->CLSB_Ogre_Setup->mWindow->resize(width, height);

	App->CLSB_Ogre_Setup->mWindow->windowMovedOrResized();
	App->CLSB_Ogre_Setup->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getHeight());

	Root::getSingletonPtr()->renderOneFrame();
}

// *************************************************************************
// *			Set_Render_Mode:- Terry and Hazel Flanigan 2023	      	   *
// *************************************************************************
void SB_ImGui::Set_Render_Mode(void)
{
	App->CLSB_GameDirector->Show_Objects(false);
	App->CLSB_Ogre_Setup->mSceneMgr->setSkyDome(0, "Examples/CloudySky", 10, 10, 1000);
	App->CLSB_Environment->EnableFog(false);
	App->CLSB_Equity->EquitySB_Dialog_Visible = 1;

	if (App->CLSB_Mesh_Mgr->World_Node && App->CLSB_Mesh_Mgr->World_Ent)
	{
		App->CLSB_Mesh_Mgr->World_Node->setVisible(false);
	}
}

// *************************************************************************
// *			Stop_Render:- Terry and Hazel Flanigan 2023	      	   *
// *************************************************************************
void SB_ImGui::Stop_Render(void)
{
	SetParent(App->ViewGLhWnd, App->Equity_Dlg_hWnd);
	SetWindowPos(App->ViewGLhWnd, HWND_TOP, 235, 11, 542, 455, SWP_NOZORDER);

	App->CLSB_Equity->Resize_3DView();

	App->CLSB_Ogre_Setup->mWindow->windowMovedOrResized();
	App->CLSB_Ogre_Setup->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getHeight());
	App->CLSB_Ogre_Setup->mCamera->yaw(Radian(0));
	Root::getSingletonPtr()->renderOneFrame();

	App->CLSB_Equity->EquitySB_Dialog_Visible = 0;
}

// *************************************************************************
// *		Render_Surface_Resize:- Terry and Hazel Flanigan 2022	   	   *
// *************************************************************************
void SB_ImGui::Render_Surface_Resize(void)
{
	RECT rect;
	GetWindowRect(ImGui_Dlg_Surface_hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	SetWindowPos(App->ViewGLhWnd, HWND_TOP, 0, 0, width, height - 15, NULL);

	App->CLSB_Ogre_Setup->mWindow->resize(width, height);

	App->CLSB_Ogre_Setup->mWindow->windowMovedOrResized();
	App->CLSB_Ogre_Setup->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getHeight());

	Root::getSingletonPtr()->renderOneFrame();
}

// *************************************************************************
// *		BackGround_Render_Loop:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_ImGui::BackGround_Render_Loop(void)
{
	Ogre::WindowEventUtilities::messagePump();

	if (App->CLSB_Ogre_Setup->mWindow->isClosed()) return;

	if (App->CLSB_Ogre_Setup->FPStimer.getMilliseconds() > 3)
	{
		App->CLSB_Ogre_Setup->mRoot->_fireFrameStarted();
		App->CLSB_Ogre_Setup->mRoot->_updateAllRenderTargets();
		App->CLSB_Ogre_Setup->mRoot->_fireFrameEnded();
		App->CLSB_Ogre_Setup->FPStimer.reset();
	}
}

// *************************************************************************
// *		ImGui_WE_Editor_Loop:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_ImGui::ImGui_WE_Editor_Loop(void)
{
	if (Show_WE_Data_F == 1)
	{
		WE_Data_GUI();
	}
}

// *************************************************************************
// *			WE_Data_GUI:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_ImGui::WE_Data_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(300, 300));

	if (!ImGui::Begin("WE Data", &Show_Model_Data_F, ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Spacing();
		ImGui::Text("Camera Info");
		ImGui::Text("  ");

		geVec3d Pos = App->CLSB_Camera_WE->FindCameraEntity()->mOrigin;

		ImGui::Text("Cam X:- %f", Pos.X);
		ImGui::Text("Cam Y:- %f", Pos.Y);
		ImGui::Text("Cam Z:- %f", Pos.Z);
		
		geVec3d Angles;
		App->CLSB_Camera_WE->FindCameraEntity()->GetAngles(&Angles, Level_GetEntityDefs(App->CLSB_Doc->pLevel));

		ImGui::Text("  ");
		ImGui::Text("Angle X:- %f", Angles.X);
		ImGui::Text("Angle Y:- %f", Angles.Y);
		ImGui::Text("Angle Z:- %f", Angles.Z);

		if (Resize_ImGui_Surface_F == 0)
		{
			Resize_ImGui_Surface_F = 1;
			//ImVec2 Size = ImGui::GetWindowSize();

			//ImGui_Test();
			//SetWindowPos(ImGui_Dlg_Surface_hWnd, NULL, 0,0, 500, 500, SWP_NOMOVE | SWP_NOZORDER);
			//Cam_Dlg_Data_PosX = ((float)App->CLSB_Ogre->OgreListener->View_Width / 2) - (Size.x / 2);
			//Cam_Dlg_Data_PosY = ((float)App->CLSB_Ogre->OgreListener->View_Height / 2) - (Size.y / 2);
		}

		ImGui::End();
	}
}

// *************************************************************************
// *		ImGui_Editor_Loop:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::ImGui_Editor_Loop(void)
{
	if (Show_Model_Data_F == 1)
	{
		Model_Data_GUI();
	}

	if (Show_Camera_Pos_F == 1)
	{
		Camera_Pos_GUI();
	}

	if (Show_BB_Data_F == 1)
	{
		Model_BB_GUI();
	}

	// SBC_Gui_Dialogs - Physics Console
	if (Show_Physics_Console == 1)
	{
		Physics_Console_Gui();
	}

	// SBC_Gui_Dialogs
	if (Show_Dialog_Float == 1)
	{
		Dialog_Float();
	}

	if (App->CLSB_Environment->Show_PropertyEditor == 1)
	{
		App->CLSB_Environment->Environ_PropertyEditor();
	}

	if (Updating_F == 1)
	{
		Updating_GUI();
	}

	if (Show_Face_Selection == 1)
	{
		Face_Selection();
	}
	
}

// *************************************************************************
// *					WE_Data  Terry Flanigan				   *
// *************************************************************************
void SB_ImGui::WE_Data(void)
{
	//ImGui::SetNextWindowPos(ImVec2(PosX, PosY));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("WE_Data", &Show_FPS))// ImGuiWindowFlags_NoResize
		//| //ImGuiWindowFlags_AlwaysAutoResize ))
	{
		ImGui::End();
	}
	else
	{
		if (StartPos == 0)
		{
			ImGui::SetWindowPos("Ogre Data", ImVec2(500, 5));
			ImGui::SetWindowSize(ImVec2(350, 90));
			StartPos = 1;
		}

		ImGui::Text("FPS average %.0f", ImGui::GetIO().Framerate);

		ImVec2 Size = ImGui::GetWindowSize();

		/*if (App->New_Render == 1)
		{
			PosX = ((float)App->CLSB_BR_Render->RB_RenderListener->View_Width / 2) - (Size.x / 2);
			PosY = 10;
		}
		else
		{
			PosX = ((float)App->CLSB_Ogre->OgreListener->View_Width / 2) - (Size.x / 2);
			PosY = 10;
		}*/

		ImGuiIO& io = ImGui::GetIO();

		//Ogre::Real tx = io.MousePos.x / (Ogre::Real)rw->getWidth();
		//Ogre::Real ty = io.MousePos.y / (Ogre::Real)rw->getHeight();

		//ImGui::Text("TX %f", App->CLSB_BR_Picking->tx);
		//ImGui::Text("TY %f", App->CLSB_BR_Picking->ty);

		if (ImGui::Button("Close"))
		{
			App->Say("Test");
		}

		if (ImGui::Button("Demo"))
		{
			
		}

		ImGui::PopStyleColor();
		ImGui::End();
	}

}

// *************************************************************************
// *					Render_FPS  Terry Flanigan				   *
// *************************************************************************
void SB_ImGui::Render_FPS(void)
{
	ImGui::SetNextWindowPos(ImVec2(PosX, PosY));

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin("Ogre Data", &Show_FPS, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		if (StartPos == 0)
		{
			ImGui::SetWindowPos("Ogre Data", ImVec2(500, 5));
			ImGui::SetWindowSize(ImVec2(350, 90));
			StartPos = 1;
		}

		ImGui::Text("FPS average %.0f", ImGui::GetIO().Framerate);

		ImVec2 Size = ImGui::GetWindowSize();
		PosX = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Width / 2) - (Size.x / 2);
		PosY = 10;

		ImGui::PopStyleColor();
		ImGui::End();
	}
	
}

// *************************************************************************
// *					Updating  Terry Flanigan				   *
// *************************************************************************
void SB_ImGui::Updating(void)
{
	if (Updating_F == 1)
	{
		Updating_GUI();
	}
}



// *************************************************************************
// *						Load_Font  Terry Bernie						   *
// *************************************************************************
void SB_ImGui::Load_Font(void)
{
	ImGuiIO& io = ImGui::GetIO();
	font1 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 20);
	font2 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 48);
	font0 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 18);
	fontDroid = io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 12);

	io.IniFilename = NULL;
}

// *************************************************************************
// *	  				ImGui_Set_Colours Terry Bernie					   *
// *************************************************************************
void SB_ImGui::ImGui_Set_Colours(void)
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	style->WindowRounding = 6.0;
	style->FrameRounding = 0.0;
	style->WindowBorderSize = 0;
	style->FrameBorderSize = 1.0;

	colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.90f);
	colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
	colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
	colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

// *************************************************************************
// *			Updating_GUI:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::Updating_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(Model_Data_PosX, Model_Data_PosY));

	if (!ImGui::Begin("Updating", &Updating_F, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Please Wait - Updating");
		
		ImVec2 Size = ImGui::GetWindowSize();

		
		Model_Data_PosX = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Width / 2) - (Size.x / 2);
		Model_Data_PosY = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Height / 2) - (Size.x / 2);
		

		ImGui::End();
	}
}

// *************************************************************************
// *						ImGui_FPS  Terry Bernie						   *
// *************************************************************************
void SB_ImGui::ImGui_FPS(void)
{
	ImGui::SetNextWindowPos(ImVec2(PosX, PosY));

	if (!ImGui::Begin("Ogre Data", &Show_FPS, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		if (StartPos == 0)
		{
			ImGui::SetWindowPos("Ogre Data", ImVec2(500, 5));
			ImGui::SetWindowSize(ImVec2(350, 90));
			StartPos = 1;
		}

		ImGui::Spacing();
		ImGui::Text("FPS average %.0f", ImGui::GetIO().Framerate);

		int Stack_Memory = App->Get_Stack();
		float used = ((float)(2097152 - Stack_Memory) / 1024);
		ImGui::Text("Stack Used %f", used);
		
		
		ImVec2 Size = ImGui::GetWindowSize();

		PosX = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Width / 2) - (Size.x / 2);
		PosY = 10;
		
		ImGui::End();
	}
}

// *************************************************************************
// *			Start_Model_Data:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_ImGui::Start_Model_Data(void)
{
	Show_Model_Data_F = 1;
}

// *************************************************************************
// *			Model_Data_GUI:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::Model_Data_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(Model_Data_PosX, Model_Data_PosY));

	if (!ImGui::Begin("Model Data", &Show_Model_Data_F, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Spacing();
		ImGui::Text("Model Info");
		ImGui::Text("  ");
		ImGui::Text("Model Name:- %s", App->CLSB_Model->JustName);
		ImGui::Text("Model File Name:- %s", App->CLSB_Model->FileName);
		ImGui::Text("Model Path:- %s", App->CLSB_Model->Path_FileName);
		ImGui::Text("Texture Path:- %s", App->CLSB_Model->Texture_FolderPath);
		ImGui::Text("  ");
		ImGui::Text("Vertices:- %i", App->CLSB_Model->VerticeCount);
		ImGui::Text("Faces:- %i", App->CLSB_Model->FaceCount);
		ImGui::Text("Groups:- %i", App->CLSB_Model->Get_Groupt_Count());
		ImGui::Text("Motions:- %i", App->CLSB_Model->MotionCount);
		ImGui::Text(" --------------------------- Brushes  ");
		ImGui::Text("XBrushes:- %i", App->CLSB_Model->XBrushCount);
		ImGui::Text("Brushes XX:- %i", App->CLSB_Model->BrushCount);

		ImGui::Text("Texture Count:- %i", App->CLSB_Mesh_Mgr->mTextureCount);

		if (ImGui::Button("Textures"))
		{
			if (Show_Textures_F == 1)
			{
				Show_Textures_F = 0;
			}
			else
			{
				Show_Textures_F = 1;
			}
		}

		if (Show_Textures_F == 1)
		{
			int Count = 0;
			while (Count < App->CLSB_Mesh_Mgr->mTextureCount)
			{
				ImGui::Text("Texture ID:-%i %s", Count, App->CLSB_Mesh_Mgr->TextureName2[Count]);
				Count++;
			}
		}

		if (ImGui::Button("Assimp Data"))
		{
			if (Show_Assimp_Data_F == 1)
			{
				Show_Assimp_Data_F = 0;
			}
			else
			{
				Show_Assimp_Data_F = 1;
			}
		}

		if (Show_Assimp_Data_F == 1)
		{
			ImGui::Text("Assimp Group Count:- %i", App->CLSB_Assimp->Total_Assimp_GroupCount);
			ImGui::Text("Assimp Vertice Count:- %i", App->CLSB_Assimp->Total_Assimp_VerticeCount);
			ImGui::Text("Assimp Face Count:- %i", App->CLSB_Assimp->Total_Assimp_FaceCount);
		}

		ImGui::Text("  ");

		ImVec2 Size = ImGui::GetWindowSize();
		Model_Data_PosX = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Width / 2) - (Size.x / 2);
		Model_Data_PosY = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Height / 2) - (Size.y / 2);;

		ImGui::Separator();

		if (ImGui::Button("Close"))
		{
			Close_Model_Data();
		}

		ImGui::End();
	}
}

// *************************************************************************
// *			Close_Model_Data:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_ImGui::Close_Model_Data(void)
{
	Show_Model_Data_F = 0;
}

// *************************************************************************
// *		Start_Camera_Pos:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::Start_Camera_Pos(void)
{
	m_pDoc = (CFusionDoc*)App->m_pMainFrame->GetCurrentDoc();

	pCameraEntity = App->CLSB_Camera_WE->FindCameraEntity();

	//CameraPosition = pCameraEntity->mOrigin;

	Show_Camera_Pos_F = 1;
}

// *************************************************************************
// *			Camera_Pos_GUI:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::Camera_Pos_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Camera_Pos", &Show_Camera_Pos_F, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Columns(2);
		ImGui::SetColumnWidth(10, 120);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

		geVec3d Angles;
		pCameraEntity->GetAngles(&Angles, Level_GetEntityDefs(App->CLSB_Doc->pLevel));

		ImGui::Text("World Editor Position");
		ImGui::Text("X = %f", pCameraEntity->mOrigin.X);
		ImGui::Text("Y = %f", pCameraEntity->mOrigin.Y);
		ImGui::Text("Z = %f", pCameraEntity->mOrigin.Z);
		
		ImGui::Text("  ");
		ImGui::Text("Equity Position");

		ImGui::Text("X = %f", App->CLSB_Ogre_Setup->mCamera->getPosition().x);
		ImGui::Text("Y = %f", App->CLSB_Ogre_Setup->mCamera->getPosition().y);
		ImGui::Text("Z = %f", App->CLSB_Ogre_Setup->mCamera->getPosition().z);
		ImGui::Text("  ");

		// --------------------------------------------------
		ImGui::NextColumn();
	
		ImGui::Text("World Editor Rotation");
		ImGui::Text("X = %f", Units_RadiansToDegrees(Angles.X));
		ImGui::Text("Y = %f", Units_RadiansToDegrees(Angles.Y));
		ImGui::Text("Z = %f", Units_RadiansToDegrees(Angles.Z));

		ImGui::Text("  ");
		ImGui::Text("Equity Rotation");

		ImGui::Text("X = %f", App->CLSB_Ogre_Setup->mCamera->getOrientation().getPitch().valueDegrees());
		ImGui::Text("Y = %f", App->CLSB_Ogre_Setup->mCamera->getOrientation().getYaw().valueDegrees());
		ImGui::Text("Z = %f", App->CLSB_Ogre_Setup->mCamera->getOrientation().getRoll().valueDegrees());

		ImVec2 Size = ImGui::GetWindowSize();
		Model_Data_PosX = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Width / 2) - (Size.x / 2);
		Model_Data_PosY = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Height / 2) - (Size.y / 2);;

		ImGui::PopStyleVar();
		ImGui::Columns(0);

		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Close"))
		{
			Close_Camera_Pos();
		}

		ImGui::End();
	}
}

// *************************************************************************
// *			Close_Camera_Pos:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_ImGui::Close_Camera_Pos(void)
{
	Show_Camera_Pos_F = 0;
}

// *************************************************************************
// *			Start_BB_Data:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::Start_BB_Data(void)
{
	Show_BB_Data_F = 1;
}

// *************************************************************************
// *			Model_BB_GUI:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::Model_BB_GUI(void)
{
	ImGui::SetNextWindowPos(ImVec2(Model_Data_PosX, Model_Data_PosY));

	if (!ImGui::Begin("BB Data", &Show_BB_Data_F, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Spacing();
		ImGui::Text("Bounding Box Info");
		ImGui::Text("  ");
		ImGui::Text("Min X:- %f", App->CLSB_Model->BB_Min.x);
		ImGui::Text("Min Y:- %f", App->CLSB_Model->BB_Min.y);
		ImGui::Text("Min Z:- %f", App->CLSB_Model->BB_Min.z);
		ImGui::Text("  ");
		ImGui::Text("Max X:- %f", App->CLSB_Model->BB_Max.x);
		ImGui::Text("Max Y:- %f", App->CLSB_Model->BB_Max.y);
		ImGui::Text("Max Z:- %f", App->CLSB_Model->BB_Max.z);
		ImGui::Text("  ");
		ImGui::Text("Size X:- %f", App->CLSB_Model->Size.x);
		ImGui::Text("Size Y:- %f", App->CLSB_Model->Size.y);
		ImGui::Text("Size Z:- %f", App->CLSB_Model->Size.z);
		ImGui::Text("  ");
		ImGui::Text("Centre X:- %f", App->CLSB_Model->Centre.x);
		ImGui::Text("Centre Y:- %f", App->CLSB_Model->Centre.y);
		ImGui::Text("Centre Z:- %f", App->CLSB_Model->Centre.z);
		ImGui::Text("  ");
		ImGui::Text("Radius:- %f", App->CLSB_Model->radius);

		ImVec2 Size = ImGui::GetWindowSize();
		Model_Data_PosX = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Width / 2) - (Size.x / 2);
		Model_Data_PosY = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Height / 2) - (Size.y / 2);;

		ImGui::Separator();

		if (ImGui::Button("Close"))
		{
			Close_BB_Data();
		}

		ImGui::End();
	}
}

// *************************************************************************
// *			Close_BB_Data:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::Close_BB_Data(void)
{
	Show_BB_Data_F = 0;
}

// *************************************************************************
// *						Physics_Console  Terry Bernie				   *
// *************************************************************************
void SB_ImGui::Physics_Console_Gui(void)
{
	ImGui::SetNextWindowPos(ImVec2(Physics_PosX, Physics_PosY), ImGuiCond_FirstUseEver);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));
	ImGuiStyle* style = &ImGui::GetStyle();

	if (!ImGui::Begin("Physics_Console", &Show_Physics_Console, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		if (Disable_Physics_Console == 1)
		{
			ImGui::BeginDisabled(true);
		}

		ImGui::Text("Physics Console");

		ImGui::SameLine(0, 270);
		if (ImGui::Button("H"))
		{
			//App->Cl_Utilities->OpenHTML("Help\\Physics_Console.html");
		}

		ImGui::SameLine();
		if (ImGui::Button("X"))
		{
			//CheckMenuItem(App->mMenu, ID_WINDOWS_SHOWPHYSICSPANEL, MF_BYCOMMAND | MF_UNCHECKED);
			Physics_Console_StartPos = 0;
			Show_Physics_Console = 0;
		}

		ImGui::Separator();

		if (App->CLSB_Ogre_Setup->OgreListener->GD_Run_Physics == 1)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}
		else
		{
			style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		}

		if (ImGui::Button("Physics On"))
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				if (App->CLSB_Ogre_Setup->OgreListener->GD_Run_Physics == 1)
				{
					App->CLSB_Ogre_Setup->OgreListener->GD_Run_Physics = 0;
				}
				else
				{
					App->CLSB_Ogre_Setup->OgreListener->GD_Run_Physics = 1;
				}

				//App->RedrawWindow_Dlg(App->Physics_Console_Hwnd);
			}
		}

		if (App->CLSB_Ogre_Setup->OgreListener->Dubug_Physics_Draw == 1)
		{
			style->Colors[ImGuiCol_Button] = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
		}
		else
		{
			style->Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
		}

		/*ImGui::SameLine();
		if (ImGui::Button("Physics Outline"))
		{
			if (App->CLSB_Ogre->OgreListener->Dubug_Physics_Draw == 1)
			{
				App->CLSB_Ogre->OgreListener->Dubug_Physics_Draw = 0;
				App->CLSB_Ogre->BulletListener->Render_Debug_Flag = 0;
			}
			else
			{
				App->CLSB_Ogre->OgreListener->Dubug_Physics_Draw = 1;
				App->CLSB_Ogre->BulletListener->Render_Debug_Flag = 1;
			}
		}*/

		ImGui::SameLine();
		if (ImGui::Button("Reset Physics"))
		{
			if (App->CLSB_Model->Model_Loaded == 1)
			{
				App->CLSB_Bullet->Reset_Physics();
			}
		}

		//ImGui::SameLine();
		//if (ImGui::Button("Reset Entities"))
		//{
		//	/*if (App->SBC_Scene->Scene_Loaded == 1)
		//	{
		//		App->SBC_Physics->Reset_Triggers();
		//	}*/
		//}

		ImGui::SameLine();
		//if (ImGui::Button("Reset Scene"))
		//{
		//	//if (App->SBC_Scene->Scene_Loaded == 1)
		//	//{

		//	//	int Saved = App->CL_Ogre->OgreListener->GD_CameraMode;
		//	//	App->CL_Ogre->OgreListener->GD_CameraMode = Enums::CamFirst;

		//	//	App->SBC_Physics->Reset_Physics();
		//	//	App->CL_Ogre->OgreListener->GD_Run_Physics = 1;
		//	//	App->SBC_Physics->Reset_Triggers();
		//	//	App->CL_Ogre->OgreListener->GD_Run_Physics = 1;

		//	//	App->SBC_TopTabs->Toggle_FirstCam_Flag = 1;
		//	//	App->SBC_TopTabs->Toggle_FreeCam_Flag = 0;
		//	//	RedrawWindow(App->SBC_TopTabs->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

		//	//	/*App->SBC_TopTabs->Toggle_FirstCam_Flag = 0;
		//	//	App->SBC_TopTabs->Toggle_FreeCam_Flag = 1;
		//	//	RedrawWindow(App->SBC_TopTabs->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		//	//	App->CL_Ogre->OgreListener->GD_CameraMode = Enums::CamDetached;*/

		//	//	App->SBC_Com_Environments->GameMode(0);

		//	//}
		//}

		if (Physics_Console_StartPos == 0)
		{
			ImVec2 Size = ImGui::GetWindowSize();
			Physics_PosX = 10;
			Physics_PosY = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Height) - (Size.y) - 5;// 50;
			ImGui::SetWindowPos("Physics_Console", ImVec2(Physics_PosX, Physics_PosY));
			
			Physics_Console_StartPos = 1;
		}

		style->Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		ImGui::PopStyleColor();


		if (Disable_Physics_Console == 1)
		{
			ImGui::EndDisabled();
		}

		ImGui::End();
	}
}

// *************************************************************************
// *		Start_Dialog_Float:- Terry and Hazel Flanigan 2023  		   *
// *************************************************************************
void SB_ImGui::Start_Dialog_Float(float Step, float StartValue, char* Banner)
{
	Float_Exit = 0;
	Float_Canceld = 0;
	Float_Step = Step;
	m_Dialog_Float = StartValue;
	strcpy(Float_Banner, Banner);

	m_Dialog_Float_Copy = StartValue;

	//App->Disable_Panels(true);

	Float_PosX = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Width / 2) - (200 / 2);
	Float_PosY = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Height / 2) - (130 / 2);

	Float_StartPos = 0;

	Show_Dialog_Float = 1;
}

// *************************************************************************
// *			Dialog_Float:- Terry and Hazel Flanigan 2023  			   *
// *************************************************************************
void SB_ImGui::Dialog_Float(void)
{

	ImGui::SetNextWindowPos(ImVec2(Float_PosX, Float_PosY), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 130), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 255));

	if (!ImGui::Begin(Float_Banner, &Show_Dialog_Float, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	else
	{
		if (Float_StartPos == 0)
		{
			Float_PosX = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Width / 2) - (200 / 2);
			Float_PosY = ((float)App->CLSB_Ogre_Setup->OgreListener->View_Height / 2) - (130 / 2);
			ImGui::SetWindowPos(Float_Banner, ImVec2(Float_PosX, Float_PosY));

			Float_StartPos = 1;
		}

		float spacingX = ImGui::GetStyle().ItemInnerSpacing.x;

		ImGui::Indent();
		ImGui::Spacing();

		ImGui::InputFloat("", &m_Dialog_Float, Float_Step, 0, "%.3f");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Indent();

		if (ImGui::Button("Apply"))
		{
			Float_Exit = 1;
			Show_Dialog_Float = 0;
			Float_StartPos = 0;
			Float_Canceld = 0;
			ImGui::PopStyleColor();
			ImGui::End();
		}

		ImGui::SameLine(0.0f, spacingX);

		if (ImGui::Button("Close"))
		{
			Float_StartPos = 0;
			Float_Exit = 1;
			Show_Dialog_Float = 0;
			Float_Canceld = 1;
			ImGui::PopStyleColor();
			ImGui::End();
		}

		if (Float_Exit == 0)
		{
			Float_Canceld = 1;
			ImGui::PopStyleColor();
			ImGui::End();
		}
	}
}

// *************************************************************************
// *			Face_Selection:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_ImGui::Face_Selection(void)
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(239, 239, 239, 200));

	if (!ImGui::Begin("Selection_Data", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) // | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
	}
	else
	{
		if (ImGui::Button("Global_Data"))
		{
			if (Show_Global_Data_F == 1)
			{
				Show_Global_Data_F = 0;
			}
			else
			{
				Show_Global_Data_F = 1;
			}
		}

		if (Show_Global_Data_F == 1)
		{
			ImGui::Text("Selected Object");
			ImGui::Separator();

			ImGui::Text("Internal Name: = %s", App->CLSB_Picking->Pl_Entity_Name.c_str());
			ImGui::Text("Object Name: = %s", App->CLSB_Ogre_Setup->OgreListener->Selected_Object_Name);

			ImGui::Text("");
			ImGui::Text("Sub Meshes: = %i", App->CLSB_Picking->Sub_Mesh_Count);
			ImGui::Text("Total Vertices: = %i", App->CLSB_Picking->Total_Vertices_count_Actual);
			ImGui::Text("Total Indices: = %i", App->CLSB_Picking->Total_index_count_Actual);
			ImGui::Text("Total Faces: = %i", App->CLSB_Picking->Total_index_count_Actual / 3);

			ImGui::Text("");

			ImGui::Text("Distance: = %f", App->CLSB_Picking->closest_distance);
			//ImGui::Text("Type: = %s", App->CLSB_Picking->TextureName);

			if (App->CLSB_Picking->TextCords)
			{
				int Face_Index = App->CLSB_Picking->Face_Index;

				ImGui::Text("Sub Mesh: = %i", App->CLSB_Picking->SubMesh_Index);
				ImGui::Text("Face Index Global: = %i", Face_Index / 3);
				ImGui::Text("Face Index Local: = %i", App->CLSB_Picking->Local_Face);
				//ImGui::Text("Test: = %i", (Face_Index / 3) - App->CLSB_Picking->Local_Face);
				ImGui::Text("");
				/*ImGui::Text("Vert1: = %.3f %.3f %.3f", App->CLSB_Grid->HitVertices[0].x, App->CLSB_Grid->HitVertices[0].y, App->CLSB_Grid->HitVertices[0].z);
				ImGui::Text("Vert1: = %.3f %.3f %.3f", App->CLSB_Grid->HitVertices[1].x, App->CLSB_Grid->HitVertices[1].y, App->CLSB_Grid->HitVertices[1].z);
				ImGui::Text("Vert1: = %.3f %.3f %.3f", App->CLSB_Grid->HitVertices[2].x, App->CLSB_Grid->HitVertices[2].y, App->CLSB_Grid->HitVertices[2].z);
				ImGui::Text("");
				ImGui::Text("UV1: = %f %f", App->CLSB_Grid->HitFaceUVs[0].x, App->CLSB_Grid->HitFaceUVs[0].y);
				ImGui::Text("UV2: = %f %f", App->CLSB_Grid->HitFaceUVs[1].x, App->CLSB_Grid->HitFaceUVs[1].y);
				ImGui::Text("UV3: = %f %f", App->CLSB_Grid->HitFaceUVs[2].x, App->CLSB_Grid->HitFaceUVs[2].y);
				ImGui::Text("");*/
			}
		}

		if (ImGui::Button("Brush_Data"))
		{
			if (Show_Brush_Data_F == 1)
			{
				Show_Brush_Data_F = 0;
			}
			else
			{
				Show_Brush_Data_F = 1;
			}
		}

		if (Show_Brush_Data_F == 1)
		{
			ImGui::Text("Brush Name: = %s", App->CLSB_Picking->Selected_Brush_Name);
			ImGui::Text("Face Count: = %i", App->CLSB_Picking->Selected_Brush_Face_Count);
			ImGui::Text("Face Material: = %s", App->CLSB_Picking->FaceMaterial);
			ImGui::Text("Texture: = %s", App->CLSB_Picking->TextureName2);
			//ImGui::Text("Texture: = %s", App->CLSB_Picking->TextureName2);

		}

		if (ImGui::Button("Real Brush"))
		{
			if (Show_Real_Brush_F == 1)
			{
				Show_Real_Brush_F = 0;
			}
			else
			{
				Show_Real_Brush_F = 1;
			}
		}

		if (Show_Real_Brush_F == 1)
		{
			if (App->CLSB_Picking->Selected_Brush)
			{
				ImGui::Text("Brush Name2: = %s", App->CLSB_Picking->Selected_Brush->Name);
				ImGui::Text("Brush_Type: = %s", App->CLSB_Picking->Real_Brush_Type);
				ImGui::Text("Total_Brushes: = %i", App->CLSB_Picking->Real_Sub_Brush_Count);
				ImGui::Text("Sub_Brushes: = %i", App->CLSB_Picking->Real_Sub_Brush_Count-1);
				ImGui::Text("Face Count: = %i", App->CLSB_Picking->Real_Face_Count);
				ImGui::Text("Real Face: = %i", App->CLSB_Picking->Real_Face_Index);
			}

			ImGui::Selectable("Select Face:- ", &Select_Face_F);
		}

		if (Select_Face_F == 1)
		{
			App->CLSB_Dialogs->Start_Select_Face_Dlg();
			//App->CLSB_Picking->Select_Face_In_Brush(Pick_Face);
		}

		if (ImGui::Button("Close"))
		{
			Show_Face_Selection = 0;
			ImGui::PopStyleColor();
			ShowWindow(App->ListPanel, true);
			ImGui::End();
		}

		ImGui::End();
	}
}


