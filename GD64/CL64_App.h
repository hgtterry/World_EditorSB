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

class CL64_App
{

public:
	CL64_App(void);
	~CL64_App(void);

	CL64_Ogre_Init*		CL_Ogre;
	CL64_Grid*			CL_Grid;
	CL64_SplitterViews* CL_SplitterViews;

	void InitApp(void);

	void Say(const char* Message);
	bool SetMainWinCentre(void) const;

	HINSTANCE hInst;
	HWND MainHwnd;
	HWND Fdlg;
	HWND ViewGLhWnd;

	char GD_Directory_FullPath[MAX_PATH];

};

extern CL64_App* App;

