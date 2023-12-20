/*
Copyright (c) 2023 World Editor -- HGT Software W.T.Flanigan H.C.Flanigan

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

#include "stdafx.h"
#include "AB_App.h"
#include "SB_BR_Render.h"

SB_BR_Render::SB_BR_Render()
{
	MeshView_3D_hWnd = nullptr;

	// ------------------------------------------------ 
	GridManual = nullptr;
	GridNode = nullptr;

	Scale_X = 4;
	Scale_Y = 4;
	Scale_Z = 4;

	Division_X = 2;
	Division_Y = 2;
	Division_Z = 2;

	YAxis_min = -8;
	YAxis_max = 8;

	ZAxis_min = -8;
	ZAxis_max = 8;

	XAxis_min = -8;
	XAxis_max = 8;

	HairExtend = 8;

	ColourHairZ = ColourValue(1, 0, 0, 1);
	ColourHairX = ColourValue(0, 0, 1, 1);
	ColourHairY = ColourValue(0, 1, 0, 1);
	ColourMain = ColourValue(0.7, 0.7, 0, 0.6);
	ColourDivision = ColourValue(1, 1, 1, 0.4);

	ShowDivisions = 1;

	// ------------------------------------------------ 
	MeshView_Window = NULL;
	mSceneMgrMeshView = NULL;
	mCameraMeshView = NULL;
	CamNode = NULL;
}

SB_BR_Render::~SB_BR_Render()
{
}

// *************************************************************************
// *			Set_Render_Window:- Terry and Hazel Flanigan 2023          *
// *************************************************************************
bool SB_BR_Render::Set_Render_Window(void)
{

	Ogre::NameValuePairList options;

	options["externalWindowHandle"] =
		Ogre::StringConverter::toString((size_t)MeshView_3D_hWnd);

	MeshView_Window = App->CLSB_Ogre->mRoot->createRenderWindow("MeshViewWin", 1024, 768, false, &options);

	mSceneMgrMeshView = App->CLSB_Ogre->mRoot->createSceneManager("DefaultSceneManager", "MeshViewGD");

	mCameraMeshView = mSceneMgrMeshView->createCamera("CameraMV");
	mCameraMeshView->setPosition(Ogre::Vector3(0, 0, 0));
	mCameraMeshView->setNearClipDistance(0.1);
	mCameraMeshView->setFarClipDistance(8000);

	Ogre::Viewport* vp = MeshView_Window->addViewport(mCameraMeshView);
	mCameraMeshView->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	vp->setBackgroundColour(ColourValue(0.5, 0.5, 0.5));

	CamNode = mSceneMgrMeshView->getRootSceneNode()->createChildSceneNode("Camera_Node");
	CamNode->attachObject(mCameraMeshView);

	////-------------------------------------------- 

	/*MvEnt = mSceneMgrMeshView->createEntity("MVTest2", Selected_MeshFile, MV_Resource_Group);
	MvNode = mSceneMgrMeshView->getRootSceneNode()->createChildSceneNode();
	MvNode->attachObject(MvEnt);
	MvNode->setVisible(true);*/

	mSceneMgrMeshView->setAmbientLight(ColourValue(0.7, 0.7, 0.7));

	// add a bright light above the scene
	Light* light = mSceneMgrMeshView->createLight();
	light->setType(Light::LT_POINT);
	light->setPosition(-10, 40, 20);
	light->setSpecularColour(ColourValue::White);

	/*Ogre::Vector3 Centre = MvEnt->getBoundingBox().getCenter();
	Ogre::Real Radius = MvEnt->getBoundingRadius();*/

	Grid_Update(1);

	//RenderListener = new SB_MeshView_Listener();

	//App->CL_Ogre->mRoot->addFrameListener(RenderListener);


	mCameraMeshView->setPosition(Ogre::Vector3(0, 90, 100));
	mCameraMeshView->lookAt(Ogre::Vector3(0, 30, 0));

	// Debug Physics Shape
	/*btDebug_Manual = mSceneMgrMeshView->createManualObject("MVManual");
	btDebug_Manual->setRenderQueueGroup(RENDER_QUEUE_MAX);

	btDebug_Manual->setDynamic(true);
	btDebug_Manual->estimateVertexCount(2000);

	btDebug_Manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
	btDebug_Manual->position(0, 0, 0);
	btDebug_Manual->colour(1, 1, 1, 1);
	btDebug_Manual->end();

	btDebug_Node = mSceneMgrMeshView->getRootSceneNode()->createChildSceneNode();
	btDebug_Node->attachObject(btDebug_Manual);*/

	return 1;
}

// *************************************************************************
// *	  		Grid_Update:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void SB_BR_Render::Grid_Update(bool Create)
{
	int i = 0;
	Real r;

	if (Create == 1)
	{
		GridManual = mSceneMgrMeshView->createManualObject("GridManual");
		GridManual->setRenderQueueGroup(1);
	}

	GridManual->clear();
	GridManual->begin("BaseWhiteAlphaBlended", RenderOperation::OT_LINE_LIST);

	//if (ShowGridFlag == 1)
	{
		for (int x = XAxis_min; x <= XAxis_max; ++x)
		{
			GridManual->position(x, 0, ZAxis_min);
			GridManual->colour(ColourMain);
			GridManual->position(x, 0, ZAxis_max);
			GridManual->colour(ColourMain);
			if (x < XAxis_max && ShowDivisions == 1)
			{
				for (int d = 0; d < Division_X; ++d)
				{
					r = x + (1.0 / Real(Division_X)) * Real(d);
					GridManual->position(r, 0, ZAxis_min);
					GridManual->colour(ColourDivision);
					GridManual->position(r, 0, ZAxis_max);
					GridManual->colour(ColourDivision);
				}
			}
		}

		for (int z = ZAxis_min; z <= ZAxis_max; ++z)
		{
			GridManual->position(Ogre::Vector3(XAxis_min, 0, z));
			GridManual->colour(ColourMain);
			GridManual->position(Ogre::Vector3(XAxis_max, 0, z));
			GridManual->colour(ColourMain);
			if (z < ZAxis_max && ShowDivisions == 1)
			{
				for (int d = 1; d < Division_Y; ++d)
				{
					r = z + (1.0 / Real(Division_Y)) * Real(d);
					GridManual->position(XAxis_min, 0, r);
					GridManual->colour(ColourDivision);
					GridManual->position(XAxis_max, 0, r);
					GridManual->colour(ColourDivision);
				}
			}
		}
	}

	GridManual->end();

	if (Create == 1)
	{
		GridNode = mSceneMgrMeshView->getRootSceneNode()->createChildSceneNode();
		GridNode->attachObject(GridManual);
	}

	GridNode->setPosition(0, 0, 0);
	GridNode->setVisible(true);
	GridNode->setScale(Scale_X, Scale_Y, Scale_Z);
}
