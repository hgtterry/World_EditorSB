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
#include "CL64_Keyboard.h"

CL64_Keyboard::CL64_Keyboard(void)
{
}

CL64_Keyboard::~CL64_Keyboard(void)
{
}

// *************************************************************************
// *						Keyboard_Mode_Model							   *
// *************************************************************************
void CL64_Keyboard::Keyboard_Mode_Model(float deltaTime)
{
	//	Pan Up
	if (GetAsyncKeyState(69) < 0) // E key 
	{
		Ogre::Real Rate;
		Rate = (App->CL_Ogre->OgreListener->mMoveSensitivity / 1000) * 2;

		Ogre::Vector3 OldPos;
		OldPos = App->CL_Ogre->OgreListener->mCamNode->getPosition();

		OldPos.y += Rate;
		App->CL_Ogre->OgreListener->mCamNode->setPosition(OldPos);
	}

	// Pan Down
	if (GetAsyncKeyState(81) < 0)  // Q Key
	{
		Ogre::Real Rate;
		Rate = (App->CL_Ogre->OgreListener->mMoveSensitivity / 1000) * 2;

		Ogre::Vector3 OldPos;
		OldPos = App->CL_Ogre->OgreListener->mCamNode->getPosition();

		OldPos.y -= Rate;
		App->CL_Ogre->OgreListener->mCamNode->setPosition(OldPos);
	}

	// Forward
	if (App->CL_Ogre->OgreListener->Wheel < 0) // Mouse Wheel Forward
	{
		App->CL_Ogre->OgreListener->mTranslateVector.z = -App->CL_Ogre->OgreListener->mMoveScale * 25;
	}
	if (GetAsyncKeyState(87) < 0) // W Key
	{
		App->CL_Ogre->OgreListener->mTranslateVector.z = -App->CL_Ogre->OgreListener->mMoveScale;
	}

	// Back
	if (App->CL_Ogre->OgreListener->Wheel > 0) // Mouse Wheel Back
	{
		App->CL_Ogre->OgreListener->mTranslateVector.z = App->CL_Ogre->OgreListener->mMoveScale * 25;
	}
	if (GetAsyncKeyState(83) < 0) // S Key	
	{
		App->CL_Ogre->OgreListener->mTranslateVector.z = App->CL_Ogre->OgreListener->mMoveScale;
	}

	// Pan Left
	if (GetAsyncKeyState(65) < 0) // A Key
	{
		App->CL_Ogre->OgreListener->mTranslateVector.x = App->CL_Ogre->OgreListener->mMoveScale;
	}

	// Pan Right
	if (GetAsyncKeyState(68) < 0)  // D Key
	{
		App->CL_Ogre->OgreListener->mTranslateVector.x = -App->CL_Ogre->OgreListener->mMoveScale;
	}

	//------------------------------------------------ Escape 
	if (GetAsyncKeyState(VK_ESCAPE) < 0) // Back to Editor mode;
	{
		//if (App->CL_Ogre->FullScreenMode_Flag == 1)
		{
			//App->CL_Ogre->ExitFullScreen();
		}
	}
}
