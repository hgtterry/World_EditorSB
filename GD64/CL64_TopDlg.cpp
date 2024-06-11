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
#include "CL64_TopDlg.h"

CL64_TopDlg::CL64_TopDlg(void)
{
	TabsHwnd = nullptr;
}

CL64_TopDlg::~CL64_TopDlg(void)
{
}

// **************************************************************************
// *	  		 Start_TopBar:- Terry and Hazel Flanigan 2024				*
// **************************************************************************
void CL64_TopDlg::Start_TopBar()
{
	TabsHwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_TOPBAR, App->MainHwnd, (DLGPROC)TopBar_Proc);
	//Init_Bmps_Globals();
}

// *************************************************************************
// *			TopBar_Proc:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
LRESULT CALLBACK CL64_TopDlg::TopBar_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_TopDlg->TabsHwnd = hDlg;

		/*App->CL_TopBar->Start_Tabs_Headers();
		App->CL_TopBar->Start_Files_TB();
		App->CL_TopBar->Start_Group_TB();
		App->CL_TopBar->Start_Model_TB();
		App->CL_TopBar->Start_Camera_TB();
		App->CL_TopBar->Start_Amimation_TB();*/

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

	case WM_COMMAND:

		break;
	}
	return FALSE;
}

