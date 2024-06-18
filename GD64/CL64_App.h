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

#include "CL64_Ogre_Init.h"
#include "CL64_Grid.h"
#include "CL64_SplitterViews.h"
#include "CL64_Keyboard.h"
#include "CL64_Camera.h"
#include "CL64_Ini_File.h"
#include "CL64_Preferences.h"
#include "CL64_Assimp.h"
#include "CL64_Scene.h"
#include "CL64_Importers.h"
#include "CL64_File_IO.h"
#include "CL64_ImGui.h"
#include "CL64_Textures.h"
#include "CL64_TopDlg.h"
#include "CL64_Panels.h"
#include "CL64_Converters.h"
#include "CL64_Bullet.h"
#include "CL64_Player.h"
#include "CL64_Demos.h"

class CL64_App
{

public:
	CL64_App(void);
	~CL64_App(void);

	CL64_Ogre_Init*		CL_Ogre;
	CL64_Grid*			CL_Grid;
	CL64_SplitterViews* CL_SplitterViews;
	CL64_Keyboard*		CL_Keyboard;
	CL64_Camera*		CL_Camera;
	CL64_Ini_File*		CL_Ini_File;
	CL64_Preferences*	CL_Preferences;
	CL64_Assimp*		CL_Assimp;
	CL64_Scene*			CL_Scene;
	CL64_Importers*		CL_Importers;
	CL64_File_IO*		CL_File_IO;
	CL64_ImGui*			CL_ImGui;
	CL64_Textures*		CL_Textures;
	CL64_TopDlg*		CL_TopDlg;
	CL64_Panels*		CL_Panels;
	CL64_Converters*	CL_Converters;
	CL64_Bullet*		CL_Bullet;
	CL64_Player*		CL_Player;
	CL64_Demos*			CL_Demos;

	void InitApp(void);

	void Say(const char* Message);
	void Say_Int(int Value);
	void Say_Float(float Value);


	bool SetMainWinCentre(void) const;
	void Flash_Window();
	void Debug_Text();

	void LoadProgramResource(void);
	void SetBrushes_Fonts(void);

	bool Custom_Button_Toggle_Tabs(LPNMCUSTOMDRAW item, bool Toggle);
	bool Custom_Button_Normal(LPNMCUSTOMDRAW item);
	bool Custom_Button_Toggle(LPNMCUSTOMDRAW item, bool Toggle);
	HBRUSH CreateGradientBrush(COLORREF top, COLORREF bottom, LPNMCUSTOMDRAW item);

	HINSTANCE hInst;
	HWND MainHwnd;
	HWND Fdlg;
	HWND ViewGLhWnd;

	int CursorPosX;
	int CursorPosY;

	bool OgreStarted;
	bool Block_Mouse_Buttons;

	char GD_Directory_FullPath[MAX_PATH];

	bool Debug_App;

	HFONT Font_CB15;

	HCURSOR CUR;
	
	HANDLE Hnd_HairOn_Bmp;
	HANDLE Hnd_HairOff_Bmp;
	HANDLE Hnd_GridOn_Bmp;
	HANDLE Hnd_GridOff_Bmp;
	HANDLE Hnd_ModelInfo_Bmp;
	HANDLE Hnd_ModelInfoOn_Bmp;

	HBRUSH AppBackground;
	HBRUSH BlackBrush;
	HBRUSH Brush_But_Normal;
	HBRUSH Brush_But_Hover;
	HBRUSH Brush_But_Pressed;
	HBRUSH Brush_Green;
	HBRUSH Brush_Tabs;
	HBRUSH Brush_Tabs_UnSelected;
};

extern CL64_App* App;

