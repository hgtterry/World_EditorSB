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

#include "pch.h"
#include "CL64_App.h"

CL64_App::CL64_App(void)
{
	CL_Ogre =			nullptr;
	CL_Grid =			nullptr;
	CL_SplitterViews =	nullptr;
	CL_Keyboard =		nullptr;
	CL_Camera =			nullptr;
	CL_Ini_File =		nullptr;
	CL_Preferences =	nullptr;
	CL_Assimp =			nullptr;
	CL_Model =			nullptr;
	CL_Importers =		nullptr;
	CL_File_IO =		nullptr;

	hInst =			nullptr;
	MainHwnd =		nullptr;
	Fdlg =			nullptr;
	ViewGLhWnd =	nullptr;

	GD_Directory_FullPath[0] = 0;

	CUR = 0;

	CursorPosX = 500;
	CursorPosY = 500;

	OgreStarted = 0;
	AppBackground = 0;

	Font_CB15 = 0;
	BlackBrush = 0;

	Debug_App = 1;

	Block_Mouse_Buttons = 0;
	//Say("Created");
}

CL64_App::~CL64_App(void)
{
	Say("Destroyed");
}

// *************************************************************************
// *			InitApp:- Terry and Hazel Flanigan 2024					   *
// *************************************************************************
void CL64_App::InitApp(void)
{
	CL_Ogre =			new CL64_Ogre_Init();
	CL_Grid =			new CL64_Grid();
	CL_SplitterViews =	new CL64_SplitterViews();
	CL_Keyboard =		new CL64_Keyboard();
	CL_Camera =			new CL64_Camera();
	CL_Ini_File =		new CL64_Ini_File();
	CL_Preferences =	new CL64_Preferences();
	CL_Assimp =			new CL64_Assimp();
	CL_Model =			new CL64_Model();
	CL_Importers =		new CL64_Importers();
	CL_File_IO =		new CL64_File_IO();

	
	SetBrushes_Fonts();
}

// *************************************************************************
// *				Say:- Terry and Hazel Flanigan 2024					   *
// *************************************************************************
void CL64_App::Say(const char* Message)
{
	char text[1024];
	strcpy(text, Message);
	MessageBox(MainHwnd, Message, Message, MB_OK);
	//App->Cl_Dialogs->Message(text);
}

// *************************************************************************
// *				Say_Int:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_App::Say_Int(int Value)
{
	char buf[255];
	_itoa(Value, buf, 10);
	MessageBox(MainHwnd, buf, "Equity Notice", MB_OK);
}

// *************************************************************************
// *			Say_Float:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_App::Say_Float(float Value)
{
	char buf[255];
	sprintf(buf, "%f", Value);
	MessageBox(MainHwnd, buf, "Equity Notice", MB_OK);
}

// *************************************************************************
// *			SetMainWinCentre:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
bool CL64_App::SetMainWinCentre(void) const
{
	int ClientWidth_X = GetSystemMetrics(SM_CXSCREEN);
	int ClientHeight_Y = GetSystemMetrics(SM_CYSCREEN);

	RECT Control;
	GetClientRect(MainHwnd, &Control);

	int AllX = (ClientWidth_X / 2) - (Control.right / 2) - 10;
	int AllY = (ClientHeight_Y / 2) - (Control.bottom / 2) - 30;

	SetWindowPos(MainHwnd, NULL, AllX, AllY, 0, 0, SWP_NOSIZE);

	return 1;
}

// *************************************************************************
// *			Flash_Window:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_App::Flash_Window()
{
	FlashWindow(App->MainHwnd, true);
}

// *************************************************************************
// *						Debug_Text Terry Flanigan					   *
// *************************************************************************
void CL64_App::Debug_Text()
{
	MessageBox(App->MainHwnd, "Here", "Debug", MB_OK);
}

// *************************************************************************
// *			SetBrushes_Fonts:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_App::SetBrushes_Fonts(void)
{
	AppBackground = CreateSolidBrush(RGB(213, 222, 242));
	//DialogBackGround = CreateSolidBrush(RGB(208, 230, 252));
	//AppTest = CreateSolidBrush(RGB(218, 240, 255));

	BlackBrush = CreateSolidBrush(RGB(0, 0, 0));

	////BannerBrush = CreateSolidBrush(RGB(200, 200, 0));

	//Brush_White = CreateSolidBrush(RGB(255, 255, 255));

	//Brush_Panel = CreateSolidBrush(RGB(218, 240, 255));

	//Brush_Red = CreateSolidBrush(RGB(252, 85, 63));
	//Brush_Green = CreateSolidBrush(RGB(0, 255, 0));
	//Brush_Blue = CreateSolidBrush(RGB(0, 180, 255));

	//Brush_But_Normal = CreateSolidBrush(RGB(255, 255, 150));
	//Brush_But_Hover = CreateSolidBrush(RGB(255, 255, 200));
	//Brush_But_Pressed = CreateSolidBrush(RGB(240, 240, 190));
	//Brush_Tabs = CreateSolidBrush(RGB(255, 255, 255));
	//Brush_Tabs_UnSelected = CreateSolidBrush(RGB(190, 190, 190));

	//Brush_Button = CreateSolidBrush(RGB(65, 147, 245));
	//Brush_Button_Hover = CreateSolidBrush(RGB(153, 204, 255));

	//Font_CB12 = CreateFont(-12, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	Font_CB15 = CreateFont(-15, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	//Font_CB18 = CreateFont(-18, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");

	//Font_Arial20 = CreateFont(-20, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Arial");
	//Font_CB15_Bold = CreateFont(-15, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");

	//Font_Banner = CreateFont(-30, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Aerial Black");
}
