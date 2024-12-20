/*
Copyright (c) World_Viewer 2023 Inflanite Software W.T.Flanigan H.C.Flanigan

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
#include "SB_Loader.h"


#include "RAM.h"

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
	GPlane		Face_Plane;
	int			LightIntensity;
	geFloat		Reflectivity;
	geFloat		Translucency;
	geFloat		MipMapBias;
	geFloat		LightXScale, LightYScale;
	TexInfo		Tex;
	geVec3d* Points;
} Face;

SB_Loader::SB_Loader(void)
{
	FileName[0] = 0;
	Path_FileName[0] = 0;
	szSelectedDir[0] = 0;

	RightGroups_Hwnd = nullptr;

	NameCount = 0;

	LoadError = 0;

}

SB_Loader::~SB_Loader(void)
{
}


// *************************************************************************
// *			Assimp_Loader:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Loader::Assimp_Loader(HWND Owner, char* Extension, char* Extension2)
{
	int Result = App->CLSB_FileIO->Open_File_Model(App->Equity_Dlg_hWnd,Extension, Extension2, NULL);
	if (Result == 0)
	{
		return 0;
	}

	//App->CLSB_Model->Clear_Model_And_Reset();
	
	strcpy(App->CLSB_Loader->Path_FileName, App->CLSB_FileIO->PathFileName);
	strcpy(App->CLSB_Loader->FileName, App->CLSB_FileIO->FileName);

	App->CLSB_Model->Set_Paths();

	return 1;
}

// *************************************************************************
// *			Ogre_Loader:- Terry and Hazel Flanigan 2023				   *
// *************************************************************************
bool SB_Loader::Ogre_Loader(char* Extension, char* Extension2)
{
	int Result = App->CLSB_FileIO->Open_File_Model(App->Equity_Dlg_hWnd, Extension, Extension2, NULL);
	if (Result == 0)
	{
		return 0;
	}

	//App->CL_Model->Clear_Model_And_Reset();

	strcpy(App->CLSB_Loader->Path_FileName, App->CLSB_FileIO->PathFileName);
	strcpy(App->CLSB_Loader->FileName, App->CLSB_FileIO->FileName);

	App->CLSB_Model->Set_Paths();

	App->CLSB_Ogre3D->Load_OgreModel();

	return 1;
}

// *************************************************************************
// *			Load_Project_File:- Terry and Hazel Flanigan 2023		   *
// *************************************************************************
bool SB_Loader::Read_Project_File(char* Path_And_File)
{
	char chr_Tag1[MAX_PATH];
	char chr_Tag2[MAX_PATH];

	chr_Tag1[0] = 0;
	chr_Tag2[0] = 0;

	App->CLSB_Ini->SetPathName(Path_And_File);

	App->CLSB_Ini->GetString("World_Data", "Pref_WE_JustFileName", chr_Tag1, MAX_PATH);
	strcpy(WE_JustFileName, chr_Tag1);

	App->CLSB_Ini->GetString("World_Data", "G3ds_Path_FileName", chr_Tag1, MAX_PATH);
	strcpy(WE_Path_FileName, chr_Tag1);

	App->CLSB_Ini->GetString("World_Data", "Txl_Path_FileName", chr_Tag2, MAX_PATH);
	strcpy(Txl_Path_FileName, chr_Tag2);

	return 1;
}

// *************************************************************************
// *			Load_File_Wepf:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Loader::Load_File_Wepf()
{
	/*App->CLSB_Model->Clear_Model_And_Reset();

	App->CLSB_Assimp->SelectedPreset = 8 + 8388608+64 + aiProcess_PreTransformVertices;

	App->CLSB_Assimp->DoTextures = 0;

	bool Test = App->CLSB_Assimp->LoadFile(WE_Path_FileName);
	if (Test == 0)
	{
		App->Say("Failed To Load");
		return 0;
	}

	App->CLSB_Model->Set_Paths();

	bool test = LoadTextures_TXL();
	if (test == 0)
	{
		App->Say("Failed Aborting");
		return 0;
	}

	App->CLSB_Model->Set_Equity();

	Adjust();
	

	Ogre::Root::getSingletonPtr()->renderOneFrame();
	Ogre::Root::getSingletonPtr()->renderOneFrame();

	App->CLSB_Model->Render_Type = Enums::LoadedFile_Assimp;

	App->CLSB_Camera_EQ->Reset_Orientation();
	App->CLSB_Camera_EQ->Set_Camera_Mode(Enums::CamDetached);
	App->CLSB_TopTabs_Equity->Camera_Set_Free();
	
	App->CLSB_Model->Set_BondingBoxes_AllGroups();*/

	return 1;
}
#include <functional>
// *************************************************************************
// *						Adjust Terry Flanigan						   *
// *************************************************************************
void SB_Loader::Adjust()
{
	Rotate_Z_Model(90);
	//Rotate_X_Model(-90);
	Centre_Model_Mid();

	App->CLSB_Model->Set_BondingBoxes_AllGroups();

	if (App->Centre_Debug == 1)
	{
		int GroupCount = App->CLSB_Model->Get_Groupt_Count();
		int test = -1;
		int Count = 0;
		Ogre::Vector3 Centre;

		while (Count < GroupCount)
		{
			test = strcmp(App->CLSB_Model->Group[Count]->MaterialName, "dummy");
			if (test == 0)
			{
				App->CLSB_Model->Group[Count]->Dont_Use = 1;

				//App->Say(App->CLSB_Model->Group[Count]->MaterialName);

				Centre.x = -1 * App->CLSB_Model->Group[Count]->Centre.x;
				Centre.y = -1 * App->CLSB_Model->Group[Count]->Centre.y;
				Centre.z = -1 * App->CLSB_Model->Group[Count]->Centre.z;

			}

			Count++;
		}

		//App->CLSB_Camera_EQ->Zero_View();
	}

	if (App->CLSB_Equity->mAutoLoad == 0)
	{
		App->CLSB_Grid->Reset_View();
	}

}


// *************************************************************************
// *		LoadTextures_TXL:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Loader::LoadTextures_TXL()
{
	geVFile* VFS = NULL;
	geVFile_Finder* Finder = NULL;
	
	NameCount = 0;

	VFS = geVFile_OpenNewSystem(NULL, GE_VFILE_TYPE_VIRTUAL, Txl_Path_FileName, NULL, GE_VFILE_OPEN_READONLY | GE_VFILE_OPEN_DIRECTORY);
	if (!VFS)
	{
		App->Say("Could not open TXL file", Txl_Path_FileName);
		return 0;
	}

	Finder = geVFile_CreateFinder(VFS, "*.*");
	if (!Finder)
	{
		App->Say("Could not create Finder *.* ");
		geVFile_Close(VFS);
		return 0;
	}

	
	BitMap_Names.resize(100);

	while (geVFile_FinderGetNextFile(Finder) != GE_FALSE)
	{
		geVFile_Properties	Properties;

		geVFile_FinderGetProperties(Finder, &Properties);

		strcpy(BitMap_Names[NameCount].Name, Properties.Name);

		NameCount++;
	}

	Copy_Texture_Names();

	Check_for_Textures(VFS);

	geVFile_Close(VFS);

	return 1;
}


// *************************************************************************
// *	  	Copy_Texture_Names:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Loader::Copy_Texture_Names()
{
	int Count = 0;
	int GroupCount = App->CLSB_Model->GroupCount;

	while (Count < GroupCount)
	{
		strcpy(App->CLSB_Model->Group[Count]->Text_FileName, App->CLSB_Model->Group[Count]->Equity_Text_FileName);
		Count++;
	}
}

// *************************************************************************
// *	  	Check_for_Textures:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
int SB_Loader::Check_for_Textures(geVFile* BaseFile)
{
	int Count = 0;
	int GroupCount = App->CLSB_Model->GroupCount;

	char JustName[255];

	while (Count < GroupCount)
	{
		strcpy(JustName, App->CLSB_Model->Group[Count]->Equity_Text_FileName);
		int Len = strlen(JustName);
		JustName[Len - 4] = 0;

		bool test = Check_in_Txl(JustName);

		if (test == 1)
		{
			if (!AddTexture(BaseFile, JustName, Count))
			{
				App->Say("Error");
				return 0;
			}
		}
		else
		{
			App->Say("unMatched");
			App->Say(JustName);
		}

		Count++;
	}

	return -1;
}

// *************************************************************************
// *			AddTexture:- Terry and Hazel Flanigan 2023		  		   *
// *************************************************************************
bool SB_Loader::AddTexture(geVFile* BaseFile, const char* TextureName, int GroupIndex)
{
	App->Get_Current_Document();

	HWND	PreviewWnd;
	HBITMAP	hbm;
	HDC		hDC;

	geBitmap* Bitmap = NULL;
	CWadFile* pWad;
	pWad = NULL;
	pWad = Level_GetWadFile(App->CLSB_Doc->pLevel);
	for (int index = 0; index < pWad->mBitmapCount; index++)
	{
		char mName[MAX_PATH];

		CString Name = pWad->mBitmaps[index].Name;
		strcpy(mName, Name);

		bool test = strcmp(mName, TextureName);
		if (test == 0)
		{
			Bitmap = pWad->mBitmaps[index].bmp;

			PreviewWnd = GetDlgItem(RightGroups_Hwnd, IDC_BASETEXTURE2);
			hDC = GetDC(PreviewWnd);
			hbm = CreateHBitmapFromgeBitmap(Bitmap, hDC);
			
			App->CLSB_Model->Group[GroupIndex]->Base_Bitmap = hbm;
			
			char TempTextureFile_BMP[MAX_PATH];
			strcpy(TempTextureFile_BMP, App->WorldEditor_Directory);
			strcat(TempTextureFile_BMP, "\\");
			strcat(TempTextureFile_BMP, "TextureLoad.bmp");

			App->CLSB_Textures->Genesis_WriteToBmp(Bitmap, TempTextureFile_BMP);

			App->CLSB_Textures->Soil_Load_Texture(App->CLSB_Ogre_Setup->RenderListener->g_Texture, TempTextureFile_BMP, GroupIndex);

			DeleteFile((LPCTSTR)TempTextureFile_BMP);
		}
	}
	
	return TRUE;
}

// *************************************************************************
// *	CreateHBitmapFromgeBitmap:- Terry and Hazel Flanigan 2023 		   *
// *************************************************************************
HBITMAP SB_Loader::CreateHBitmapFromgeBitmap(geBitmap* Bitmap, HDC hdc)
{
	geBitmap* Lock;
	gePixelFormat Format;
	geBitmap_Info info;
	HBITMAP hbm = NULL;

	// <> choose format to be 8,16,or 24, whichever is closest to Bitmap
	Format = GE_PIXELFORMAT_24BIT_BGR;

	if (geBitmap_GetBits(Bitmap))
	{
		Lock = Bitmap;
	}
	else
	{
		if (!geBitmap_LockForRead(Bitmap, &Lock, 0, 0, Format, GE_FALSE, 0))
		{
			return NULL;
		}
	}

	geBitmap_GetInfo(Lock, &info, NULL);

	if (info.Format != Format)
		return NULL;

	{
		void* bits;
		BITMAPINFOHEADER bmih;
		int pelbytes;

		pelbytes = gePixelFormat_BytesPerPel(Format);
		bits = geBitmap_GetBits(Lock);

		bmih.biSize = sizeof(bmih);
		bmih.biHeight = -info.Height;
		bmih.biPlanes = 1;
		bmih.biBitCount = 24;
		bmih.biCompression = BI_RGB;
		bmih.biSizeImage = 0;
		bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 10000;
		bmih.biClrUsed = bmih.biClrImportant = 0;

		if ((info.Stride * pelbytes) == (((info.Stride * pelbytes) + 3) & (~3)))
		{
			bmih.biWidth = info.Stride;
			hbm = CreateDIBitmap(hdc, &bmih, CBM_INIT, bits, (BITMAPINFO*)&bmih, DIB_RGB_COLORS);
		}
		else
		{
			void* newbits;
			int Stride;

			bmih.biWidth = info.Width;
			Stride = (((info.Width * pelbytes) + 3) & (~3));
			newbits = geRam_Allocate(Stride * info.Height);
			if (newbits)
			{
				char* newptr, * oldptr;
				int y;

				newptr = (char*)newbits;
				oldptr = (char*)bits;
				for (y = 0; y < info.Height; y++)
				{
					memcpy(newptr, oldptr, (info.Width) * pelbytes);
					oldptr += info.Stride * pelbytes;
					newptr += Stride;
				}
				hbm = CreateDIBitmap(hdc, &bmih, CBM_INIT, newbits, (BITMAPINFO*)&bmih, DIB_RGB_COLORS);
				geRam_Free(newbits);
			}
		}
	}

	if (Lock != Bitmap)
	{
		geBitmap_UnLock(Lock);
	}

	return hbm;
}

// *************************************************************************
// *	  		Check_in_Txl:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Loader::Check_in_Txl(char* FileName)
{
	int loop = 0;
	int TxlNameCount = NameCount;

	while (loop < TxlNameCount)
	{
		int Result = 1;
		Result = strcmp(FileName, BitMap_Names[loop].Name);
		if (Result == 0)
		{
			return 1;
		}

		loop++;
	}

	return 0;
}

// *************************************************************************
// *				OpenFile:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Loader::Open_File_Model(const char* Extension, const char* Title, const char* StartDirectory)
{

	FileName[0] = 0;
	Path_FileName[0] = 0;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = App->MainHwnd;
	ofn.hInstance = App->hInst;
	ofn.lpstrFile = Path_FileName;						// full path and file name
	ofn.nMaxFile = sizeof(Path_FileName);
	ofn.lpstrFilter = Extension;

	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = FileName;						// Just File Name
	ofn.nMaxFileTitle = sizeof(FileName);;
	ofn.lpstrInitialDir = StartDirectory;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_PATHMUSTEXIST |
		OFN_FILEMUSTEXIST |
		OFN_EXPLORER |
		OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		return 1;
	}

	return 0;
}

// *************************************************************************
// *			Start_Groups:- Terry and Hazel Flanigan 2023	  		   *
// *************************************************************************
bool SB_Loader::Start_Groups_Dialog()
{
//	RightGroups_Hwnd = CreateDialog(App->hInst, (LPCTSTR)IDD_RIGHTGROUPS, App->MainHwnd, (DLGPROC)Groups_Proc);
	ShowWindow(RightGroups_Hwnd, 0);
	return 1;
}

// *************************************************************************
// *			Groups_Proc:- Terry and Hazel Flanigan 2023		  		   *
// *************************************************************************
LRESULT CALLBACK SB_Loader::Groups_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		
	}

	case WM_CTLCOLORSTATIC:
	{
		
		return FALSE;
	}

	case WM_CTLCOLORDLG:
	{
		return (LONG)App->AppBackground;
	}

	case WM_CLOSE:
	{
		/*ShowWindow(App->CL_Groups->RightGroups_Hwnd, 0);
		App->CL_Groups->RightGroups_Visable = 0;
		CheckMenuItem(App->mMenu, ID_WINDOWS_GROUPS, MF_BYCOMMAND | MF_UNCHECKED);*/
		break;
	}


	case WM_NOTIFY:
	{
		

		return CDRF_DODEFAULT;
	}

	case WM_COMMAND:
	{
		
	}

	}
	return FALSE;
}

// *************************************************************************
// *	  				Rotate_Z_Model Terry Bernie						   *
// *************************************************************************
void SB_Loader::Rotate_Z_Model(float Z)
{
	if (App->CLSB_Model->Model_Loaded == 1)
	{

		Ogre::Vector3 Centre;

		Centre.x = App->CLSB_Model->Centre.x;
		Centre.y = App->CLSB_Model->Centre.y;
		Centre.z = App->CLSB_Model->Centre.z;

		Ogre::Vector3 Rotate;
		Rotate.x = 0;
		Rotate.y = 0;
		Rotate.z = Z;


		int Count = 0;
		int VertCount = 0;

		int GroupCount = App->CLSB_Model->Get_Groupt_Count();

		while (Count < GroupCount)
		{
			VertCount = 0;
			while (VertCount < App->CLSB_Model->Group[Count]->GroupVertCount)
			{
				Ogre::Vector3 VertPos;
				Ogre::Vector3 RotatedVert;

				VertPos.x = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x;
				VertPos.y = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y;
				VertPos.z = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z;

				if (Rotate.z != 0) // Dont bother if Zero
				{
					RotatedVert = (Ogre::Quaternion(Ogre::Degree(Rotate.z), Ogre::Vector3::UNIT_X) * (VertPos - Centre));
				}
				else
				{
					RotatedVert = VertPos - Centre;
				}

				RotatedVert += Centre;

				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x = RotatedVert.x;
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y = RotatedVert.y;
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z = RotatedVert.z;

				VertCount++;
			}
			Count++;
		}

		App->CLSB_Model->Set_BondingBox_Model(0);
	}
}

// *************************************************************************
// *	  					Rotate_X_Model Terry Bernie					   *
// *************************************************************************
void SB_Loader::Rotate_X_Model(float X)
{
	if (App->CLSB_Model->Model_Loaded == 1)
	{

		Ogre::Vector3 Centre;

		Centre.x = App->CLSB_Model->Centre.x;
		Centre.y = App->CLSB_Model->Centre.y;
		Centre.z = App->CLSB_Model->Centre.z;

		Ogre::Vector3 Rotate;
		Rotate.x = X;
		Rotate.y = 0;
		Rotate.z = 0;

		int Count = 0;
		int VertCount = 0;

		int GroupCount = App->CLSB_Model->Get_Groupt_Count();

		while (Count < GroupCount)
		{
			VertCount = 0;
			while (VertCount < App->CLSB_Model->Group[Count]->GroupVertCount)
			{
				Ogre::Vector3 VertPos;
				Ogre::Vector3 RotatedVert;

				VertPos.x = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x;
				VertPos.y = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y;
				VertPos.z = App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z;

				if (Rotate.x != 0) // Dont bother if Zero
				{
					RotatedVert = (Ogre::Quaternion(Ogre::Degree(Rotate.x), Ogre::Vector3::UNIT_Y) * (VertPos - Centre));
				}
				else
				{
					RotatedVert = VertPos - Centre;
				}

				RotatedVert += Centre;

				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x = RotatedVert.x;
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y = RotatedVert.y;
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z = RotatedVert.z;

				VertCount++;
			}
			Count++;
		}

		App->CLSB_Model->Set_BondingBox_Model(0);
	}
}

// *************************************************************************
// *	  				Centre_Model_Mid Terry Bernie					   *
// *************************************************************************
void SB_Loader::Centre_Model_Mid(void)
{
	if (App->CLSB_Model->Model_Loaded == 1)
	{
		float X = -App->CLSB_Model->Centre.x;
		float Y = -App->CLSB_Model->Centre.y;
		float Z = -App->CLSB_Model->Centre.z;

		int Count = 0;
		int VertCount = 0;
		int Index = App->CLSB_Model->Get_Groupt_Count();

		while (Count < Index)
		{
			VertCount = 0;
			while (VertCount < App->CLSB_Model->Group[Count]->GroupVertCount)
			{
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x += X;
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y += Y;
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z += Z;
				VertCount++;
			}
			Count++;
		}

		App->CLSB_Model->Set_BondingBox_Model(0);

	}
}

// *************************************************************************
// *	  				Translate_Model Terry Bernie					   *
// *************************************************************************
void SB_Loader::Translate_Model(float X, float Y, float Z)
{
	if (App->CLSB_Model->Model_Loaded == 1)
	{
		int Count = 0;
		int VertCount = 0;

		int GroupCount = App->CLSB_Model->Get_Groupt_Count();

		while (Count < GroupCount)
		{
			VertCount = 0;
			while (VertCount < App->CLSB_Model->Group[Count]->GroupVertCount)
			{
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].x += X;
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].y += Y;
				App->CLSB_Model->Group[Count]->vertex_Data[VertCount].z += Z;
				VertCount++;
			}
			Count++;
		}

		App->CLSB_Model->Set_BondingBox_Model(0);
	}
}

// *************************************************************************
// *			RFActor_Loader:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Loader::RFActor_Loader(void)
{
	int Result = App->CLSB_FileIO->Open_File_Model(App->Equity_Dlg_hWnd,"RF Actor   *.act\0*.act\0", "RF Actor", NULL);
	if (Result == 0)
	{
		return 0;
	}


	//App->CLSB_Model->Clear_Model_And_Reset();

	strcpy(App->CLSB_Loader->Path_FileName, App->CLSB_FileIO->PathFileName);
	strcpy(App->CLSB_Loader->FileName, App->CLSB_FileIO->FileName);

	App->CLSB_Model->Set_Paths();


	App->CLSB_Model->Render_Type = Enums::LoadedFile_Actor;

	//App->CL_Recent_Files->RecentFile_Models_History_Update();
	//App->CL_Prefs->Update_User_File(Model_Path_And_File);

	return 1;
}

#include "FaceList.h"
typedef Gint16 geBody_Index;

// *************************************************************************
// *	  Import_Wavefront_Obj_File:- Terry and Hazel Flanigan 2024        *
// *************************************************************************
bool SB_Loader::Wavefront_Obj_File()
{
	/*App->Get_Current_Document();

	App->CLSB_Assimp->SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
	App->CLSB_Loader->Assimp_Loader(App->Equity_Dlg_hWnd, "Wavefront OBJ   *.obj\0*.obj\0", "Wavefront OBJ");

	FaceList* fl;
	Face* f;

	fl = FaceList_Create(App->CLSB_Assimp->Total_Assimp_FaceCount);
	if (!fl)
	{
		App->Say("Error creating FaceList");
	}

	int Count = 0;
	int GroupCount = App->CLSB_Assimp->Total_Assimp_GroupCount;*/

	//while (Count < GroupCount)
	//{
	//	int FaceCount = 0;
	//	int A = 0;
	//	int B = 0;
	//	int C = 0;

	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//	while (FaceCount < App->CLSB_Assimp->Assimp_Group[Count]->GroupFaceCount)
	//	{
	//		geVec3d FaceVerts[3];
	//		Ogre::Vector2 UV_Cords[3];

	//		A = App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[FaceCount].a;
	//		B = App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[FaceCount].b;
	//		C = App->CLSB_Assimp->Assimp_Group[Count]->Face_Data[FaceCount].c;

	//		//----------------------- Vertices 1
	//		FaceVerts[2].X = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[A].x;
	//		FaceVerts[2].Y = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[A].y;
	//		FaceVerts[2].Z = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[A].z;

	//		UV_Cords[2].x = App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[A].u;
	//		UV_Cords[2].y = App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[A].v;

	//		//----------------------- Vertices 2
	//		FaceVerts[1].X = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[B].x;
	//		FaceVerts[1].Y = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[B].y;
	//		FaceVerts[1].Z = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[B].z;

	//		UV_Cords[1].x = App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[B].u;
	//		UV_Cords[1].y = App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[B].v;

	//		//----------------------- Vertices 3
	//		FaceVerts[0].X = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[C].x;
	//		FaceVerts[0].Y = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[C].y;
	//		FaceVerts[0].Z = App->CLSB_Assimp->Assimp_Group[Count]->vertex_Data[C].z;

	//		UV_Cords[0].x = App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[C].u;
	//		UV_Cords[0].y = App->CLSB_Assimp->Assimp_Group[Count]->MapCord_Data[C].v;
	//		//-----------------------------------------------
	//		f = Face_Create(3, FaceVerts, 0);

	//		if (f)
	//		{
	//			strcpy(f->Tex.Name, "Dummy");
	//			f->Tex.Dib = Level_GetDibId(App->CLSB_Doc->pLevel, "Dummy");

	//			f->Tex.TVecs.uVec.X = 0;
	//			f->Tex.TVecs.uVec.Y = 0;
	//			f->Tex.TVecs.uVec.Z = 0;

	//			f->Tex.txSize = 256;
	//			f->Tex.tySize = 256;

	//			float x = 256 / 20;
	//			float y = 256 / 20;

	//			x = x / 100;
	//			y = y / 100;

	//			x = x / 1.55;
	//			y = y / 1.55;

	//			//App->Say_Float(x);

	//			f->Tex.xScale = x;
	//			f->Tex.yScale = y;

	//			f->Tex.xShift = 256/2;
	//			f->Tex.yShift = 4;

	//			//f->Tex.TVecs.uVec.
	//			Face_SetTextureLock(f, true);
	//
	//			FaceList_AddFace(fl, f);

	//			//Face_UpdateLockedTextureVecs();
	//		}
	//		else
	//		{
	//			App->Say("Error create Face");
	//		}

	//		FaceCount++;
	//		//-----------------------------------------------
	//	}

	//	Count++;
	//}

	/*Brush* NewBrush;

	NewBrush = Brush_Create(BRUSH_LEAF, fl, NULL);
	if (!NewBrush)
	{
		App->Say("Failed to Create Brush");
	}*/

	//geVec3d	mOrigin;

	//mOrigin.X = 0;
	//mOrigin.Y = 0;
	//mOrigin.Z = 0;

	//Brush_Move(NewBrush, &mOrigin);

	//Level_AppendBrush(App->CLSB_Doc->pLevel, NewBrush);
	//Brush_SetVisible(NewBrush, GE_TRUE);
	//SelBrushList_Add(App->CLSB_Doc->pSelBrushes, NewBrush);

	////App->m_pDoc->SetDefaultBrushTexInfo(NewBrush);
	//Brush_Bound(NewBrush);
	//Brush_Center(NewBrush, &mOrigin);

	//Brush_SetName(NewBrush, "TestXX");

	//App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);
	//App->CL_Render_App->Render3D_Mode(ID_VIEW_TEXTUREVIEW);

	//App->m_pDoc->SetModifiedFlag();

	//App->Say("Added");
	return 1;
}

// *************************************************************************
// *	  Strange_UV_Stuff:- Terry and Hazel Flanigan 2024        *
// *************************************************************************
void SB_Loader::Strange_UV_Stuff()
{
	//int UVIndex = 0;
	//// -----------------------------------  UVS
	//App->CLSB_Model->B_Brush[App->CLSB_Model->BrushCount]->MapCord_Data.resize(num_verts);
	//for (i = 0; i < pList->NumFaces; i++)
	//{
	//	const TexInfo_Vectors* TVecs = Face_GetTextureVecs(pList->Faces[i]);
	//	const geVec3d* verts;
	//	geVec3d uVec, vVec;
	//	geFloat U, V;

	//	int txSize, tySize;

	//	Face_GetTextureSize(pList->Faces[i], &txSize, &tySize);

	//	if (txSize == 0)
	//		txSize = 32;
	//	if (tySize == 0)
	//		tySize = 32;

	//	geVec3d_Scale(&TVecs->uVec, 1.f / (geFloat)txSize, &uVec);
	//	geVec3d_Scale(&TVecs->vVec, -1.f / (geFloat)tySize, &vVec);

	//	verts = Face_GetPoints(pList->Faces[i]);
	//	curnum_verts = Face_GetNumPoints(pList->Faces[i]);

	//	for (j = 0; j < curnum_verts; j++)
	//	{
	//		U = geVec3d_DotProduct(&(verts[j]), &uVec);
	//		V = geVec3d_DotProduct(&(verts[j]), &vVec);
	//		U += (TVecs->uOffset / txSize);
	//		V -= (TVecs->vOffset / tySize);

	//		App->CLSB_Model->B_Brush[App->CLSB_Model->BrushCount]->MapCord_Data[UVIndex].u = U;
	//		App->CLSB_Model->B_Brush[App->CLSB_Model->BrushCount]->MapCord_Data[UVIndex].v = V;
	//		UVIndex++;
	//	}
	//}
}
