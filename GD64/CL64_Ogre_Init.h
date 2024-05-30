#pragma once
class CL64_Ogre_Init
{
public:

	CL64_Ogre_Init(void);
	~CL64_Ogre_Init(void);

	void InitOgre(void);
	bool OgreCreateRoot(void);

	Ogre::Root* mRoot;

	Ogre::String mResourcePath;
};

