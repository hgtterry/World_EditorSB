/****************************************************************************************/
/*  StdAfx.h                                                                            */
/*                                                                                      */
/*  Author:       Jim Mischel, Ken Baird, Jeff Lomax                                    */
/*  Description:  Genesis world editor header file                                      */
/*                                                                                      */
/*  The contents of this file are subject to the Genesis3D Public License               */
/*  Version 1.01 (the "License"); you may not use this file except in                   */
/*  compliance with the License. You may obtain a copy of the License at                */
/*  http://www.genesis3d.com                                                            */
/*                                                                                      */
/*  Software distributed under the License is distributed on an "AS IS"                 */
/*  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See                */
/*  the License for the specific language governing rights and limitations              */
/*  under the License.                                                                  */
/*                                                                                      */
/*  The Original Code is Genesis3D, released March 25, 1999.                            */
/*  Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved           */
/*                                                                                      */
/****************************************************************************************/
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#ifndef STDAFX_H
#define STDAFX_H

#define WINVER 0x0501


#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#pragma comment(lib, "legacy_stdio_definitions.lib")

/*
  Disable warnings for Windows headers
	4201 - nonstandard extension used : nameless struct/union
	4214 - nonstandard extension used : bit field types other than int
	4115 - 'type' : named type definition in parentheses
	4514 - unreferenced inline/local function has been removed
*/
#pragma warning(disable : 4201 4214 4115 4514 4711 4505)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxtempl.h>		// Support for templates.

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows 95 Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "btBulletDynamicsCommon.h"
#include <direct.h>
#include "Genesis.h"

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreException.h"
#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainMaterialGeneratorA.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgreBillboardParticleRenderer.h"
#include "OgreStringConverter.h"
#include "OgreDefaultHardwareBufferManager.h"
#include "OgreHardwareVertexBuffer.h"
#include "OgreVertexIndexData.h"
#include "OgreResourceGroupManager.h"

#include <gl/gl.h>
#include <gl/glu.h>

namespace Enums
{
	enum Mesh_Viewer
	{
		Mesh_Viewer_None = -1,
		Mesh_Viewer_Objects = 0,
		Mesh_Viewer_Collectables = 1,
		Mesh_Viewer_Area = 2
	};

	enum Check_Text
	{
		Check_Name_None = 0,
		Check_Name_Brushes = 1,
		Check_Names_Objects = 2
	};

	enum Equity_Render
	{
		EQ_Mode_None = 0,
		EQ_Mode_Equity = 1,
		EQ_Mode_GameDirector = 2,
		EQ_Mode_Selected = 3
	};

	enum Camera
	{
		CamNone = 0,
		CamFirst = 1,
		CamDetached = 2,
		CamThird = 3,
		CamModel = 4
	};

	enum Mesh_Mgr
	{
		Mesh_Mgr_None = -1,
		Mesh_Mgr_Compiled = 0,
		Mesh_Mgr_Groups = 1,
		Mesh_Mgr_Brushes = 2
	};

	enum RenderMode
	{
		Render_Nothing = -1,
		Render_Brushes = 0,
		Render_Groups = 1,
		Render_Ogre = 2,
		Render_Assimp = 3
	};

	enum LoadedFile
	{
		LoadedFile_None = 0,
		LoadedFile_Actor = 1,
		LoadedFile_Assimp = 2,
		LoadedFile_Ogre = 3,
		LoadedFile_STL = 4,
		LoadedFile_Brushes = 5
	};

	enum Usage
	{
		Usage_None = -1,
		Usage_Room = 0,
		Usage_Static = 1,
		Usage_Dynamic = 2,
		Usage_Object = 3,
		Usage_Ammo = 4,
		Usage_Weapon = 5,
		Usage_Teleport = 6,
		Usage_ChangeLevel = 7,
		Usage_MediKit = 8,
		Usage_Recevier = 9,
		Usage_Particle = 10,
		Usage_Light = 11,
		Usage_Colectable = 12,
		Usage_Message = 13,
		Usage_Move = 14,
		Usage_Sound = 15,
		Usage_PlayerDeath = 16,
		Usage_Counter = 17,
		Usage_Player = 18,
		Usage_Environment = 19,
		Usage_RoomChange = 20,
		Usage_EnvironEntity = 21,
		Usage_UserObject = 22
	};

	enum Folder_Type
	{
		Folder_None = 0,
		Folder_Objects = 1,
		Folder_Sound_Entity = 2,
		Folder_Message_Entity = 3,
		Folder_Move_Entity = 4,
		Folder_Collectables = 5,
		Folder_Teleports = 6,
		Folder_Environment = 7,
		Folder_RoomChange = 8,
		Folder_Change_Level = 9,
		Folder_Particle = 10
	};

	enum Shape
	{
		Shape_None = 0,
		Shape_Box = 1,
		Shape_Sphere = 2,
		Shape_Capsule = 3,
		Shape_Cylinder = 4,
		Shape_Cone = 5
		//Shape_TriMesh = 6,
		//Shape_VolumeBox = 7
	};

	enum Bullet_Type
	{
		Bullet_Type_None = 0,
		Bullet_Type_Dynamic = 1, // Free Moving Physics
		Bullet_Type_Static = 2, // Fixed Non Moving Shape
		Bullet_Type_TriMesh = 3,
		Type_NXS_Convex = 4,
		Type_JustOgre = 5,
		Bullet_Type_Volume = 6
	};

	enum Edit_Type
	{
		Edit_None = 0,
		FV_Edit_Object = 1,
		Edit_Physics_Object = 2,
		Edit_Player = 3,
		Edit_Message = 4,
		Edit_Move_Entity = 5,
		Edit_Sounds = 6,
		Edit_Collectable = 7,
		Edit_Teleport = 8,
		Edit_Environment = 9,
		Edit_Area_Change = 10,
		Edit_Change_Level = 11,
		Edit_Particles = 12,
		Edit_Camera = 13,
		Edit_Area = 14,
		FV_Edit_Level = 15,
		Edit_Counters = 16,
		Edit_Environs = 17,
		Edit_Lights = 18,
		Edit_UserObjects = 19
	};

	enum FileView_Context
	{
		FileView_None = 0,
		FileView_Objects_Folder = 1,
		FileView_Objects_File = 2,
		FileView_Messages_Triggers_Folder = 3,
		FileView_Messages_Triggers_File = 4,
		FileView_Sounds_Folder = 5,
		FileView_Sounds_File = 6,
		FileView_Cameras_Folder = 7,
		FileView_Cameras_File = 8,
		FileView_Areas_Folder = 9,
		FileView_Areas_File = 10,
		FileView_Move_Folder = 11,
		FileView_Move_File = 12,
		FileView_Player_Folder = 13,
		FileView_Player_File = 14,
		FileView_Teleports_Folder = 15,
		FileView_Teleports_File = 16,
		FileView_Collectables_Folder = 17,
		FileView_Collectables_File = 18,
		FileView_Counters_Folder = 19,
		FileView_Counters_File = 20,
		FileView_TextMessage_Folder = 21,
		FileView_TextMessage_File = 22,
		FileView_Environments_Folder = 23,
		FileView_Environments_File = 24,
		FileView_EnvironEntity_Folder = 25,
		FileView_EnvironEntity_File = 26,
		FileView_Particle_Folder = 27,
		FileView_Particle_File = 28,
		FileView_Light_Folder = 29,
		FileView_Light_File = 30,
		FileView_UserObjects_Folder = 31,
		FileView_UserObjects_File = 32
	};
}
// re-enable warnings except 4514
#pragma warning(default : 4201 4214 4115)

#define Debug App->Say("Here", "  Internal Debug Message  ");

#endif
