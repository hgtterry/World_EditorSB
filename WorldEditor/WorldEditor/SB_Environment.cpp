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

#include "stdafx.h"
#include "AB_App.h"
#include "SB_Environment.h"

SB_Environment::SB_Environment(void)
{
	Show_PropertyEditor = 0;
	PropertyEditor_Page = 0;

	Ambient_Int_Red = 0;
	Ambient_Int_Green = 0;
	Ambient_Int_Blue = 0;

	Fog_Colour_Int_Red = 0;
	Fog_Colour_Int_Green = 0;
	Fog_Colour_Int_Blue = 0;

	Float_PosX = 0;
	Float_PosY = 0;
	Float_StartPos = 0;

	Eviron_Index = 0;

	Is_Teleport = 0;

	ClickOnTrack = 0;
	ClickOnVolume = 0;
	ClickOnPlay = 0;
	ClickOnLoop = 0;

	ClickOnFogVisible = 0;
	ClickOnFogMode = 0;
	ClickOnFogColour = 0;
	ClickOnFogStart = 0;
	ClickOnFogEnd = 0;

	ClickOnSkyEnabled = 0;
	ClickOnSkyTiling = 0;
	ClickOnSkyCurve = 0;
}

SB_Environment::~SB_Environment(void)
{
}

// *************************************************************************
// *		V_Set_Environ_Defaults:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Environment::V_Set_Environ_Defaults(int Index)
{
	Base_Object* V_Object = App->CLSB_Game_Editor->V_Object[Index];

	V_Object->Altered = 0;
	
	V_Object->S_Environ[0]->Environment_ID = 0;
	strcpy(V_Object->S_Environ[0]->Environment_Name, "Not_Set");

	V_Object->S_Environ[0]->Environ_Enabled = 1;

	//----------------------- Light
	V_Object->S_Environ[0]->AmbientColour.x = 0.5;
	V_Object->S_Environ[0]->AmbientColour.y = 0.5;
	V_Object->S_Environ[0]->AmbientColour.z = 0.5;

	V_Object->S_Environ[0]->Light_Position.x = 0;
	V_Object->S_Environ[0]->Light_Position.y = 0;
	V_Object->S_Environ[0]->Light_Position.z = 0;

	// Sky
	V_Object->S_Environ[0]->Curvature = 15;
	V_Object->S_Environ[0]->Distance = 4000;
	V_Object->S_Environ[0]->Enabled = 1;
	strcpy(V_Object->S_Environ[0]->Material, "Examples/CloudySky");
	V_Object->S_Environ[0]->Tiling = 15;
	V_Object->S_Environ[0]->type = 1;

	// Fog
	V_Object->S_Environ[0]->Fog_On = 1;
	V_Object->S_Environ[0]->Fog_Mode = FOG_LINEAR;
	V_Object->S_Environ[0]->Fog_Density = 0.001000;
	V_Object->S_Environ[0]->Fog_Start = 150;
	V_Object->S_Environ[0]->Fog_End = 1000;
	V_Object->S_Environ[0]->Fog_Colour = Ogre::Vector3(1, 1, 1);

	V_Object->S_Environ[0]->IsMainEnvironment = 0;
}

// *************************************************************************
// *		Add_New_Environ_Entiry:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool SB_Environment::Add_New_Environ_Entity(bool FirstOne)
{
	char B_Name[MAX_PATH];
	char ConNum[MAX_PATH];

	int Index = App->CLSB_Scene_Data->Object_Count;

	App->CLSB_Game_Editor->V_Object[Index] = new Base_Object();
	App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0] = new Environ_type;
	V_Set_Environ_Defaults(Index);


	App->CLSB_Game_Editor->V_Object[Index]->Type = Enums::Bullet_Type_Static;
	App->CLSB_Game_Editor->V_Object[Index]->Shape = Enums::Shape_Box;
	//App->CLSB_Scene->V_Object[Index]->This_Object_UniqueID = App->CLSB_Scene->UniqueID_Object_Counter; // Unique ID

	strcpy(App->CLSB_Game_Editor->V_Object[Index]->Mesh_FileName, "EnvironmentEntity_GD.mesh");

	strcpy_s(B_Name, "Environ_");
	_itoa(Index, ConNum, 10);
	strcat(B_Name, ConNum);
	strcpy(App->CLSB_Game_Editor->V_Object[Index]->Mesh_Name, B_Name);

	//if (FirstOne == 0)
	//{
	//	Ogre::Vector3 Pos = App->CL_Object->GetPlacement(-50);
	//	App->SBC_Scene->V_Object[Index]->Mesh_Pos = Pos;
	//}
	//else
	//{
	//	Ogre::Vector3 Pos = Ogre::Vector3(0, 0, 0);
	//}

	//Create_Environ_Entity(Index);

	HTREEITEM Temp = App->CLSB_FileView->Add_Item(App->CLSB_FileView->FV_Evirons_Folder, App->CLSB_Game_Editor->V_Object[Index]->Mesh_Name, Index, false);
	App->CLSB_Game_Editor->V_Object[Index]->FileViewItem = Temp;

	App->CLSB_FileView->Set_FolderActive(App->CLSB_FileView->FV_Evirons_Folder);
	//App->SBC_FileView->SelectItem(App->SBC_Scene->V_Object[Index]->FileViewItem);

	//App->SBC_Scene->UniqueID_Object_Counter++;
	App->CLSB_Scene_Data->Object_Count++;

	//App->CLSB_FileView->Set_FolderActive(App->CLSB_FileView->FV_Sounds_Folder);
	return 1;
}

// *************************************************************************
// *		Set_First_Environment:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_Environment::Set_First_Environment(int Index)
{
	float x = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->AmbientColour.x;
	float y = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->AmbientColour.y;
	float z = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->AmbientColour.z;

	App->CLSB_Ogre_Setup->mSceneMgr->setAmbientLight(ColourValue(x, y, z));

	if (App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_On == 1)
	{
		float Start = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Start;
		float End = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_End;
		float Density = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Density;

		float x = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Colour.x;
		float y = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Colour.y;
		float z = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Colour.z;

		App->CLSB_Ogre_Setup->mSceneMgr->setFog(FOG_LINEAR, ColourValue(x, y, z), Density, (Ogre::Real)Start, (Ogre::Real)End);
	}
	else
	{
		App->CLSB_Ogre_Setup->mSceneMgr->setFog(FOG_NONE, ColourValue(0.7, 0.7, 0.8), 0, 100, 1000);
	}

	if (App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Enabled == 1)
	{
		App->CLSB_Ogre_Setup->mSceneMgr->setSkyDome(true,
			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Material,
			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Curvature,
			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Tiling,
			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Distance);
	}
	else
	{

	}

	App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->IsMainEnvironment = 1;
	App->CLSB_Game_Editor->V_Object[Index]->Usage = Enums::Usage_EnvironEntity;
}

// *************************************************************************
// *	Mark_As_Altered_Environ:- Terry and Hazel Flanigan 2022		 	   *
// *************************************************************************
void SB_Environment::Mark_As_Altered_Environ(int Index)
{
	App->CLSB_Game_Editor->V_Object[Index]->Altered = 1;

	App->CLSB_Scene_Data->Scene_Modified = 1;

	App->CLSB_FileView->Mark_Altered(App->CLSB_Game_Editor->V_Object[Index]->FileViewItem);
}

// *************************************************************************
// *		Set_Environment_By_Index:- Terry and Hazel Flanigan 2022 	   *
// *************************************************************************
int SB_Environment::Set_Environment_By_Index(bool PlayMusic, int Index)
{

	float x = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->AmbientColour.x;
	float y = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->AmbientColour.y;
	float z = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->AmbientColour.z;
	App->CLSB_Ogre_Setup->mSceneMgr->setAmbientLight(ColourValue(x, y, z));


	// Fog
	if (App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_On == 1)
	{
		float Start = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Start;
		float End = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_End;
		float Density = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Density;

		float x = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Colour.x;
		float y = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Colour.y;
		float z = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Colour.z;

		App->CLSB_Ogre_Setup->mSceneMgr->setFog(FOG_LINEAR, ColourValue(x, y, z), Density, (Ogre::Real)Start, (Ogre::Real)End);
	}
	else
	{
		App->CLSB_Ogre_Setup->mSceneMgr->setFog(FOG_NONE, ColourValue(0.7, 0.7, 0.8), 0, 100, 1000);
	}

	if (PlayMusic == 1)
	{
		/*char buff[1024];
		strcpy(buff, App->WorldEditor_Directory);
		strcat(buff, "\\Media\\Sounds\\");

		if (App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Play == 1)
		{
			strcat(buff, App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Sound_File);

			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->SndFile = App->CLSB_SoundMgr->SoundEngine->play2D(buff, App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Loop, true, true);

			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->SndFile->setVolume(App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->SndVolume);
			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->SndFile->setIsPaused(false);*/
		//}
	}
	else
	{
		/*if (App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->SndFile == NULL)
		{
		}
		else
		{
			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->SndFile->setIsPaused(true);
			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->SndFile->drop();
			App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->SndFile = NULL;
		}*/
	}

	return 1;
}

// *************************************************************************
// *		Get_First_Environ:- Terry and Hazel Flanigan 2022		 	   *
// *************************************************************************
int SB_Environment::Get_First_Environ()
{
	int Count = 0;
	while (Count < App->CLSB_Scene_Data->Object_Count)
	{
		if (App->CLSB_Game_Editor->V_Object[Count]->Usage == Enums::Usage_EnvironEntity && App->CLSB_Game_Editor->V_Object[Count]->S_Environ[0]->IsMainEnvironment == 1)
		{
			return Count;
		}

		Count++;
	}

	return -1;
}

// ----------------------------------------------------------------------------------------------------------------
// *************************************************************************
// *	 Start_Environment_Editor:- Terry and Hazel Flanigan 2023  		   *
// *************************************************************************
void SB_Environment::Start_Environment_Editor(int Index, bool IsTeleport)
{
	Eviron_Index = Index;
	Is_Teleport = IsTeleport;

	Float_Exit = 0;
}

// *************************************************************************
// *		Environ_PropertyEditor:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_Environment::Environ_PropertyEditor()
{

}

// *************************************************************************
// *	 Close_Environment_Editor:- Terry and Hazel Flanigan 2022  		   *
// *************************************************************************
void SB_Environment::Close_Environment_Editor()
{
	/*App->Disable_Panels(false);
	App->Show_Panels(true);*/

	App->CLSB_FileView->Show_FileView(true);
	int Index = App->CLSB_Properties->Current_Selected_Object;
	Set_Environment_By_Index(0, Index);

	Index = App->CLSB_Environment->Get_First_Environ();
	Set_Environment_By_Index(0, Index);
}

// *************************************************************************
// *		 Set_To_PlayerView:- Terry and Hazel Flanigan 2022  		   *
// *************************************************************************
void SB_Environment::Set_To_PlayerView()
{
	App->CLSB_Ogre_Setup->OgreListener->GD_CameraMode = Enums::CamFirst;
	//App->CLSB_TopTabs->Toggle_FirstCam_Flag = 1;
	//App->CLSB_TopTabs->Toggle_FreeCam_Flag = 0;

	App->CLSB_Scene_Data->B_Player[0]->Player_Node->setVisible(false);

	int f = App->CLSB_Scene_Data->B_Player[0]->Phys_Body->getCollisionFlags();
	App->CLSB_Scene_Data->B_Player[0]->Phys_Body->setCollisionFlags(f | (1 << 5));

	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 0;
	App->CLSB_Ogre_Setup->BulletListener->Render_Debug_Flag = 1;

	App->CLSB_Ogre_Setup->OgreListener->GD_Run_Physics = 1;

	//RedrawWindow(App->CLSB_TopTabs->Camera_TB_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

// *************************************************************************
// *	  		SetSky:- Terry and Hazel Flanigan 2023					   *
// *************************************************************************
void SB_Environment::SetSky(bool Enable)
{
	int Index = App->CLSB_Properties->Current_Selected_Object;

	App->CLSB_Ogre_Setup->mSceneMgr->setSkyDome(Enable,
		App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Material,
		App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Curvature,
		App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Tiling,
		App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Distance);
}

// *************************************************************************
// *	  		EnableFog:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
bool SB_Environment::EnableFog(bool SetFog)
{
	int Index = App->CLSB_Properties->Current_Selected_Object;

	if (SetFog == true)
	{
		float Start = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Start;
		float End = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_End;
		float Density = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Density;

		float x = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Colour.x;
		float y = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Colour.y;
		float z = App->CLSB_Game_Editor->V_Object[Index]->S_Environ[0]->Fog_Colour.z;

		App->CLSB_Ogre_Setup->mSceneMgr->setFog(FOG_LINEAR, ColourValue(x, y, z), Density, (Ogre::Real)Start, (Ogre::Real)End);
	}
	else
	{
		App->CLSB_Ogre_Setup->mSceneMgr->setFog(FOG_NONE, ColourValue(0.7, 0.7, 0.8), 0, 100, 1000);
	}

	return 1;
}

