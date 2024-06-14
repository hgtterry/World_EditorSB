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

#pragma once

#include "Ogre.h"
#include "CL64_App.h"

using namespace Ogre;

class CL64_OgreListener : public FrameListener
{
public:
	CL64_OgreListener(void);
	~CL64_OgreListener(void);

	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);
	bool frameRenderingQueued(const FrameEvent& evt);
	void MoveCamera(void);
	void Get_View_Height_Width(void);

	// Model Mode
	void Mode_Camera_Model(float DeltaTime);
	bool Capture_LeftMouse_Model(void);
	bool Capture_RightMouse_Model(void);

	// Free Mode
	void Mode_Camera_Free(float DeltaTime);
	bool Capture_Left_Mouse_Free(void);
	bool Capture_Right_Mouse_Free(void);

	Ogre::Camera* mCam;
	Ogre::SceneNode* mCamNode;

	POINT		Mouse_point;
	//Ogre::Overlay* mDebugOverlay;

	Radian		mRotX;
	Radian		mRotY;
	Vector3		mTranslateVector;

	float		mMoveScale;
	float		mMoveSensitivity; 
	float		mMoveSensitivityMouse;

	int	View_Height; // Ogre window Height
	int	View_Width;	 // Ogre window WIdth

	int CameraMode;

	int	Wheel;

	int	Pl_MouseX;
	int	Pl_MouseY;
	
	int Bullet_Step;

	long Pl_Cent500X;
	long Pl_Cent500Y;

	float Pl_DeltaMouse;

	bool Pl_LeftMouseDown;
	bool Pl_RightMouseDown;

	bool StopOgre;
};

