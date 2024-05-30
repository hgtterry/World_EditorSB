#pragma once
class CL64_Ogre_Init
{
public:

	CL64_Ogre_Init(void);
	~CL64_Ogre_Init(void);

	void InitOgre(void);
	bool OgreCreateRoot(void);
	bool SetUpResources(void);

	bool ReverseBackSlash(char* buf);

	Ogre::Root* mRoot;

	Ogre::String mResourcePath;
	Ogre::String App_Resource_Group;		// App Resource Group


	char Return_Chr[255];
};

