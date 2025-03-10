#pragma once

/*
Copyright (c) 2023 World Editor -- HGT Software W.T.Flanigan H.C.Flanigan

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

class SB_TopTabs_Equity
{
public:
	SB_TopTabs_Equity(void);
	~SB_TopTabs_Equity(void);

	void Start_GridHair_Buttons();
	void Start_Render_Buttons_EQ();

	void Start_Tabs(void);
	void Start_Tabs_Headers(void);

	void Hide_Render_Buttons_EQ(bool Flag);

	void Camera_Set_Model(void);
	void Camera_Set_Free(void);
	void Camera_Set_First(void);

	void Hide_Tabs(void);

	bool Toggle_Camera_First_Flag;

	bool Toggle_Tabs_Camera_Flag;
	bool Toggle_Camera_Model_Flag;;

	HWND Tabs_TB_hWnd_Eq;
	HWND Test_TB_hWnd;
	HWND Camera_TB_hWnd;
	HWND Render_Buttons_EQ_hWnd;
	HWND GridHair_Buttons_hWnd;

private:

	static LRESULT CALLBACK Tabs_Headers_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Files_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Camera_TB_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK Render_Buttons_Proc_EQ(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK GridHair_Buttons_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Start_Files_TB(void);
	void Start_Camera_TB(void);

	void Init_Bmps_Globals_EQ(void);

	bool Toggle_Tabs_Test_Flag;
	bool Toggle_Dimensions_Flag;
	bool Toggle_Camera_Free_Flag;
	bool Picking_Active_Flag;
};


