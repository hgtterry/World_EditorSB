/*
Copyright (c) 2021 Scene Builder and Equity -- Inflanite Software W.T.Flanigan H.C.Flanigan

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
class EB_Options
{
public:
	EB_Options();
	~EB_Options();

	bool Start_Options_Dialog();
	bool Read_Preferences();

	char Quick_Load_File_Chr[1024];

protected:

	static LRESULT CALLBACK Options_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK QuickLoad_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	HWND Main_Window_Hwnd;
	HWND FileView_Hwnd;

	void AddRootFolder(void);
	void Get_Selection(LPNMHDR lParam);

	bool Start_QuickLoad_Dialog();

	bool Reset_Options();
	bool Write_Preferences();
	

	FILE *WriteOptions_File;

	HTREEITEM Root;
	HTREEITEM FV_File;

	TV_INSERTSTRUCT tvinsert;

	char FileView_Folder[255];
	char FileView_File[255];

};

