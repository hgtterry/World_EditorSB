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

#include "CL64_OgreListener.h"
#include "CL64_OGL_Listener.h"
#include "OgreOverlaySystem.h"
#include "OgreImGuiOverlay.h"
#include "OgreAdvancedRenderControls.h"
#include "OgreTrays.h"
#include "SdkSample.h"
#include "OgreImGuiOverlay.h"
#include <OgreImGuiInputListener.h>

#include "CL64_DebugBulletWorld.h"

using namespace Ogre;
using namespace OgreBites;

class CL64_Ogre_Init : public SdkSample, public RenderTargetListener
{
public:

	CL64_Ogre_Init(void);
	~CL64_Ogre_Init(void);

	// Setup --------------------------------
	void InitOgre(void);
	bool OgreCreateRoot(void);
	bool SetUpResources(void);
	bool Configure(void);
	bool chooseSceneManager(void);
	bool createCamera(void);
	bool createViewports(void);
	bool Initialise_Resources(void);
	bool createFrameListener(void);
	// --------------------------------------


	void Show_Test_Cube(void);

	bool Ogre_Render_Loop(void);

	bool ReverseBackSlash(char* buf);
	
	HWND RenderHwnd;

	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* camNode;

	Ogre::String mResourcePath;

	Ogre::String App_Resource_Group;		// App Resource Group
	Ogre::String World_Resource_Group;

	OgreBites::TrayManager* mTrayMgr;

	CL64_OgreListener*		OgreListener;
	CL64_OGL_Listener*		RenderListener;
	CL64_DebugBulletWorld*	BulletListener;


	Ogre::OverlaySystem* mOverlaySystem;
	
	Ogre::ManualObject* manObj;
	Ogre::SceneNode* ModelNode;

	Ogre::Entity* OgreEntity;
	Ogre::SceneNode* OgreNode;

	char Return_Chr[MAX_PATH];

	bool flag_Hide_Test_Cube;

};

