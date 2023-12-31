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

#include "stdafx.h"
#include "AB_App.h"
#include "resource.h"
#include "SB_Tabs_Brushes_Dlg.h"

#pragma warning( disable : 4101)

enum BrushFlags
{
	BRUSH_SOLID = 0x0001,
	BRUSH_WINDOW = 0x0002,
	BRUSH_WAVY = 0x0004,
	BRUSH_DETAIL = 0x0008,	//not included in vis calculations
	BRUSH_HOLLOWCUT = 0x0010,
	BRUSH_TRANSLUCENT = 0x0020,
	BRUSH_EMPTY = 0x0040,
	BRUSH_SUBTRACT = 0x0080,
	BRUSH_CLIP = 0x0100,
	BRUSH_FLOCKING = 0x0200,
	BRUSH_HOLLOW = 0x0400,
	BRUSH_SHEET = 0x0800,
	BRUSH_HIDDEN = 0x1000,
	BRUSH_LOCKED = 0x2000,
	BRUSH_HINT = 0x4000,
	BRUSH_AREA = 0x8000
};

struct tag_BrushList
{
	Brush* First;
	Brush* Last;
};

typedef struct TexInfoTag
{
	geVec3d VecNormal;
	geFloat xScale, yScale;
	int xShift, yShift;
	geFloat	Rotate;			// texture rotation angle in degrees
	TexInfo_Vectors TVecs;
	int Dib;				// index into the wad
	char Name[16];
	geBoolean DirtyFlag;
	geVec3d Pos;
	int txSize, tySize;		// texture size (not currently used)
	geXForm3d XfmFaceAngle;	// face rotation angle
} TexInfo;

typedef struct FaceTag
{
	int			NumPoints;
	int			Flags;
	Plane		Face_Plane;
	int			LightIntensity;
	geFloat		Reflectivity;
	geFloat		Translucency;
	geFloat		MipMapBias;
	geFloat		LightXScale, LightYScale;
	TexInfo		Tex;
	geVec3d* Points;
} Face;

struct tag_FaceList
{
	int NumFaces;
	int Limit;
	Face** Faces;
	geBoolean Dirty;
	Box3d Bounds;
};

SB_Tabs_Brushes_Dlg::SB_Tabs_Brushes_Dlg(void)
{
	GroupsDlg_Hwnd = NULL;
	Groups_Dlg_Created = NULL;

	Selected_Index = -1;
	Selected_Brush = NULL;
	Properties_Dialog_Active = 0;
}

SB_Tabs_Brushes_Dlg::~SB_Tabs_Brushes_Dlg(void)
{
}

// *************************************************************************
// *	  	Show_GroupsDialog:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::Show_GroupsDialog(bool Show)
{
	ShowWindow(GroupsDlg_Hwnd, Show);
}

// *************************************************************************
// *	  	Start_Brush_Tabs_Dialog:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::Start_Brush_Tabs_Dialog()
{
	GroupsDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TABSBRUSHES, App->CLSB_TabsControl->Tabs_Control_Hwnd, (DLGPROC)Brush_Tabs_Proc);

	Groups_Dlg_Created = 1;

	Fill_ListBox();
	Update_Dlg_Controls();
}

// *************************************************************************
// *			 Groups_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_Tabs_Brushes_Dlg::Brush_Tabs_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendDlgItemMessage(hDlg, IDC_GD_BRUSHLIST, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_GD_GROUPS, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BRUSHCOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_GD_BRUSHCOUNT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_GD_BRUSHPROPERTIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_GD_SELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_SELECTED, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_DIMENSIONS, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_DELETEBRUSH, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_GD_GROUPS) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_BRUSHCOUNT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_GD_BRUSHCOUNT) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_GD_SELECTED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_ST_SELECTED) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDC_BT_GD_BRUSHPROPERTIES && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_GD_BRUSHPROPERTIES));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}

			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_DIMENSIONS && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_DIMENSIONS));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_DELETEBRUSH && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			bool test = IsWindowEnabled(GetDlgItem(hDlg, IDC_BT_DELETEBRUSH));
			if (test == 0)
			{
				App->Custom_Button_Greyed(item);
			}
			else
			{
				App->Custom_Button_Normal(item);
			}
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BT_DELETEBRUSH)
		{
			App->CLSB_Dialogs->YesNo("Are you sure", "Do you want to Delete the selected Brushes");

			bool Doit = App->CLSB_Dialogs->Canceled;
			if (Doit == 0)
			{
				App->m_pDoc = (CFusionDoc*)App->m_pMainFrame->GetCurrentDoc();
				App->CLSB_Doc->DeleteCurrentThing();
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_DIMENSIONS)
		{
			App->CLSB_Brushes->Start_Dimensions_Dlg();
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_GD_BRUSHLIST)
		{
			if (App->CL_TabsGroups_Dlg->Groups_Dlg_Created == 1)
			{
				App->CL_TabsGroups_Dlg->List_Selection_Changed(1);
			}
			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_GD_BRUSHPROPERTIES)
		{
			App->CL_TabsGroups_Dlg->Start_Brush_Properties_Dlg();
			SendDlgItemMessage(hDlg, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)App->CL_TabsGroups_Dlg->Selected_Index, (LPARAM)0);
			return TRUE;
		}

		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			//App->CL_TextureDialog->f_TextureDlg_Active = 0;
			//EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			//App->CL_TextureDialog->f_TextureDlg_Active = 0;
			//EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *	  		Get_Index:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::Get_Index(const Brush* b)
{
	App->Get_Current_Document();

	Level* pLevel = App->CLSB_Doc->pLevel;
	BrushList* pList = Level_GetBrushes(App->CLSB_Doc->pLevel);

	int Selected = 0;
	int Count = 0;

	b = pList->First;

	while (b != NULL)
	{
		Selected = App->m_pDoc->BrushIsSelected(b);

		if (Selected == 1)
		{
			SendDlgItemMessage(GroupsDlg_Hwnd, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)Count, (LPARAM)0);
			Selected_Index = Count;
			List_Selection_Changed(0);
		}

		Count++;
		b = b->Next;
	}

	Update_Dlg_SelectedBrushesCount();
}

// *************************************************************************
// *	  	List_Selection_Changed:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::List_Selection_Changed(bool Clear)
{
	int Index = SendDlgItemMessage(GroupsDlg_Hwnd, IDC_GD_BRUSHLIST, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if	(Index == -1)
	{
		//App->Say("ListBox No Selection Available");
	}
	else
	{
		if (Groups_Dlg_Created == 1)
		{
			Selected_Index = Index;
			OnSelchangeBrushlist(Index,Clear);
		}
	}
}

// *************************************************************************
// *		OnSelchangeBrushlist:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::OnSelchangeBrushlist(int Index, bool Clear)
{
	App->Get_Current_Document();

	int			c ;
	geBoolean	bChanged = FALSE ;

	c = App->CL_World->Get_Brush_Count();

	if( c > 0 )
	{
		if (Clear == 1)
		{
			App->CLSB_Doc->ResetAllSelections() ;
			App->CLSB_Doc->UpdateSelected();
		}
		
		Selected_Brush = App->CL_World->Get_Brush_ByIndex( Index );

		SelBrushList_Add(App->CLSB_Doc->pSelBrushes, Selected_Brush);

		if (Clear == 1)
		{
			Update_Dlg_Controls();
			App->CLSB_TopTabs->Update_Dlg_Controls();
		}
		//m_pDoc->DoBrushSelection( Selected_Brush, brushSelToggle) ;
		bChanged = GE_TRUE;
	}


	if( bChanged )
	{
		App->CLSB_Doc->UpdateSelected() ;
		App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);
	}
}

// *************************************************************************
// *			 Fill_ListBox:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::Fill_ListBox()
{
	if (Groups_Dlg_Created == 1)
	{
		SendDlgItemMessage(GroupsDlg_Hwnd, IDC_GD_BRUSHLIST, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		App->m_pDoc = (CFusionDoc*)App->m_pMainFrame->GetCurrentDoc();

		Level *pLevel = App->CLSB_Doc->pLevel;
		BrushList *pList = Level_GetBrushes (App->CLSB_Doc->pLevel);

		int Count = 0;
		Brush * b;
		b = pList->First;
		while (b != NULL)
		{
			SendDlgItemMessage(GroupsDlg_Hwnd, IDC_GD_BRUSHLIST, LB_ADDSTRING, (WPARAM)0,(LPARAM)App->CL_Brush->Brush_GetName(b));
			Count++;
			b = b->Next;
		}


		char buff[100];
		SetDlgItemText(GroupsDlg_Hwnd, IDC_BRUSHCOUNT, itoa(Count, buff, 10));
	}
}

// *************************************************************************
// *	Update_Dlg_SelectedBrushesCount:- Terry and Hazel Flanigan 2023	   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::Update_Dlg_SelectedBrushesCount()
{
	char buff[100];
	int NumSelBrushes = SelBrushList_GetSize(App->CLSB_Doc->pSelBrushes);
	SetDlgItemText(GroupsDlg_Hwnd, IDC_ST_GD_SELECTED, itoa(NumSelBrushes, buff, 10));
}

// ---------------      Properties     ----------------
// ----------------------------------------------------
// ----------------------------------------------------
// ----------------------------------------------------
 
// *************************************************************************
// *	  	Start_Properties_Dlg:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::Start_Brush_Properties_Dlg()
{
	if (Properties_Dialog_Active == 0)
	{
		DialogBox(App->hInst, (LPCTSTR)IDD_SB_BRUSH_PROPERTIES, App->MainHwnd, (DLGPROC)Brush_Properties_Proc);
	}
}

// *************************************************************************
// *			Properties_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_Tabs_Brushes_Dlg::Brush_Properties_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		App->CL_TabsGroups_Dlg->Properties_Dialog_Active = 1;

		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_CB_SELECTED_BRUSH, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_ST_BP_SELECTEDBRUSHES, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BT_RENAME, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		
		SendDlgItemMessage(hDlg, IDOK, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		

		App->CL_TabsGroups_Dlg->Fill_Brush_Combo(hDlg);
		App->CL_TabsGroups_Dlg->List_BrushData(hDlg);

		return TRUE;
	}
	
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_BP_SELECTEDBRUSHES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}
		return FALSE;
	}
	
	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_NOTIFY:
	{
		LPNMHDR some_item = (LPNMHDR)lParam;

		if (some_item->idFrom == IDOK && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BT_RENAME && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{

		if (LOWORD(wParam) == IDC_CB_SELECTED_BRUSH)
		{
			switch (HIWORD(wParam)) // Find out what message it was
			{
			case CBN_DROPDOWN:
				break;
			case CBN_CLOSEUP:
			{
				char buff[MAX_PATH]{ 0 };

				HWND temp = GetDlgItem(hDlg, IDC_CB_SELECTED_BRUSH);
				int Index = SendMessage(temp, CB_GETCURSEL, 0, 0);
				
				App->CL_TabsGroups_Dlg->Selected_Index = Index;
				App->CL_TabsGroups_Dlg->Selected_Brush = App->CL_Brush->Get_By_Index(Index);
				App->CL_TabsGroups_Dlg->List_BrushData(hDlg);

				App->CL_TabsGroups_Dlg->OnSelchangeBrushlist(Index, 1);
			}
			}

			return TRUE;
		}

		if (LOWORD(wParam) == IDC_BT_RENAME)
		{
			char Name[MAX_PATH];

			strcpy(App->CLSB_Dialogs->btext, "Change File Name");
			strcpy(App->CLSB_Dialogs->Chr_Text, App->CL_TabsGroups_Dlg->Selected_Brush->Name);

			App->CLSB_Dialogs->Dialog_Text(Enums::Check_Name_Brushes);

			if (App->CLSB_Dialogs->Canceled == 0)
			{
				strcpy(Name, App->CLSB_Dialogs->Chr_Text);
			}
			else
			{
				return TRUE;
			}
			
			Brush_SetName(App->CL_TabsGroups_Dlg->Selected_Brush, Name);
			
			App->CL_TabsGroups_Dlg->Fill_Brush_Combo(hDlg);
			App->CL_TabsGroups_Dlg->List_BrushData(hDlg);
			App->CL_TabsGroups_Dlg->Fill_ListBox();

			App->m_pDoc->SetModifiedFlag(TRUE);

			return TRUE;
		}
		
		// -----------------------------------------------------------------
		if (LOWORD(wParam) == IDOK)
		{
			App->CL_TabsGroups_Dlg->Properties_Dialog_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			App->CL_TabsGroups_Dlg->Properties_Dialog_Active = 0;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}

		break;
	}
	}
	return FALSE;
}

// *************************************************************************
// *	  	Fill_Brush_Combo:- Terry and Hazel Flanigan 2023	    	   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::Fill_Brush_Combo(HWND hDlg)
{
	App->Get_Current_Document();

	SendDlgItemMessage(hDlg, IDC_CB_SELECTED_BRUSH, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	Level* pLevel = App->CLSB_Doc->pLevel;
	BrushList* pList = Level_GetBrushes(App->CLSB_Doc->pLevel);

	int Count = 0;
	Brush* b;
	b = pList->First;
	while (b != NULL)
	{
		SendDlgItemMessage(hDlg, IDC_CB_SELECTED_BRUSH, CB_ADDSTRING, (WPARAM)0, (LPARAM)App->CL_Brush->Brush_GetName(b));
		Count++;
		b = b->Next;
	}

	SendDlgItemMessage(hDlg, IDC_CB_SELECTED_BRUSH, CB_SETCURSEL, (WPARAM)Selected_Index, (LPARAM)0);
}

// *************************************************************************
// *	  	   List_BrushData:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::List_BrushData(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	char buf[255];
	int c = App->CL_World->Get_Brush_Count();

	if( c > 0 )
	{
		if (Selected_Brush && Selected_Index > -1)
		{
			Show_Brush_Info(Selected_Brush, hDlg);
		}
		else
		{
			sprintf(buf, "%s", "No Brush Selected");
			SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		}
	}
	else
	{
		sprintf(buf, "%s", "The World has No Brushes");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
}

// *************************************************************************
// *	  	Show_Brush_Info:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
bool SB_Tabs_Brushes_Dlg::Show_Brush_Info(const Brush *b, HWND hDlg)
{
	char buf[255];

	sprintf(buf, "%s%s", "Brush Name ",b->Name);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	// ----------------------------------- Flags
	if (b->Flags & BRUSH_SUBTRACT)
	{
		sprintf(buf, "%s%d%s", "Type Flags ", b->Flags, "  BRUSH_SUBTRACT");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else if (b->Flags & BRUSH_SOLID)
	{
		sprintf(buf, "%s%d%s", "Type Flags ", b->Flags, "  BRUSH_SOLID");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else if (b->Flags & BRUSH_HOLLOW)
	{
		sprintf(buf, "%s%d%s", "Type Flags ", b->Flags, "  Hollow Brush");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else
	{
		sprintf(buf, "%s%d%s", "Type Flags XX ", b->Flags, "  Unknown");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	
	// ----------------------------------- Data
	sprintf(buf, "%s%d", "Model ID ",b->ModelId);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%d", "Group ID ",b->GroupId);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%f", "Hull Size ",b->HullSize);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	// ----------------------------------- Type
	if (b->Type == BRUSH_MULTI)
	{
		sprintf(buf, "%s%i%s", "Type ", b->Type, "  - BRUSH_MULTI");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else if (b->Type == BRUSH_LEAF)
	{
		sprintf(buf, "%s%i%s", "Type ", b->Type, "  - BRUSH_LEAF");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else if (b->Type == BRUSH_CSG)
	{
		sprintf(buf, "%s%i%s", "Type ", b->Type, "  - BRUSH_CSG");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	else
	{
		sprintf(buf, "%s%i%s", "Type ", b->Type,"Unknown");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	if (b->Type == BRUSH_MULTI)
	{
		return Show_Brush_ListInfo(b->BList, hDlg);
	}
	if (b->Type == BRUSH_LEAF)
	{
		return Show_Brush_Faces_Info(b->Faces,  hDlg);
	}
	if (b->Type == BRUSH_CSG)
	{
		App->Say("BRUSH_CSG");
	}

	//// Bounding Box
	//sprintf(buf, "Max X = %f Y = %f Z = %f", b->BoundingBox.Max.X, b->BoundingBox.Max.Y, b->BoundingBox.Max.Z);
	//SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	//sprintf(buf, "Min X = %f Y = %f Z = %f", b->BoundingBox.Min.X, b->BoundingBox.Min.Y, b->BoundingBox.Min.Z);
	//SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	return 1;
}

// *************************************************************************
// *	  	Show_Brush_ListInfo:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Tabs_Brushes_Dlg::Show_Brush_ListInfo(BrushList *BList, HWND hDlg)
{
	char buf[MAX_PATH];
	Brush *pBrush;
	BrushIterator bi;
	int Count;

	Count = BrushList_Count (BList, (BRUSH_COUNT_MULTI | BRUSH_COUNT_LEAF | BRUSH_COUNT_NORECURSE));
	if (Count < 0)
	{
		Debug
		sprintf(buf, "%s%d", " ===== Sub Brushes ", Count);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
		return 0;
	}
	else
	{
		sprintf(buf, "%s%d", " ===== Sub Brushes ",Count-1);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	
	pBrush = BrushList_GetFirst (BList, &bi);
	while (pBrush != NULL)
	{
		Show_Brush_Info(pBrush,hDlg);
		pBrush = BrushList_GetNext (&bi);
	}

	return 1;
}

// *************************************************************************
// *	  Show_Brush_Faces_Info:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Tabs_Brushes_Dlg::Show_Brush_Faces_Info(const FaceList *pList, HWND hDlg)
{
	int i;
	char buf[MAX_PATH];

	if (pList->NumFaces < 0)
	{
	}
	else
	{
		sprintf(buf, "%s%d", "BrushFaces ",pList->NumFaces);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

		for(i=0;i < pList->NumFaces; i++)
		{
			if (!Show_Face_Data(i,pList->Faces[i], hDlg)) return 0;
		}

		sprintf(buf, "%s", " -------------------------------------------");
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	return 1;
}

// *************************************************************************
// *		  Show_Face_Data:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Tabs_Brushes_Dlg::Show_Face_Data(int Index, const Face *f, HWND hDlg)
{
	
	char buf[MAX_PATH];
	int		i, xShift, yShift, Rotate;
	geFloat xScale, yScale, rot;

	sprintf(buf, "%s", " -------------------------------------------");
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s %i", "Face Index",Index+1);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%i", "Flags ",f->Flags);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	sprintf(buf, "%s%d", "NumPoints",f->NumPoints);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);

	for(i=0;i < f->NumPoints;i++)
	{
		sprintf(buf, "XYZ %f %f %f",f->Points[i].X,f->Points[i].Y,f->Points[i].Z);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}
	

	const TexInfo_Vectors* TVecs = Face_GetTextureVecs(f);
	geVec3d uVec, vVec;
	geFloat U, V;

	int txSize, tySize;

	Face_GetTextureSize(f, &txSize, &tySize);

	// make sure that the texture size is set correctly (division!)
	if (txSize == 0)
		txSize = 32;
	if (tySize == 0)
		tySize = 32;

	geVec3d_Scale(&TVecs->uVec, 1.f / (geFloat)txSize, &uVec);
	geVec3d_Scale(&TVecs->vVec, -1.f / (geFloat)tySize, &vVec);

	const geVec3d* verts = Face_GetPoints(f);

	int j = 0;
	for (j = 0; j < f->NumPoints; j++)
	{
		U = geVec3d_DotProduct(&(verts[j]), &uVec);
		V = geVec3d_DotProduct(&(verts[j]), &vVec);
		U += (TVecs->uOffset / txSize);
		V -= (TVecs->vOffset / tySize);
		
		sprintf(buf, "UV %.3f %.3f",U,V);
		SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf);
	}

	char buf2[MAX_PATH];
	strcpy(buf, Face_GetTextureName(f));
	sprintf(buf2, "%s%s", "Texture: = ", buf);
	SendDlgItemMessage(hDlg, IDC_BRUSH_PROPERTIESLIST, LB_ADDSTRING, (WPARAM)0, (LPARAM)buf2);

	return 1;
}

// *************************************************************************
// *		  Update_Dlg_Controls:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Tabs_Brushes_Dlg::Update_Dlg_Controls()
{
	App->Get_Current_Document();

	int NumSelBrushes = SelBrushList_GetSize(App->CLSB_Doc->pSelBrushes);
	if (NumSelBrushes == 0)
	{
		EnableWindow(GetDlgItem(GroupsDlg_Hwnd, IDC_BT_GD_BRUSHPROPERTIES), 0);
		EnableWindow(GetDlgItem(GroupsDlg_Hwnd, IDC_BT_DIMENSIONS), 0);
		EnableWindow(GetDlgItem(GroupsDlg_Hwnd, IDC_BT_DELETEBRUSH), 0);

		SendDlgItemMessage(GroupsDlg_Hwnd, IDC_GD_BRUSHLIST, LB_SETCURSEL, (WPARAM)-1, (LPARAM)0);
	}
	else
	{
		EnableWindow(GetDlgItem(GroupsDlg_Hwnd, IDC_BT_GD_BRUSHPROPERTIES), 1);
		EnableWindow(GetDlgItem(GroupsDlg_Hwnd, IDC_BT_DIMENSIONS), 1);
		EnableWindow(GetDlgItem(GroupsDlg_Hwnd, IDC_BT_DELETEBRUSH), 1);
	}
}

