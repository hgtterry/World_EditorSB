/*
Copyright (c) 2022 GameDirectorSB and EquityME -- HGTInflanite Software W.T.Flanigan H.C.Flanigan

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
#include "GD19_App.h"
#include "SB_Project_Create.h"

SB_Project_Create::SB_Project_Create()
{
}

SB_Project_Create::~SB_Project_Create()
{
}

// *************************************************************************
// *	  		Start_New_Project:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_Project_Create::Start_New_Project()
{
	App->SBC_Scene->Clear_Level();
	App->SBC_Scene->Create_Resources_Group();

	App->SBC_Project->Start_Save_Project_Dialog();

	App->SBC_Dialogs->YesNo("Add Area", "Do you want to add a new Area", 1);

	bool Doit = App->SBC_Dialogs->Canceled;
	if (Doit == 0)
	{
		App->SBC_MeshViewer->Mesh_Viewer_Mode = Enums::Mesh_Viewer_Area;
		App->SBC_MeshViewer->StartMeshViewer();
	}
	else
	{
		//First_Area_Build_Project(1);
	}
}

// *************************************************************************
// *	  	Add_First_New_Area:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_Project_Create::Add_First_New_Area()
{
	First_Area_Build_Project(0);
}

// *************************************************************************
//		First_Area_Build_Project:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool SB_Project_Create::First_Area_Build_Project(bool NoArea)
{
	
	char Temp[MAX_PATH];
	strcpy(Temp, App->SBC_Scene->Project_Resource_Group.c_str());

	// ------------------------ Add Area
	if (NoArea == 0)
	{
		App->SBC_Scene->B_Area[0] = new Base_Area();
		App->SBC_Com_Area->Add_Aera_To_Project(0, App->SBC_MeshViewer->Selected_MeshFile, Temp);
		strcpy(App->SBC_Scene->B_Area[0]->Material_File, App->SBC_MeshViewer->m_Material_File);
		App->SBC_Scene->Area_Count++;
		App->SBC_Scene->Area_Added = 1;

		HTREEITEM Temp2 = App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Areas_Folder, "Area_1", 0, true);
		App->SBC_Scene->B_Area[0]->FileViewItem = Temp2;
		App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Areas_Folder);
	}

	// ------------------------ Add Default Camera
	App->SBC_Com_Camera->Add_New_Camera();
	App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Cameras_Folder);
	App->CL_Ogre->OgreListener->GD_CameraMode = Enums::CamDetached;

	// ------------------------ Add Default Player
	App->SBC_Player->Create_Player_Object();
	strcpy(App->SBC_Scene->B_Player[0]->Player_Name, "Player_1");

	HTREEITEM Temp1 =App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Players_Folder, "Player_1", 0, true);
	App->SBC_Scene->B_Player[0]->FileViewItem = Temp1;
	App->SBC_FileView->Set_FolderActive(App->SBC_FileView->FV_Players_Folder);

	// ------------------------ Add Counter
	App->SBC_Display->Add_New_Counter();

	// ------------------------ Add Location
	App->SBC_Locations->Create_Location_Entity("Start_Position");

	// ------------------------ Add Environ

	App->SBC_Com_Environments->Add_New_Environ_Entity(1);
	int mIndex = App->SBC_Com_Environments->Get_First_Environ();
	App->SBC_Com_Environments->Set_First_Environment(mIndex);

	// ------------------------ Set Scene
	App->SBC_Grid->Grid_SetVisible(1);
	App->SBC_FileView->Redraw_FileView();

	App->SBC_FileView->SelectItem(App->SBC_Scene->B_Area[0]->FileViewItem);

	App->SBC_Physics->Reset_Physics();
	App->SBC_Physics->Enable_Physics(1);
	App->SBC_Scene->Scene_Loaded = 1;
	//App->SBC_Scene->Area_Added = 1;
	App->CL_Ogre->Block_RenderingQueued = 0;

	//------------------------------------------------------------------------------ WHY
	int f = App->SBC_Scene->B_Player[0]->Phys_Body->getCollisionFlags();
	App->SBC_Scene->B_Player[0]->Phys_Body->setCollisionFlags(f | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	App->CL_Ogre->Block_RenderingQueued = 0;

	return 1;
}
