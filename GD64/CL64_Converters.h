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

#pragma once
class CL64_Converters
{
public:

	CL64_Converters(void);
	~CL64_Converters(void);

	void Set_Paths(void);
	void CreateMaterialFile();
	void Convert_ToOgre3D(bool Create);
	void Get_Data(int Index, int FaceIndex);

	void CreateMaterial_Resource(char* MatName);

	Ogre::ManualObject* Export_Manual;
	Ogre::ManualObject* World_Manual;

	Ogre::SceneNode*	World_Node;
	Ogre::Entity*		World_Ent;

	char mWorld_Mesh_JustName[MAX_PATH];
	char mWorld_File_PathAndFile[MAX_PATH];
	char mWorld_File_Path[MAX_PATH];
	char mExport_Just_Name[MAX_PATH];

	float nx;
	float ny;
	float nz;

	float u;
	float v;

	float x;
	float y;
	float z;

};

