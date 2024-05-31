#include "pch.h"
#include "CL64_OgreListener.h"

CL64_OgreListener::CL64_OgreListener(void)
{
}

CL64_OgreListener::~CL64_OgreListener(void)
{
}

// *************************************************************************
// *				frameStarted   Terry Bernie							   *
// *************************************************************************
bool CL64_OgreListener::frameStarted(const FrameEvent& evt)
{
	//Update_Game_Logic(evt.timeSinceLastFrame);
	return true;
}

// *************************************************************************
// *			frameRenderingQueued   Terry Bernie						   *
// *************************************************************************
bool CL64_OgreListener::frameRenderingQueued(const FrameEvent& evt)
{
	return 1;
}

// *************************************************************************
// *				frameEnded   Terry Bernie							   *
// *************************************************************************
bool CL64_OgreListener::frameEnded(const FrameEvent& evt)
{

	//if (StopOgre == 1)
	//{
		//return false;
	//}

	return true;
}
