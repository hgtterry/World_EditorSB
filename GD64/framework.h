#pragma once

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma warning( disable : 4251 4193 4275 4005 4267 4244)
#pragma warning( disable : 4305 4311 4302)

#include "targetver.h"

// Windows Header Files
#include <windows.h>
#include "btBulletDynamicsCommon.h"

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <direct.h>
#include <Commctrl.h>   // includes the Common control header
#include <string>

#include "Ogre.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreException.h"
//#include "OgreTerrain.h"
//#include "OgreTerrainGroup.h"
//#include "OgreTerrainMaterialGeneratorA.h"
#include "OgreTextAreaOverlayElement.h"
//#include "OgreBillboardParticleRenderer.h"
//#include "OgreStringConverter.h"
//#include "OgreDefaultHardwareBufferManager.h"
//#include "OgreHardwareVertexBuffer.h"
//#include "OgreVertexIndexData.h"
//#include "OgreResourceGroupManager.h"

#include <gl/gl.h>
#include <gl/glu.h>

namespace Enums
{
	enum LoadedFile
	{
		LoadedFile_None = 0,
		LoadedFile_Actor = 1,
		LoadedFile_Assimp = 2,
		LoadedFile_Ogre = 3,
		LoadedFile_STL = 4
	};

	enum Camera
	{
		Cam_Mode_None = 0,
		Cam_Mode_First = 1,
		Cam_Mode_Free = 2,
		Cam_Mode_Third = 3,
		Cam_Mode_Model = 4
	};

	enum Usage
	{
		Usage_None = -1,
		Usage_Player = 2
		
	};
}

#define Debug App->Debug_Text();
