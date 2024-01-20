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

class SB_MeshViewer
{
public:
	SB_MeshViewer();
	~SB_MeshViewer();

	bool Start_Mesh_Viewer();
	void Copy_Assets();
	void Physics_Rotation(void);

	char mResource_Folder[MAX_PATH];
	char Selected_MeshFile[MAX_PATH];
	char Object_Name[MAX_PATH];
	char Last_MeshFile[MAX_PATH];
	char m_Material_File[MAX_PATH];
	char m_Current_Folder[MAX_PATH];

	char DestinationFile[MAX_PATH];
	char SourceFile[MAX_PATH];

	HWND MainDlgHwnd;

	Ogre::String			MV_Resource_Group;

	Ogre::SceneNode*		MvNode;
	Ogre::Entity*			MvEnt;

	Ogre::ManualObject*		Saved_btDebug_Manual;

	Ogre::ManualObject*		btDebug_Manual;
	Ogre::SceneNode*		btDebug_Node;

	btRigidBody*			Phys_Body;
	
	int Physics_Type;
	int Physics_Shape;
	int Last_Selected_Physics_Shape;

	bool SelectDynamic;
	bool SelectStatic;
	bool SelectTriMesh;

	bool Placement_Camera;

	bool Mesh_Render_Running;
	bool Mesh_Render_Running_New;

	int CursorPosX;
	int CursorPosY;

	int Mesh_Viewer_Mode;

private:

	static LRESULT CALLBACK MeshViewer_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Mesh_Properties_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Start_Render(void);
	void Stop_Render(void);

	void Set_For_Objects(HWND hDlg);
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

	void Set_ResourceMesh_File(HWND hDlg);

	void Update_Mesh(char* MeshFile);
	void Get_Mesh_Assets();
	void Reset_Shape_Flags();

	void Enable_ShapeButtons(bool state);
	void Enable_TypeButtons(bool state);

	Ogre::Vector3 GetMesh_BB_Size(SceneNode* mNode);
	Ogre::Vector3 Get_BoundingBox_World_Centre();
	float GetMesh_BB_Radius(SceneNode* mNode);

	bool Create_Resources_Group();
	bool Add_Resources_Location();
	bool Delete_Resources_Group();

	bool Get_Media_Folders_Actors(HWND DropHwnd);
	//bool GetMeshFiles(char* Location, bool ResetList);

	bool Get_Mesh_Files_From_Folder(HWND hDlg);

	void Show_Mesh_Properties();
	void Reselect_Shape();
	void RedrawWindow_Dlg_Buttons();
	void Show_Exsisting_Objects(bool flag);

	//----------------------------------
	bool Selected_Shape_Box;
	bool Selected_Shape_Sphere;
	bool Selected_Shape_Capsule;
	bool Selected_Shape_Cylinder;
	bool Selected_Shape_Cone;

	bool Show_Just_This_Mesh;

	bool View_Centred_Flag;
	bool View_Zoomed_Flag;

	int Texure_Count;
	
	std::vector<std::string> v_Texture_Names;

	HWND ListHwnd;
	HWND MeshView_3D_hWnd;
	HWND CB_hWnd;

	btCollisionShape	*Phys_Shape;

};

