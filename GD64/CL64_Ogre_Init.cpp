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
	flag_Hide_Test_Cube = 0;
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

	mTrayMgr = new OgreBites::TrayManager("InterfaceName", mWindow);

	mTrayMgr->showAll();
	mTrayMgr->showFrameStats(OgreBites::TL_TOP);
	mTrayMgr->hideCursor();

	Init_ImGui();

}

// *************************************************************************
// *			Init_ImGui:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Ogre_Init::Init_ImGui(void)
{
	bool mOwnsImGuiOverlay = !Ogre::OverlayManager::getSingleton().getByName("ImGuiOverlay");

	OgreBites::ApplicationContextBase* poo = new OgreBites::ApplicationContextBase();

	Ogre::ImGuiOverlay* imguiOverlay = poo->initialiseImGui();

	if (imguiOverlay)
	{
		Load_Font();
		//imguiOverlay->addFont("SdkTrays/Caption", App->CL_Ogre->App_Resource_Group);
		imguiOverlay->setZOrder(300);
		imguiOverlay->show();

		if (imguiOverlay->isInitialised())
		{
			if (mOwnsImGuiOverlay)
			{
				ImGui::GetStyle().FrameRounding = 8;
				ImVec4* colors = ImGui::GetStyle().Colors;

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
				//colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.90f);
				colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
				////colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
				//colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
				//colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
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

			//mWindow->addListener(this);
			Ogre::ImGuiOverlay::NewFrame();

			//ImGui::PushFont(font1);
			//App->Say("Ok");
		}
		else
		{
			App->Say("Not Ok");
		}
	}
}

// *************************************************************************
// *						Load_Font  Terry Bernie						   *
// *************************************************************************
void CL64_Ogre_Init::Load_Font(void)
{

	ImFont* font0;
	ImFont* font1;
	ImFont* font2;
	ImFont* fontDroid;

	ImGuiIO& io = ImGui::GetIO();
	font1 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 20);
	font2 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 48);
	font0 = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 18);
	fontDroid = io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 12);
	
	//font2 = io.Fonts->AddFontDefault().;

	//io.IniFilename = NULL;
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

	
	////-------------------------------- File System
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation(File + "/Media/App_Resources", "FileSystem", App_Resource_Group);
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation(File + "/Media/New_Particles", "FileSystem", App_Resource_Group);

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

	//	rs->setConfigOption("Full Screen", "No");
	//	rs->setConfigOption("Video Mode", "1024 x 768 @ 32-bit colour");

	mRoot->setRenderSystem(rs);

	mWindow = mRoot->initialise(false);
	Ogre::NameValuePairList options;

	options["vsync"] = true;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)RenderHwnd);

	mWindow = mRoot->createRenderWindow("Main RenderWindow", 1024, 768, false, &options);

	//auto params = rs->getFixedFunctionParams(TVC_NONE, FOG_NONE);
	//params->setConstant(8, Matrix4()); // the "magic" 8 is defined in getFixedFunctionParams
	//rs->applyFixedFunctionParams(params, GPV_GLOBAL);
	// 
	//App->Cl_Panels->Width = mWindow->getWidth();
	//App->Cl_Panels->Height = mWindow->getHeight();

	return true;
}

// *************************************************************************
// *		chooseSceneManager:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Ogre_Init::chooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager"); // Temp
	//ImGui::ImguiManager::getSingleton().init(scnMgr);

	mOverlaySystem = new Ogre::OverlaySystem();
	mSceneMgr->addRenderQueueListener(mOverlaySystem);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	//mSceneMgr->setDisplaySceneNodes(true);

	// add a bright light above the scene
	/*Light* light = mSceneMgr->createLight();
	light->setType(Light::LT_SPOTLIGHT);
	light->setPosition(0, 0, 0);
	light->setSpecularColour(ColourValue::White);

	Ogre::SceneNode* PartNode = App->SBC_Ogre->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	PartNode->attachObject(light);*/

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
	//mCamera->setAutoAspectRatio(true);

	camNode->attachObject(mCamera);
	camNode->setPosition(0, 90, 100);
	camNode->lookAt(Ogre::Vector3(0, 30, 0),Ogre::Node::TS_WORLD);

	//PlacementCam = mSceneMgr->createCamera("PlacementCam");
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
	}
	else
	{
		App->Say("OpenGL Rendering Subsystem not allocated ");
		return FALSE;
	}

	return TRUE;
}

// *************************************************************************
// *			Hide_Test_Cube:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_Ogre_Init::Hide_Test_Cube(void)
{
	if (flag_Hide_Test_Cube == 1)
	{
		OgreNode->setVisible(false);
	}
	else
	{
		OgreNode->setVisible(true);
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

		/*App->Flash_Window();*/
		//		FPStimer.reset();

		//	}
		//}
	}

	return 1;
}
