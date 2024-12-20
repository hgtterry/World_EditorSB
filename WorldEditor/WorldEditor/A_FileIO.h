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

#pragma once
class SB_FileIO  
{
public:
	SB_FileIO();
	~SB_FileIO();

	bool StartBrowser(char* szInitDir);

	bool Open_File_Model(HWND Owner, char* Extension, char* Title, char* StartDirectory);

	bool SaveSelectedFile(char* Extension,char* TszFile);
	bool CheckExtention(char *FileName);
	
	bool Delete_Directory_Contents(char* dir);

	// 1 = File Exsits    0 = No File
	bool Check_File_Exist(char* Full_Path);

	bool SearchFolders(char* Path, char* File);

	char FileName[MAX_PATH];
	char PathFileName[MAX_PATH];
	char BrowserMessage[MAX_PATH];

	char Project_File_Name[MAX_PATH];
	char Project_Path_File_Name[MAX_PATH];
	char DeskTop_Folder[MAX_PATH];
	TCHAR szSelectedDir[MAX_PATH];

private:
	static int __stdcall BrowseCallbackProc(HWND  hwnd, UINT  uMsg, LPARAM  lParam, LPARAM  lpData);

};

