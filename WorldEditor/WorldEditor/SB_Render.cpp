/*
Copyright (c) 2022 Equity_ME Model Editor -- Inflanite Software W.T.Flanigan H.C.Flanigan

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
#include "SB_Render.h"


SB_Render::SB_Render()
{
	RX = 0;
	RZ = 0;

	Hair_1PosX = 0;
	Hair_1PosY = 0;
	Hair_1PosZ = 0;

	Hair_1RotX = 0;
	Hair_1RotY = 0;
	Hair_1RotZ = 0;

	ShowTextured = 0;
	ShowFaces = 0;
	ShowBrushes = 0;
	ShowBones = 0;
	ShowPoints = 0;
	ShowNormals = 0;
	ShowBoundingBox = 0;
	ShowBoundingGroup = 0;
	Show_Selected_Face = 0;
	
	PlayActive = 0;
	Light_Activated = 0;
	ShowOnlySubMesh = 0;
	Show_HideGroup = 0;
	Show_Crosshair = 0;

	JustTexture_ID = 0;
	TextureID = 0;

	// Brushes
	Render_Just_Brush = 0;
	Selected_Brush_Index = 0;
	Selected_Group_Index = 0;

	Selected_Group = 0;
	GroupNumber = 0;
	//TestLight();
}


SB_Render::~SB_Render()
{
}

// *************************************************************************
// *						Reset_Class Terry Bernie					   *
// *************************************************************************
void SB_Render::Reset_Class(void)
{
	RX = 0;
	RZ = 0;

	Hair_1PosX = 0;
	Hair_1PosY = 0;
	Hair_1PosZ = 0;

	Hair_1RotX = 0;
	Hair_1RotY = 0;
	Hair_1RotZ = 0;

	Show_Marker_Face = 0;
	ShowFaces = 1;
	ShowTextured = 0;
	ShowFaces = 0;
	ShowBones = 0;
	ShowPoints = 0;
	ShowNormals = 0;
	ShowBoundingBox = 0;
	PlayActive = 0;
	Light_Activated = 0;
	ShowOnlySubMesh = 0;
	Show_HideGroup = 0;
	Show_Crosshair = 0;
	Show_Brush_Faces = 0;
	Render_Brush_Group_Flag = 0;
}

// *************************************************************************
// *						TestLight Terry Bernie						   *
// *************************************************************************
void SB_Render::TestLight(void)
{
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	float ld[4] = { (float) 0.4,(float) 0.4, (float)0.4,(float)0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ld);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ld);

	Debug
	/*GLfloat a[4] = { 1, 1, 1, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, a);
	GLfloat d[4] = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, d);
	glMaterialf(GL_FRONT, GL_SHININESS, 128);*/

	//glEnable(GL_LIGHTING);
	//Light_Activated = 1;
}
// *************************************************************************
// *					renderQueueStarted Terry Bernie					   *
// *************************************************************************
void SB_Render::renderQueueStarted(Ogre::uint8 queueGroupId, const String& invocation, bool& skipThisInvocation)
{
}

// *************************************************************************
// *					renderQueueEnded Terry Bernie					   *
// *************************************************************************
void SB_Render::renderQueueEnded(Ogre::uint8 queueGroupId, const String& invocation, bool& repeatThisInvocation)
{
	if (queueGroupId != RENDER_QUEUE_MAIN)
	{
		return;
	}

	//if (PlayActive == 1)
	//{
	//	if (App->CL_Model->Model_Type == Enums::LoadedFile_Actor)
	//	{
	//		App->CL_Genesis3D->GetBoneMoveMent();	// Update Bones 
	//		App->CL_Genesis3D->Animate(1);

	//		/*if (App->Cl_Ogre->RenderListener->Show_Crosshair == 1)
	//		{
	//			App->Cl_Bones->Move_BoneCrosshair();
	//		}*/
	//	}
	//}

	PreRender();
	Render_Loop();
	PostRender();
}
// *************************************************************************
// *				PreRender   Terry Bernie							   *
// *************************************************************************
void SB_Render::PreRender()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity(); //Texture addressing should start out as direct.

	RenderSystem* renderSystem = App->CLSB_Ogre_Setup->manObj->_getManager()->getDestinationRenderSystem();
	Node* parentNode = App->CLSB_Ogre_Setup->manObj->getParentNode();
	renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
	renderSystem->_setViewMatrix(App->CLSB_Ogre_Setup->mCamera->getViewMatrix());
	renderSystem->_setProjectionMatrix(App->CLSB_Ogre_Setup->mCamera->getProjectionMatrixRS());

	static Pass* clearPass = NULL;
	if (!clearPass)
	{
		MaterialPtr clearMat = MaterialManager::getSingleton().getByName("BaseWhite");
		clearPass = clearMat->getTechnique(0)->getPass(0);
	}
	//Set a clear pass to give the renderer a clear renderstate
	App->CLSB_Ogre_Setup->mSceneMgr->_setPass(clearPass, true, false);

	// save attribs
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}
// *************************************************************************
// *				PostRender   Terry Bernie							   *
// *************************************************************************
void SB_Render::PostRender()
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
// *					Render_Loop   Terry Bernie						   *
// *************************************************************************
void SB_Render::Render_Loop()
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

	//if (App->CLSB_ImGui->ImGui_Surface_Active == 0)
	{
		//---------------------- Textured
		if (App->CLSB_Model->Model_Loaded == 1 && ShowTextured == 1)
		{
			glEnable(GL_DEPTH_TEST);
			glShadeModel(GL_SMOOTH);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


			if (App->CLSB_Model->Render_Type == Enums::Render_Groups)
			{
				Assimp_Render_Textures();
			}

			if (App->CLSB_Model->Render_Type == Enums::Render_Brushes)
			{

				//Brush_Render_Group_Texture();
				//WE_Render_By_Texture();

				if (Render_Brush_Group_Flag == 1)
				{
					Brush_Render_Group_Texture();
				}
				else
				{
					Brush_Render_Brush_Texture();
				}


				//XBrush_Render_Textures();
			}

			// Equity View
			if (App->CLSB_Model->Render_Type == Enums::Render_Assimp)
			{
				/*if (App->CLSB_Assimp->Assimp_Model_Loaded == 1)
				{
					New_Assimp_Render_Textures();
				}*/
			}

		}

		// ---------------------- Mesh
		if (App->CLSB_Model->Model_Loaded == 1 && ShowFaces == 1)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			if (App->CLSB_Model->Render_Type == Enums::Render_Assimp)
			{
				/*if (App->CLSB_Assimp->Assimp_Model_Loaded == 1)
				{
					Assimp_Render_Faces();
				}*/
			}

			//if (App->CLSB_Model->Render_Type == Enums::LoadedFile_Brushes)
			//{
			//	//Brushes_Render_Faces();
			//	//XBrushes_Render_Faces();
			//	Brushes_Render_Faces_XX();
			//}
		}


		// ---------------------- Points
		if (App->CLSB_Model->Model_Loaded == 1 && ShowPoints == 1)
		{
			/*if (App->CLSB_Model->Render_Type == Enums::LoadedFile_Actor)
			{
				RF_Render_Points();
			}

			if (App->CLSB_Model->Render_Type == Enums::LoadedFile_Assimp)
			{*/
			Assimp_Render_Points();
			/*}

			if (App->CLSB_Model->Render_Type == Enums::LoadedFile_Brushes)
			{
				Brush_Render_Points();
			}*/

		}

		// ---------------------- Normals
		if (App->CLSB_Model->Model_Loaded == 1 && ShowNormals == 1)
		{
			if (App->CLSB_Model->Render_Type == Enums::LoadedFile_Actor)
			{
				RF_Render_Normals();
			}

			if (App->CLSB_Model->Render_Type == Enums::LoadedFile_Assimp)
			{
				Assimp_Render_Normals();
			}

			if (App->CLSB_Model->Render_Type == Enums::LoadedFile_Brushes)
			{
				//Brush_Render_Normals();
			}
		}

		// ---------------------- Bounding Box
		if (App->CLSB_Model->Model_Loaded == 1 && ShowBoundingBox == 1)
		{
			if (App->CLSB_Model->Render_Type == Enums::Render_Assimp)
			{
				/*if (App->CLSB_Assimp->Assimp_Model_Loaded == 1)
				{
					Render_BoundingBoxModel();
				}*/
			}

			//if (App->CL_Vm_Model->Model_Type == LoadedFile_Obj)
			{
				//Render_BoundingBoxModel();
				//Render_BB_Selected_Brush();
			}
			/*if (App->CL_Vm_Model->Model_Type == LoadedFile_Actor)
			{
			RenderPoints();
			}*/
		}

		// ---------------------- Bounding Box Group
		if (App->CLSB_Model->Model_Loaded == 1 && ShowBoundingGroup == 1)
		{
			Render_BoundingBoxGroup();
		}

		// ---------------------- Bones
		if (App->CLSB_Model->Model_Loaded == 1 && ShowBones == 1)
		{
			As_RenderBones();
		}

		// ---------------------- Crosshair
		if (Show_Crosshair == 1)
		{
			RenderCrossHair();
		}

		// ---------------------- Face Marker
		if (Show_Marker_Face == 1)
		{
			Marker_Face_Selection();
		}

		// ---------------------- Render_Selected_Face
		if (Show_Selected_Face == 1)
		{
			Render_Selected_Face();
		}

		// ---------------------- Show_Brush_Faces
		if (Show_Brush_Faces == 1)
		{
			//Render_Brush_Faces();
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
}

// *************************************************************************
// *					Translate Terry Bernie							   *
// *************************************************************************
void SB_Render::Translate(void)
{

	glRotatef(RX, 1.0, 0.0, 0.0); // Rotations of the object 

	glRotatef(RZ, 0.0, 1.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
}


// *************************************************************************
// *						Brush_Render_Textures_Terry Bernie	   		   *
// *************************************************************************
bool SB_Render::Brush_Render_Brush_Texture(void)
{
	int Count = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	if (Render_Just_Brush == 0)
	{
		int BrushCount = App->CLSB_Model->BrushCount;
		Count = 0;

		while (Count < BrushCount)
		{
			Brush_Textured_Parts_XX(Count);
			Count++;
		}
	}
	else
	{
		Brush_Textured_Parts_XX(Selected_Brush_Index);
	}

	glDisable(GL_TEXTURE_2D);

	return 1;
}

// *************************************************************************
// *				Brush_Render_Group_Texture Terry Bernie	   			   *
// *************************************************************************
bool SB_Render::Brush_Render_Group_Texture(void)
{
	int Count = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	int BrushCount = App->CLSB_Model->BrushCount;
	Count = 0;

	while (Count < BrushCount)
	{
		if (App->CLSB_Model->B_Brush[Count]->Group_Index == Selected_Group_Index)
		{
			Brush_Textured_Parts_XX(Count);
		}

		Count++;
	}
	
	glDisable(GL_TEXTURE_2D);

	return 1;
}

// *************************************************************************
// *				Brush_Textured_Parts_XX Terry Bernie		 		   *
// *************************************************************************
bool SB_Render::Brush_Textured_Parts_XX(int Count)
{
	int FaceCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	int OldId = 0;

	if (App->CLSB_Model->B_Brush[Count]->Face_Count > 0)
	{
		OldId = App->CLSB_Model->B_Brush[Count]->Picking_Data[0].TextID;
		glBindTexture(GL_TEXTURE_2D, g_BrushTexture[App->CLSB_Model->B_Brush[Count]->Picking_Data[0].TextID]);
	}

	while (FaceCount < App->CLSB_Model->B_Brush[Count]->Face_Count)
	{
		if (OldId > App->CLSB_Model->B_Brush[Count]->Picking_Data[FaceCount].TextID || OldId < App->CLSB_Model->B_Brush[Count]->Picking_Data[FaceCount].TextID)
		{
			glBindTexture(GL_TEXTURE_2D, g_BrushTexture[App->CLSB_Model->B_Brush[Count]->Picking_Data[FaceCount].TextID]);
			OldId = App->CLSB_Model->B_Brush[Count]->Picking_Data[FaceCount].TextID;
		}

		A = App->CLSB_Model->B_Brush[Count]->Face_Data[FaceCount].a;
		B = App->CLSB_Model->B_Brush[Count]->Face_Data[FaceCount].b;
		C = App->CLSB_Model->B_Brush[Count]->Face_Data[FaceCount].c;

		glBegin(GL_POLYGON);

		//-----------------------------------------------
		glTexCoord2f(App->CLSB_Model->B_Brush[Count]->MapCord_Data[A].u, App->CLSB_Model->B_Brush[Count]->MapCord_Data[A].v);
		glNormal3fv(&App->CLSB_Model->B_Brush[Count]->Normal_Data[A].x);
		glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[A].x);

		//-----------------------------------------------
		glTexCoord2f(App->CLSB_Model->B_Brush[Count]->MapCord_Data[B].u, App->CLSB_Model->B_Brush[Count]->MapCord_Data[B].v);
		glNormal3fv(&App->CLSB_Model->B_Brush[Count]->Normal_Data[B].x);
		glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[B].x);

		//-----------------------------------------------
		glTexCoord2f(App->CLSB_Model->B_Brush[Count]->MapCord_Data[C].u, App->CLSB_Model->B_Brush[Count]->MapCord_Data[C].v);
		glNormal3fv(&App->CLSB_Model->B_Brush[Count]->Normal_Data[C].x);
		glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[C].x);
		FaceCount++;
		//-----------------------------------------------

		glEnd();

	}

	return 1;
}


// *************************************************************************
// *		WE_Render_By_Texture:- Terry and Hazel Flanigan 2023	 	   *
// *************************************************************************
bool SB_Render::WE_Render_By_Texture()
{
	int Count = 0;
	int FaceCount = 0;

	int A = 0;
	int B = 0;
	int C = 0;

	int OldId = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	int BrushCount = App->CLSB_Model->BrushCount;
	Count = 0;

	glBindTexture(GL_TEXTURE_2D, g_BrushTexture[TextureID]);

	while (Count < BrushCount)
	{

		FaceCount = 0;
		while (FaceCount < App->CLSB_Model->B_Brush[Count]->Face_Count)
		{
			if (App->CLSB_Model->B_Brush[Count]->Picking_Data[FaceCount].TextID == TextureID)
			{
				
				A = App->CLSB_Model->B_Brush[Count]->Face_Data[FaceCount].a;
				B = App->CLSB_Model->B_Brush[Count]->Face_Data[FaceCount].b;
				C = App->CLSB_Model->B_Brush[Count]->Face_Data[FaceCount].c;

				glBegin(GL_POLYGON);

				//-----------------------------------------------
				glTexCoord2f(App->CLSB_Model->B_Brush[Count]->MapCord_Data[A].u, App->CLSB_Model->B_Brush[Count]->MapCord_Data[A].v);
				glNormal3fv(&App->CLSB_Model->B_Brush[Count]->Normal_Data[A].x);
				glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[A].x);

				//-----------------------------------------------
				glTexCoord2f(App->CLSB_Model->B_Brush[Count]->MapCord_Data[B].u, App->CLSB_Model->B_Brush[Count]->MapCord_Data[B].v);
				glNormal3fv(&App->CLSB_Model->B_Brush[Count]->Normal_Data[B].x);
				glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[B].x);

				//-----------------------------------------------
				glTexCoord2f(App->CLSB_Model->B_Brush[Count]->MapCord_Data[C].u, App->CLSB_Model->B_Brush[Count]->MapCord_Data[C].v);
				glNormal3fv(&App->CLSB_Model->B_Brush[Count]->Normal_Data[C].x);
				glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[C].x);
				//-----------------------------------------------

				glEnd();
			}

			FaceCount++;
		}

		Count++;
	}

	glDisable(GL_TEXTURE_2D);

	return 1;
}

// *************************************************************************
// *						Bruses_Render_Faces_XX Terry Bernie	   		   *
// *************************************************************************
bool SB_Render::Brushes_Render_Faces_XX(void)
{
	int Count = 0;

	int BrushCount = App->CLSB_Model->BrushCount;

	while (Count < BrushCount)
	{
		if (Render_Just_Brush == 0)
		{
			glColor3f(1, 1, 1);
			glLineWidth(1);

			Brushes_Face_Parts_XX(Count);
		}
		else
		{
			if (Count == Selected_Brush_Index)
			{
				glColor3f(1, 0, 0);
				glLineWidth(3);

				Brushes_Face_Parts_XX(Count);
			}
			else
			{
				glColor3f(1, 1, 1);
				glLineWidth(1);

				Brushes_Face_Parts_XX(Count);
			}
		}

		Count++;
	}

	return 1;
}
// *************************************************************************
// *					Bruses_Face_Parts_xx Terry Bernie		   			   *
// *************************************************************************
bool SB_Render::Brushes_Face_Parts_XX(int Count)
{
	int FaceCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	while (FaceCount < App->CLSB_Model->B_Brush[Count]->Face_Count)
	{
		A = App->CLSB_Model->B_Brush[Count]->Face_Data[FaceCount].a;
		B = App->CLSB_Model->B_Brush[Count]->Face_Data[FaceCount].b;
		C = App->CLSB_Model->B_Brush[Count]->Face_Data[FaceCount].c;

		glBegin(GL_POLYGON);

		//-----------------------------------------------
		glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[A].x);

		//-----------------------------------------------
		glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[B].x);

		//-----------------------------------------------
		glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[C].x);

		FaceCount++;

		glEnd();
	}

	return 1;
}

// *************************************************************************
// *						XBruses_Render_Faces Terry Bernie	   		   *
// *************************************************************************
bool SB_Render::XBrushes_Render_Faces(void)
{
	int Count = 0;

	glColor3f(1, 1, 1);

	int BrushCount = App->CLSB_Model->XBrushCount;

	while (Count < BrushCount)
	{
		XBrushes_Face_Parts(Count);
		Count++;
	}

	return 1;
}
// *************************************************************************
// *					XBruses_Face_Parts Terry Bernie		   			   *
// *************************************************************************
bool SB_Render::XBrushes_Face_Parts(int Count)
{
	int FaceCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	int BrushLoop = 0;
	int SubBrushCount = App->CLSB_Model->B_XBrush[Count]->Brush_Count;

	while (BrushLoop < SubBrushCount)
	{
		FaceCount = 0;

		while (FaceCount < App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Face_Count)
		{
			A = App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Face_Data[FaceCount].a;
			B = App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Face_Data[FaceCount].b;
			C = App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Face_Data[FaceCount].c;

			glBegin(GL_POLYGON);

			//-----------------------------------------------
			glVertex3fv(&App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[A].x);

			//-----------------------------------------------
			glVertex3fv(&App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[B].x);

			//-----------------------------------------------
			glVertex3fv(&App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[C].x);

			FaceCount++;
	
			glEnd();
		}

		BrushLoop++;
	}


	return 1;
}

// *************************************************************************
// *						XBrush_Render_Textures_Terry Bernie	   		   *
// *************************************************************************
bool SB_Render::XBrush_Render_Textures(void)
{
	int Count = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	int BrushCount = App->CLSB_Model->XBrushCount;

	Count = 0;
	while (Count < BrushCount)
	{
		XBrush_Textured_Parts(Count);
		Count++;
	}

	glDisable(GL_TEXTURE_2D);

	return 1;
}
// *************************************************************************
// *					XBrush_Textured_Parts Terry Bernie		 		   *
// *************************************************************************
bool SB_Render::XBrush_Textured_Parts(int Count)
{
	int FaceCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	int OldId = 0;

	int BrushLoop = 0;
	int SubBrushCount = App->CLSB_Model->B_XBrush[Count]->Brush_Count;
	
	while (BrushLoop < SubBrushCount)
	{
		OldId = 0;
		FaceCount = 0;
		A = 0;
		B = 0;
		C = 0;

		if (App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Face_Count > 0)
		{
			OldId = App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->TextID_Data[0].ID;
			glBindTexture(GL_TEXTURE_2D, g_BrushTexture[App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->TextID_Data[0].ID]);
		}

		while (FaceCount < App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Face_Count)
		{
			if (OldId > App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->TextID_Data[FaceCount].ID || OldId < App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->TextID_Data[FaceCount].ID)
			{
				glBindTexture(GL_TEXTURE_2D, g_BrushTexture[App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->TextID_Data[FaceCount].ID]);
				OldId = App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->TextID_Data[FaceCount].ID;
			}

			//if (App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->TextID_Data[FaceCount].ID == JustTexture_ID)
			{
				A = App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Face_Data[FaceCount].a;
				B = App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Face_Data[FaceCount].b;
				C = App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Face_Data[FaceCount].c;

				glBegin(GL_POLYGON);

				//-----------------------------------------------
				glTexCoord2f(App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->MapCord_Data[A].u, App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->MapCord_Data[A].v);
				glNormal3fv(&App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Normal_Data[A].x);
				glVertex3fv(&App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[A].x);

				//-----------------------------------------------
				glTexCoord2f(App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->MapCord_Data[B].u, App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->MapCord_Data[B].v);
				glNormal3fv(&App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Normal_Data[B].x);
				glVertex3fv(&App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[B].x);

				//-----------------------------------------------
				glTexCoord2f(App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->MapCord_Data[C].u, App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->MapCord_Data[C].v);
				glNormal3fv(&App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->Normal_Data[C].x);
				glVertex3fv(&App->CLSB_Model->B_XBrush[Count]->B_Brush[BrushLoop]->vertex_Data[C].x);
			}

			FaceCount++;

			glEnd();

		}

		BrushLoop++;
	}

	return 1;
}

// *************************************************************************
// *						Assimp_Render_Faces Terry Bernie	   		   *
// *************************************************************************
bool SB_Render::Assimp_Render_Faces(void)
{
	int Count = 0;

	glColor3f(1, 1, 1);

	/*int GroupCount = App->CLSB_Assimp->Total_Assimp_GroupCount;

	while (Count<GroupCount)
	{
		Assimp_Face_Parts(Count);
		Count++;
	}*/

	return 1;
}
// *************************************************************************
// *					Assimp_Face_Parts Terry Bernie		   			   *
// *************************************************************************
bool SB_Render::Assimp_Face_Parts(int Count)
{
	int FaceCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//while (FaceCount<App->CLSB_Assimp->Assimp_Group[Count]->GroupFaceCount)
	//{
	//	A = App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[FaceCount].a;
	//	B = App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[FaceCount].b;
	//	C = App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[FaceCount].c;

	//	glBegin(GL_POLYGON);

	//	//-----------------------------------------------
	//	glVertex3fv(&App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[A].x);

	//	//-----------------------------------------------
	//	glVertex3fv(&App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[B].x);

	//	//-----------------------------------------------
	//	glVertex3fv(&App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[C].x);
	//	FaceCount++;
	//	//-----------------------------------------------

	//	glEnd();
	//}

	return 1;
}

// *************************************************************************
// *						Assimp_Render_Points Terry Bernie	   		   *
// *************************************************************************
bool SB_Render::Assimp_Render_Points(void)
{
	int Count = 0;

	glColor3f(1.0f, 1.0f, 0.0f);

	int GroupCount = App->CLSB_Model->Get_Groupt_Count();

	while (Count < GroupCount)
	{
		Render_As_Points_Parts(Count);
		Count++;
	}

	return 1;
}

// *************************************************************************
// *					Render_As_Points_Parts Terry Bernie	   			   *
// *************************************************************************
bool SB_Render::Render_As_Points_Parts(int Count)
{
	glPointSize(5);

	int VertCount = 0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int GroupVertCount = App->CLSB_Model->Group[Count]->GroupVertCount;

	while (VertCount < GroupVertCount)
	{
		glBegin(GL_POINTS);

		glVertex3fv(&App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x);

		glEnd();

		VertCount++;
	}

	return 1;
}

// *************************************************************************
// *						Brush_Render_Points Terry Bernie	   		   *
// *************************************************************************
bool SB_Render::Brush_Render_Points(void)
{
	/*int Count = 0;

	glColor3f(1.0f, 1.0f, 0.0f);

	int BrushCount = App->CLSB_Model->BrushCount;

	while (Count < BrushCount)
	{
		Brush_As_Points_Parts(Count);
		Count++;
	}*/

	return 1;
}
// *************************************************************************
// *					Brush_As_Points_Parts Terry Bernie	   			   *
// *************************************************************************
bool SB_Render::Brush_As_Points_Parts(int Count)
{
	//glPointSize(5);

	//int VertCount = 0;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	////int GroupVertCount = App->CLSB_Model->B_Brush[Count]->Face_Count;

	//while (VertCount < GroupVertCount)
	//{
	//	glBegin(GL_POINTS);

	//	//glVertex3fv(&App->CLSB_Model->B_Brush[Count]->vertex_Data[VertCount].x);

	//	glEnd();

	//	VertCount++;
	//}

	return 1;
}

// *************************************************************************
// *						Assimp_Render_Textures_Terry Bernie	   		   *
// *************************************************************************
bool SB_Render::Assimp_Render_Textures(void)
{
	int Count = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	int GroupCount = App->CLSB_Model->Get_Groupt_Count();

	if (ShowOnlySubMesh == 1) // Show Only Selected SubMesh
	{
		Assimp_Textured_Parts(Selected_Group);
		glDisable(GL_TEXTURE_2D);
		return 1;
	}

	Count = 0;
	if (Show_HideGroup == 1) // Hide Selected SubMesh
	{
		while (Count < GroupCount)
		{
			if (Selected_Group == Count)
			{
			}
			else
			{
				Assimp_Textured_Parts(Count);
			}
			Count++;
		}

		glDisable(GL_TEXTURE_2D);
		return 1;
	}

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
// *					Assimp_Textured_Parts Terry Bernie		 		   *
// *************************************************************************
bool SB_Render::Assimp_Textured_Parts(int Count)
{
	int VertCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	if (App->CLSB_Model->Group[Count]->MaterialIndex > -1)
	{
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);

		//glBindTexture(GL_TEXTURE_2D, g_Texture[App->CLSB_Model->Group[Count]->MaterialIndex]);
		glBindTexture(GL_TEXTURE_2D, g_BrushTexture[App->CLSB_Model->Group[Count]->MaterialIndex]);
		
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	while (VertCount<App->CLSB_Model->Group[Count]->GroupFaceCount)
	{
		A = App->CLSB_Model->Group[Count]->Face_Data[VertCount].a;
		B = App->CLSB_Model->Group[Count]->Face_Data[VertCount].b;
		C = App->CLSB_Model->Group[Count]->Face_Data[VertCount].c;

		glBegin(GL_POLYGON);

		//-----------------------------------------------
		glTexCoord2f(App->CLSB_Model->Group[Count]->MapCord_Data[A].u, App->CLSB_Model->Group[Count]->MapCord_Data[A].v);
		glNormal3fv(&App->CLSB_Model->Group[Count]->Normal_Data[A].x);
		glVertex3fv(&App->CLSB_Model->Group[Count]->vertex_Data[A].x);
		
		//-----------------------------------------------
		glTexCoord2f(App->CLSB_Model->Group[Count]->MapCord_Data[B].u, App->CLSB_Model->Group[Count]->MapCord_Data[B].v);
		glNormal3fv(&App->CLSB_Model->Group[Count]->Normal_Data[B].x);
		glVertex3fv(&App->CLSB_Model->Group[Count]->vertex_Data[B].x);
		
		//-----------------------------------------------
		glTexCoord2f(App->CLSB_Model->Group[Count]->MapCord_Data[C].u, App->CLSB_Model->Group[Count]->MapCord_Data[C].v);
		glNormal3fv(&App->CLSB_Model->Group[Count]->Normal_Data[C].x);
		glVertex3fv(&App->CLSB_Model->Group[Count]->vertex_Data[C].x);
		VertCount++;
		//-----------------------------------------------

		glEnd();

	}

	return 1;
}

//*************************************************************************
// *					Assimp_Render_Normals Terry Bernie	   			   *
// *************************************************************************
void SB_Render::Assimp_Render_Normals(void)
{
	int Count = 0;

	glColor3f(1, 1, 1);

	int GroupCount = App->CLSB_Model->Get_Groupt_Count();

	while (Count < GroupCount)
	{
		Render_As_Normals_Parts(Count);
		Count++;
	}
}
// *************************************************************************
// *					Render_AsNormals_Part Terry Bernie	   			   *
// *************************************************************************
void SB_Render::Render_As_Normals_Parts(int Count)
{
#define Scaler 2

	int VertCount = 0;

	glPointSize(3);
	glBegin(GL_LINES);

	while (VertCount<App->CLSB_Model->Group[Count]->GroupVertCount)
	{
		//-----------------------------------------------
		glVertex3fv(&App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x);

		glVertex3f(App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x + App->CLSB_Model->Group[Count]->Normal_Data[VertCount].x*Scaler,
			App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y + App->CLSB_Model->Group[Count]->Normal_Data[VertCount].y*Scaler,
			App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z + App->CLSB_Model->Group[Count]->Normal_Data[VertCount].z*Scaler);
		VertCount++;

	}

	glEnd();
}

// *************************************************************************
// *					RF_Render_Normals Terry Bernie		  			   *
// *************************************************************************
void SB_Render::RF_Render_Normals(void)
{
//#define t 2
//
//	geBody_Index V1;
//	geBody_Index V2;
//	geBody_Index V3;
//
//	const geBody_Triangle *SF;
//
//	SF = App->CL_Genesis3D->ActorDef_Memory->Body->SkinFaces[GE_BODY_HIGHEST_LOD].FaceArray;
//
//	int Start = 0;
//	glPointSize(3);//PointSize
//	glBegin(GL_LINES);
//
//	while (Start < App->CL_Model->FaceCount)
//	{
//
//		glColor3f(1, 1, 0);
//
//		int indexA = App->CL_Model->Face_Data[Start].a;
//		int indexB = App->CL_Model->Face_Data[Start].b;
//		int indexC = App->CL_Model->Face_Data[Start].c;
//
//		V1 = SF[Start].NormalIndex[0];
//		V2 = SF[Start].NormalIndex[1];
//		V3 = SF[Start].NormalIndex[2];
//
//
//		glVertex3f(App->CL_Model->vertex_Data[indexA].x, App->CL_Model->vertex_Data[indexA].y, App->CL_Model->vertex_Data[indexA].z);
//
//		glVertex3f(App->CL_Model->vertex_Data[indexA].x + App->CL_Model->Normal_Data[V1].x*t,
//			App->CL_Model->vertex_Data[indexA].y + App->CL_Model->Normal_Data[V1].y*t,
//			App->CL_Model->vertex_Data[indexA].z + App->CL_Model->Normal_Data[V1].z*t);
//
//		glVertex3f(App->CL_Model->vertex_Data[indexB].x, App->CL_Model->vertex_Data[indexB].y, App->CL_Model->vertex_Data[indexB].z);
//
//		glVertex3f(App->CL_Model->vertex_Data[indexB].x + App->CL_Model->Normal_Data[V2].x*t,
//			App->CL_Model->vertex_Data[indexB].y + App->CL_Model->Normal_Data[V2].y*t,
//			App->CL_Model->vertex_Data[indexB].z + App->CL_Model->Normal_Data[V2].z*t);
//
//		glVertex3f(App->CL_Model->vertex_Data[indexC].x, App->CL_Model->vertex_Data[indexC].y, App->CL_Model->vertex_Data[indexC].z);
//
//		glVertex3f(App->CL_Model->vertex_Data[indexC].x + App->CL_Model->Normal_Data[V3].x*t,
//			App->CL_Model->vertex_Data[indexC].y + App->CL_Model->Normal_Data[V3].y*t,
//			App->CL_Model->vertex_Data[indexC].z + App->CL_Model->Normal_Data[V3].z*t);
//
//		Start++;
//	}
//
//	glEnd();
}

// *************************************************************************
// *				RenderBones 04/05/08   ( Terry Bernie ) 		  	   *
// *************************************************************************
bool SB_Render::As_RenderBones()
{

	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
	//int Start = 0;

	//glColor3f(1, 1, 0);
	//glPointSize(6);//PointSize
	//int Point = 0;

	//while (Start<App->CL_Model->BoneCount)
	//{

	//	if (App->CL_Model->S_Bones[Start]->Parent == -1)
	//	{
	//		glColor3f(1, 0, 0);			// Root Joint Colour
	//	}
	//	else { glColor3f(0, 0, 1); }		// Joint Colours

	//	glBegin(GL_POINTS);
	//	glVertex3f(App->CL_Model->S_Bones[Start]->TranslationStart.X,
	//		App->CL_Model->S_Bones[Start]->TranslationStart.Y,
	//		App->CL_Model->S_Bones[Start]->TranslationStart.Z);

	//	glEnd();
	//	Start++;
	//}

	//Start = 0;
	//while (Start<App->CL_Model->BoneCount)
	//{

	//	if (App->CL_Model->S_Bones[Start]->Parent == -1)
	//	{
	//		glColor3f(1, 0, 0);			// Root Joint Color Again Both the same
	//		glBegin(GL_POINTS);
	//		glVertex3f(App->CL_Model->S_Bones[Start]->TranslationStart.X,
	//			App->CL_Model->S_Bones[Start]->TranslationStart.Y,
	//			App->CL_Model->S_Bones[Start]->TranslationStart.Z);
	//		glEnd();
	//	}
	//	else
	//	{
	//		glLineWidth(3);
	//		glBegin(GL_LINES);
	//		glColor3f(1, 1, 0);			// Bone Colours Between Joints
	//		glVertex3f(App->CL_Model->S_Bones[Start]->TranslationStart.X,
	//			App->CL_Model->S_Bones[Start]->TranslationStart.Y,
	//			App->CL_Model->S_Bones[Start]->TranslationStart.Z);

	//		glVertex3f(App->CL_Model->S_Bones[App->CL_Model->S_Bones[Start]->Parent]->TranslationStart.X,
	//			App->CL_Model->S_Bones[App->CL_Model->S_Bones[Start]->Parent]->TranslationStart.Y,
	//			App->CL_Model->S_Bones[App->CL_Model->S_Bones[Start]->Parent]->TranslationStart.Z);

	//		glEnd();
	//	}

	//	Start++;
	//}

	return 1;
}

// *************************************************************************
// *				Render_BB_Selected_Brush Terry Bernie	  			   *
// *************************************************************************
void SB_Render::Render_BB_Selected_Brush(void)
{
	float m_xMin = App->CLSB_Model->Sel_Brush_BB_Min.x;
	float m_yMin = App->CLSB_Model->Sel_Brush_BB_Min.y;
	float m_zMin = App->CLSB_Model->Sel_Brush_BB_Min.z;

	float m_xMax = App->CLSB_Model->Sel_Brush_BB_Max.x;
	float m_yMax = App->CLSB_Model->Sel_Brush_BB_Max.y;
	float m_zMax = App->CLSB_Model->Sel_Brush_BB_Max.z;

	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMin, m_yMin, m_zMax);

	glVertex3f(m_xMax, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMin, m_zMax);

	glVertex3f(m_xMin, m_yMax, m_zMin);
	glVertex3f(m_xMin, m_yMax, m_zMax);

	glVertex3f(m_xMax, m_yMax, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMin, m_zMin);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMin, m_yMax, m_zMin);

	glVertex3f(m_xMax, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMin);

	glVertex3f(m_xMin, m_yMax, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMin);

	glVertex3f(m_xMin, m_yMin, m_zMax);
	glVertex3f(m_xMax, m_yMin, m_zMax);

	glVertex3f(m_xMin, m_yMin, m_zMax);
	glVertex3f(m_xMin, m_yMax, m_zMax);

	glVertex3f(m_xMax, m_yMin, m_zMax);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glVertex3f(m_xMin, m_yMax, m_zMax);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glEnd();
	glEnable(GL_TEXTURE_2D);
}

// *************************************************************************
// *				Render_BoundingBoxModel Terry Bernie	  			   *
// *************************************************************************
void SB_Render::Render_BoundingBoxModel(void)
{
	float m_xMin = App->CLSB_Model->BB_Min.x;
	float m_yMin = App->CLSB_Model->BB_Min.y;
	float m_zMin = App->CLSB_Model->BB_Min.z;

	float m_xMax = App->CLSB_Model->BB_Max.x;
	float m_yMax = App->CLSB_Model->BB_Max.y;
	float m_zMax = App->CLSB_Model->BB_Max.z;

	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMin, m_yMin, m_zMax);

	glVertex3f(m_xMax, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMin, m_zMax);

	glVertex3f(m_xMin, m_yMax, m_zMin);
	glVertex3f(m_xMin, m_yMax, m_zMax);

	glVertex3f(m_xMax, m_yMax, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMin, m_zMin);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMin, m_yMax, m_zMin);

	glVertex3f(m_xMax, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMin);

	glVertex3f(m_xMin, m_yMax, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMin);

	glVertex3f(m_xMin, m_yMin, m_zMax);
	glVertex3f(m_xMax, m_yMin, m_zMax);

	glVertex3f(m_xMin, m_yMin, m_zMax);
	glVertex3f(m_xMin, m_yMax, m_zMax);

	glVertex3f(m_xMax, m_yMin, m_zMax);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glVertex3f(m_xMin, m_yMax, m_zMax);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glEnd();
	glEnable(GL_TEXTURE_2D);
}

// *************************************************************************
// *				Render_BoundingBoxGroup Terry Bernie	  			   *
// *************************************************************************
void SB_Render::Render_BoundingBoxGroup(void)
{
	float m_xMin = App->CLSB_Model->Group[GroupNumber]->BB_Min.x;
	float m_yMin = App->CLSB_Model->Group[GroupNumber]->BB_Min.y;
	float m_zMin = App->CLSB_Model->Group[GroupNumber]->BB_Min.z;

	float m_xMax = App->CLSB_Model->Group[GroupNumber]->BB_Max.x;
	float m_yMax = App->CLSB_Model->Group[GroupNumber]->BB_Max.y;
	float m_zMax = App->CLSB_Model->Group[GroupNumber]->BB_Max.z;

	/*m_xMin = App->CLSB_Model->Group[GroupNumber]->vertex_Data[0].x;
	m_yMin = App->CLSB_Model->Group[GroupNumber]->vertex_Data[0].y;
	m_zMin = App->CLSB_Model->Group[GroupNumber]->vertex_Data[0].z;

	m_xMax = App->CLSB_Model->Group[GroupNumber]->vertex_Data[0].x;
	m_yMax = App->CLSB_Model->Group[GroupNumber]->vertex_Data[0].y;
	m_zMax = App->CLSB_Model->Group[GroupNumber]->vertex_Data[0].z;

	int Count = GroupNumber;
	int VertCount = 0;

	while (VertCount < App->CLSB_Model->Group[GroupNumber]->GroupVertCount)
	{
		if (App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x < m_xMin) m_xMin = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x;
		if (App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y < m_yMin) m_yMin = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y;
		if (App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z < m_zMin) m_zMin = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z;
		if (App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x > m_xMax) m_xMax = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x;
		if (App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y > m_yMax) m_yMax = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y;
		if (App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z > m_zMax) m_zMax = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z;
		VertCount++;
	}*/
	
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMin, m_yMin, m_zMax);

	glVertex3f(m_xMax, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMin, m_zMax);

	glVertex3f(m_xMin, m_yMax, m_zMin);
	glVertex3f(m_xMin, m_yMax, m_zMax);

	glVertex3f(m_xMax, m_yMax, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMin, m_zMin);

	glVertex3f(m_xMin, m_yMin, m_zMin);
	glVertex3f(m_xMin, m_yMax, m_zMin);

	glVertex3f(m_xMax, m_yMin, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMin);

	glVertex3f(m_xMin, m_yMax, m_zMin);
	glVertex3f(m_xMax, m_yMax, m_zMin);

	glVertex3f(m_xMin, m_yMin, m_zMax);
	glVertex3f(m_xMax, m_yMin, m_zMax);

	glVertex3f(m_xMin, m_yMin, m_zMax);
	glVertex3f(m_xMin, m_yMax, m_zMax);

	glVertex3f(m_xMax, m_yMin, m_zMax);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glVertex3f(m_xMin, m_yMax, m_zMax);
	glVertex3f(m_xMax, m_yMax, m_zMax);

	glEnd();
	glEnable(GL_TEXTURE_2D);
}


// *************************************************************************
// *					RF_Render_All_Groups  ( Terry Bernie ) 			   *
// *************************************************************************
bool SB_Render::RF_Render_All_Groups()
{
	//const geBody_Triangle *SF;
	//SF = App->CL_Vm_Genesis3D->ActorDef_Memory->Body->SkinFaces[GE_BODY_HIGHEST_LOD].FaceArray;

	//int Count = 0;
	//int MatIndex;
	//int UVIndex = 0;

	//float x = 0;
	//float y = 0;
	//float z = 0;

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_TEXTURE_2D);
	//glColor3f(1, 1, 1);
	//glEnable(GL_ALPHA_TEST);

	//while (Count < App->CL_Vm_Model->FaceCount)
	//{
	//	MatIndex = App->CL_Vm_Model->MatIndex_Data[Count];

	//	glBindTexture(GL_TEXTURE_2D, App->CL_Vm_Textures->g_Texture[MatIndex]);

	//	glBegin(GL_POLYGON);

	//	x = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].a].x;
	//	y = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].a].y;
	//	z = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].a].z;


	//	glNormal3f(App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[0]].x, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[0]].y, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[0]].z);
	//	glTexCoord2f(App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].a].u, App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].a].v);
	//	glVertex3f(x, y, z);//Vertex definition

	//	x = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].b].x;
	//	y = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].b].y;
	//	z = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].b].z;


	//	glNormal3f(App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[1]].x, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[1]].y, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[1]].z);
	//	glTexCoord2f(App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].b].u, App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].b].v);
	//	glVertex3f(x, y, z);

	//	x = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].c].x;
	//	y = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].c].y;
	//	z = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].c].z;

	//	glNormal3f(App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[2]].x, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[2]].y, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[2]].z);
	//	glTexCoord2f(App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].c].u, App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].c].v);
	//	glVertex3f(x, y, z);

	//	glEnd();

	//	Count++;
	//}

	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_ALPHA_TEST);

	return 1;
}

// *************************************************************************
// *					RF_Render_Just_Group  ( Terry Bernie )			   *
// *************************************************************************
bool SB_Render::RF_Render_Just_Group()
{
	//const geBody_Triangle *SF;
	//SF = App->CL_Vm_Genesis3D->ActorDef_Memory->Body->SkinFaces[GE_BODY_HIGHEST_LOD].FaceArray;

	//int Count = 0;
	//int MatIndex;
	//int UVIndex = 0;

	//float x = 0;
	//float y = 0;
	//float z = 0;

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_TEXTURE_2D);
	//glColor3f(1, 1, 1);
	//glEnable(GL_ALPHA_TEST);

	//int SelectedIndex = App->CL_Vm_Model->S_TextureInfo[App->EBC_Groups->SelectedGroup]->ActorMaterialIndex;

	//while (Count<App->CL_Vm_Model->FaceCount)
	//{
	//	MatIndex = App->CL_Vm_Model->MatIndex_Data[Count];

	//	if (MatIndex == SelectedIndex)
	//	{

	//		glBindTexture(GL_TEXTURE_2D, App->CL_Vm_Textures->g_Texture[MatIndex]);

	//		glBegin(GL_POLYGON);

	//		x = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].a].x;
	//		y = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].a].y;
	//		z = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].a].z;


	//		glNormal3f(App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[0]].x, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[0]].y, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[0]].z);
	//		glTexCoord2f(App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].a].u, App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].a].v);
	//		glVertex3f(x, y, z);//Vertex definition

	//		x = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].b].x;
	//		y = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].b].y;
	//		z = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].b].z;


	//		glNormal3f(App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[1]].x, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[1]].y, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[1]].z);
	//		glTexCoord2f(App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].b].u, App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].b].v);
	//		glVertex3f(x, y, z);

	//		x = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].c].x;
	//		y = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].c].y;
	//		z = App->CL_Vm_Model->vertex_Data[App->CL_Vm_Model->Face_Data[Count].c].z;

	//		glNormal3f(App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[2]].x, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[2]].y, App->CL_Vm_Model->Normal_Data[SF[Count].NormalIndex[2]].z);
	//		glTexCoord2f(App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].c].u, App->CL_Vm_Model->MapCord_Data[App->CL_Vm_Model->Face_Data[Count].c].v);
	//		glVertex3f(x, y, z);

	//		glEnd();
	//	}

	//	Count++;
	//}

	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_ALPHA_TEST);

	return 1;
}

// *************************************************************************
// *					RF_Render_Faces Terry Bernie	   				   *
// *************************************************************************
bool SB_Render::RF_Render_Faces(void)
{
	int Count = 0;
	int MatIndex = 0;

	glColor3f(0.9f, 0.9f, 0.9f);
	while (Count < App->CLSB_Model->FaceCount)
	{
		glBegin(GL_POLYGON);
		glVertex3f(App->CLSB_Model->vertex_Data[App->CLSB_Model->Face_Data[Count].a].x, App->CLSB_Model->vertex_Data[App->CLSB_Model->Face_Data[Count].a].y, App->CLSB_Model->vertex_Data[App->CLSB_Model->Face_Data[Count].a].z);
		glVertex3f(App->CLSB_Model->vertex_Data[App->CLSB_Model->Face_Data[Count].b].x, App->CLSB_Model->vertex_Data[App->CLSB_Model->Face_Data[Count].b].y, App->CLSB_Model->vertex_Data[App->CLSB_Model->Face_Data[Count].b].z);
		glVertex3f(App->CLSB_Model->vertex_Data[App->CLSB_Model->Face_Data[Count].c].x, App->CLSB_Model->vertex_Data[App->CLSB_Model->Face_Data[Count].c].y, App->CLSB_Model->vertex_Data[App->CLSB_Model->Face_Data[Count].c].z);
		glEnd();

		Count++;
	}

	return 1;
}

// *************************************************************************
// *					RF_Render_Points Terry Bernie					   *
// *************************************************************************
bool SB_Render::RF_Render_Points(void)
{
	int Count = 0;
	glPointSize(5);

	glColor3f(1.0f, 1.0f, 0.0f);

	while (Count < App->CLSB_Model->VerticeCount)
	{
		glBegin(GL_POINTS);

		glVertex3fv(&App->CLSB_Model->vertex_Data[Count].x);

		glEnd();

		Count++;
	}

	return 1;
}

// **************************************************************************
// *					RenderCrossHair Terry Bernie						*
// **************************************************************************
void SB_Render::RenderCrossHair(void)
{
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);

	//float Length = App->CL_Model->S_BoundingBox[0]->radius * 2;
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

// *************************************************************************
// *					RenderByTexture  ( Terry Bernie ) 				   *
// *************************************************************************
bool SB_Render::RenderByTexture()
{
	//const geBody_Triangle *SF;
	//SF = App->CL_Genesis3D->ActorDef_Memory->Body->SkinFaces[GE_BODY_HIGHEST_LOD].FaceArray;

	//int Count = 0;
	//int MatIndex;
	//int UVIndex = 0;

	//float x = 0;
	//float y = 0;
	//float z = 0;

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_TEXTURE_2D);
	//glColor3f(1, 1, 1);
	//glEnable(GL_ALPHA_TEST);

	//while (Count<App->CL_Model->FaceCount)
	//{
	//	//MatIndex = App->CL_Genesis_Import->ActorDef_Memory->Body->SkinFaces[GE_BODY_HIGHEST_LOD].FaceArray[Count].MaterialIndex;
	//	MatIndex = App->CL_Model->MatIndex_Data[Count];

	//	glBindTexture(GL_TEXTURE_2D,g_Texture[MatIndex]);

	//	glBegin(GL_POLYGON);

	//	x = App->CL_Model->vertex_Data[App->CL_Model->Face_Data[Count].a].x;
	//	y = App->CL_Model->vertex_Data[App->CL_Model->Face_Data[Count].a].y;
	//	z = App->CL_Model->vertex_Data[App->CL_Model->Face_Data[Count].a].z;


	//	glNormal3f(App->CL_Model->Normal_Data[SF[Count].NormalIndex[0]].x, App->CL_Model->Normal_Data[SF[Count].NormalIndex[0]].y, App->CL_Model->Normal_Data[SF[Count].NormalIndex[0]].z);
	//	glTexCoord2f(App->CL_Model->MapCord_Data[App->CL_Model->Face_Data[Count].a].u, App->CL_Model->MapCord_Data[App->CL_Model->Face_Data[Count].a].v);
	//	glVertex3f(x, y, z);//Vertex definition

	//	x = App->CL_Model->vertex_Data[App->CL_Model->Face_Data[Count].b].x;
	//	y = App->CL_Model->vertex_Data[App->CL_Model->Face_Data[Count].b].y;
	//	z = App->CL_Model->vertex_Data[App->CL_Model->Face_Data[Count].b].z;


	//	glNormal3f(App->CL_Model->Normal_Data[SF[Count].NormalIndex[1]].x, App->CL_Model->Normal_Data[SF[Count].NormalIndex[1]].y, App->CL_Model->Normal_Data[SF[Count].NormalIndex[1]].z);
	//	glTexCoord2f(App->CL_Model->MapCord_Data[App->CL_Model->Face_Data[Count].b].u, App->CL_Model->MapCord_Data[App->CL_Model->Face_Data[Count].b].v);
	//	glVertex3f(x, y, z);

	//	x = App->CL_Model->vertex_Data[App->CL_Model->Face_Data[Count].c].x;
	//	y = App->CL_Model->vertex_Data[App->CL_Model->Face_Data[Count].c].y;
	//	z = App->CL_Model->vertex_Data[App->CL_Model->Face_Data[Count].c].z;

	//	glNormal3f(App->CL_Model->Normal_Data[SF[Count].NormalIndex[2]].x, App->CL_Model->Normal_Data[SF[Count].NormalIndex[2]].y, App->CL_Model->Normal_Data[SF[Count].NormalIndex[2]].z);
	//	glTexCoord2f(App->CL_Model->MapCord_Data[App->CL_Model->Face_Data[Count].c].u, App->CL_Model->MapCord_Data[App->CL_Model->Face_Data[Count].c].v);
	//	glVertex3f(x, y, z);

	//	glEnd();

	//	Count++;
	//}

	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_ALPHA_TEST);

	return 1;
}

// *************************************************************************
// *					Marker_Face_Selection Terry Bernie		   		   *
// *************************************************************************
void SB_Render::Marker_Face_Selection()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 1, 0);
	glLineWidth(2);

	glBegin(GL_POLYGON);

	glVertex3fv(&App->CLSB_BaseFaces->HitVertices[0].x);
	glVertex3fv(&App->CLSB_BaseFaces->HitVertices[1].x);
	glVertex3fv(&App->CLSB_BaseFaces->HitVertices[2].x);
	
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

typedef struct TexInfoTag
{
	geVec3d VecNormal;
	geFloat xScale, yScale;
	int xShift, yShift;
	geFloat	Rotate;			// texture rotation angle in degrees
	TexInfo_Vectors TVecs;
	int Dib;				// index into the wad
	char Name[16];
	geBoolean DirtyFlag;
	geVec3d Pos;
	int txSize, tySize;		// texture size (not currently used)
	geXForm3d XfmFaceAngle;	// face rotation angle
} TexInfo;

typedef struct FaceTag
{
	int			NumPoints;
	int			Flags;
	Plane		Face_Plane;
	int			LightIntensity;
	geFloat		Reflectivity;
	geFloat		Translucency;
	geFloat		MipMapBias;
	geFloat		LightXScale, LightYScale;
	TexInfo		Tex;
	geVec3d* Points;
} Face;

// *************************************************************************
// *		Render_Selected_Face:- Terry and Hazel Flanigan 2023	 	   *
// *************************************************************************
void SB_Render::Render_Selected_Face()
{
	int Points = App->CLSB_Picking->Selected_Face->NumPoints;
	int Count = 0;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 1, 0);
	glLineWidth(3);

	glBegin(GL_LINE_LOOP);

	while (Count < Points)
	{
		glVertex3fv(&App->CLSB_Picking->Selected_Face->Points[Count].X);
		Count++;
	}

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

}

// *************************************************************************
// *					Render_Brush_Faces Terry Bernie		   			   *
// *************************************************************************
void SB_Render::Render_Brush_Faces()
{
	int Count = 0;
	while (Count < App->CLSB_BaseFaces->Face_Count)
	{
		int Points = App->CLSB_BaseFaces->Faces_Data[Count].Number_of_Points;
		
		Render_Brush_Faces_Parts(Points,Count);

		Count++;
	}
}

// *************************************************************************
// *					Render_Brush_Face Terry Bernie		   			   *
// *************************************************************************
void SB_Render::Render_Brush_Faces_Parts(int NumPoints, int Index)
{
	int Count = 0;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0, 1, 0);
	glLineWidth(1);

	glBegin(GL_LINE_LOOP);

	while (Count < NumPoints)
	{
		glVertex3fv(&App->CLSB_BaseFaces->Faces_Data[Index].Face_Points[Count].x);
		Count++;
	}

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

// *************************************************************************
// *				New_Assimp_Render_Textures_Terry Bernie	    		   *
// *************************************************************************
bool SB_Render::New_Assimp_Render_Textures(void)
{
	int Count = 0;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);

	/*int GroupCount = App->CLSB_Assimp->Total_Assimp_GroupCount;

	Count = 0;
	while (Count < GroupCount)
	{
		New_Assimp_Textured_Parts(Count);
		Count++;
	}*/

	glDisable(GL_TEXTURE_2D);

	return 1;
}
// *************************************************************************
// *					New_Assimp_Textured_Parts Terry Bernie		 		   *
// *************************************************************************
bool SB_Render::New_Assimp_Textured_Parts(int Count)
{
	int VertCount = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	//if (App->CLSB_Assimp->Assimp_Group[Count]->MaterialIndex > -1)
	//{
	//	glEnable(GL_TEXTURE_2D);
	//	glColor3f(1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, g_Texture[App->CLSB_Assimp->Assimp_Group[Count]->MaterialIndex]);
	//	//glBindTexture(GL_TEXTURE_2D, g_BrushTexture[App->CLSB_Assimp->Assimp_Group[Count]->MaterialIndex]);

	//}
	//else
	//{
	//	glDisable(GL_TEXTURE_2D);
	//}

	//while (VertCount < App->CLSB_Assimp->Assimp_Group[Count]->GroupFaceCount)
	//{
	//	A = App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[VertCount].a;
	//	B = App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[VertCount].b;
	//	C = App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[VertCount].c;

	//	glBegin(GL_POLYGON);

	//	//-----------------------------------------------
	//	glTexCoord2f(App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[A].u, App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[A].v);
	//	glNormal3fv(&App->CLSB_Assimp->Assimp_Group[Count]->Normal_Data[A].x);
	//	glVertex3fv(&App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[A].x);

	//	//-----------------------------------------------
	//	glTexCoord2f(App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[B].u, App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[B].v);
	//	glNormal3fv(&App->CLSB_Assimp->Assimp_Group[Count]->Normal_Data[B].x);
	//	glVertex3fv(&App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[B].x);

	//	//-----------------------------------------------
	//	glTexCoord2f(App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[C].u, App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[C].v);
	//	glNormal3fv(&App->CLSB_Assimp->Assimp_Group[Count]->Normal_Data[C].x);
	//	glVertex3fv(&App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[C].x);
	//	VertCount++;
	//	//-----------------------------------------------

	//	glEnd();

	//}

	return 1;
}

