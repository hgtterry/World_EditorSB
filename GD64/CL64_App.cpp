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
#include "resource.h"
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
	CL_Scene =			nullptr;
	CL_Importers =		nullptr;
	CL_File_IO =		nullptr;
	CL_ImGui =			nullptr;
	CL_Textures =		nullptr;
	CL_TopDlg =			nullptr;
	CL_Panels =			nullptr;
	CL_Converters =		nullptr;
	CL_Bullet =			nullptr;
	CL_Player =			nullptr;
	CL_Demos =			nullptr;

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

	Hnd_HairOn_Bmp = NULL;
	Hnd_HairOff_Bmp = NULL;
	Hnd_ModelInfo_Bmp = NULL;
	Hnd_ModelInfoOn_Bmp = NULL;
	Hnd_GridOn_Bmp = NULL;
	Hnd_GridOff_Bmp = NULL;

	BlackBrush = 0;
	Brush_But_Normal = 0;
	Brush_But_Normal = 0;
	Brush_But_Hover = 0;
	Brush_But_Pressed = 0;
	Brush_Green = 0;
	Brush_Tabs = 0;
	Brush_Tabs_UnSelected = 0;

	Debug_App = 1;

	Block_Mouse_Buttons = 0;
}

CL64_App::~CL64_App(void)
{
	//Say("Destroyed");
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
	CL_Camera =			new	CL64_Camera();
	CL_Ini_File =		new CL64_Ini_File();
	CL_Preferences =	new CL64_Preferences();
	CL_Assimp =			new CL64_Assimp();
	CL_Scene =			new CL64_Scene();
	CL_Importers =		new CL64_Importers();
	CL_File_IO =		new CL64_File_IO();
	CL_ImGui =			new CL64_ImGui();
	CL_Textures =		new CL64_Textures();
	CL_TopDlg =			new CL64_TopDlg();
	CL_Panels =			new CL64_Panels();
	CL_Converters =		new CL64_Converters();
	CL_Bullet =			new CL64_Bullet();
	CL_Player =			new CL64_Player();
	CL_Demos =			new CL64_Demos();

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
// *			Debug_Text:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_App::Debug_Text()
{
	MessageBox(App->MainHwnd, "Here", "Debug", MB_OK);
}

// *************************************************************************
// *		LoadProgramResource:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
void CL64_App::LoadProgramResource(void)
{
	
	Hnd_GridOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_GRIDON);
	Hnd_GridOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_GRIDOFF);

	Hnd_HairOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HAIRON);
	Hnd_HairOff_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_HAIROFF);

	/*Hnd_ModelInfo_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TB_MODELDATA);
	Hnd_ModelInfoOn_Bmp = LoadBitmap(hInst, (LPCTSTR)IDB_TB_MODELDATAON);*/
}

// *************************************************************************
// *			SetBrushes_Fonts:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_App::SetBrushes_Fonts(void)
{
	AppBackground = CreateSolidBrush(RGB(213, 222, 242));
	BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	Brush_But_Normal = CreateSolidBrush(RGB(255, 255, 180));
	
	Brush_Green = CreateSolidBrush(RGB(0, 255, 0));
	
	Brush_But_Normal = CreateSolidBrush(RGB(255, 255, 150));
	Brush_But_Hover = CreateSolidBrush(RGB(255, 255, 200));
	Brush_But_Pressed = CreateSolidBrush(RGB(240, 240, 190));
	Brush_Tabs = CreateSolidBrush(RGB(255, 255, 255));
	Brush_Tabs_UnSelected = CreateSolidBrush(RGB(190, 190, 190));

	Font_CB15 = CreateFont(-15, 0, 0, 0, 0, 0, 0, 0, 0, OUT_TT_ONLY_PRECIS, 0, 0, 0, "Courier Black");
	
}

// *************************************************************************
// *	Custom_Button_Toggle_Tabs:- Terry and Hazel Flanigan 2024  	  	   *
// *************************************************************************
bool CL64_App::Custom_Button_Toggle_Tabs(LPNMCUSTOMDRAW item, bool Toggle)
{
	static HBRUSH defaultbrush = NULL;
	static HBRUSH hotbrush = NULL;
	static HBRUSH selectbrush = NULL;

	{
		if (item->uItemState & CDIS_HOT) //Our mouse is over the button
		{
			//Select our colour when the mouse hovers our button

			if (Toggle == 1)
			{
				hotbrush = CreateGradientBrush(RGB(240, 240, 240), RGB(240, 240, 240), item);
			}
			else
			{
				//hotbrush = Brush_Tabs_UnSelected; // Unselected 
				hotbrush = CreateGradientBrush(RGB(240, 240, 240), RGB(240, 240, 240), item);;
			}

			HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(240, 240, 240));

			HGDIOBJ old_pen = SelectObject(item->hdc, pen);
			HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

			RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 0, 0);

			SelectObject(item->hdc, old_pen);
			SelectObject(item->hdc, old_brush);
			DeleteObject(pen);

			return CDRF_DODEFAULT;
		}

		//Select our colour when our button is doing nothing

		if (Toggle == 1)
		{
			defaultbrush = App->Brush_Tabs; // Selected
		}
		else
		{
			defaultbrush = Brush_Tabs_UnSelected; // Unselected 
		}

		HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 255, 255));

		HGDIOBJ old_pen = SelectObject(item->hdc, pen);
		HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 0, 0);

		SelectObject(item->hdc, old_pen);
		SelectObject(item->hdc, old_brush);
		DeleteObject(pen);

		return CDRF_DODEFAULT;
	}

	return CDRF_DODEFAULT;
}

// *************************************************************************
// *		Custom_Button_Normal:- Terry and Hazel Flanigan 2024  	  	   *
// *************************************************************************
bool CL64_App::Custom_Button_Normal(LPNMCUSTOMDRAW item)
{

	if (item->uItemState & CDIS_SELECTED) // Push Down
	{
		//Create pen for button border
		HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

		//Select our brush into hDC
		HGDIOBJ old_pen = SelectObject(item->hdc, pen);
		HGDIOBJ old_brush = SelectObject(item->hdc, App->Brush_But_Pressed);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		//Clean up
		SelectObject(item->hdc, old_pen);
		SelectObject(item->hdc, old_brush);
		DeleteObject(pen);

		return CDRF_DODEFAULT;
	}
	else
	{
		if (item->uItemState & CDIS_HOT) //Our mouse is over the button
		{

			HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 255, 0));

			HGDIOBJ old_pen = SelectObject(item->hdc, pen);
			HGDIOBJ old_brush = SelectObject(item->hdc, App->Brush_But_Hover);

			RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

			SelectObject(item->hdc, old_pen);
			SelectObject(item->hdc, old_brush);
			DeleteObject(pen);

			return CDRF_DODEFAULT;
		}

		HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0)); // Idle 

		HGDIOBJ old_pen = SelectObject(item->hdc, pen);
		HGDIOBJ old_brush = SelectObject(item->hdc, App->Brush_But_Normal);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);

		SelectObject(item->hdc, old_pen);
		SelectObject(item->hdc, old_brush);
		DeleteObject(pen);

		return CDRF_DODEFAULT;
	}

	return CDRF_DODEFAULT;
}

// *************************************************************************
// *		Custom_Button_Toggle:- Terry and Hazel Flanigan 2024   	 	   *
// *************************************************************************
bool CL64_App::Custom_Button_Toggle(LPNMCUSTOMDRAW item, bool Toggle)
{
	static HBRUSH defaultbrush = NULL;
	static HBRUSH hotbrush = NULL;
	static HBRUSH selectbrush = NULL;

	{
		if (item->uItemState & CDIS_HOT) //Our mouse is over the button
		{
			//Select our colour when the mouse hovers our button

			if (Toggle == 1)
			{
				hotbrush = CreateGradientBrush(RGB(0, 240, 0), RGB(0, 240, 0), item);
			}
			else
			{
				hotbrush = CreateGradientBrush(RGB(240, 240, 240), RGB(240, 240, 240), item);;
			}

			HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

			HGDIOBJ old_pen = SelectObject(item->hdc, pen);
			HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

			RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 0, 0);

			SelectObject(item->hdc, old_pen);
			SelectObject(item->hdc, old_brush);
			DeleteObject(pen);

			return CDRF_DODEFAULT;
		}

		//Select our colour when our button is doing nothing

		if (Toggle == 1)
		{
			defaultbrush = App->Brush_Green;
		}
		else
		{
			defaultbrush = Brush_But_Normal;
		}

		HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0));

		HGDIOBJ old_pen = SelectObject(item->hdc, pen);
		HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

		RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 0, 0);

		SelectObject(item->hdc, old_pen);
		SelectObject(item->hdc, old_brush);
		DeleteObject(pen);

		return CDRF_DODEFAULT;
	}

	return CDRF_DODEFAULT;
}

// *************************************************************************
// *						CreateGradientBrush					 	 	   *
// *************************************************************************
HBRUSH CL64_App::CreateGradientBrush(COLORREF top, COLORREF bottom, LPNMCUSTOMDRAW item)
{
	HBRUSH Brush = NULL;
	HDC hdcmem = CreateCompatibleDC(item->hdc);
	HBITMAP hbitmap = CreateCompatibleBitmap(item->hdc, item->rc.right - item->rc.left, item->rc.bottom - item->rc.top);
	SelectObject(hdcmem, hbitmap);

	int r1 = GetRValue(top), r2 = GetRValue(bottom), g1 = GetGValue(top), g2 = GetGValue(bottom), b1 = GetBValue(top), b2 = GetBValue(bottom);
	for (int i = 0; i < item->rc.bottom - item->rc.top; i++)
	{
		RECT temp;
		int r, g, b;
		r = int(r1 + double(i * (r2 - r1) / item->rc.bottom - item->rc.top));
		g = int(g1 + double(i * (g2 - g1) / item->rc.bottom - item->rc.top));
		b = int(b1 + double(i * (b2 - b1) / item->rc.bottom - item->rc.top));
		Brush = CreateSolidBrush(RGB(r, g, b));
		temp.left = 0;
		temp.top = i;
		temp.right = item->rc.right - item->rc.left;
		temp.bottom = i + 1;
		FillRect(hdcmem, &temp, Brush);
		DeleteObject(Brush);
	}

	HBRUSH pattern = CreatePatternBrush(hbitmap);

	DeleteDC(hdcmem);
	DeleteObject(hbitmap);

	return pattern;
}
