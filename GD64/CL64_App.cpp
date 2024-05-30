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

CL64_App::CL64_App(void)
{
	CL_Ogre = nullptr;



	hInst = nullptr;
	MainHwnd = nullptr;
	Fdlg = nullptr;
	ViewGLhWnd = nullptr;

	//Say("Created");
}

CL64_App::~CL64_App(void)
{
	Say("Destroyed");
}

// *************************************************************************
// *			InitApp:- Terry and Hazel Flanigan 2024					   *
// *************************************************************************
void CL64_App::InitApp(void)
{
	CL_Ogre = new CL64_Ogre_Init();
}

// *************************************************************************
// *				Say:- Terry and Hazel Flanigan 2024					   *
// *************************************************************************
void CL64_App::Say(const char* Message)
{
	char text[1024];
	strcpy(text, Message);
	MessageBox(NULL, Message, Message, MB_OK);
	//App->Cl_Dialogs->Message(text);
}
