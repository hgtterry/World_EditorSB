/*
Copyright (c) 2022 - 2024 Reality Factory Scene Builder -- HGT Software W.T.Flanigan H.C.Flanigan

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

class SB_Project_Create
{
public:
	SB_Project_Create();
	~SB_Project_Create();

	void Start_New_Project();
	void Start_Save_Project_Dialog();
	bool Create_New_Area();
	void Create_Ogre_Model(bool Create);

protected:
	static LRESULT CALLBACK Save_Project_Dialog_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	bool Create_New_Project();
	
	void Get_Data(int Index, int FaceIndex);
	bool DecompileTextures_TXL(char* PathAndFile);
	void CreateMaterialFile(char* MatFileName);
	void Set_Paths(void);

	char mWorld_Mesh_JustName[MAX_PATH];

	char User_Mesh_PathAndFile[MAX_PATH];
	char User_Mesh_Path[MAX_PATH];

	char mWorld_File_Path[MAX_PATH];
	char mExport_Just_Name[MAX_PATH];

	Ogre::ManualObject* World_Manual;

	float nx;
	float ny;
	float nz;

	float u;
	float v;

	float x;
	float y;
	float z;

	bool Dialog_cancelled_F;
};

