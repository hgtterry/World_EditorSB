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

#include "stdafx.h"
#include "GD19_App.h"
#include "resource.h"
#include "SB_Camera.h"


SB_Com_Camera::SB_Com_Camera()
{
	Cam_Props_HWND = nullptr;

	CamPos_X = 0;
	CamPos_Y = 90;
	CamPos_Z = 100;

	LookAt_X = 0;
	LookAt_Y = 30;
	LookAt_Z = 0;

	strcpy(Camera_Name, "Cam_1");

	FileViewItem = nullptr;
}


SB_Com_Camera::~SB_Com_Camera()
{
}

// *************************************************************************
// *	  			Start_Camera_PropsPanel Terry Bernie				   *
// *************************************************************************
bool SB_Com_Camera::Start_Camera_PropsPanel()
{
	
	Cam_Props_HWND = CreateDialog(App->hInst,(LPCTSTR)IDD_PROPS_CAMERA,App->SBC_Properties->Properties_Dlg_hWnd,(DLGPROC)Camera_PropsPanel_Proc);
	return 1;
}
// *************************************************************************
// *		  	Camera_PropsPanel_Proc  Terry Bernie					   *
// *************************************************************************
LRESULT CALLBACK SB_Com_Camera::Camera_PropsPanel_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_BTCAMSAVE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTCAMGOTO, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->DialogBackGround;
	}

	case WM_NOTIFY:
	{
	
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BTCAMSAVE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTCAMGOTO && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		
		if (LOWORD(wParam) == IDC_BTCAMSAVE)
		{
			int Index = App->SBC_Properties->Current_Selected_Object;

			App->SBC_Com_Camera->Update_Camera(Index);

			App->SBC_Scene->B_Camera[Index]->Altered = 1;
			App->SBC_FileView->Mark_Altered(App->SBC_Scene->B_Camera[Index]->FileViewItem);
			App->SBC_Scene->Scene_Modified = 1;
			App->Say("Camera Saved");

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BTCAMGOTO)
		{
			App->SBC_Com_Camera->Set_Camera(App->SBC_Properties->Current_Selected_Object);
			return TRUE;
		}
		
		
		break;
	}
	return FALSE;
}

// *************************************************************************
// *					Hide_Cam_Dlg Terry Bernie 						   *
// *************************************************************************
void SB_Com_Camera::Hide_Cam_Dlg(bool Show)
{
		ShowWindow(Cam_Props_HWND, Show);
}

// *************************************************************************
// *	  				Reset_View Terry Bernie							   *
// *************************************************************************
void SB_Com_Camera::Reset_View(void)
{

	App->SBC_Grid->GridNode->setPosition(0, 0, 0);
	App->SBC_Grid->GridNode->resetOrientation();

	App->SBC_Grid->HairNode->setPosition(0, 0, 0);
	App->SBC_Grid->HairNode->resetOrientation();

	App->CL_Ogre->mCamera->setPosition(Ogre::Vector3(0, 90, 100));
	
	App->CL_Ogre->mCamera->lookAt(Ogre::Vector3(0, 30, 0));

	/*Ogre::Quaternion Test;
	Test.IDENTITY;

	App->CL_Ogre->mCamera->setOrientation(Test);

	App->CL_Ogre->mCamera->pitch(Radian(Ogre::Degree(-30)));

	App->CL_Ogre->mCamera->yaw(Radian(Ogre::Degree(-101.260)));
	App->CL_Ogre->mCamera->roll(Ogre::Degree(0));*/

	//App->CL_Ogre->mCamera->lookAt(Ogre::Vector3(0, 30, 0));
}

// *************************************************************************
// *	  	Update_Camera_StartUp:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
void SB_Com_Camera::Update_Camera_StartUp(void)
{
	//App->SBC_Scene->SBC_Base_Area[0]->Area_Node->setPosition()
}

// *************************************************************************
// *	  		Set_Camera:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
void SB_Com_Camera::Set_Camera(int Index)
{
	App->CL_Ogre->mCamera->setPosition(App->SBC_Scene->B_Camera[Index]->CamPos);
	App->CL_Ogre->mCamera->setOrientation(App->SBC_Scene->B_Camera[Index]->Cam_Quat);
}

// *************************************************************************
// *	  		Update_Camera:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_Com_Camera::Update_Camera(int Index)
{
	App->SBC_Scene->B_Camera[Index]->CamPos = App->CL_Ogre->mCamera->getPosition();
	
	App->SBC_Scene->B_Camera[Index]->Cam_Quat = App->CL_Ogre->mCamera->getOrientation();
	
}

// *************************************************************************
// *				Zoom   Terry Bernie								   *
// *************************************************************************
void SB_Com_Camera::Zoom(void)
{
	//if (App->CL_Vm_Model->Model_Loaded == 1)
	//{
	//	Reset_View();
	//	App->CL_Ogre->mCamera->setPosition(App->CL_Vm_Model->S_BoundingBox[0]->Centre[0].x, App->CL_Vm_Model->S_BoundingBox[0]->Centre[0].y, App->CL_Vm_Model->S_BoundingBox[0]->Centre[0].z);

	//	Ogre::Vector3 Move;
	//	Move.x = 0;
	//	Move.y = 0;//App->CL_Model_Data->S_BoundingBox[0]->Size[0].y;
	//			   //Move.z = App->CL_Model_Data->S_BoundingBox[0]->Size[0].y*2;

	//	if (App->CL_Vm_Model->S_BoundingBox[0]->Size[0].z > App->CL_Vm_Model->S_BoundingBox[0]->Size[0].y)
	//	{
	//		Move.z = App->CL_Vm_Model->S_BoundingBox[0]->Size[0].z * 2;
	//	}
	//	else
	//	{
	//		Move.z = App->CL_Vm_Model->S_BoundingBox[0]->Size[0].y * 2;
	//	}

	//	App->CL_Ogre->mCamera->moveRelative(Move);
	//}
}

// *************************************************************************
// *	  		Add_New_Camera:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_Com_Camera::Add_New_Camera(void)
{
	char Camera_Name[MAX_PATH];
	char Cbuff[MAX_PATH];

	int Index = App->SBC_Scene->Camera_Count;

	App->SBC_Scene->B_Camera[Index] = new Base_Camera();

	strcpy(Camera_Name, "Camera_");
	_itoa(Index, Cbuff, 10);
	strcat(Camera_Name, Cbuff);

	strcpy(App->SBC_Scene->B_Camera[Index]->Camera_Name, Camera_Name);

	App->SBC_Scene->Scene_Modified = 1;
	App->SBC_Scene->B_Camera[Index]->Altered = 1;
	App->SBC_Scene->B_Camera[Index]->FileViewItem = App->SBC_FileView->Add_Item(App->SBC_FileView->FV_Cameras_Folder,App->SBC_Scene->B_Camera[Index]->Camera_Name, Index,true);

	App->SBC_Scene->Camera_Count++;
}

// *************************************************************************
// *	  		Add_New_Camera:- Terry and Hazel Flanigan 2022			   *
// *************************************************************************
void SB_Com_Camera::Rename_Camera(int Index)
{
	strcpy(App->Cl_Dialogs->btext, "Change Camera Name");
	strcpy(App->Cl_Dialogs->Chr_Text, App->SBC_Scene->B_Camera[Index]->Camera_Name);

	App->Cl_Dialogs->Dialog_Text(1, 1);

	if (App->Cl_Dialogs->Canceled == 1)
	{
		return;
	}

	strcpy(App->SBC_Scene->B_Camera[Index]->Camera_Name, App->Cl_Dialogs->Chr_Text);

	App->SBC_Scene->B_Camera[Index]->Altered = 1;
	App->SBC_Scene->Scene_Modified = 1;
	App->SBC_FileView->Mark_Altered(App->SBC_Scene->B_Camera[Index]->FileViewItem);

	App->SBC_FileView->Change_Item_Name(App->SBC_Scene->B_Camera[Index]->FileViewItem, App->Cl_Dialogs->Chr_Text);
}
