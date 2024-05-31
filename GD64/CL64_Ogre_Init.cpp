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

	mRoot = nullptr;
	mWindow = nullptr;


	mResourcePath = "";
	App_Resource_Group = "App_Resource_Group";
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

	App->Say("Ogre Init Done");
}

// *************************************************************************
// *		OgreCreateRoot:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_Ogre_Init::OgreCreateRoot(void)
{
	Ogre::String pluginsPath;
	pluginsPath = mResourcePath + "plugins.cfg";

	//if (App->Debug_App == 1)
	{
		mRoot = OGRE_NEW Ogre::Root(pluginsPath, mResourcePath + "Equity_CFG.cfg", mResourcePath + "EquitySB.log");
		Ogre::LogManager::getSingleton().createLog(mResourcePath + "App.log");
	}
	/*else
	{
		mRoot = OGRE_NEW Ogre::Root(pluginsPath, mResourcePath + "Equity_CFG.cfg", mResourcePath + "");
	}*/

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

	//App->Cl_Panels->Width = mWindow->getWidth();
	//App->Cl_Panels->Height = mWindow->getHeight();

	return true;
}

// *************************************************************************
// *				ReverseBackSlash  Terry Bernie				  	 	   *
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
