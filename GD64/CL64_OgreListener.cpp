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
#include "CL64_OgreListener.h"



CL64_OgreListener::CL64_OgreListener(void)
{
	mCam = App->CL_Ogre->mCamera;
	mCamNode = App->CL_Ogre->camNode;

	mRotX = 0;;
	mRotY = 0;

	mTranslateVector = Ogre::Vector3::ZERO;
	
	mMoveScale = 0;
	mMoveSensitivity = 50;
	mMoveSensitivityMouse = 50;

	Pl_LeftMouseDown = 0;
	Pl_RightMouseDown = 0;

	Pl_Cent500X = App->CursorPosX;
	Pl_Cent500Y = App->CursorPosY;
	Pl_DeltaMouse = 0;
	Pl_MouseX = 0;
	Pl_MouseY = 0;

	Wheel = 0;
	StopOgre = 0;
}

CL64_OgreListener::~CL64_OgreListener(void)
{
}

// *************************************************************************
// *			frameStarted:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_OgreListener::frameStarted(const FrameEvent& evt)
{
	//Update_Game_Logic(evt.timeSinceLastFrame);
	return true;
}

// *************************************************************************
// *		frameRenderingQueued:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_OgreListener::frameRenderingQueued(const FrameEvent& evt)
{
	App->CL_Ogre->mTrayMgr->frameRendered(evt);

	Camera_Mode_Model(evt.timeSinceLastFrame);

	return 1;
}

// *************************************************************************
// *			frameEnded:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_OgreListener::frameEnded(const FrameEvent& evt)
{

	if (StopOgre == 1)
	{
		return false;
	}

	return true;
}

// *************************************************************************
// *		Camera_Mode_Model:- Terry and Hazel Flanigan 2023  			   *
// *************************************************************************
void CL64_OgreListener::Camera_Mode_Model(float DeltaTime)
{
	//App->CL_Ogre->m_imgui.render();

	mRotX = 0;
	mRotY = 0;

	mTranslateVector = Ogre::Vector3::ZERO;

	mMoveScale = mMoveSensitivity * DeltaTime;

	//App->CL_Keyboard->Keyboard_Mode_Model(DeltaTime);

	// Left Mouse
	if (Pl_LeftMouseDown == 1 && Pl_RightMouseDown == 0)
	{
		Capture_LeftMouse_Model();
	}

	// Right Mouse
	if (Pl_LeftMouseDown == 0 && Pl_RightMouseDown == 1)
	{
		Capture_RightMouse_Model();
	}

	MoveCamera();
}

// *************************************************************************
// *				moveCamera   Terry Bernie							   *
// *************************************************************************
void CL64_OgreListener::MoveCamera(void)
{
	mCamNode->yaw(mRotX);
	mCamNode->pitch(mRotY);
	mCamNode->translate(mTranslateVector); // Position Relative
	Wheel = 0;

}
// *************************************************************************
// *		Capture_LeftMouse_Model:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_OgreListener::Capture_LeftMouse_Model(void)
{
	GetCursorPos(&Mouse_point);

	Pl_MouseX = (int(Mouse_point.x));
	Pl_MouseY = (int(Mouse_point.y));

	//// Left Right
	if (Pl_MouseX < Pl_Cent500X)
	{
		long test = Pl_Cent500X - Pl_MouseX; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_Cent500X - Pl_MouseX);
			App->CL_Grid->GridNode->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			//App->CL_Grid->HairNode->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			//App->CL_Grid->DummyNode->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			//App->CL_Ogre->RenderListener->RZ = App->CL_Ogre->RenderListener->RZ - (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);

			App->CL_Ogre->OgreNode->yaw(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseX > Pl_Cent500X)
	{
		long test = Pl_MouseX - Pl_Cent500X; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseX - Pl_Cent500X);
			App->CL_Grid->GridNode->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			//App->CL_Grid->HairNode->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			//App->CL_Grid->DummyNode->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
			//App->CL_Ogre->RenderListener->RZ = App->CL_Ogre->RenderListener->RZ + (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);
			App->CL_Ogre->OgreNode->yaw(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_LOCAL);
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
			App->CL_Grid->GridNode->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			//App->CL_Grid->HairNode->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			//App->CL_Grid->DummyNode->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			//App->CL_Ogre->RenderListener->RX = App->CL_Ogre->RenderListener->RX - (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);
			App->CL_Ogre->OgreNode->pitch(Ogre::Degree(-Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}
	else if (Pl_MouseY > Pl_Cent500Y)
	{
		long test = Pl_MouseY - Pl_Cent500Y; // Positive

		if (test > 2)
		{
			Pl_DeltaMouse = float(Pl_MouseY - Pl_Cent500Y);
			App->CL_Grid->GridNode->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			//App->CL_Grid->HairNode->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			//App->CL_Grid->DummyNode->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			//App->CL_Ogre->RenderListener->RX = App->CL_Ogre->RenderListener->RX + (Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2);
			App->CL_Ogre->OgreNode->pitch(Ogre::Degree(Pl_DeltaMouse * (mMoveSensitivityMouse / 1000) * 2), Ogre::Node::TS_PARENT);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}
	}

	return 1;
}

// *************************************************************************
// *					Capture_RightMouse_Model Terry					   *
// *************************************************************************
bool CL64_OgreListener::Capture_RightMouse_Model(void)
{
	GetCursorPos(&Mouse_point);

	Pl_MouseX = (int(Mouse_point.x));
	Pl_MouseY = (int(Mouse_point.y));

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
			OldPos = mCamNode->getPosition();

			OldPos.y -= Rate;
			mCamNode->setPosition(OldPos);
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
			OldPos = mCamNode->getPosition();

			OldPos.y += Rate;
			mCamNode->setPosition(OldPos);
			SetCursorPos(App->CursorPosX, App->CursorPosY);
		}

	}

	return 1;
}
