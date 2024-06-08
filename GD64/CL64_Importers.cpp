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
#include "CL64_Importers.h"

CL64_Importers::CL64_Importers()
{
}

CL64_Importers::~CL64_Importers()
{
}

// *************************************************************************
// *			Assimp_Loader:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
bool CL64_Importers::Assimp_Loader(char* Extension, char* Extension2)
{
	int Result = App->CL_File_IO->Open_File_Model(Extension, Extension2, NULL);
	if (Result == 0)
	{
		return 0;
	}

	App->CL_Model->Clear_Model_And_Reset();

	char Model_Path_And_File[MAX_PATH];
	strcpy(Model_Path_And_File, App->CL_File_IO->Get_Model_Path_File_Name().c_str());

	App->CL_Model->Set_Paths();

	bool Test = App->CL_Assimp->LoadFile(Model_Path_And_File);
	if (Test == 0)
	{
		//App->Say_Win("Failed To Load");
		return 0;
	}

	App->CL_Model->Model_Type = Enums::LoadedFile_Assimp;

	//Set_Equity();

	//App->CL_Recent_Files->RecentFile_Models_History_Update();

	//App->CL_Prefs->Update_User_File(Model_Path_And_File);

	return 1;
}
