/*
Copyright (c) 2023 Equity_ME World Editor -- HGT Software W.T.Flanigan H.C.Flanigan

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
#include "SB_Tabs_Texture_Dlg.h"

#include "WadFile.h"
#include "Ram.h"

#include "FUSIONView.h"
#include "FUSIONDoc.h"

#pragma warning(disable : 4715)

SB_Tabs_Texture_Dlg::SB_Tabs_Texture_Dlg(void)
{
	f_TextureDlg_Active = 0;
	TextureDlg_Hwnd = NULL;

	strcpy(m_CurrentTexture,"aztec66");

	Sel_BaseBitmap = NULL;
	BasePicWidth = NULL;
	BasePicHeight = NULL;
}

SB_Tabs_Texture_Dlg::~SB_Tabs_Texture_Dlg(void)
{
}

// *************************************************************************
// *	  		Show_Dialog:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Tabs_Texture_Dlg::Show_Dialog(bool Show)
{
	if (App->CLSB_TextureDialog->f_TextureDlg_Active == 1)
	{
		ShowWindow(TextureDlg_Hwnd, Show);
	}
}

// *************************************************************************
// *	  	Start_TextureDialog:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Tabs_Texture_Dlg::Start_TextureDialog()
{
	TextureDlg_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_SB_TABSTEXTURES, App->CLSB_TabsControl->Tabs_Control_Hwnd, (DLGPROC)TextureDialog_Proc);

	Set_Txl_FileName();
	Fill_ListBox();
	Get_BitMap();
}

// *************************************************************************
// *        TextureDialog_Proc:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
LRESULT CALLBACK SB_Tabs_Texture_Dlg::TextureDialog_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		
		SendDlgItemMessage(hDlg, IDC_STTEXTURE, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_ST_GD_TEXTURES, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));

		SendDlgItemMessage(hDlg, IDC_STTDTXLNAME, WM_SETFONT, (WPARAM)App->Font_CB18, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_LISTTDTEXTURES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTDAPPLY, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTEDITFILE, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_BTTDFACEPROPERTIES, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));
		SendDlgItemMessage(hDlg, IDC_STWIDTHHEIGHT, WM_SETFONT, (WPARAM)App->Font_CB15, MAKELPARAM(TRUE, 0));

		App->CLSB_TextureDialog->f_TextureDlg_Active = 1;

		SetWindowLong(GetDlgItem(hDlg, IDC_BASETEXTURE2), GWL_WNDPROC, (LONG)ViewerBasePic);

		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		if (GetDlgItem(hDlg, IDC_ST_GD_TEXTURES) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 255));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STTDTXLNAME) == (HWND)lParam)
		{
			SetBkColor((HDC)wParam, RGB(0, 0, 0));
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (UINT)App->AppBackground;
		}

		if (GetDlgItem(hDlg, IDC_STWIDTHHEIGHT) == (HWND)lParam)
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

		if (some_item->idFrom == IDC_BTTDAPPLY && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTEDITFILE && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		if (some_item->idFrom == IDC_BTTDFACEPROPERTIES && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
			App->Custom_Button_Normal(item);
			return CDRF_DODEFAULT;
		}

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDC_BTTDFACEPROPERTIES)
			{
				App->CL_FaceDialog->Start_FaceDialog();
				return TRUE;
			}
			
			if (LOWORD(wParam) == IDC_BTEDITFILE)
			{
				App->CL_TxlEditor->Start_Texl_Dialog();

				App->Get_Current_Document();

				Level_SetWadPath(App->CLSB_Doc->pLevel, Level_GetWadPath(App->CLSB_Doc->pLevel));
				App->CL_World->Set_Current_TxlPath();
				App->CLSB_Doc->UpdateAfterWadChange();

				App->CLSB_TextureDialog->Fill_ListBox();
				return TRUE;
			}
	
			if (LOWORD(wParam) == IDC_LISTTDTEXTURES)
			{
				App->CLSB_TextureDialog->List_Selection_Changed();
				return TRUE;
			}

			if (LOWORD(wParam) == IDC_BTTDAPPLY)
			{
				App->CLSB_TextureDialog->Apply_Texture();
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
// *						ViewerBasePic Terry Flanigan	  			   *
// *************************************************************************
bool CALLBACK SB_Tabs_Texture_Dlg::ViewerBasePic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_PAINT)
	{
		PAINTSTRUCT	ps;
		HDC			hDC;
		RECT		Rect;

		hDC = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &Rect);
		Rect.left--;
		Rect.bottom--;
		FillRect(hDC, &Rect, (HBRUSH)(RGB(0, 255, 0)));

		if (App->CLSB_TextureDialog->Sel_BaseBitmap != NULL)
		{
			RECT	Source;
			RECT	Dest;
			HDC		hDC;

			Source.left = 0;
			Source.top = 0;
			Source.bottom = App->CLSB_TextureDialog->BasePicHeight;
			Source.right = App->CLSB_TextureDialog->BasePicWidth;

			Dest = Rect;

			hDC = GetDC(hwnd);
			SetStretchBltMode(hDC, HALFTONE);

			App->CLSB_TextureDialog->RenderTexture_Blit(hDC, App->CLSB_TextureDialog->Sel_BaseBitmap, &Source, &Dest);
			ReleaseDC(hwnd, hDC);

		}

		EndPaint(hwnd, &ps);
		return 0;
	}

	DefWindowProc(hwnd, msg, wParam, lParam);
}

// *************************************************************************
// *					RenderTexture_Blit Terry Bernie		  		   *
// *************************************************************************
bool SB_Tabs_Texture_Dlg::RenderTexture_Blit(HDC hDC, HBITMAP Bmp, const RECT *SourceRect, const RECT *DestRect)
{
	HDC		MemDC;
	int		SourceWidth;
	int		SourceHeight;
	int		DestWidth;
	int		DestHeight;

	MemDC = CreateCompatibleDC(hDC);
	if (MemDC == NULL)
		return FALSE;

	if (Bmp)
	{
		SelectObject(MemDC, Bmp);

		SourceWidth = SourceRect->right - SourceRect->left;
		SourceHeight = SourceRect->bottom - SourceRect->top;
		DestWidth = DestRect->right - DestRect->left;
		DestHeight = DestRect->bottom - DestRect->top;
		SetStretchBltMode(hDC, COLORONCOLOR);
		StretchBlt(hDC,
			DestRect->left,
			DestRect->top,
			DestHeight,
			DestHeight,
			MemDC,
			SourceRect->left,
			SourceRect->top,
			SourceWidth,
			SourceHeight,
			SRCCOPY);
	}

	DeleteDC(MemDC);

	return TRUE;
}

static void TextureFace
	(
	  Face *pFace,
	  int SelId,
	  char const *Name,
	  WadFileEntry* pbmp // changed QD 12/03
	)
{
	Face_SetTextureDibId (pFace, SelId);
	Face_SetTextureName (pFace, Name);
// changed QD 12/03
	Face_SetTextureSize (pFace, pbmp->Width, pbmp->Height);
// end change
}

// changed QD 12/03
static void TextureBrushList(BrushList *pList, int SelId, char const *Name, WadFileEntry* pbmp);

static void TextureBrush(Brush *pBrush,int SelId,char const *Name,WadFileEntry* pbmp) // changed QD 12/03)
{
	int j;

	assert(pBrush);
	
	if(Brush_IsMulti(pBrush))
	{
		// changed QD 12/03
		TextureBrushList((BrushList *)App->CL_Brush->Brush_GetBrushList(pBrush), SelId, Name, pbmp);
	}
	else
	{
		for (j = 0; j < Brush_GetNumFaces (pBrush); ++j)
		{
			Face *pFace;

			pFace = Brush_GetFace (pBrush, j);
			TextureFace (pFace, SelId, Name, pbmp); // changed QD 12/03
		}
	}
}

static void TextureBrushList
	(
	  BrushList *pList,
	  int SelId,
	  char const *Name,
	  WadFileEntry* pbmp // changed QD 12/03
	)
{
	Brush *b;
	BrushIterator bi;

	assert(pList);
	assert(Name);

#pragma message ("Change this and the function above to use BrushList_EnumAll")
	for(b=BrushList_GetFirst(pList, &bi);b;b=BrushList_GetNext(&bi))
	{
		TextureBrush(b, SelId, Name, pbmp); // changed QD 12/03
	}	
}


// *************************************************************************
// *			Apply_Texture:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Tabs_Texture_Dlg::Apply_Texture()
{
	App->Get_Current_Document();

	int SelectedItem;
	int		i;

	char TextureName[MAX_PATH];

	SelectedItem = SendDlgItemMessage(TextureDlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	
	SendDlgItemMessage(TextureDlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETTEXT, (WPARAM)SelectedItem, (LPARAM)TextureName);
	
	SelectedItem = Get_Index_FromName(TextureName);
	if (SelectedItem == -1)
	{
		App->Say("Cant Find Texture");
		return;
	}

	SelectedItem = SelectedItem;

	if (App->CLSB_Doc->mModeTool == ID_TOOLS_TEMPLATE)
	{
		return;
	}
	
	App->m_pDoc->SetModifiedFlag();

	switch (App->CLSB_Doc->mAdjustMode)
	{
		case ADJUST_MODE_FACE :
		{
			int Size;

			Size = SelFaceList_GetSize (App->CLSB_Doc->pSelFaces);
			for (i = 0; i < Size; ++i)
			{
				Face *pFace;
				pFace = SelFaceList_GetFace (App->CLSB_Doc->pSelFaces, i);

				// changed QD 12/03
				WadFileEntry* BitmapPtr = App->m_pDoc->GetDibBitmap( m_CurrentTexture );
				::TextureFace (pFace, SelectedItem, (LPCSTR)m_CurrentTexture, BitmapPtr);
				// end change
			}
			// have to go through the selected brushes and update their child faces
			int NumSelBrushes = SelBrushList_GetSize (App->CLSB_Doc->pSelBrushes);
			for (i = 0; i < NumSelBrushes; ++i)
			{
				Brush *pBrush;

				pBrush = SelBrushList_GetBrush (App->CLSB_Doc->pSelBrushes, i);
				Brush_UpdateChildFaces (pBrush);
			}
			break;
		}

		case ADJUST_MODE_BRUSH :
		{
			if(App->CLSB_Doc->GetSelState() & MULTIBRUSH)
			{
				int NumSelBrushes = SelBrushList_GetSize (App->CLSB_Doc->pSelBrushes);
				for (i = 0; i < NumSelBrushes; ++i)
				{
					Brush *pBrush = SelBrushList_GetBrush (App->CLSB_Doc->pSelBrushes, i);
					// changed QD 12/03
					WadFileEntry* BitmapPtr = App->m_pDoc->GetDibBitmap( m_CurrentTexture );
					::TextureBrush (pBrush, SelectedItem, (LPCSTR)m_CurrentTexture, BitmapPtr);
					// end change
					Brush_UpdateChildFaces (pBrush);
				}
			}
			else
			{
				// changed QD 12/03
				WadFileEntry* BitmapPtr = App->m_pDoc->GetDibBitmap( m_CurrentTexture );
				::TextureBrush (App->CLSB_Doc->CurBrush, SelectedItem, (LPCSTR)m_CurrentTexture, BitmapPtr);
				// end change
				Brush_UpdateChildFaces (App->CLSB_Doc->CurBrush);
			}
			break;
		}

		default :
			return;
	}

	App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);

	if (App->CLSB_Equity->EquitySB_Dialog_Visible == 1)
	{
		App->CLSB_Mesh_Mgr->Update_World();
	}

}

// *************************************************************************
// *			Select_Texture:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Tabs_Texture_Dlg::Select_Texture(int SelNum, char* TextName)
{
	int test = SendDlgItemMessage(TextureDlg_Hwnd, IDC_LISTTDTEXTURES, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)TextName);
	if (test == LB_ERR)
	{
		return 0;
	}
	else
	{
		List_Selection_Changed();
	}

	return 1;
}

// *************************************************************************
// *	  	List_Selection_Changed:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
void SB_Tabs_Texture_Dlg::List_Selection_Changed()
{
	int Index = SendDlgItemMessage(TextureDlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if	(Index == LB_ERR)
	{
		App->Say("ListBox No Selection Available");
	}
	else
	{
		char TextureName[MAX_PATH];
		SendDlgItemMessage(TextureDlg_Hwnd, IDC_LISTTDTEXTURES, LB_GETTEXT, (WPARAM)Index, (LPARAM)TextureName);
		strcpy(m_CurrentTexture,TextureName);
		Get_BitMap();
	}
	
	char buf[255];
	sprintf(buf, "Index = %i        %i X %i", Index, BasePicWidth, BasePicHeight);
	SetDlgItemText(TextureDlg_Hwnd, IDC_STWIDTHHEIGHT, (LPCTSTR)buf);
	
	//IDC_STWIDTHHEIGHT
	//	Also set the current selection value in the document...
	//m_pDoc->mCurTextureSelection = SelNum;
}

// *************************************************************************
// *	  	Set_Txl_FileName:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Tabs_Texture_Dlg::Set_Txl_FileName()
{
	SetDlgItemText(TextureDlg_Hwnd, IDC_STTDTXLNAME, (LPCTSTR)App->CL_World->mCurrent_TXL_FileName);
}

// *************************************************************************
// *	  		Get_BitMap:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
void SB_Tabs_Texture_Dlg::Get_BitMap()
{
	App->m_pDoc = (CFusionDoc*)App->m_pMainFrame->GetCurrentDoc();
	WadFileEntry* BitmapPtr = App->m_pDoc->GetDibBitmap(m_CurrentTexture);

	HWND	PreviewWnd;
	HBITMAP	hbm;
	//HBITMAP	ahbm;
	HDC		hDC;

	PreviewWnd = GetDlgItem(TextureDlg_Hwnd, IDC_BASETEXTURE2);
	hDC = GetDC(PreviewWnd);
	hbm = CreateHBitmapFromgeBitmap(BitmapPtr->bmp, hDC);

	if(geBitmap_HasAlpha(BitmapPtr->bmp))
	{
		//hbm = CreateHBitmapFromgeBitmap(BitmapPtr->bmp, hDC);
		hbm = CreateHBitmapFromgeBitmap(geBitmap_GetAlpha(BitmapPtr->bmp), hDC);
		if (hbm == NULL)
		{
			App->Say("Cant Assign Bitmap");
			Sel_BaseBitmap = NULL;
			return;
		}

		Sel_BaseBitmap = hbm;
	}
	else
	{
		hbm = CreateHBitmapFromgeBitmap(BitmapPtr->bmp, hDC);
		if (hbm == NULL)
		{
			App->Say("Cant Assign Bitmap");
			Sel_BaseBitmap = NULL;
			return;
		}

		Sel_BaseBitmap = hbm;
	}

	App->CLSB_TextureDialog->BasePicHeight = BitmapPtr->Height;
	App->CLSB_TextureDialog->BasePicWidth = BitmapPtr->Width;

	ReleaseDC(PreviewWnd, hDC);
	InvalidateRect(GetDlgItem(TextureDlg_Hwnd, IDC_BASETEXTURE2), NULL, TRUE);

}

// *************************************************************************
// *				CreateHBitmapFromgeBitmap  06/06/08 		  		   *
// *************************************************************************
HBITMAP SB_Tabs_Texture_Dlg::CreateHBitmapFromgeBitmap (geBitmap *Bitmap, HDC hdc)
{
	geBitmap * Lock;
	gePixelFormat Format;
	geBitmap_Info info;
	HBITMAP hbm = NULL;

	// <> choose format to be 8,16,or 24, whichever is closest to Bitmap
	Format = GE_PIXELFORMAT_24BIT_BGR;

	if ( geBitmap_GetBits(Bitmap) )
	{
		Lock = Bitmap;
	}
	else
	{
		if ( ! geBitmap_LockForRead(Bitmap, &Lock, 0, 0, Format,	GE_FALSE,0) )
		{
			return NULL;
		}
	}

	geBitmap_GetInfo(Lock,&info,NULL);

	if ( info.Format != Format )
		return NULL;

	{
		void * bits;
		BITMAPINFOHEADER bmih;
		int pelbytes;

		pelbytes = gePixelFormat_BytesPerPel(Format);
		bits = geBitmap_GetBits(Lock);

		bmih.biSize = sizeof(bmih);
		bmih.biHeight = - info.Height;
		bmih.biPlanes = 1;
		bmih.biBitCount = 24;
		bmih.biCompression = BI_RGB;
		bmih.biSizeImage = 0;
		bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 10000;
		bmih.biClrUsed = bmih.biClrImportant = 0;

		if ( (info.Stride*pelbytes) == (((info.Stride*pelbytes)+3)&(~3)) )
		{
			bmih.biWidth = info.Stride;
			hbm = CreateDIBitmap( hdc, &bmih , CBM_INIT , bits, (BITMAPINFO *)&bmih , DIB_RGB_COLORS );
		}
		else
		{
			void * newbits;
			int Stride;

			bmih.biWidth = info.Width;
			Stride = (((info.Width*pelbytes)+3)&(~3));
			newbits = geRam_Allocate(Stride * info.Height);
			if ( newbits )
			{
				char *newptr,*oldptr;
				int y;

				newptr = (char *)newbits;
				oldptr = (char *)bits;
				for(y=0; y<info.Height; y++)
				{
					memcpy(newptr,oldptr,(info.Width)*pelbytes);
					oldptr += info.Stride*pelbytes;
					newptr += Stride;
				}
				hbm = CreateDIBitmap( hdc, &bmih , CBM_INIT , newbits, (BITMAPINFO *)&bmih , DIB_RGB_COLORS );
				geRam_Free(newbits);
			}
		}
	}

	if ( Lock != Bitmap )
	{
		geBitmap_UnLock (Lock);
	}

	return hbm;
}

// *************************************************************************
// *			Fill_ListBox:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Tabs_Texture_Dlg::Fill_ListBox()
{
	App->Get_Current_Document();
	int LBIndex;

	if (f_TextureDlg_Active == 1)
	{
		CWadFile *pWad;
		pWad = NULL;

		SendDlgItemMessage(TextureDlg_Hwnd, IDC_LISTTDTEXTURES, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

		pWad = Level_GetWadFile (App->CLSB_Doc->pLevel);
		if (pWad == NULL)
		{
			App->Say("Error Getting Wad File");
			return;
		}

		for (int index = 0; index < pWad->mBitmapCount; index++)
		{
			char mName[MAX_PATH];

			CString Name = pWad->mBitmaps[index].Name;
			strcpy(mName,Name);

			if (App->Centre_Debug == 1)
			{
				LBIndex = SendDlgItemMessage(TextureDlg_Hwnd, IDC_LISTTDTEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);
			}
			else
			{
				LBIndex = SendDlgItemMessage(TextureDlg_Hwnd, IDC_LISTTDTEXTURES, LB_ADDSTRING, (WPARAM)0, (LPARAM)mName);
			}

		}

		SendDlgItemMessage(TextureDlg_Hwnd, IDC_LISTTDTEXTURES,LB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		Set_Txl_FileName();
		List_Selection_Changed();
	}
}

// *************************************************************************
// *	  	Get_Index_FromName:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
int SB_Tabs_Texture_Dlg::Get_Index_FromName(char* TextureName)
{
	CWadFile* pWad;
	pWad = NULL;

	pWad = Level_GetWadFile(App->CLSB_Doc->pLevel);
	if (pWad == NULL)
	{
		App->Say("Error Getting Wad File");
		return -1;
	}

	for (int index = 0; index < pWad->mBitmapCount; index++)
	{
		char mName[MAX_PATH];
		CString Name = pWad->mBitmaps[index].Name;
		strcpy(mName, Name);

		bool test = strcmp(mName, TextureName);
		if (test == 0)
		{
			return index;
		}
	}

	return -1;
}

// *************************************************************************
// *			Open_TXL_File:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Tabs_Texture_Dlg::Open_TXL_File(char* TXL_Filename)
{
	geVFile* VFS;
	geVFile_Finder* Finder = NULL;

	pData = new TPack_WindowData;
	//pData->hwnd = ChDlg;
	pData->BitmapCount = 0;

	int TextureCount = 0;

	VFS = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_VIRTUAL, TXL_Filename, NULL, GE_VFILE_OPEN_READONLY | GE_VFILE_OPEN_DIRECTORY);
	if (!VFS)
	{
		//App->Say("Could not open file2 %s");
		return 0;
	}

	geVFile_Finder* Finder2;
	Finder2 = geVFile_CreateFinder(VFS, "*.*");
	if (!Finder2)
	{
		App->Say("XX Could not load textures from %s");
		geVFile_Close(VFS);
		return 0;
	}

	while (geVFile_FinderGetNextFile(Finder2) != GE_FALSE)
	{
		TextureCount++;
	}

	Finder = geVFile_CreateFinder(VFS, "*.*");
	if (!Finder)
	{
		App->Say("Could not load textures from %s");
		geVFile_Close(VFS);
		return 0;
	}

	while (geVFile_FinderGetNextFile(Finder) != GE_FALSE)
	{
		geVFile_Properties	Properties;

		geVFile_FinderGetProperties(Finder, &Properties);
		if (!AddTexture(VFS, Properties.Name))
		{
			geVFile_Close(VFS);
			return 0;
		}
	}

	strcpy(pData->TXLFileName, TXL_Filename);
	pData->FileNameIsValid = TRUE;
	pData->Dirty = FALSE;
	geVFile_Close(VFS);

	return 1;
}

// *************************************************************************
// *						AddTexture  06/06/08 				  		   *
// *************************************************************************
bool SB_Tabs_Texture_Dlg::AddTexture(geVFile* BaseFile, const char* Path)
{
	geBitmap_Info	PInfo;
	geBitmap_Info	SInfo;
	geBitmap* Bitmap;

	geVFile* File;
	char			FileName[_MAX_FNAME];
	char* Name;

	Bitmap = NULL;
	File = NULL;

	_splitpath(Path, NULL, NULL, FileName, NULL);
	Name = strdup(FileName);
	if (!Name)
	{
		App->Say("Error");
		return FALSE;
	}

	if (BaseFile)
		File = geVFile_Open(BaseFile, Path, GE_VFILE_OPEN_READONLY);
	else
		File = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_DOS, Path, NULL, GE_VFILE_OPEN_READONLY);

	if (!File)
	{
		App->Say("Error");
		return TRUE;
	}
	//geBitmap_Create()
	Bitmap = geBitmap_CreateFromFile(File);
	geVFile_Close(File);
	if (!Bitmap)
	{
		App->Say("Error");
		return TRUE;
	}
	geBitmap_GetInfo(Bitmap, &PInfo, &SInfo);
	
	NewBitmapList[pData->BitmapCount] = new BitmapEntry;
	if (!NewBitmapList)
	{
		App->Say("Error");
		return TRUE;
	}

	NewBitmapList[pData->BitmapCount]->Name = Name;
	NewBitmapList[pData->BitmapCount]->Bitmap = Bitmap;
	NewBitmapList[pData->BitmapCount]->WinBitmap = NULL;
	NewBitmapList[pData->BitmapCount]->WinABitmap = NULL;
	NewBitmapList[pData->BitmapCount]->Flags = 0;
	NewBitmapList[pData->BitmapCount]->Deleted = 0;
	pData->BitmapCount++;

	return TRUE;
}

// *************************************************************************
// *						Save/SaveAs  13/06/08 				  		   *
// *************************************************************************
bool SB_Tabs_Texture_Dlg::Save(const char* Path)
{
	char		FileName[_MAX_PATH];
	geVFile* VFS;
	int			i;

	if (!Path)
	{
		OPENFILENAME ofn;	// Windows open filename structure...
		char Filter[_MAX_PATH];
		char	Dir[_MAX_PATH];

		FileName[0] = '\0';

		GetCurrentDirectory(sizeof(Dir), Dir);

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = pData->hwnd;
		ofn.hInstance = pData->Instance;
		{
			char* c;

			// build actor file filter string
			strcpy(Filter, "Texture Libraries (*.txl)");
			c = &Filter[strlen(Filter)] + 1;
			// c points one beyond end of string
			strcpy(c, "*.txl");
			c = &c[strlen(c)] + 1;
			*c = '\0';	// 2nd terminating nul character
		}
		ofn.lpstrFilter = Filter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = FileName;
		ofn.nMaxFile = sizeof(FileName);
		ofn.lpstrFileTitle = FileName;
		ofn.nMaxFileTitle = sizeof(FileName);
		ofn.lpstrInitialDir = Dir;
		ofn.lpstrTitle = NULL;
		ofn.Flags = OFN_HIDEREADONLY;
		ofn.nFileOffset = 0;
		ofn.nFileExtension = 0;
		ofn.lpstrDefExt = "txl";
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;

		if (!GetSaveFileName(&ofn))
			return 0;

		Path = FileName;
	}

	_unlink(Path);
	VFS = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_VIRTUAL, Path, NULL, GE_VFILE_OPEN_CREATE | GE_VFILE_OPEN_DIRECTORY);
	if (!VFS)
	{
		App->Say("Error");
		return 0;
	}

	for (i = 0; i < pData->BitmapCount; i++)
	{
		geVFile* File;
		geBoolean	WriteResult;

		if (NewBitmapList[i]->Deleted == 0)
		{
			File = geVFile_Open(VFS, NewBitmapList[i]->Name, GE_VFILE_OPEN_CREATE);
			if (!File)
			{
				App->Say("Error");
				geVFile_Close(VFS);
				return 0;
			}

			WriteResult = geBitmap_WriteToFile(NewBitmapList[i]->Bitmap, File);
			geVFile_Close(File);
			if (WriteResult == GE_FALSE)
			{
				App->Say("Error");
				geVFile_Close(VFS);
				return 0;
			}
		}
	}

	strcpy(pData->TXLFileName, Path);
	pData->FileNameIsValid = TRUE;

	if (geVFile_Close(VFS) == GE_FALSE)
	{
		App->Say("Error");
	}
	else
	{
		pData->Dirty = FALSE;
	}

	return 1;
}
