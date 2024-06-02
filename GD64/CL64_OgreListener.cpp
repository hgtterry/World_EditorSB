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
#include "CL64_OgreListener.h"

#include "Overlay/OgreOverlay.h"
#include "Overlay/OgreOverlaySystem.h"
#include "Overlay/OgreOverlayManager.h"
#include "Overlay/OgreBorderPanelOverlayElement.h"
#include "Overlay/OgreTextAreaOverlayElement.h"
#include "Overlay/OgreFontManager.h"

CL64_OgreListener::CL64_OgreListener(void)
{
	/*mDebugOverlay = OverlayManager::getSingleton().create("POO");

	Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(OverlayManager.createOverlayElement("Panel", "PanelName"));

	mDebugOverlay->add2D("POO");*/

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
	App->CL_Ogre->mTrayMgr->frameRendered(evt);
	//UpDateStats();
	//App->Flash_Window();
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
//#include "OgreOverlayContainer.h"
//#include "OgreOverlaySystem.h"
// *************************************************************************
// *					UpDateStats   Terry Bernie						   *
// *************************************************************************
void CL64_OgreListener::UpDateStats(void)
{
	//mDebugOverlay->show();
	static String currFps = "Current FPS: ";
	static String avgFps = "Average FPS: ";
	
	// update stats when necessary
	try {
		
		OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps2");
		//	OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		//	OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const RenderTarget::FrameStats& stats = App->CL_Ogre->mWindow->getStatistics();
		//guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		//guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
		//	+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
		//guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
		//	+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		/*OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));*/

		//OverlayElement* guiDistance = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		//guiBatches->setCaption(distance + Pl_Entity_Name);

		/*OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setCaption(mDebugText);*/

		//guiCurr->setVisible(true);
	}
	catch (...) { /* ignore */ }
}
