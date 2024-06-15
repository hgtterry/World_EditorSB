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

#include "Base_Player.h"
#include "Base_Group.h"

class CL64_Scene
{
public:

	CL64_Scene(void);
	~CL64_Scene(void);

	void Reset_Class(void);

	void Create_Mesh_Group(int Index);
	void Clear_Model_And_Reset(void);
	void Set_Paths(void);

	Base_Group* Group[100];

	char FileName[MAX_PATH];
	char Path_FileName[MAX_PATH];
	char Model_FolderPath[MAX_PATH];
	char Texture_FolderPath[MAX_PATH];
	char JustName[MAX_PATH];

	// Internal
	int GroupCount;
	int TextureCount;
	int MotionCount;
	int VerticeCount;
	int FaceCount;
	int Model_Type;

	// Ogre
	int Ogre_Face_Count;

	bool Model_Loaded;

	std::vector<Base_Player*> B_Player;
};

