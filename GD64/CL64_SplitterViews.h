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
class CL64_SplitterViews
{
public:
	CL64_SplitterViews(void);
	~CL64_SplitterViews(void);

	void Init_Views();

	void Create_Top_Left_Win();
	void Create_Top_Right_Win();
	void Create_Bottom_Left_Window();

	bool Resize_Windows(HWND hDlg, int NewWidth, int NewDepth);
	bool Resize_OgreWin(void);

	void Max_3D_win(void);

	HWND Left_Window_Hwnd;
	HWND Right_Window_Hwnd;

	HWND Bottom_Left_Hwnd;
	HWND Bottom_Right_Hwnd;

	int nleftWnd_width;
	int nleftWnd_Depth;

	int LEFT_WINDOW_WIDTH;

	int LEFT_WINDOW_DEPTH;
	int TOP_POS_BOTLEFT;

	int RIGHT_MINIMUM_SPACE;
	int LEFT_MINIMUM_SPACE;

	bool Max_Window;

	static LRESULT CALLBACK ViewerMain_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:

	static LRESULT CALLBACK Top_Right_Win_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Top_Left_Win_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Bottom_Left_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	bool Do_Width;
	bool Do_Depth;
	bool Do_All;

};

