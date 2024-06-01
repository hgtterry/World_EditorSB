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
#include "CL64_OgreListener.h"

CL64_OgreListener::CL64_OgreListener(void)
{
	StopOgre = 0;
}

CL64_OgreListener::~CL64_OgreListener(void)
{
}

// *************************************************************************
// *			frameStarted:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_OgreListener::frameStarted(const FrameEvent& evt)
{
	//Update_Game_Logic(evt.timeSinceLastFrame);
	return true;
}

// *************************************************************************
// *		frameRenderingQueued:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_OgreListener::frameRenderingQueued(const FrameEvent& evt)
{
	return 1;
}

// *************************************************************************
// *			frameEnded:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
bool CL64_OgreListener::frameEnded(const FrameEvent& evt)
{

	if (StopOgre == 1)
	{
		return false;
	}

	return true;
}
