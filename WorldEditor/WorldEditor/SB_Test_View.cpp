#include "stdafx.h"
#include "resource.h"
#include "AB_App.h"
#include "SB_Test_View.h"

#include "FUSIONDoc.h"
#include "FUSIONView.h"

#pragma warning(disable : 4100)

#define fdoc_SHOW_ALL_GROUPS -1
#define	M_PI		((geFloat)3.14159265358979323846f)

#define	TOP_POS					8
#define	BOTTOM_POS				400
#define	SPLITTER_BAR_WIDTH		5
#define WIDTH_ADJUST			2

#define BOTTOM_POS_BOTLEFT		5

typedef struct tagBrushDrawData
{
	const Box3d* pViewBox;
	HDC pDC;
	ViewVars* v;
	int GroupId;
	CFusionDoc* pDoc;
	BrushFlagTest FlagTest;
	Ogre::uint32		Color;
} BrushDrawData;

SB_Test_View::SB_Test_View()
{
	Spliter_Main_Hwnd = NULL;
	Left_Window_Hwnd = NULL;
	Right_Window_Hwnd = NULL;

	Bottom_Left_Hwnd = NULL;
	Bottom_Right_Hwnd = NULL;

	LEFT_WINDOW_WIDTH = 500;

	LEFT_WINDOW_DEPTH = 215;
	TOP_POS_BOTLEFT = 215;

	RIGHT_MINIMUM_SPACE = 1000;
	LEFT_MINIMUM_SPACE = 15;

	nleftWnd_width = LEFT_WINDOW_WIDTH;
	nleftWnd_Depth = LEFT_WINDOW_DEPTH;

	Do_Width = 0;
	Do_Depth = 0;
	Do_All = 0;
	m_View = 0;
	BackGround_Brush = CreateSolidBrush(RGB(64, 64, 64));
}

SB_Test_View::~SB_Test_View()
{
}

// *************************************************************************
// *	  	Start_Main_View_Dlg:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Test_View::Start_Main_View_Dlg()
{
	Start_Splitter();
	return 1;
}

// *************************************************************************
// *	  			Init_Views:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Test_View::Init_Views()
{
	RECT rect;
	GetClientRect(Spliter_Main_Hwnd, &rect);

	LEFT_WINDOW_WIDTH = rect.right / 2;
	nleftWnd_width = rect.right / 2;

	LEFT_WINDOW_DEPTH = rect.bottom / 2;
	TOP_POS_BOTLEFT = rect.bottom / 2;
	nleftWnd_Depth = LEFT_WINDOW_DEPTH;

	RIGHT_MINIMUM_SPACE = rect.right - 15;
	LEFT_MINIMUM_SPACE = rect.left + 15;
	return 1;
}

// *************************************************************************
// *	  			Start_Splitter:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Test_View::Start_Splitter()
{
	CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TESTDIALOG, App->MainHwnd, (DLGPROC)Splitter_Proc);
}

// *************************************************************************
// *			Splitter_Proc:- Terry and Hazel Flanigan 2023 			   *
// *************************************************************************
LRESULT CALLBACK SB_Test_View::Splitter_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

		App->CL_Test_View->Spliter_Main_Hwnd = hDlg;

		App->CL_Test_View->Init_Views();
		App->CL_Test_View->Create_Left_Window();
		App->CL_Test_View->Create_Right_Window();
		App->CL_Test_View->Create_Bottom_Left_Window();
		App->CL_Test_View->Create_Bottom_Right_Window();
		App->CL_Test_View->Resize_Windows(App->CL_Test_View->Spliter_Main_Hwnd, App->CL_Test_View->nleftWnd_width, App->CL_Test_View->nleftWnd_Depth);
		return TRUE;
	}

	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)App->AppBackground);
		EndPaint(hDlg, &ps);
	}

	case WM_NOTIFY:
	{
		return CDRF_DODEFAULT;
	}

	case WM_SIZE:
	{
		App->CL_Test_View->Init_Views();
		App->CL_Test_View->Resize_Windows(hDlg, App->CL_Test_View->nleftWnd_width, App->CL_Test_View->nleftWnd_Depth);

		GetClientRect(hDlg, &rect);
		RedrawWindow(App->CL_Test_View->Spliter_Main_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
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
		xSizing = (xPos > App->CL_Test_View->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_Test_View->nleftWnd_width + SPLITTER_BAR_WIDTH);
		ySizing = (yPos > App->CL_Test_View->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_Test_View->nleftWnd_Depth + SPLITTER_BAR_WIDTH);

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
				SetRect(&focusrect, App->CL_Test_View->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
					App->CL_Test_View->nleftWnd_width + WIDTH_ADJUST,
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

		App->CL_Test_View->Resize_Windows(hDlg, App->CL_Test_View->nleftWnd_width, App->CL_Test_View->nleftWnd_Depth);

		return 1;
	}

	case WM_MOUSEMOVE:
	{

		int   xPos;
		int   yPos;

		// Get the x and y co-ordinates of the mouse
		xPos = (int)LOWORD(lParam);
		yPos = (int)HIWORD(lParam);

		if (xPos < App->CL_Test_View->LEFT_MINIMUM_SPACE || xPos > App->CL_Test_View->RIGHT_MINIMUM_SPACE)
		{
			return 0;
		}

		// Checks if the left button is pressed during dragging the splitter
		if (wParam == MK_LBUTTON)
		{

			if (xSizing && App->CL_Test_View->Do_Width == 1)
			{
				RECT    focusrect;
				HDC     hdc;

				hdc = GetDC(hDlg);
				GetClientRect(hDlg, &rect);

				if (xSizing && App->CL_Test_View->Do_Width == 1)
				{
					SetRect(&focusrect, App->CL_Test_View->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
						App->CL_Test_View->nleftWnd_width + WIDTH_ADJUST,
						rect.bottom - 6);

					DrawFocusRect(hdc, &focusrect);

					App->CL_Test_View->nleftWnd_width = xPos;

					SetRect(&focusrect, App->CL_Test_View->nleftWnd_width - (WIDTH_ADJUST * 2), rect.top + TOP_POS,
						App->CL_Test_View->nleftWnd_width + WIDTH_ADJUST,
						rect.bottom - 6);

					DrawFocusRect(hdc, &focusrect);
				}

				ReleaseDC(hDlg, hdc);
			}

			if (ySizing && App->CL_Test_View->Do_Depth == 1)
			{
				RECT    focusrect;
				HDC     hdc;

				hdc = GetDC(hDlg);
				GetClientRect(hDlg, &rect);

				if (ySizing && App->CL_Test_View->Do_Depth == 1)
				{
					SetRect(&focusrect, 0, App->CL_Test_View->nleftWnd_Depth, rect.right, App->CL_Test_View->nleftWnd_Depth + (WIDTH_ADJUST * 2));

					DrawFocusRect(hdc, &focusrect);

					App->CL_Test_View->nleftWnd_Depth = yPos;

					SetRect(&focusrect, 0, App->CL_Test_View->nleftWnd_Depth, rect.right, App->CL_Test_View->nleftWnd_Depth + (WIDTH_ADJUST * 2));

					DrawFocusRect(hdc, &focusrect);
				}

				ReleaseDC(hDlg, hdc);
			}

		}

		if ((xPos > App->CL_Test_View->nleftWnd_width - SPLITTER_BAR_WIDTH && xPos < App->CL_Test_View->nleftWnd_width + SPLITTER_BAR_WIDTH))
		{
			if (App->CL_Test_View->Do_All == 0)
			{
				SetCursor(hcSizeEW);
			}

			App->CL_Test_View->Do_Width = 1;
		}
		else
		{
			App->CL_Test_View->Do_Width = 0;
		}

		if ((yPos > App->CL_Test_View->nleftWnd_Depth - SPLITTER_BAR_WIDTH && yPos < App->CL_Test_View->nleftWnd_Depth + SPLITTER_BAR_WIDTH))
		{
			if (App->CL_Test_View->Do_All == 0)
			{
				SetCursor(hcSizeNS);
			}

			App->CL_Test_View->Do_Depth = 1;
		}
		else
		{
			App->CL_Test_View->Do_Depth = 0;
		}

		if (App->CL_Test_View->Do_Width == 1 && App->CL_Test_View->Do_Depth == 1)
		{
			SetCursor(hcBoth);
			App->CL_Test_View->Do_All = 1;
		}
		else
		{
			App->CL_Test_View->Do_All = 0;
		}


		return 1;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	}

	break;

	}
	return FALSE;
}

// *************************************************************************
// *	  	Create_Left_Window:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Test_View::Create_Left_Window()
{
	Left_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_VIEW_TOPLEFT, Spliter_Main_Hwnd, (DLGPROC)Left_Window_Proc);
}

// *************************************************************************
// *			Left_Window_Proc:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
LRESULT CALLBACK SB_Test_View::Left_Window_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_PAINT:
	{
		App->CL_Test_View->m_View = 1;
		App->CL_Test_View->Draw_Screen(hDlg);
		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  	Create_Right_Window:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Test_View::Create_Right_Window()
{
	Right_Window_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_VIEW_TOPRIGHT, Spliter_Main_Hwnd, (DLGPROC)Right_Window_Proc);
}

// *************************************************************************
// *			Right_Window_Proc:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
LRESULT CALLBACK SB_Test_View::Right_Window_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_Test_View->BackGround_Brush;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_PAINT:
	{
		App->CL_Test_View->m_View = 2;
		App->CL_Test_View->Draw_Screen(hDlg);
		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  Create_Bottom_Left_Window:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Test_View::Create_Bottom_Left_Window()
{
	Bottom_Left_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_VIEW_BOTTOMLEFT, Spliter_Main_Hwnd, (DLGPROC)Bottom_Left_Proc);
}

// *************************************************************************
// *			Bottom_Left_Proc:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
LRESULT CALLBACK SB_Test_View::Bottom_Left_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_Test_View->BackGround_Brush;
	}

	case WM_ERASEBKGND:
	{
		return (LRESULT)1;
	}

	case WM_PAINT:
	{
		App->CL_Test_View->m_View = 3;
		App->CL_Test_View->Draw_Screen(hDlg);
		return 0;
	}

	}

	return FALSE;
}

// *************************************************************************
// *	  Create_Bottom_Right_Window:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void SB_Test_View::Create_Bottom_Right_Window()
{
	Bottom_Right_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_VIEW_BOTTOMRIGHT, Spliter_Main_Hwnd, (DLGPROC)Bottom_Right_Proc);
}

// *************************************************************************
// *			Bottom_Right_Proc:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
LRESULT CALLBACK SB_Test_View::Bottom_Right_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->CL_Test_View->BackGround_Brush;
	}
	}

	return FALSE;
}

// *************************************************************************
// *			Resize_Windowns:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
bool SB_Test_View::Resize_Windows(HWND hDlg, int NewWidth, int NewDepth)
{
	RECT rect;
	GetClientRect(hDlg, &rect);

	int Top_Windows_Top_Y = 8;
	int Left_Windows_Start_X = 0;

	int NewDepth_Depth = NewDepth - 11;

	MoveWindow(App->CL_Test_View->Left_Window_Hwnd,
		Left_Windows_Start_X,
		Top_Windows_Top_Y,
		rect.left + (NewWidth - WIDTH_ADJUST),
		NewDepth_Depth,
		FALSE);

	MoveWindow(App->CL_Test_View->Right_Window_Hwnd,
		Left_Windows_Start_X + NewWidth + WIDTH_ADJUST,
		Top_Windows_Top_Y,
		rect.right - (NewWidth + WIDTH_ADJUST),
		NewDepth_Depth,
		FALSE);

	// Bottom Windows
	MoveWindow(App->CL_Test_View->Bottom_Left_Hwnd,
		Left_Windows_Start_X,
		rect.top + NewDepth,
		Left_Windows_Start_X + (NewWidth - WIDTH_ADJUST),
		rect.bottom - (NewDepth + BOTTOM_POS_BOTLEFT),
		FALSE);

	MoveWindow(App->CL_Test_View->Bottom_Right_Hwnd,
		Left_Windows_Start_X + NewWidth + WIDTH_ADJUST,
		rect.top + NewDepth,
		rect.right - (NewWidth + WIDTH_ADJUST),
		rect.bottom - (NewDepth + BOTTOM_POS_BOTLEFT),
		FALSE);

	RedrawWindow(Spliter_Main_Hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return 1;
}

static void fdocDrawEntity(CEntity const* pEnt, ViewVars  const* v, HDC pDC, EntityTable const* pEntityDefs, BOOL	bShowUI)
{
#define ENTITY_SIZE (8.0f)  // 16" across
	geVec3d VecOrigin;
	geVec3d EntSizeWorld;	// entity size in world space

	POINT EntPosView;
	POINT EntSizeView;
	POINT EntWidthHeight;
	POINT OriginView;

	POINT TopLeft, BottomRight;
	POINT TopRight, BottomLeft;

	static const float COS45 = (float)cos(M_PI / 4.0f);
	static const float SIN45 = (float)sin(M_PI / 4.0f);
	static const float MCOS45 = (float)cos(-(M_PI / 4.0f));
	static const float MSIN45 = (float)sin(-(M_PI / 4.0f));

	// compute entity size in view coordinates
	geVec3d_Set(&EntSizeWorld, ENTITY_SIZE, ENTITY_SIZE, ENTITY_SIZE);
	EntSizeView = Render_OrthoWorldToView(v, &EntSizeWorld);
	geVec3d_Clear(&VecOrigin);
	OriginView = Render_OrthoWorldToView(v, &VecOrigin);
	// This one is the width and height of the Entity
	EntWidthHeight.x = std::max(OriginView.x, EntSizeView.x) - std::min(OriginView.x, EntSizeView.x);
	EntWidthHeight.y = std::max(OriginView.y, EntSizeView.y) - std::min(OriginView.y, EntSizeView.y);

	// This can have negative numbers
	EntSizeView.x -= OriginView.x;
	EntSizeView.y -= OriginView.y;

	// entity's position in the view
	EntPosView = Render_OrthoWorldToView(v, &(pEnt->mOrigin));

	{
		// Draw an X at the entity's position...
		TopLeft.x = EntPosView.x - EntSizeView.x;
		TopLeft.y = EntPosView.y - EntSizeView.y;
		BottomRight.x = EntPosView.x + EntSizeView.x;
		BottomRight.y = EntPosView.y + EntSizeView.y;
		TopRight.x = BottomRight.x;
		TopRight.y = TopLeft.y;
		BottomLeft.x = TopLeft.x;
		BottomLeft.y = BottomRight.y;

		MoveToEx(pDC, TopLeft.x, TopLeft.y,NULL);
		LineTo(pDC, BottomRight.x, BottomRight.y);

		MoveToEx(pDC, TopRight.x, TopRight.y, NULL);
		LineTo(pDC, BottomLeft.x, BottomLeft.y);

	}

	// and then show the aiming arrow and arc stuff...
	if (bShowUI)
	{
		POINT		ArcTopLeft, ArcBottomRight;
		POINT		ptDirSlope;		// Slope of the "Direction" line
		POINT		ptRotationPoint;	// Point near end of "Direction" line we rotate to get arrowhead points
		POINT		ptRelRotatePoint;	// Rotation points about zero
		POINT		ptPlus45;			// Final Arrowhead point
		POINT		ptMinus45;			// Final Arrowhead point
		POINT		ptStart;			// Start point for Arc
		POINT		ptEnd;				// End point of Arc
		float		fPercentIntoLine;	// Distance into Direction line for rotation point
		float		fDirLength;		// Direction line length
		float		fEntityLength;		// Entity length
		float		fAngleToTarget;	// Radians of arc midpoint
		geFloat		fRadius;
		geVec3d		Angles;
		geXForm3d	Xfm;
		geVec3d		VecTarg;
		float		fArc;
		POINT		LineEndView;
		geBoolean	bUIAvailable;

		// Get the Radius and the Angle  ONE of these must be present to show UI
		bUIAvailable = GE_FALSE;
		if (pEnt->GetRadius(&fRadius, pEntityDefs) == GE_FALSE)
			fRadius = 100.0f;
		else
			bUIAvailable = GE_TRUE;

		if (pEnt->GetAngles(&Angles, pEntityDefs) == GE_FALSE)
			geVec3d_Clear(&Angles);
		else
			bUIAvailable = GE_TRUE;

		if (bUIAvailable == GE_FALSE)
			return;

		// The camera angles are given in camera coordinates rather than
		// world coordinates (don't ask).
		// So we convert them here.
		if (pEnt->IsCamera())
		{
			geVec3d_Set(&Angles, Angles.Z, (-Angles.Y - M_PI / 2.0f), Angles.X);
		}

		geXForm3d_SetEulerAngles(&Xfm, &Angles);
		geVec3d_Set(&VecTarg, fRadius, 0.0f, 0.0f);
		geXForm3d_Transform(&Xfm, &VecTarg, &VecTarg);
		geVec3d_Add(&(pEnt->mOrigin), &VecTarg, &VecTarg);

		LineEndView = Render_OrthoWorldToView(v, &VecTarg);

		// Draw to the end point

		MoveToEx(pDC, EntPosView.x, EntPosView.y, NULL);
		LineTo(pDC, LineEndView.x, LineEndView.y);

		ptDirSlope.x = LineEndView.x - EntPosView.x;	// Slope of Direction line
		ptDirSlope.y = LineEndView.y - EntPosView.y;

		fDirLength = sqrt((float)(ptDirSlope.x * ptDirSlope.x) + (ptDirSlope.y * ptDirSlope.y));	// Length of Direction line
		fEntityLength = sqrt((float)(EntSizeView.x * EntSizeView.x) + (EntSizeView.y * EntSizeView.y));
		fEntityLength *= 2;	// Arrow 2x entity size
		fPercentIntoLine = 1.0f - (fEntityLength / fDirLength);
		ptRotationPoint.x = (long)(ptDirSlope.x * fPercentIntoLine);
		ptRotationPoint.y = (long)(ptDirSlope.y * fPercentIntoLine);
		ptRotationPoint.x += EntPosView.x;
		ptRotationPoint.y += EntPosView.y;

		ptRelRotatePoint.x = ptRotationPoint.x - LineEndView.x;
		ptRelRotatePoint.y = ptRotationPoint.y - LineEndView.y;

		ptPlus45.x = (long)(ptRelRotatePoint.x * COS45 - ptRelRotatePoint.y * SIN45);
		ptPlus45.y = (long)(ptRelRotatePoint.y * COS45 + ptRelRotatePoint.x * SIN45);
		ptMinus45.x = (long)(ptRelRotatePoint.x * MCOS45 - ptRelRotatePoint.y * MSIN45);
		ptMinus45.y = (long)(ptRelRotatePoint.y * MCOS45 + ptRelRotatePoint.x * MSIN45);

		ptPlus45.x += LineEndView.x;
		ptPlus45.y += LineEndView.y;
		ptMinus45.x += LineEndView.x;
		ptMinus45.y += LineEndView.y;

		LineTo(pDC, ptPlus45.x, ptPlus45.y);
		LineTo(pDC, ptMinus45.x, ptMinus45.y);
		LineTo(pDC, LineEndView.x, LineEndView.y);

		if (pEnt->GetArc(&fArc, pEntityDefs) == GE_FALSE)
		{
			fArc = 0.0f;	// All Directions
		}
		if (fArc != 0.0f)			// Draw the arc
		{
			fArc = 2 * M_PI - fArc;
			fArc /= 2.0f;	// We need half the angle
			EntSizeView.x *= 3;
			EntSizeView.y *= 3;
			EntWidthHeight.x *= 3;
			EntWidthHeight.y *= 3;
			// Arc BB is an enlarged Entity BB
			ArcTopLeft.x = EntPosView.x - EntSizeView.x;
			ArcTopLeft.y = EntPosView.y - EntSizeView.y;
			ArcBottomRight.x = EntPosView.x + EntSizeView.x;
			ArcBottomRight.y = EntPosView.y + EntSizeView.y;

			fAngleToTarget = atan2((float)ptDirSlope.y, ptDirSlope.x);	// Angle line leaves
			fAngleToTarget += M_PI;	// The other side is where the angle starts

			ptStart.x = (long)((EntWidthHeight.x) * cos(fAngleToTarget + fArc));
			ptStart.y = (long)((EntWidthHeight.y) * sin(fAngleToTarget + fArc));
			ptEnd.x = (long)((EntWidthHeight.x) * cos(fAngleToTarget - fArc));
			ptEnd.y = (long)((EntWidthHeight.y) * sin(fAngleToTarget - fArc));
			ptStart.x += EntPosView.x;
			ptStart.y += EntPosView.y;
			ptEnd.x += EntPosView.x;
			ptEnd.y += EntPosView.y;

			// If Start and end point are different
			if (!(ptStart.x == ptEnd.x && ptStart.y == ptEnd.y))
			{
				/*pDC->Arc
				(
					ArcTopLeft.x, ArcTopLeft.y, ArcBottomRight.x, ArcBottomRight.y,
					ptStart.x, ptStart.y,
					ptEnd.x, ptEnd.y
				);*/
			}

			// Draw the two rays out the same distance as the Direction
			ptStart.x = (long)((fDirLength)*cos(fAngleToTarget + fArc));
			ptStart.y = (long)((fDirLength)*sin(fAngleToTarget + fArc));
			ptStart.x += EntPosView.x;
			ptStart.y += EntPosView.y;
			/*pDC->MoveTo(EntPosView);
			pDC->LineTo(ptStart);*/

			ptEnd.x = (long)((fDirLength)*cos(fAngleToTarget - fArc));
			ptEnd.y = (long)((fDirLength)*sin(fAngleToTarget - fArc));
			ptEnd.x += EntPosView.x;
			ptEnd.y += EntPosView.y;
			/*pDC->MoveTo(EntPosView);
			pDC->LineTo(ptEnd);*/
		}// Arc for this entity exists
	}
}/* fdocDrawEntity */

static geBoolean fdocBrushNotDetail(const Brush* b)
{
	return !Brush_IsDetail(b);
}

struct tag_BrushList
{
	Brush* First;
	Brush* Last;
};

// *************************************************************************
// *						Draw_Screen Terry Flanigan		  			   *
// *************************************************************************
void SB_Test_View::Draw_Screen(HWND hwnd)
{
	App->Get_Current_Document();

	CMainFrame* m_pMainFrame;
	m_pMainFrame = (CMainFrame*)AfxGetMainWnd(); // MFC POO

	CFusionDoc* m_pDoc = (CFusionDoc*)m_pMainFrame->GetCurrentDoc();

	HDC			RealhDC;
	HDC			MemoryhDC;
	RECT		Rect;

	POSITION pos = m_pDoc->GetFirstViewPosition();
	CView* pView = m_pDoc->GetNextView(pos);
	CFusionView* pFusionView = (CFusionView*)pView;

	float GridSize = 2;
	float GridSnapSize = 2;

	GetClientRect(hwnd, &Rect);
	Rect.left--;
	Rect.bottom--;

	RealhDC = GetDC(hwnd);

	MemoryhDC = CreateCompatibleDC(RealhDC);

	GetClipBox(RealhDC, &Rect);

	HBITMAP OffScreenBitmap;

	OffScreenBitmap = CreateCompatibleBitmap(RealhDC, Rect.right - Rect.left, Rect.bottom - Rect.top);

	SelectObject(MemoryhDC, OffScreenBitmap);

	geVec3d		XTemp;
	Box3d ViewBox;
	Box3d_SetBogusBounds(&ViewBox);
	Render_ViewToWorld(pFusionView->VCam, 0, 0, &XTemp);
	Box3d_AddPoint(&ViewBox, XTemp.X, XTemp.Y, XTemp.Z);
	Render_ViewToWorld(pFusionView->VCam, Render_GetWidth(pFusionView->VCam), Render_GetHeight(pFusionView->VCam), &XTemp);
	Box3d_AddPoint(&ViewBox, XTemp.X, XTemp.Y, XTemp.Z);

	//VectorToSUB(ViewBox.Min, inidx) = -FLT_MAX;
	//VectorToSUB(ViewBox.Max, inidx) = FLT_MAX;

	BrushDrawData	brushDrawData;

	brushDrawData.pViewBox = &ViewBox;
	brushDrawData.pDC = MemoryhDC;
	brushDrawData.v = pFusionView->VCam;
	brushDrawData.pDoc = m_pDoc;//this;
	brushDrawData.GroupId = 0;
	brushDrawData.FlagTest = NULL;

	HBRUSH hBrush = CreateSolidBrush(RGB(64, 64, 64));
	FillRect(MemoryhDC, &Rect, (HBRUSH)hBrush); // BackGround
	DeleteObject(hBrush);

	int Center_X, Center_Y;
	int Width, Depth;

	Width = Rect.right;
	Depth = Rect.bottom;

	Center_Y = (Depth / 2);
	Center_X = (Width / 2);

	GridSize = Render_GetFineGrid(pFusionView->VCam, (Level_GetGridType(App->CLSB_Doc->pLevel) == GridTexel) ? GRID_TYPE_TEXEL : GRID_TYPE_METRIC);

	
	if (Level_GetGridType(App->CLSB_Doc->pLevel) == GridMetric)
	{
		GridSize /= 2.54f;
	}

	GridSnapSize = Level_GetGridSnapSize(App->CLSB_Doc->pLevel);

	HPEN pen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	SelectObject(MemoryhDC, pen);

	m_Draw_Grid(MemoryhDC, GridSnapSize, Rect); // Snap grid


	HPEN pen2 = CreatePen(PS_SOLID, 0, RGB(112, 112, 112));
	SelectObject(MemoryhDC, pen2);

	m_Draw_Grid(MemoryhDC, 128, Rect); // Big grid

	int BrushCount = App->CL_Brush->Get_Brush_Count();

	HPEN pen3 = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	SelectObject(MemoryhDC, pen3);

	int GroupVis = Level_GetGroupVisibility(App->CLSB_Doc->pLevel);

	GroupListType* Groups = Level_GetGroups(App->CLSB_Doc->pLevel);

	GroupIterator	gi;
	int				GroupId;
	GroupId = Group_GetFirstId(Groups, &gi);

	Brush* b;
	BrushList* pList = Level_GetBrushes(App->CLSB_Doc->pLevel);
	b = pList->First;

	while (b != NULL)
	{
		brushDrawData.FlagTest = ::fdocBrushNotDetail;
		brushDrawData.GroupId = GroupId;
		if ((GroupVis == Group_ShowAll) ||
			((GroupVis == Group_ShowCurrent) && (GroupId == App->CLSB_Doc->mCurrentGroup)) ||
			((GroupVis == Group_ShowVisible) && (Group_IsVisible(Groups, GroupId)))
		)
		{
			Level_EnumLeafBrushes(App->CLSB_Doc->pLevel, &brushDrawData, m_BrushDraw); // Draw Brushes
		}

		b = b->Next;
		//GroupId = Group_GetNextId(Groups, &gi);
	}


	CEntity* pCameraEntity = App->CLSB_Camera_WE->FindCameraEntity();

	if ((pCameraEntity != NULL))
	{
		/*if (pCameraEntity->IsSelected())
			pDC->SelectObject(&PenSelected);
		else
			pDC->SelectObject(&PenCamera);*/

		fdocDrawEntity(pCameraEntity, pFusionView->VCam, MemoryhDC, Level_GetEntityDefs(App->CLSB_Doc->pLevel), GE_TRUE);
	}

	int TopLeft, BottomRight;
	int CrossSize = 16;

	int CamX = 0;
	int CamBottomRight = 0;

	Width = Rect.right;
	Depth = Rect.bottom;

	Center_Y = (Depth / 2);
	Center_X = (Width / 2);

	TopLeft = Center_Y - 4;
	BottomRight = Center_X - 4;

	BitBlt(RealhDC, Rect.left, Rect.top, Rect.right - Rect.left, Rect.bottom - Rect.top, MemoryhDC, 0, 0, SRCCOPY);

	SelectObject(MemoryhDC, &OffScreenBitmap);

	DeleteObject(pen);
	DeleteObject(pen2);
	DeleteObject(pen3);
	DeleteObject(hBrush);

	DeleteObject(OffScreenBitmap);
	DeleteDC(MemoryhDC);
}

// *************************************************************************
// *	  			Draw_Grid:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Test_View::m_Draw_Grid(HDC hDC, int Interval, RECT Rect)
{
	int cnt = Rect.bottom / Interval;

	int SP = 0;
	int Count = 0;

	// horizontal lines
	while (Count < cnt + 1)
	{
		MoveToEx(hDC, 0, SP, NULL);
		LineTo(hDC, Rect.right, SP);

		SP = SP + Interval;
		Count++;
	}

	cnt = Rect.right / Interval;
	SP = 0;
	Count = 0;
	// vertical lines
	while (Count < cnt + 1)
	{
		MoveToEx(hDC, SP, 0, NULL);
		LineTo(hDC, SP, Rect.bottom);

		SP = SP + Interval;
		Count++;
	}

	return 1;
}

// *************************************************************************
// *	  						BrushDraw		Genesis					   *
// *************************************************************************
geBoolean SB_Test_View::m_BrushDraw(Brush* pBrush, void* lParam)
{
	BrushDrawData* pData = (BrushDrawData*)lParam;
	CFusionDoc* pDoc = pData->pDoc;

	if (App->CL_Test_View->m_View == 1)
	{
		pData->v->ViewType = 8;
	}

	if (App->CL_Test_View->m_View == 2)
	{
		pData->v->ViewType = 32;
	}

	if (App->CL_Test_View->m_View == 3)
	{
		pData->v->ViewType = 16;
	}


	if ((pData->GroupId == fdoc_SHOW_ALL_GROUPS) || (Brush_GetGroupId(pBrush) == pData->GroupId))
	{
		if ((pData->FlagTest == NULL) || pData->FlagTest(pBrush))
		{
			//if (pDoc->fdocShowBrush(pBrush, pData->pViewBox))
			{
				m_Render_RenderBrushFacesOrtho(pData->v, pBrush, pData->pDC);
			}
		}
	}

	return GE_TRUE;
}

static POINT plist[64];

// *************************************************************************
// *	  		m_Render_RenderBrushFacesOrtho							   *
// *************************************************************************
void SB_Test_View::m_Render_RenderBrushFacesOrtho(const ViewVars* Cam, Brush* b, HDC ViewDC)
{
	int	i, j;

	assert(b != NULL);

	for (i = 0; i < Brush_GetNumFaces(b); i++)
	{
		Face* f = Brush_GetFace(b, i);
		const geVec3d* pnts = Face_GetPoints(f);
		for (j = 0; j < Face_GetNumPoints(f); j++)
		{
			plist[j] = m_Render_OrthoWorldToView(Cam, &pnts[j]);
		}
		plist[j] = plist[0];
		Polyline(ViewDC, plist, j + 1);
	}
}

// *************************************************************************
// *	  			m_Render_OrthoWorldToView							   *
// *************************************************************************
POINT SB_Test_View::m_Render_OrthoWorldToView(const ViewVars* v, geVec3d const* wp)
{
	POINT	sc = { 0, 0 };
	geVec3d ptView;

	switch (v->ViewType)
	{
	case VIEWTOP:
	{
		geVec3d_Subtract(wp, &v->CamPos, &ptView);
		geVec3d_Scale(&ptView, v->ZoomFactor, &ptView);
		sc.x = (int)(v->XCenter + ptView.X);
		sc.y = (int)(v->YCenter + ptView.Z);
		break;
	}
	case VIEWFRONT:
	{
		geVec3d_Subtract(wp, &v->CamPos, &ptView);
		geVec3d_Scale(&ptView, v->ZoomFactor, &ptView);
		sc.x = (int)(v->XCenter + ptView.X);
		sc.y = (int)(v->YCenter - ptView.Y);
		break;
	}
	case VIEWSIDE:
	{
		geVec3d_Subtract(wp, &v->CamPos, &ptView);
		geVec3d_Scale(&ptView, v->ZoomFactor, &ptView);
		sc.x = (int)(v->XCenter + ptView.Z);
		sc.y = (int)(v->YCenter - ptView.Y);
		break;
	}
	default:
		//			assert (0);	// bad view type
					// I should be able to assert here, but I can't.
					// somebody is calling this function for the rendered
					// view.  Bad stuff, really, but I don't have time to change it.
		break;
	}

	return sc;
}

