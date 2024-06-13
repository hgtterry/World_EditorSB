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

	Do_Width = 0;
	Do_Depth = 0;
	Do_All = 0;

	Max_Window = 0;
}

CL64_SplitterViews::~CL64_SplitterViews(void)
{
}

// *************************************************************************
// *			ViewerMain_Proc:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
LRESULT CALLBACK CL64_SplitterViews::ViewerMain_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR	hcSizeEW = NULL;
	static HCURSOR	hcSizeNS = NULL;
	static HCURSOR	hcBoth = NULL;

	static  BOOL        xSizing;
	static  BOOL        ySizing;

	static UINT			nwnd_resizing = 0;

	RECT rect;

	switch (message)
	{

	case WM_INITDIALOG:
	{
		hcSizeEW = LoadCursor(NULL, IDC_SIZEWE);
		hcSizeNS = LoadCursor(NULL, IDC_SIZENS);
		hcBoth = LoadCursor(NULL, IDC_SIZEALL);
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_SIZE:
	{
		if (App->CL_SplitterViews->Max_Window == 0)
		{
			App->CL_SplitterViews->Init_Views();
			App->CL_SplitterViews->Resize_Windows(hDlg, App->CL_SplitterViews->nleftWnd_width, App->CL_SplitterViews->nleftWnd_Depth);

			GetClientRect(hDlg, &rect);
			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		else
		{
			App->CL_SplitterViews->Max_3D_win();
			//App->CL_SplitterViews->Resize_Fldg();
			//App->CL_SplitterViews->Resize_OgreWin();
		}

		App->CL_Panels->Resize_TopDlg();
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		int                 xPos;
		int                 yPos;

		// Varible used to get the mouse cursor x and y co-ordinates
		xPos = (int)LOWORD(lParam);
		yPos = (int)HIWORD(lParam);

		// Checks whether the mouse is over the splitter window
		xSizing = (xPos > App->CL_SplitterViews->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_SplitterViews->nleftWnd_width + SPLITTER_BAR_WIDTH);
		ySizing = (yPos > App->CL_SplitterViews->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_SplitterViews->nleftWnd_Depth + SPLITTER_BAR_WIDTH);

		if (xSizing && ySizing == 0)
		{
			// Api to capture mouse input
			SetCapture(hDlg);
			if (xSizing)
			{
				SetCursor(hcSizeEW);
			}
		}

		if (ySizing && xSizing == 0)
		{
			// Api to capture mouse input
			SetCapture(hDlg);
			if (ySizing)
			{
				SetCursor(hcSizeNS);
			}
		}

		if (xSizing && ySizing)
		{
			SetCapture(hDlg);
			if (ySizing)
			{
				SetCursor(hcBoth);;
			}
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		if (xSizing)
		{

			RECT    focusrect;
			HDC     hdc;

			ReleaseCapture();

			hdc = GetDC(hDlg);
			GetClientRect(hDlg, &rect);

			if (xSizing)
			{
				SetRect(&focusrect, App->CL_SplitterViews->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
					App->CL_SplitterViews->nleftWnd_width + WIDTH_ADJUST,
					rect.bottom - 80);

				DrawFocusRect(hdc, &focusrect);

				xSizing = FALSE;
			}

			ReleaseDC(hDlg, hdc);
		}

		if (ySizing)
		{
			
			HDC     hdc;

			ReleaseCapture();

			hdc = GetDC(hDlg);
			GetClientRect(hDlg, &rect);

			if (ySizing)
			{
				ySizing = FALSE;
			}

			ReleaseDC(hDlg, hdc);
		}
		
		App->CL_SplitterViews->Resize_Windows(hDlg, App->CL_SplitterViews->nleftWnd_width, App->CL_SplitterViews->nleftWnd_Depth);
		App->CL_SplitterViews->Resize_OgreWin();

		return 1;
	}

	case WM_MOUSEMOVE:
	{

		int   xPos;
		int   yPos;

		// Get the x and y co-ordinates of the mouse
		xPos = (int)LOWORD(lParam);
		yPos = (int)HIWORD(lParam);

		if (xPos < App->CL_SplitterViews->LEFT_MINIMUM_SPACE || xPos > App->CL_SplitterViews->RIGHT_MINIMUM_SPACE)
		{
			return 0;
		}

		// Checks if the left button is pressed during dragging the splitter
		if (wParam == MK_LBUTTON)
		{

			if (xSizing && App->CL_SplitterViews->Do_Width == 1)
			{
				RECT    focusrect;
				HDC     hdc;

				hdc = GetDC(hDlg);
				GetClientRect(hDlg, &rect);

				if (xSizing && App->CL_SplitterViews->Do_Width == 1)
				{
					SetRect(&focusrect, App->CL_SplitterViews->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
						App->CL_SplitterViews->nleftWnd_width + WIDTH_ADJUST,
						rect.bottom - 6);

					DrawFocusRect(hdc, &focusrect);

					App->CL_SplitterViews->nleftWnd_width = xPos;

					SetRect(&focusrect, App->CL_SplitterViews->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
						App->CL_SplitterViews->nleftWnd_width + WIDTH_ADJUST,
						rect.bottom - 6);

					DrawFocusRect(hdc, &focusrect);
				}

				ReleaseDC(hDlg, hdc);
			}

			if (ySizing && App->CL_SplitterViews->Do_Depth == 1)
			{
				RECT    focusrect;
				HDC     hdc;

				hdc = GetDC(hDlg);
				GetClientRect(hDlg, &rect);

				if (ySizing && App->CL_SplitterViews->Do_Depth == 1)
				{
					SetRect(&focusrect, 0, App->CL_SplitterViews->nleftWnd_Depth, rect.right, App->CL_SplitterViews->nleftWnd_Depth + (WIDTH_ADJUST * 2));

					DrawFocusRect(hdc, &focusrect);

					App->CL_SplitterViews->nleftWnd_Depth = yPos;

					SetRect(&focusrect, 0, App->CL_SplitterViews->nleftWnd_Depth, rect.right, App->CL_SplitterViews->nleftWnd_Depth + (WIDTH_ADJUST * 2));

					DrawFocusRect(hdc, &focusrect);
				}

				ReleaseDC(hDlg, hdc);
			}

		}

		if ((xPos > App->CL_SplitterViews->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_SplitterViews->nleftWnd_width + SPLITTER_BAR_WIDTH))
		{
			if (App->CL_SplitterViews->Do_All == 0)
			{
				SetCursor(hcSizeEW);
			}

			App->CL_SplitterViews->Do_Width = 1;
		}
		else
		{
			App->CL_SplitterViews->Do_Width = 0;
		}

		if ((yPos > App->CL_SplitterViews->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_SplitterViews->nleftWnd_Depth + SPLITTER_BAR_WIDTH))
		{
			if (App->CL_SplitterViews->Do_All == 0)
			{
				SetCursor(hcSizeNS);
			}

			App->CL_SplitterViews->Do_Depth = 1;
		}
		else
		{
			App->CL_SplitterViews->Do_Depth = 0;
		}

		if (App->CL_SplitterViews->Do_Width == 1 && App->CL_SplitterViews->Do_Depth == 1)
		{
			SetCursor(hcBoth);
			App->CL_SplitterViews->Do_All = 1;
		}
		else
		{
			App->CL_SplitterViews->Do_All = 0;
		}


		return 1;
	}

	case WM_COMMAND:
	{

	}
	break;
	}
	return FALSE;
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
// *	  	Create_Top_Left_Win:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_SplitterViews::Create_Top_Left_Win()
{
	Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)ID_VIEW_TOPVIEW, App->Fdlg, (DLGPROC)Top_Left_Win_Proc);
}

// *************************************************************************
// *			Top_Left_Win_Proc:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_SplitterViews::Top_Left_Win_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//App->CL_CView->OnViewType(ID_VIEW_TOPVIEW);

		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
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
		return FALSE;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  	Create_Top_Right_Win:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_SplitterViews::Create_Top_Right_Win()
{
	RECT rect;

	Right_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)ID_VIEW_SIDEVIEW, App->Fdlg, (DLGPROC)Top_Right_Win_Proc);

	GetClientRect(Right_Window_Hwnd, &rect);

	SetWindowPos(Right_Window_Hwnd, NULL,
		rect.left + LEFT_WINDOW_WIDTH + SPLITTER_BAR_WIDTH,
		rect.top + TOP_POS,
		rect.right - (rect.left + LEFT_WINDOW_WIDTH + SPLITTER_BAR_WIDTH),
		(rect.bottom - rect.top) - (TOP_POS + BOTTOM_POS),
		SWP_NOZORDER);
}

// *************************************************************************
// *			Top_Right_Win_Proc:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
LRESULT CALLBACK CL64_SplitterViews::Top_Right_Win_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
// *			Resize_Windows:- Terry and Hazel Flanigan 2024			   *
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

// *************************************************************************
// *			Resize_OgreWin:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_SplitterViews::Resize_OgreWin(void)
{
	if (App->OgreStarted == 1)
	{
		RECT rect;
		GetClientRect(App->ViewGLhWnd, &rect);

		if (App->CL_Ogre->mCamera != 0)
		{
			App->CL_Ogre->mWindow->windowMovedOrResized();
			App->CL_Ogre->mCamera->setAspectRatio((Ogre::Real)App->CL_Ogre->mWindow->getWidth() / (Ogre::Real)App->CL_Ogre->mWindow->getHeight());
			App->CL_Ogre->camNode->yaw(Radian(0));
			Root::getSingletonPtr()->renderOneFrame();
		}

		//App->CL_Grid->Reset_View();
	}

	return 1;
}

// *************************************************************************
// *				Max_3D_win:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
void CL64_SplitterViews::Max_3D_win(void)
{
	if (App->OgreStarted == 1)
	{
		RECT rect;
		GetClientRect(App->Fdlg, &rect);
		MoveWindow(App->ViewGLhWnd, 2, 2, rect.right-2, rect.bottom-2, TRUE);
		App->CL_SplitterViews->Resize_OgreWin();

		Max_Window = 1;
	}
}
