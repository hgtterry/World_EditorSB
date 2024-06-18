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
class CL64_TopDlg
{
public:

	CL64_TopDlg(void);
	~CL64_TopDlg(void);

	void Start_TopBar();
	void Start_Tabs_Headers(void);
	void Hide_Tabs(void);

	// Tab Options
	void Start_Debug_TB(void);
	void Start_Camera_TB(void);
	void Start_Demos_TB(void);

	bool Toggle_Tabs_Debug_Flag;
	bool Toggle_Tabs_Camera_Flag;
	bool Toggle_Tabs_Demos_Flag;
	
	bool Toggle_PhysicaDebug_Node_Flag;

	void Init_Bmps_Globals(void);

	HWND TabsHwnd;
	HWND Tabs_TB_hWnd;

	// Tab Options
	HWND Debug_TB_hWnd;
	HWND Camera_TB_hWnd;
	HWND Demos_TB_hWnd;

private:

	static LRESULT CALLBACK TopBar_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Tabs_Headers_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	// Tab Options
	static LRESULT CALLBACK Debug_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Camera_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Demos_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	bool Toggle_Cam_ModelMode_Flag;
	bool Toggle_Cam_FreeMode_Flag;

	bool Toggle_Demos_Demo_1_Flag;
	bool Toggle_Demos_Demo_2_Flag;

};

