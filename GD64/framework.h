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
//#include "OgreTextAreaOverlayElement.h"
//#include "OgreBillboardParticleRenderer.h"
//#include "OgreStringConverter.h"
//#include "OgreDefaultHardwareBufferManager.h"
//#include "OgreHardwareVertexBuffer.h"
//#include "OgreVertexIndexData.h"
//#include "OgreResourceGroupManager.h"

