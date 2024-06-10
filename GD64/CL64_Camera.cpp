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
#include "CL64_Camera.h"

CL64_Camera::CL64_Camera(void)
{
}

CL64_Camera::~CL64_Camera(void)
{

}

// *************************************************************************
// *			Reset_View:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Camera::Reset_View(void)
{
	App->CL_Grid->GridNode->setPosition(0, 0, 0);
	App->CL_Grid->GridNode->resetOrientation();

	App->CL_Grid->HairNode->setPosition(0, 0, 0);
	App->CL_Grid->HairNode->resetOrientation();

	App->CL_Ogre->OgreNode->resetOrientation();

	App->CL_Ogre->RenderListener->RX = 0;
	App->CL_Ogre->RenderListener->RZ = 0;

	App->CL_Ogre->camNode->resetOrientation();
	App->CL_Ogre->camNode->setPosition(Ogre::Vector3(0, 90, 100));
	App->CL_Ogre->camNode->lookAt(Ogre::Vector3(0, 30, 0), Ogre::Node::TS_WORLD);
}
