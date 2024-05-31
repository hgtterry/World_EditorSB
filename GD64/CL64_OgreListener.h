#pragma once

#include "Ogre.h"
//#include "OgreRenderWindow.h"

using namespace Ogre;

class CL64_OgreListener : public FrameListener
{
public:
	CL64_OgreListener(void);
	~CL64_OgreListener(void);

	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);
	bool frameRenderingQueued(const FrameEvent& evt);
};

