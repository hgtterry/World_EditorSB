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

	void InitApp(void);

	void Say(const char* Message);
	void Say_Int(int Value);
	void Say_Float(float Value);


	bool SetMainWinCentre(void) const;
	void Flash_Window();
	void Debug_Text();

	void SetBrushes_Fonts(void);

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
	HBRUSH AppBackground;
	HBRUSH BlackBrush;
};

extern CL64_App* App;

