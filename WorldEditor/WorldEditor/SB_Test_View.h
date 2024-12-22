#pragma once

#include "FUSIONView.h"

class SB_Test_View
{
public:
	SB_Test_View();
	~SB_Test_View();

	bool Start_Main_View_Dlg();
	void Start_Splitter();
	void Draw_Screen(HWND hwnd);
	bool m_Draw_Grid(HDC hDC, int Interval, RECT Rect);
	static geBoolean m_BrushDraw(Brush* pBrush, void* lParam);
	static void m_Render_RenderBrushFacesOrtho(const ViewVars* Cam, Brush* b, HDC ViewDC);

	static POINT SB_Test_View::m_Render_OrthoWorldToView(const ViewVars* v, geVec3d const* wp);

	//CFusionView* p_View;

private:
	static LRESULT CALLBACK Splitter_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK Left_Window_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Right_Window_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK Bottom_Left_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Bottom_Right_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	
	bool Init_Views();

	void Create_Left_Window();
	void Create_Right_Window();

	void Create_Bottom_Left_Window();
	void Create_Bottom_Right_Window();

	bool Resize_Windows(HWND hDlg, int NewWidth, int NewDepth);
	void Get_View();

	HPEN Pen_CutBrush;
	HPEN Pen_Camera;

	HWND Spliter_Main_Hwnd;
	HWND Left_Window_Hwnd;
	HWND Right_Window_Hwnd;

	HWND Bottom_Left_Hwnd;
	HWND Bottom_Right_Hwnd;

	HBRUSH BackGround_Brush;

	int m_View;

	int LEFT_WINDOW_WIDTH;

	int LEFT_WINDOW_DEPTH;
	int TOP_POS_BOTLEFT;

	int RIGHT_MINIMUM_SPACE;
	int LEFT_MINIMUM_SPACE;

	int nleftWnd_width;
	int nleftWnd_Depth;

	bool Do_Width;
	bool Do_Depth;
	bool Do_All;
};

