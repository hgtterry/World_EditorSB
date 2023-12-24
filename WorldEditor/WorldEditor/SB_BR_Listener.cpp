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
#include "SB_BR_Listener.h"

SB_BR_Listener::SB_BR_Listener()
{
	mMoveScale = 0;
	mMoveSensitivity = 50;
	Wheel_Move = 0;

	Pl_DeltaMouse = 0;
	Pl_MouseX = 0;
	Pl_MouseY = 0;

	Pl_Cent500X = App->CursorPosX;
	Pl_Cent500Y = App->CursorPosY;

	mMoveSensitivityMouse = 50;

	Pl_LeftMouseDown = 0;
	Pl_RightMouseDown = 0;

	WE_Cam = nullptr;
	WE_Cam = App->CLSB_BR_Render->mCameraMeshView;
}

SB_BR_Listener::~SB_BR_Listener()
{
}

// *************************************************************************
// *				frameStarted   Terry Bernie							   *
// *************************************************************************
bool SB_BR_Listener::frameStarted(const FrameEvent& evt)
{
	Get_View_Height_Width();
	App->CLSB_Ogre->RB_m_imgui.NewFrame(evt.timeSinceLastFrame, (float)View_Width, (float)View_Height);

	if (App->CLSB_ImGui->Updating_F == 1)
	{
		App->CLSB_ImGui->Updating();
	}

	//App->CLSB_ImGui->Show_FPS = 1;
	//App->CLSB_ImGui->Render_FPS();
	
	return true;
}

// *************************************************************************
// *		Get_View_Height_Width:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
bool SB_BR_Listener::Get_View_Height_Width(void)
{
	Ogre::Viewport* vp = App->CLSB_BR_Render->RB_View_Window->getViewport(0);

	View_Width = vp->getActualWidth();
	View_Height = vp->getActualHeight();

	return 1;
}


// *************************************************************************
// *			frameRenderingQueued   Terry Bernie						   *
// *************************************************************************
bool SB_BR_Listener::frameRenderingQueued(const FrameEvent& evt)
{
	App->CLSB_Ogre->RB_m_imgui.render();

	Camera_Mode_Free(evt.timeSinceLastFrame);
	return 1;
}

// *************************************************************************
// *		Camera_Mode_Free:- Terry and Hazel Flanigan 2023   			   *
// *************************************************************************
void SB_BR_Listener::Camera_Mode_Free(float DeltaTime)
{
	App->CLSB_Ogre->m_imgui.render();

	mRotX = 0;
	mRotY = 0;
	mTranslateVector = Ogre::Vector3::ZERO;

	mMoveScale = mMoveSensitivity * DeltaTime;

	App->CLSB_Keyboard->RB_Keyboard_Mode_Free(DeltaTime);

	if (Pl_LeftMouseDown == 1 && Pl_RightMouseDown == 0)
	{
		Capture_Left_Mouse_Free();
		SetCursorPos(App->CursorPosX, App->CursorPosY);
		
	}

	// Right Mouse
	if (Pl_LeftMouseDown == 0 && Pl_RightMouseDown == 1)
	{
		//Capture_Right_Mouse_Free();
	}

	//ModelMode(mMoveScale);

	MoveCamera();
}

// *************************************************************************
// *							ModelMode   							   *
// *************************************************************************
void SB_BR_Listener::ModelMode(float DeltaTime)
{
	mRotX = 0;
	mRotY = 0;
	mTranslateVector = Ogre::Vector3::ZERO;

	mMoveScale = mMoveSensitivity * DeltaTime;

	if (GetAsyncKeyState(69) < 0) // Q key Down in Fly Mode
	{
		Ogre::Real Rate;
		Rate = (mMoveSensitivity / 1000) * 4;    //0.1;//FlyRate;

		Ogre::Vector3 OldPos;
		OldPos = WE_Cam->getPosition();

		OldPos.y += Rate;
		WE_Cam->setPosition(OldPos);
	}
	//------------------------------------------------
	if (GetAsyncKeyState(81) < 0) // E key Up in Fly Mode
	{
		Ogre::Real Rate;
		Rate = (mMoveSensitivity / 1000) * 4;// 0.1;//FlyRate;

		Ogre::Vector3 OldPos;
		OldPos = WE_Cam->getPosition();

		OldPos.y -= Rate;
		WE_Cam->setPosition(OldPos);
	}

	////------------------------------------------------
	if (Wheel_Move < 0) // Mouse Wheel Forward
	{
		mTranslateVector.z = -mMoveScale * 30;
	}
	if (GetAsyncKeyState(87) < 0) // W Key
	{
		mTranslateVector.z = -mMoveScale;
	}

	////------------------------------------------------
	//// back
	if (Wheel_Move > 0) // Mouse Wheel Back
	{
		mTranslateVector.z = mMoveScale * 30;
	}
	if (GetAsyncKeyState(83) < 0) // S Key	
	{
		mTranslateVector.z = mMoveScale;
	}

	//------------------------------------------------
	// Right
	if (GetAsyncKeyState(65) < 0)
	{
		mTranslateVector.x = mMoveScale;
	}
	// Left
	if (GetAsyncKeyState(68) < 0)
	{
		mTranslateVector.x = -mMoveScale;
	}

	//if (GetAsyncKeyState(VK_ESCAPE) < 0) // Back to full Screen;
	//{
	//	if (App->FullScreen == 1)
	//	{
	//		App->FullScreen = 0;
	//		App->Cl19_Ogre->ExitFullScreen();
	//	}
	//}

	// Left Mouse
	//if (Pl_LeftMouseDown == 1 && Pl_RightMouseDown == 0)
	//{
	//	//Capture_LeftMouse_Model();
	//	Capture_Left_Mouse_Free();
	//}

	// Right Mouse
	/*if (Pl_LeftMouseDown == 0 && Pl_RightMouseDown == 1)
	{
		Capture_RightMouse_Model();
	}*/

	//MoveCamera();
}

// *************************************************************************
// *				moveCamera   Terry Bernie							   *
// *************************************************************************
void SB_BR_Listener::MoveCamera(void)
{
	WE_Cam->yaw(mRotX);
	WE_Cam->pitch(mRotY);
	WE_Cam->moveRelative(mTranslateVector); // Position Relative
	Wheel_Move = 0;
}

// *************************************************************************
// *					Capture_LeftMouse_Model							   *
// *************************************************************************
bool SB_BR_Listener::Capture_LeftMouse_Model(void)
{
	GetCursorPos(&Pl_pt);

	Pl_MouseX = (int(Pl_pt.x));
	Pl_MouseY = (int(Pl_pt.y));

	//// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
			App->CLSB_BR_Render->GridNode->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			App->CLSB_Export_Ogre3D->World_Node->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			App->CLSB_BR_Render->GridNode->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			App->CLSB_Export_Ogre3D->World_Node->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);

			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	// Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);
			App->CLSB_BR_Render->GridNode->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			App->CLSB_Export_Ogre3D->World_Node->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);
			App->CLSB_BR_Render->GridNode->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			App->CLSB_Export_Ogre3D->World_Node->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	return 1;
}

// *************************************************************************
// *					Capture_RightMouse_Model Terry					   *
// *************************************************************************
bool SB_BR_Listener::Capture_RightMouse_Model(void)
{
	GetCursorPos(&Pl_pt);

	Pl_MouseX = (int(Pl_pt.x));
	Pl_MouseY = (int(Pl_pt.y));

	//// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
			mTranslateVector.x = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			mTranslateVector.x = -Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	//// Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);

			Ogre::Real Rate;
			Rate = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);

			Ogre::Vector3 OldPos;
			OldPos = WE_Cam->getPosition();

			OldPos.y -= Rate;
			WE_Cam->setPosition(OldPos);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);

			Ogre::Real Rate;
			Rate = Pl_DeltaMouse * (mMoveSensitivityMouse / 1000);

			Ogre::Vector3 OldPos;
			OldPos = WE_Cam->getPosition();

			OldPos.y += Rate;
			WE_Cam->setPosition(OldPos);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	return 1;
}

// *************************************************************************
// *				Capture_Left_Mouse_Free   Terry Bernie				   *
// *************************************************************************
void SB_BR_Listener::Capture_Left_Mouse_Free(void)
{
	//App->Flash_Window();
	//if (!ImGui::GetIO().WantCaptureMouse)
	//{
	GetCursorPos(&Pl_pt);
	Pl_MouseX = (int(Pl_pt.x));
	Pl_MouseY = (int(Pl_pt.y));

	// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Positive
		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
			mRotX = Degree(Pl_DeltaMouse * (float)0.02);//S_Player[0]->TurnRate);
			
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			mRotX = Degree(-Pl_DeltaMouse * (float)0.02);//S_Player[0]->TurnRate);
			
		}
	}

	// Up Down
	if (Pl_MouseY < Pl_Cent500Y)
	{
		long test = Pl_Cent500Y - Pl_MouseY; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500Y - Pl_MouseY);
			mRotY = Degree(Pl_DeltaMouse * (float)0.02);//S_Player[0]->TurnRate);
			
		}
	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);
			mRotY = Degree(-Pl_DeltaMouse * (float)0.02);//S_Player[0]->TurnRate);
			
		}
	}
}

