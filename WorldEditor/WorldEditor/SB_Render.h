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

#pragma once

#include "Ogre.h"

class SB_Render : public Ogre::RenderQueueListener
{
public:
	SB_Render();
	~SB_Render();

	void Reset_Class(void);

	float RX;
	float RZ;

	float Hair_1PosX;
	float Hair_1PosY;
	float Hair_1PosZ;

	float Hair_1RotX;
	float Hair_1RotY;
	float Hair_1RotZ;
	//int SelectedJointIndex;

	bool ShowTextured;
	bool ShowFaces;
	bool ShowBrushes;
	bool ShowBones;
	bool ShowPoints;
	bool ShowNormals;
	bool ShowBoundingBox;
	bool ShowBoundingGroup;
	bool PlayActive;
	bool Light_Activated;
	bool ShowOnlySubMesh;
	bool Show_HideGroup;
	bool Show_Crosshair;
	bool Show_Marker_Face;
	bool Show_Selected_Face;
	bool Show_Brush_Faces;
	bool Render_Brush_Group_Flag;

	// Brushes
	bool Render_Just_Brush;
	int Selected_Brush_Index;
	int Selected_Group_Index;

	int TextureID;
	int JustTexture_ID;

	int Selected_Group;
	int GroupNumber;

	bool RenderByTexture();

	void TestLight(void);

	UINT  g_Texture[400];
	UINT  g_BrushTexture[400];

protected:

	virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& skipThisInvocation);

	virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& repeatThisInvocation);

	void PreRender();
	void PostRender();
	void Render_Loop();
	void Translate(void);

	bool Assimp_Render_Textures(void);
	bool Assimp_Textured_Parts(int Count);
	bool Assimp_Render_Points(void);
	bool Render_As_Points_Parts(int Count);
	bool Assimp_Render_Faces(void);
	bool Assimp_Face_Parts(int Count);

	//bool New_Assimp_Face();
	bool New_Assimp_Render_Textures(void);
	bool New_Assimp_Textured_Parts(int Count);

	void Assimp_Render_Normals(void);
	void Render_As_Normals_Parts(int Count);

	bool WE_Render_By_Texture();
	bool Brush_Render_Brush_Texture(void);
	bool Brush_Textured_Parts_XX(int Count);
	bool Brushes_Render_Faces_XX(void);
	bool Brushes_Face_Parts_XX(int Count);

	bool Brush_Render_Points(void);
	bool Brush_As_Points_Parts(int Count);
	
	bool XBrushes_Render_Faces(void);
	bool XBrushes_Face_Parts(int Count);

	bool XBrush_Render_Textures(void);
	bool XBrush_Textured_Parts(int Count);

	bool As_RenderBones();
	
	void Render_BoundingBoxModel(void);
	void Render_BoundingBoxGroup(void);
	void Render_BB_Selected_Brush(void);
	bool Brush_Render_Group_Texture(void);


	bool RF_Render_Points(void);
	bool RF_Render_Faces(void);
	bool RF_Render_All_Groups();
	bool RF_Render_Just_Group();
	void RF_Render_Normals(void);
	
	// -------------------------------------- Brushes

	void Marker_Face_Selection();

	void Render_Selected_Face();
	
	void Render_Brush_Faces();
	void Render_Brush_Faces_Parts(int NumPoints, int Index);

	void RenderCrossHair(void);
};

