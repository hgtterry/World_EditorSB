#include "pch.h"
#include "CL64_App.h"
#include "CL64_Ogre_Init.h"

CL64_Ogre_Init::CL64_Ogre_Init(void)
{
	mRoot = nullptr;

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

	App->Say("Ogre Init");
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
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation("Media/Core_Data/OgreCore.zip", "Zip", App_Resource_Group);

	////-------------------------------- File System
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation(File + "/Media/App_Resources", "FileSystem", App_Resource_Group);
	//Ogre::ResourceGroupManager::getSingleton().addResourceLocation(File + "/Media/New_Particles", "FileSystem", App_Resource_Group);

	return 1;
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
