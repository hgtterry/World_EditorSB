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
#include "resource.h"
#include "CL64_SplitterViews.h"

#define	TOP_POS					0
#define	BOTTOM_POS				400
#define	SPLITTER_BAR_WIDTH		5
#define WIDTH_ADJUST			2
#define BOTTOM_POS_BOTLEFT		5

CL64_SplitterViews::CL64_SplitterViews(void)
{
	Left_Window_Hwnd =	nullptr;
	Right_Window_Hwnd = nullptr;

	Bottom_Left_Hwnd =	nullptr;
	Bottom_Right_Hwnd = nullptr;

	LEFT_WINDOW_WIDTH = 500;

	LEFT_WINDOW_DEPTH = 215;
	TOP_POS_BOTLEFT = 215;

	nleftWnd_width = LEFT_WINDOW_WIDTH;
	nleftWnd_Depth = LEFT_WINDOW_DEPTH;

	RIGHT_MINIMUM_SPACE = 1000;
	LEFT_MINIMUM_SPACE = 15;

	//App->Say("CL64_SplitterViews Created");
}

CL64_SplitterViews::~CL64_SplitterViews(void)
{
}

// *************************************************************************
// *	  			Init_Views:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_SplitterViews::Init_Views()
{
	RECT rect;
	GetClientRect(App->Fdlg, &rect);

	LEFT_WINDOW_WIDTH = rect.right / 2;
	nleftWnd_width = rect.right / 2;

	LEFT_WINDOW_DEPTH = rect.bottom / 2;
	TOP_POS_BOTLEFT = rect.bottom / 2;
	nleftWnd_Depth = LEFT_WINDOW_DEPTH;

	RIGHT_MINIMUM_SPACE = rect.right - 15;
	LEFT_MINIMUM_SPACE = rect.left + 15;
}

// *************************************************************************
// *	  	Create_Left_Window:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_SplitterViews::Create_Left_Window()
{
	Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)ID_VIEW_TOPVIEW, App->Fdlg, (DLGPROC)Top_Left_Window_Proc);
}

// *************************************************************************
// *			Left_Window_Proc:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_SplitterViews::Top_Left_Window_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//SendDlgItemMessage(hDlg, IDC_STBANNERTL, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		//App->CL_CView->OnViewType(ID_VIEW_TOPVIEW);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		/*if (GetDlgItem(hDlg, IDC_STTEST) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, RGB(0, 255, 0));
			return (UINT)App->Brush_White;
		}*/
		return FALSE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_PAINT:
	{
		//App->CL_CView->OnViewType(ID_VIEW_TOPVIEW);
		//App->CL_SplitterViews->MoveCamera(hDlg);
		return 0;
	}

	case WM_COMMAND:
	{
		/*if (LOWORD(wParam) == IDC_BT_TEST)
		{
			Debug
				return TRUE;
		}*/

		return FALSE;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  	Create_Right_Window:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_SplitterViews::Create_Right_Window()
{
	RECT rect;

	Right_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)ID_VIEW_SIDEVIEW, App->Fdlg, (DLGPROC)Top_Right_Window_Proc);

	GetClientRect(Right_Window_Hwnd, &rect);

	SetWindowPos(Right_Window_Hwnd, NULL,
		rect.left + LEFT_WINDOW_WIDTH + SPLITTER_BAR_WIDTH,
		rect.top + TOP_POS,
		rect.right - (rect.left + LEFT_WINDOW_WIDTH + SPLITTER_BAR_WIDTH),
		(rect.bottom - rect.top) - (TOP_POS + BOTTOM_POS),
		SWP_NOZORDER);
}

// *************************************************************************
// *			Right_Window_Proc:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_SplitterViews::Top_Right_Window_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//App->CL_CView->OnViewType(ID_VIEW_SIDEVIEW);
		return TRUE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_PAINT:
	{
		//App->CL_CView->OnViewType(ID_VIEW_SIDEVIEW);
		//App->CL_SplitterViews->MoveCamera(hDlg);
		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  Create_Bottom_Left_Window:- Terry and Hazel Flanigan 2024		   *
// *************************************************************************
void CL64_SplitterViews::Create_Bottom_Left_Window()
{
	RECT rect;

	Bottom_Left_Hwnd = CreateDialog(App->hInst, (LPCTSTR)ID_VIEW_FRONTVIEW, App->Fdlg, (DLGPROC)Bottom_Left_Proc);

	GetClientRect(Bottom_Left_Hwnd, &rect);

	SetWindowPos(Bottom_Left_Hwnd, NULL,
		rect.left, rect.top + TOP_POS_BOTLEFT,
		LEFT_WINDOW_WIDTH,
		(rect.bottom - rect.top) - (TOP_POS_BOTLEFT + BOTTOM_POS_BOTLEFT),
		SWP_NOZORDER);
}

// *************************************************************************
// *			Bottom_Left_Proc:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_SplitterViews::Bottom_Left_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//App->CL_CView->OnViewType(ID_VIEW_FRONTVIEW);
		return TRUE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_PAINT:
	{
		//App->CL_SplitterViews->MoveCamera(hDlg);
		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *			Resize_Windowns:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_SplitterViews::Resize_Windows(HWND hDlg, int NewWidth, int NewDepth)
{
	//HWND Temp = NULL;
	RECT rect;
	GetClientRect(hDlg, &rect);

	int Top_Windows_Top_Y = 8;
	int Left_Windows_Start_X = 0;

	int NewDepth_Depth = NewDepth - 11;

	MoveWindow(App->CL_SplitterViews->Left_Window_Hwnd,
		Left_Windows_Start_X,
		Top_Windows_Top_Y,
		rect.left + (NewWidth - WIDTH_ADJUST),
		NewDepth_Depth,
		FALSE);

	//Temp = GetDlgItem(App->CL_SplitterViews->Left_Window_Hwnd, IDC_STBANNERTL);
	//MoveWindow(Temp,
	//	0,
	//	0,
	//	rect.left + (NewWidth - WIDTH_ADJUST),
	//	15,
	//	FALSE);



	MoveWindow(App->CL_SplitterViews->Right_Window_Hwnd,
		Left_Windows_Start_X + NewWidth + WIDTH_ADJUST,
		Top_Windows_Top_Y,
		rect.right - (NewWidth + WIDTH_ADJUST),
		NewDepth_Depth,
		FALSE);

	// Bottom Windows
	MoveWindow(App->CL_SplitterViews->Bottom_Left_Hwnd,
		Left_Windows_Start_X,
		rect.top + NewDepth,
		Left_Windows_Start_X + (NewWidth - WIDTH_ADJUST),
		rect.bottom - (NewDepth + BOTTOM_POS_BOTLEFT),
		FALSE);

	MoveWindow(App->ViewGLhWnd,
		Left_Windows_Start_X + NewWidth + WIDTH_ADJUST,
		rect.top + NewDepth,
		rect.right - (NewWidth + WIDTH_ADJUST),
		rect.bottom - (NewDepth + BOTTOM_POS_BOTLEFT),
		FALSE);

	RedrawWindow(App->MainHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return 1;
}
