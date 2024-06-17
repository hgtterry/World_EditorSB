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
#include "CL64_Ogre_Init.h"

#include "SdkSample.h"

CL64_Ogre_Init::CL64_Ogre_Init(void)
{
	RenderHwnd = nullptr;

	mRoot =		nullptr;
	mWindow =	nullptr;
	mSceneMgr = nullptr;
	mCamera =	nullptr;
	camNode =	nullptr;

	OgreListener = nullptr;

	manObj = nullptr;
	ModelNode = nullptr;

	OgreEntity = nullptr;
	OgreNode = nullptr;

	mResourcePath = "";
	App_Resource_Group = "App_Resource_Group";
	World_Resource_Group = "World_Resource_Group";

	flag_Hide_Test_Cube = 1;
}

CL64_Ogre_Init::~CL64_Ogre_Init(void)
{
}

// *************************************************************************
// *				InitOgre:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Ogre_Init::InitOgre(void)
{
	OgreCreateRoot();
	SetUpResources();
	Configure();
	chooseSceneManager();
	createCamera();
	createViewports();
	Initialise_Resources();

	createFrameListener();

	manObj = mSceneMgr->createManualObject("sampleArea");
	ModelNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ModelNode->attachObject(manObj);

	// Set Up Grid Functions
	App->CL_Grid->Grid_Update(1);
	App->CL_Grid->Hair_Update(1);

	OgreEntity = mSceneMgr->createEntity("Cube.mesh");
	OgreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	OgreNode->attachObject(OgreEntity);
	Show_Test_Cube();

	mTrayMgr = new OgreBites::TrayManager("InterfaceName", mWindow);

	mTrayMgr->showAll();
	mTrayMgr->showFrameStats(OgreBites::TL_TOPLEFT);
	mTrayMgr->hideCursor();

	App->CL_ImGui->Init_ImGui();

	App->CL_Player->Create_Player_Object();

	//Ogre::Entity* Poo = mSceneMgr->createEntity("C:\\Users\\Terry\\Desktop\\Models\\Obj_Models\\crate.obj");
	
}

// *************************************************************************
// *		OgreCreateRoot:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Ogre_Init::OgreCreateRoot(void)
{
	Ogre::String pluginsPath;
	pluginsPath = mResourcePath + "plugins.cfg";

	if (App->Debug_App == 1)
	{
		mRoot = OGRE_NEW Ogre::Root(pluginsPath, mResourcePath + "Equity_CFG.cfg", mResourcePath + "GD64_Ogre.log");
		//Ogre::LogManager::getSingleton().createLog(mResourcePath + "App.log");
		Ogre::LogManager::getSingleton().setMinLogLevel(Ogre::LogMessageLevel::LML_NORMAL);
	}
	else
	{
		mRoot = OGRE_NEW Ogre::Root(pluginsPath, mResourcePath + "Equity_CFG.cfg", mResourcePath + "");
	}

	return 1;
}

// **************************************************************************
// *			SetUpResources:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
bool CL64_Ogre_Init::SetUpResources(void)
{

	Ogre::String File;
	char Copy[MAX_PATH];

	strcpy(Copy, App->GD_Directory_FullPath);

	ReverseBackSlash(Copy);
	File = Return_Chr;

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(App_Resource_Group);

	////-------------------------------- Zip Files
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/Sinbad.zip", "Zip", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/SdkTrays.zip", "Zip", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/Cube.zip", "Zip", App_Resource_Group);
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/GDCore.zip", "Zip", App_Resource_Group);
	return 1;
}

// *************************************************************************
// *				Configure:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre_Init::Configure(void)
{
	Ogre::RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	if (!(rs->getName() == "OpenGL Rendering Subsystem"))
	{
		return false; //No RenderSystem found
	}

	mRoot->setRenderSystem(rs);

	mWindow = mRoot->initialise(false);
	Ogre::NameValuePairList options;

	options["vsync"] = true;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)RenderHwnd);

	mWindow = mRoot->createRenderWindow("Main RenderWindow", 1024, 768, false, &options);

	//App->CL_Panels->Width = mWindow->getWidth();
	//App->CL_Panels->Height = mWindow->getHeight();

	return true;
}

// *************************************************************************
// *		chooseSceneManager:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre_Init::chooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager"); // Temp
	
	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

	// add a bright light above the scene
	//Light* light = mSceneMgr->createLight();
	//light->setType(Light::LT_SPOTLIGHT);
	//light->setPosition(0, 0, 0);
	//light->setSpecularColour(ColourValue::White);
	return 1;
}

// *************************************************************************
// *			createCamera:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre_Init::createCamera(void)
{
	camNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	mCamera = mSceneMgr->createCamera("MainCamera");
	mCamera->setNearClipDistance(Ogre::Real(0.1));
	mCamera->setFarClipDistance(Ogre::Real(8000));

	camNode->attachObject(mCamera);
	camNode->setPosition(0, 90, 100);
	camNode->lookAt(Ogre::Vector3(0, 30, 0),Ogre::Node::TS_WORLD);

	return 1;
}

// *************************************************************************
// *		createViewports:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Ogre_Init::createViewports(void)
{
	mWindow->removeAllViewports();
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(0);

	vp->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));

	return 1;
}

// *************************************************************************
// *		Initialise_Resources:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre_Init::Initialise_Resources(void)
{
	// Initialize, parse scripts etc
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return 1;
}

// *************************************************************************
// *			ReverseBackSlash:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_Ogre_Init::ReverseBackSlash(char* buf)
{
	strcpy(Return_Chr, "");
	char bufnew[256];
	strcpy(bufnew, "");
	char seps[] = "\\";
	char* token;
	bool tt = 0;

	token = strtok(buf, seps);
	while (token != NULL)
	{
		tt = 1;
		strcat(bufnew, token);
		strcat(bufnew, "/");
		token = strtok(NULL, seps);
	}

	if (tt == 1)
	{
		int len = strlen(bufnew);
		bufnew[len - 1] = 0;
		strcpy(Return_Chr, bufnew);
	}
	else
	{
		tt = 0;
		strcpy(Return_Chr, buf);
	}
	return 1;
}

// *************************************************************************
// *		createFrameListener:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre_Init::createFrameListener(void)
{

	Ogre::String RenderSystemName = mSceneMgr->getDestinationRenderSystem()->getName();

	if (RenderSystemName == "OpenGL Rendering Subsystem")
	{
		OgreListener = new CL64_OgreListener();
		mRoot->addFrameListener(OgreListener);

		RenderListener = new CL64_OGL_Listener();
		mSceneMgr->addRenderQueueListener(RenderListener);

		BulletListener = new CL64_DebugBulletWorld();

		mSceneMgr->addRenderQueueListener(BulletListener);

		BulletListener->setDebugMode(BulletListener->getDebugMode()
			| btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
		App->CL_Bullet->dynamicsWorld->setDebugDrawer(BulletListener);

	}
	else
	{
		App->Say("OpenGL Rendering Subsystem not allocated ");
		return FALSE;
	}

	return TRUE;
}

// *************************************************************************
// *			Show_Test_Cube:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Ogre_Init::Show_Test_Cube(void)
{
	if (flag_Hide_Test_Cube == 1)
	{
		OgreNode->setVisible(true);
	}
	else
	{
		OgreNode->setVisible(false);
	}

}

// *************************************************************************
// *		Ogre_Render_Loop:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre_Init::Ogre_Render_Loop(void)
{
	mRoot->clearEventTimes();
	
	while (true)
	{
		MSG  msg;
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (mWindow->isClosed()) return false;

		//if (FPStimer.getMicroseconds() > Fps_Tick) // FPSLock)
		//{
		//	if (Block_RenderingQueued == 0)
		//	{

		if (!mRoot->_fireFrameStarted())
		{
			return false;
		}

		if (!mRoot->_updateAllRenderTargets())
		{
			return false;
		}

		if (!mRoot->_fireFrameEnded())
		{
			return false;
		}

		//		FPStimer.reset();

		//	}
		//}
	}

	return 1;
}

