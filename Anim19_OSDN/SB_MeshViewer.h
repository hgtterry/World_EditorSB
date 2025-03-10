/*
Copyright (c) 2021 Scene Builder and Equity -- Inflanite Software W.T.Flanigan H.C.Flanigan

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

// :- Terry and Hazel Flanigan 2022

*/

#pragma once

#include "SB_MeshView_Listener.h"

class SB_MeshViewer
{
public:
	SB_MeshViewer();
	~SB_MeshViewer();

	bool StartMeshViewer();
	void Copy_Assets();

	char mResource_Folder[MAX_PATH];
	char Selected_MeshFile[MAX_PATH];
	char Object_Name[MAX_PATH];
	char Last_MeshFile[MAX_PATH];
	char m_Material_File[MAX_PATH];
	char m_Current_Folder[MAX_PATH];

	char DestinationFile[MAX_PATH];
	char SourceFile[MAX_PATH];

	HWND MainDlgHwnd;

	Ogre::String MV_Resource_Group;
	Ogre::SceneNode*		MvNode;
	
	Ogre::ManualObject*		btDebug_Manual;
	Ogre::SceneNode*		btDebug_Node;
	Ogre::SceneManager* mSceneMgrMeshView;
	Ogre::Camera*		mCameraMeshView;

	ManualObject*		GridManual;
	SceneNode*			GridNode;

	SB_MeshView_Listener*	RenderListener;

	int Physics_Type;
	int Physics_Shape;

	bool SelectDynamic;
	bool SelectStatic;
	bool SelectTriMesh;

	bool Placement_Camera;

	int Mesh_Viewer_Mode;
	
protected:

	static LRESULT CALLBACK MeshViewer_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Mesh_Properties_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MeshView_3D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_For_Collectables();
	void Set_For_Areas(HWND hDlg);
	void Set_Shape_Buttons();

	// -------------------------------- Physics Shapes
	void Set_Debug_Shapes();
	void Show_Physics_Box();
	void Show_Physics_Capsule();
	void Show_Physics_Cone();
	void Show_Physics_Sphere();
	void Show_Physics_Cylinder();
	void Show_Physics_Trimesh();
	void Show_Physics_None();

	// -------------------------------- 

	void Grid_Update(bool Create);

	// -------------------------------- 

	void Set_ResourceMesh_File(HWND hDlg);

	bool Set_OgreWindow(void);
	void Close_OgreWindow(void);

	void Update_Mesh(char* MeshFile);
	void Get_Mesh_Assets();
	void Reset_Shape_Flags();

	void Enable_ShapeButtons(bool state);
	void Enable_TypeButtons(bool state);

	bool Get_Files();

	void Show_Mesh_Properties();
	void Reset_Camera(void);

	bool Create_Resources_Group();
	bool Add_Resources();
	bool Delete_Resources_Group();

	bool Get_Media_Folders_Actors(HWND DropHwnd);
	bool GetMeshFiles(char* Location, bool ResetList);

	Ogre::Vector3 GetMesh_BB_Size(SceneNode* mNode);
	float GetMesh_BB_Radius(SceneNode* mNode);

	//----------------------------------
	bool Selected_Shape_Box;
	bool Selected_Shape_Sphere;
	bool Selected_Shape_Capsule;
	bool Selected_Shape_Cylinder;
	bool Selected_Shape_Cone;

	bool View_Centred_Flag;
	bool View_Zoomed_Flag;

	int Texure_Count;
	std::vector<std::string> v_Texture_Names;

	HWND ListHwnd;
	HWND MeshView_3D_hWnd;
	HWND CB_hWnd;

	Ogre::Entity*		MvEnt;
	btRigidBody			*Phys_Body;
	btCollisionShape	*Phys_Shape;

	Ogre::RenderWindow* MeshView_Window;
	Ogre::SceneNode*	CamNode;

	int Scale_X;
	int Scale_Y;
	int Scale_Z;

	int Division_X;
	int Division_Y;
	int Division_Z;

	int XAxis_min;
	int	XAxis_max;
	int YAxis_min;
	int	YAxis_max;
	int ZAxis_min;
	int	ZAxis_max;
	int HairExtend;

	Ogre::ColourValue ColourMain;
	Ogre::ColourValue ColourHairZ;
	Ogre::ColourValue ColourHairX;
	Ogre::ColourValue ColourHairY;
	Ogre::ColourValue ColourDivision;

	bool ShowDivisions;
};

