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
#include "CL64_OGL_Listener.h"

CL64_OGL_Listener::CL64_OGL_Listener(void)
{
	RX = 0;
	RZ = 0;

	Hair_1PosX = 0;
	Hair_1PosY = 0;
	Hair_1PosZ = 0;

	Hair_1RotX = 0;
	Hair_1RotY = 0;
	Hair_1RotZ = 0;

	ShowFaces = 1;
	Light_Activated = 0;
}

CL64_OGL_Listener::~CL64_OGL_Listener(void)
{
}


// *************************************************************************
// *			renderQueueStarted:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_OGL_Listener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
{
}

// *************************************************************************
// *			renderQueueEnded:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_OGL_Listener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
{
	if (queueGroupId != Ogre::RENDER_QUEUE_MAIN)
	{
		return;
	}

	

	PreRender();


	Render_Loop();


	PostRender();
}

// *************************************************************************
// *				PreRender:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_OGL_Listener::PreRender()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity(); //Texture addressing should start out as direct.

	Ogre::RenderSystem* renderSystem = App->CL_Ogre->manObj->_getManager()->getDestinationRenderSystem();
	Ogre::Node* parentNode = App->CL_Ogre->manObj->getParentNode();

#pragma warning(disable : 4996) // Nightmare why

	renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
	renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
	renderSystem->_setViewMatrix(App->CL_Ogre->mCamera->getViewMatrix());
	renderSystem->_setProjectionMatrix(App->CL_Ogre->mCamera->getProjectionMatrixWithRSDepth());

	static Ogre::Pass* clearPass = NULL;
	if (!clearPass)
	{
		Ogre::MaterialPtr clearMat = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");
		clearPass = clearMat->getTechnique(0)->getPass(0);
	}
	//Set a clear pass to give the renderer a clear renderstate
	App->CL_Ogre->mSceneMgr->_setPass(clearPass, true);

	// save attribs
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

// *************************************************************************
// *				PostRender:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_OGL_Listener::PostRender()
{
	// restore original state
	glPopAttrib();

	// restore matrices
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// *************************************************************************
// *				Render_Loop:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_OGL_Listener::Render_Loop()
{
	
	GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	GLboolean stencilTestEnabled = glIsEnabled(GL_STENCIL_TEST);
	glDisable(GL_STENCIL_TEST);

	if (Light_Activated == 0)
	{
		glDisable(GL_LIGHTING);
	}
	else
	{
		glEnable(GL_LIGHTING);

		GLfloat ambientLightFull[] = { 0.2f, 0.2f, 0.2f, 1.0f };

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,
			ambientLightFull);
		glEnable(GL_LIGHT0);
	}

	glColor3f(0.8f, 0.8f, 0.8f);
	Translate();

	//---------------------- Textured
	//if (App->CL_Scene->Model_Loaded == 1 && ShowTextured == 1)
	{
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*if (App->CL_Scene->Model_Type == Enums::LoadedFile_Actor)
		{
			RenderByTexture();
		}*/

		if (App->CL_Scene->Model_Type == Enums::LoadedFile_Assimp)
		{
			Assimp_Render_Textures();
		}
	}

	// ---------------------- Crosshair
	//if (Show_Crosshair == 1)
	{
		//RenderCrossHair();
	}

	if (depthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (stencilTestEnabled)
	{
		glEnable(GL_STENCIL_TEST);
	}
}

// *************************************************************************
// *				Translate:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_OGL_Listener::Translate(void)
{

	glRotatef(RX, 1.0, 0.0, 0.0); // Rotations of the object 

	glRotatef(RZ, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
}

// *************************************************************************
// *		Assimp_Render_Textures:- Terry and Hazel Flanigan 2024	   	   *
// *************************************************************************
bool CL64_OGL_Listener::Assimp_Render_Textures(void)
{
	int Count = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	int GroupCount = App->CL_Scene->GroupCount;

	Count = 0;
	while (Count < GroupCount)
	{
		Assimp_Textured_Parts(Count);
		Count++;
	}

	glDisable(GL_TEXTURE_2D);

	return 1;
}
// *************************************************************************
// *		Assimp_Textured_Parts:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_OGL_Listener::Assimp_Textured_Parts(int Count)
{
	int VertCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	if (App->CL_Scene->Group[Count]->MaterialIndex > -1)
	{
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);

		glBindTexture(GL_TEXTURE_2D, App->CL_Textures->g_Texture[App->CL_Scene->Group[Count]->MaterialIndex]);

	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	while (VertCount < App->CL_Scene->Group[Count]->GroupFaceCount)
	{
		A = App->CL_Scene->Group[Count]->Face_Data[VertCount].a;
		B = App->CL_Scene->Group[Count]->Face_Data[VertCount].b;
		C = App->CL_Scene->Group[Count]->Face_Data[VertCount].c;

		glBegin(GL_POLYGON);

		//-----------------------------------------------
		glTexCoord2f(App->CL_Scene->Group[Count]->MapCord_Data[A].u, App->CL_Scene->Group[Count]->MapCord_Data[A].v);
		glNormal3fv(&App->CL_Scene->Group[Count]->Normal_Data[A].x);
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[A].x);

		//-----------------------------------------------
		glTexCoord2f(App->CL_Scene->Group[Count]->MapCord_Data[B].u, App->CL_Scene->Group[Count]->MapCord_Data[B].v);
		glNormal3fv(&App->CL_Scene->Group[Count]->Normal_Data[B].x);
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[B].x);

		//-----------------------------------------------
		glTexCoord2f(App->CL_Scene->Group[Count]->MapCord_Data[C].u, App->CL_Scene->Group[Count]->MapCord_Data[C].v);
		glNormal3fv(&App->CL_Scene->Group[Count]->Normal_Data[C].x);
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[C].x);
		VertCount++;
		//-----------------------------------------------

		glEnd();

	}

	return 1;
}

// *************************************************************************
// *		Assimp_Render_Faces:- Terry and Hazel Flanigan 2024	   		   *
// *************************************************************************
bool CL64_OGL_Listener::Assimp_Render_Faces(void)
{
	int Count = 0;

	glColor3f(1, 1, 1);

	int GroupCount = App->CL_Scene->GroupCount;

	while (Count < GroupCount)
	{
		Assimp_Face_Parts(Count);
		Count++;
	}

	return 1;
}

// *************************************************************************
// *		Assimp_Face_Parts:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_OGL_Listener::Assimp_Face_Parts(int Count)
{
	int FaceCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (FaceCount < App->CL_Scene->Group[Count]->GroupFaceCount)
	{
		A = App->CL_Scene->Group[Count]->Face_Data[FaceCount].a;
		B = App->CL_Scene->Group[Count]->Face_Data[FaceCount].b;
		C = App->CL_Scene->Group[Count]->Face_Data[FaceCount].c;

		glBegin(GL_POLYGON);

		//-----------------------------------------------
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[A].x);

		//-----------------------------------------------
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[B].x);

		//-----------------------------------------------
		glVertex3fv(&App->CL_Scene->Group[Count]->vertex_Data[C].x);
		FaceCount++;
		//-----------------------------------------------

		glEnd();
	}

	return 1;
}

// **************************************************************************
// *			RenderCrossHair:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_OGL_Listener::RenderCrossHair(void)
{
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);

	float Length = 40;
	glLineWidth(3);

	glTranslatef(Hair_1PosX, Hair_1PosY, Hair_1PosZ);

	glRotatef(Hair_1RotX, 1.0, 0.0, 0.0); // Rotations of the object 
	glRotatef(Hair_1RotY, 0.0, 1.0, 0.0);
	glRotatef(Hair_1RotZ, 0.0, 0.0, 1.0);

	glScalef(1, 1, 1);

	//-------------------------------------------  x hair] Blue
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f); // Color
	glVertex3f(-Length, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(Length, 0, 0);
	glEnd();

	//-------------------------------------------  z hair Red
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f); // Color
	glVertex3f(0, 0, -Length);
	glVertex3f(0, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, Length);
	glEnd();

	//-------------------------------------------  y hair Green
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f); // Color
	glVertex3f(0, -Length, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, Length, 0);
	glEnd();
}