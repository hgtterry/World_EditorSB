#pragma once

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

class SB_Mesh_Mgr
{
public:
	SB_Mesh_Mgr(void);
	~SB_Mesh_Mgr(void);

	void Start_Brush_Viewer();

	void Delete_Brush_List();
	void Delete_Group_Brushes();

	void WE_Build_Brush_List(int ExpSelected);

	void Set_BBox_Selected_Brush(int Index);
	void Set_BBox_All_Selected_Brushes();

	bool WE_Convert_All_Texture_Groups();
	bool WE_Convert_To_Texture_Group(int TextureID);
	int WE_Get_Vertice_Count(int TextureID);

	bool Update_World();

	void Store_Faces_Data();
	void Store_Faces_Data_Parts(int NumPoints, int Index);

	int ActualFaceCount;
	int	mTextureCount;

	char TextureName2[20][MAX_PATH];
	int IsTextureAlpha[20];

	int Global_Faces_Index;

	bool Brush_Viewer_Dialog_Active;

	Ogre::SceneNode* World_Node;
	Ogre::Entity* World_Ent;

	HWND Mesh_Viewer_HWND;

private:

	static LRESULT CALLBACK Brush_Viewer_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static signed int Brush_CSG_Callback(const Brush* pBrush, void* lParam);

	void Update_World_Model_Info(HWND hDlg);
	void UpdateBrushData(HWND hDlg, int Index);
	void Update_Brush_List(HWND hDlg);

	bool WE_Level_Build_Brushes(Level3* pLevel, const char* Filename, BrushList* BList, int ExpSelected, geBoolean ExpLights, int GroupID);
	bool WE_BrushList_Decode(BrushList* BList, geBoolean SubBrush);
	bool WE_Brush_Create(const Brush* b , int Actual_Brush_Index);
	bool WE_FaceList_Create(const Brush* b, const FaceList* pList, int BrushCount, int SubBrushCount, int Actual_Brush_Index);
	bool AddTexture_GL(geVFile* BaseFile, const char* TextureName, int GroupIndex);
	int Get_Adjusted_Index(int RealIndex);

	void Populate_RenderMode_Combo(HWND DropHwnd);

	void Set_RenderMode_NoRender();
	void Set_RenderMode_Compiled();
	void Set_RenderMode_Groups();
	void Set_RenderMode_Brushes();

	int mAdjusedIndex_Store[500];
	int mBrush_Index;
	char mBrush_Name[200];

	int	mBrushCount;
	int	mSubBrushCount;
	int Selected_Render_Mode;
	
	int Actual_Brush_Index;

	int Compiled_List_Index;
	int Groups_List_Index;
	int Brushes_List_Index;

	bool Picking_Active_Flag;
	bool Show_Data_Flag;
};

