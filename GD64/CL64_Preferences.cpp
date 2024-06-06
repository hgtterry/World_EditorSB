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
#include "CL64_Preferences.h"

CL64_Preferences::CL64_Preferences(void)
{
}

CL64_Preferences::~CL64_Preferences(void)
{
}

// *************************************************************************
// *	  		Clean_Up:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
void CL64_Preferences::Clean_Up(void)
{
	//MessageBox(NULL, "Deleted", "Deleted", MB_OK);
}

// *************************************************************************
// *			Read_Preferences:- Terry and Hazel Flanigan 2024 		   *
// *************************************************************************
void CL64_Preferences::Read_Preferences()
{
	char Preferences_Path[MAX_PATH];

	strcpy(Preferences_Path, App->GD_Directory_FullPath);
	strcat(Preferences_Path, "\\");
	strcat(Preferences_Path, "Data");
	strcat(Preferences_Path, "\\");
	strcat(Preferences_Path, "Preferences.ini");

	App->CL_Ini_File->SetPathName(Preferences_Path);

	int test = 0;
	char buff[MAX_PATH];

	test = App->CL_Ini_File->GetInt("Start_Up", "Start_FullScreen", 0, 10);

	App->CL_Ini_File->GetString("Version", "Current_Version", buff, MAX_PATH);


	App->Say_Int(test);
	App->Say(buff);
}
