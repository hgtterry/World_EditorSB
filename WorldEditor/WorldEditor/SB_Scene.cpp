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
#include "SB_Scene.h"
#include "EntTypeName.h"
#include "facelist.h"

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
	// All flags larger than 0x8000 (i.e. 0x00010000 through 0x80000000)
	// are reserved for user contents.
};

#define NUM_VIEWS (4)

struct tag_Level3
{
	BrushList* Brushes;
	CEntityArray* Entities;
	char* WadPath;
	char* HeadersDir;
	// changed QD Actors
	char* ActorsDir;
	geBoolean ShowActors;
	char* PawnIniPath;
	// end change
	EntTypeNameList* EntTypeNames;
	GroupListType* Groups;
	SizeInfo* WadSizeInfos;
	CWadFile* WadFile;
	EntityTable* pEntityDefs;

	ModelInfo_Type	ModelInfo;

	SkyFaceTexture SkyFaces[6];
	geVec3d SkyRotationAxis;
	geFloat SkyRotationSpeed;
	geFloat	SkyTextureScale;

	// level edit settings
	CompileParamsType CompileParams;
	int GroupVisSetting;
	EntityViewList* pEntityView;

	GridInfo GridSettings;
	geBoolean BspRebuildFlag;
	ViewStateInfo ViewInfo[NUM_VIEWS];

	BrushTemplate_Arch ArchTemplate;
	BrushTemplate_Box	BoxTemplate;
	BrushTemplate_Cone	ConeTemplate;
	BrushTemplate_Cylinder CylinderTemplate;
	BrushTemplate_Spheroid	SpheroidTemplate;
	BrushTemplate_Staircase StaircaseTemplate;

	geVec3d TemplatePos;

	float DrawScale;		// default draw scale
	float LightmapScale;	// default lightmap scale
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
	geVec3d*	Points;
} Face;

struct tag_FaceList
{
	int NumFaces;
	int Limit;
	Face** Faces;
	geBoolean Dirty;
	Box3d Bounds;
};

SB_Scene_Data::SB_Scene_Data()
{
	Project_Resource_Group = "Project_Resource_Group";

	FullScreenMode_Flag = 0;
	SameBrush = 0;
	Scene_Modified = 0;
	BrushChange = -1;
	Area_Added = 0;
	Selected_Brush = NULL;

	Player_Count = 0;
	Player_Added = 0;

	Object_Count = 0;
	UniqueID_Object_Counter = 0;

	Area_Count = 0;
	UniqueID_Area_Count = 0;

	Camera_Count = 0;
	UniqueID_Camera_Count = 0;

	Counters_Count = 0;
	UniqueID_Counters_Count = 0;

	Brush_Index = 0;

	Project_Resources_Created = 0;

	B_Player.reserve(2);
}

SB_Scene_Data::~SB_Scene_Data()
{
}

static geBoolean fdocBrushCSGCallback2(const Brush* pBrush, void* lParam)
{
	return (App->m_pDoc->BrushIsVisible(pBrush) && (!Brush_IsHint(pBrush)) && (!Brush_IsClip(pBrush)));
}

// *************************************************************************
// *			Clear_Level:- Terry and Hazel Flanigan 2022				   *
// *************************************************************************
bool SB_Scene_Data::Clear_Level()
{
	//App->SBC_Gui_Environ->Reset_Class();
	//App->SBC_Project->Reset_Class();
	//App->SBC_FileView->Reset_Class();
	//App->SBC_TopTabs->Reset_Class();
	//App->SBC_Properties->Reset_Class();

	//App->SBC_Markers->BoxNode->setVisible(false);
	//App->SBC_Markers->Arrow_Node->setVisible(false);

	//App->Set_Main_TitleBar(" ");


	//if (App->SBC_Scene->Scene_Loaded == 1)
	//{
	//	App->SBC_Physics->Enable_Physics(0);

	//	App->SBC_Player->Reset_Class();

	//	App->SBC_Com_Area->Reset_Class();

		// Bullet Related
	/*int i;
	for (i = App->CLSB_Bullet->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = App->CLSB_Bullet->dynamicsWorld->getCollisionObjectArray()[i];
		App->CLSB_Bullet->dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}*/
	//}


	//Scene_Modified = 0;

	//Reset_Class(); // This One

	//App->CL_Ogre->OgreListener->GD_CameraMode = Enums::CamNone;

	//App->SBC_Com_Camera->Reset_View();

	Delete_Resources_Group();
	Project_Resources_Created = 0;

	//Reset_Counters();

	//App->CLSB_FileView->SelectItem(App->SBC_FileView->FV_LevelFolder);

	//App->CL_Ogre->mSceneMgr->destroyCamera("PlayerRay");
	//App->CL_Ogre->mSceneMgr->destroyAllParticleSystems();

	return 1;
}

// *************************************************************************
// * 				Build_World:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
void SB_Scene_Data::Build_World(int ExpSelected)
{
	BrushChange = -1;
	SubBrushChange = 0;

	BrushList* BList;
	geBoolean fResult;

	BList = Level_GetBrushes(App->CLSB_Doc->pLevel);
	if (!ExpSelected)
	{
		fResult = Level_Build_G3ds(reinterpret_cast<tag_Level3*> (App->CLSB_Doc->pLevel), "FileName", BList, 0, 0, -1);
	}
	else
	{
		int i, GroupID, GroupCount;
		char NewFileName[MAX_PATH];
		GroupID = -1;
		GroupCount = 1;

		for (i = 0; i < GroupCount; i++)
		{
			BrushList* SBList;
			Brush* pBrush;
			BrushIterator bi;

			SBList = BrushList_Create();
			pBrush = BrushList_GetFirst(BList, &bi);

			while (pBrush != NULL)
			{

				if (SelBrushList_Find(App->CLSB_Doc->pSelBrushes, pBrush))
				{
					Brush* pClone = Brush_Clone(pBrush);
					BrushList_Append(SBList, pClone);
				}

				pBrush = BrushList_GetNext(&bi);
			}
			// do CSG
			{
				ModelIterator	mi;
				int				i, CurId = 0;
				ModelInfo_Type* ModelInfo;
				Model* pMod;

				BrushList_ClearAllCSG(SBList);

				BrushList_DoCSG(SBList, CurId, ::fdocBrushCSGCallback2, this);

				//build individual model mini trees
				ModelInfo = Level_GetModelInfo(App->CLSB_Doc->pLevel);
				pMod = ModelList_GetFirst(ModelInfo->Models, &mi);

				for (i = 0; i < ModelList_GetCount(ModelInfo->Models); i++)
				{
					CurId = Model_GetId(pMod);

					BrushList_DoCSG(SBList, CurId, ::fdocBrushCSGCallback2, this);
				}
			}

			fResult = Level_Build_G3ds(reinterpret_cast<tag_Level3*> (App->CLSB_Doc->pLevel), NewFileName, SBList, 0, 0, -1);
			if (!fResult)
			{
				App->Say("Error exporting group");
			}

			BrushList_Destroy(&SBList);
		}
	}

	//App->Say("Saved");
}

// *************************************************************************
// *		Level_Build_G3ds:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Scene_Data::Level_Build_G3ds(Level3* pLevel, const char* Filename, BrushList* BList, int ExpSelected, geBoolean ExpLights, int GroupID)
{
	App->CLSB_Model->XBrushCount = 0;
	AdjustedIndex_Count = 0;

	int i;
	geBoolean* WrittenTex;

	WrittenTex = (geBoolean*)calloc(sizeof(geBoolean), pLevel->WadFile->mBitmapCount);

	// which textures are used?
	BrushList_GetUsedTextures(BList, WrittenTex, pLevel->WadFile);

	// Add Textures GL
	int AdjustedIndex = 0;
	for (i = 0; i < pLevel->WadFile->mBitmapCount; i++)
	{
		if (WrittenTex[i])
		{
			char matname[MAX_PATH];
			int j, k;
			strncpy(matname, pLevel->WadFile->mBitmaps[i].Name, MAX_PATH - 1);
			AdjusedIndex_Store[AdjustedIndex] = i;

			AddTexture_GL(NULL, matname, AdjustedIndex);

			AdjustedIndex++;

		}
	}
	

	AdjustedIndex_Count = AdjustedIndex;

	BrushList_Export(BList, GE_FALSE);

	free(WrittenTex);

	return 1;
}

static int	BrushCount;
static int	SubBrushCount;

// *************************************************************************
// *		BrushList_Export:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Scene_Data::BrushList_Export(BrushList* BList, geBoolean SubBrush)
{
	Brush* pBrush;
	BrushIterator bi;

	pBrush = BrushList_GetFirst(BList, &bi);

	while (pBrush != NULL)
	{
		
		if (SubBrushCount == 0 && pBrush->Flags & 1 || pBrush->Flags & 1024)
		{
			if (SubBrush == 0)
			{
				App->CLSB_Model->Create_XBrush(BrushCount);
				strcpy(App->CLSB_Model->B_XBrush[BrushCount]->BrushName, pBrush->Name);

				App->CLSB_Model->B_XBrush[BrushCount]->Index = BrushCount;

				App->CLSB_Model->XBrushCount = BrushCount + 1;
			}

		}

		if (!Brush_Export(pBrush))
		{
			App->Say("Cant Convert Brush");
			return GE_FALSE;
		}

		pBrush = BrushList_GetNext(&bi);
		
		if (SubBrush)
		{
			SubBrushCount++;
		}
		else
		{
			BrushCount++;
		}

		if (SubBrush == GE_FALSE)
		{
			SameBrush = 0;
		}
	}

	SubBrushCount = 0;

	if (!SubBrush)
	{
		BrushCount = 0;
	}

	return GE_TRUE;
}

// *************************************************************************
// *			Brush_Export:- Terry and Hazel Flanigan 2023			   *
// *************************************************************************
bool SB_Scene_Data::Brush_Export(const Brush* b)
{
	
	switch (b->Type)
	{
	case BRUSH_MULTI:
		return BrushList_Export(b->BList, GE_TRUE);

	case BRUSH_LEAF:
		if (b->BList)
		{
			return BrushList_Export(b->BList, GE_TRUE);
		}
		else
		{
			if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
			{
				App->Message_ToFile(b->Name,"BRUSH_LEAF");
				return FaceList_Export(b, b->Faces, BrushCount, SubBrushCount);
			}
			else if ((b->Flags & BRUSH_SUBTRACT) && !(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT)))
			{
				BrushCount--;
			}
		}
		break;

	case BRUSH_CSG:
		if (!(b->Flags & (BRUSH_HOLLOW | BRUSH_HOLLOWCUT | BRUSH_SUBTRACT)))
		{
			App->Message_ToFile(b->Name,"BRUSH_CSG");
			return FaceList_Export(b, b->Faces, BrushCount, SubBrushCount);
		}
		break;
	default:
	{
		assert(0);		// invalid brush type
		break;
	}

	}

	return GE_TRUE;
}

// *************************************************************************
// *						FaceList_Export								   *
// *************************************************************************
bool SB_Scene_Data::FaceList_Export(const Brush* b, const FaceList* pList, int BrushCount, int SubBrushCount)
{
	if (BrushCount > BrushChange)
	{
		BrushChange = BrushCount;
		SubBrushChange = 0;
	}

	bool test =App->CLSB_Model->B_XBrush[BrushCount]->Create_Brush(SubBrushChange);
	if (test == 0)
	{
		App->Say("Cant create sub Brush");
	}

	App->Message_ToFile(b->Name, "created sub Brush");

	int i, j, k, num_faces, num_verts, num_mats, num_chars, curnum_verts;
	char matname[MAX_PATH];

	char* matf = (char*)calloc(sizeof(char), pList->NumFaces);

	assert(pList != NULL);
	assert(f != NULL);

	num_faces = num_verts = num_mats = num_chars = 0;
	// get the total number of verts, faces and materials of the object

	for (i = 0; i < pList->NumFaces; i++)
	{
		curnum_verts = Face_GetNumPoints(pList->Faces[i]);
		num_faces += (curnum_verts - 2);
		num_verts += curnum_verts;

		if (!matf[i])
		{
			matf[i] = 1;
			num_mats++;

			for (j = i + 1; j < pList->NumFaces; j++)
			{
				if (strcmp(Face_GetTextureName(pList->Faces[i]), Face_GetTextureName(pList->Faces[j])) == 0)
					matf[j] = 1;
			}

			strncpy(matname, Face_GetTextureName(pList->Faces[i]), MAX_PATH);
		}
	}

	for (i = 0; i < pList->NumFaces; i++)
		matf[i] = 0;

	/*Brush* btemp = NULL;

	btemp = App->CL_Brush->Get_By_Index(BrushCount);
	if (btemp == NULL)
	{
		App->Say("Cant Find Brush");
	}*/

	char buf[MAX_PATH];
	sprintf(buf, "%s_%i", b->Name, BrushCount);
	
	// -----------------------------------  Vertices
	int VertIndex = 0;
	
	App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Vertice_Count = num_verts;
	App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->vertex_Data.resize(num_verts);
	App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Normal_Data.resize(num_verts);

	for (i = 0; i < pList->NumFaces; i++)
	{
		const geVec3d* verts;
		verts = Face_GetPoints(pList->Faces[i]);
		curnum_verts = Face_GetNumPoints(pList->Faces[i]);
		for (j = 0; j < curnum_verts; j++)
		{
			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->vertex_Data[VertIndex].x = verts[j].X;
			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->vertex_Data[VertIndex].y = verts[j].Y;
			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->vertex_Data[VertIndex].z = verts[j].Z;

			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Normal_Data[VertIndex].x = 0.5;
			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Normal_Data[VertIndex].y = 0.5;
			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Normal_Data[VertIndex].z = 0.5;

			VertIndex++;
		}
	}

	int UVIndex = 0;
	// -----------------------------------  UVS
	App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->MapCord_Data.resize(num_verts);

	for (i = 0; i < pList->NumFaces; i++)
	{
		const TexInfo_Vectors* TVecs = Face_GetTextureVecs(pList->Faces[i]);
		const geVec3d* verts;
		geVec3d uVec, vVec;
		geFloat U, V;

		int txSize, tySize;

		Face_GetTextureSize(pList->Faces[i], &txSize, &tySize);

		// make sure that the texture size is set correctly (division!)
		if (txSize == 0)
			txSize = 32;
		if (tySize == 0)
			tySize = 32;

		geVec3d_Scale(&TVecs->uVec, 1.f / (geFloat)txSize, &uVec);
		geVec3d_Scale(&TVecs->vVec, -1.f / (geFloat)tySize, &vVec);

		verts = Face_GetPoints(pList->Faces[i]);
		curnum_verts = Face_GetNumPoints(pList->Faces[i]);

		for (j = 0; j < curnum_verts; j++)
		{
			U = geVec3d_DotProduct(&(verts[j]), &uVec);
			V = geVec3d_DotProduct(&(verts[j]), &vVec);
			U += (TVecs->uOffset / txSize);
			V -= (TVecs->vOffset / tySize);

			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->MapCord_Data[UVIndex].u = U;
			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->MapCord_Data[UVIndex].v = V;

			UVIndex++;
		}
	}

	int FaceIndex = 0;
	// -----------------------------------  Faces
	
	App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Face_Count = num_faces;
	App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Face_Data.resize(num_faces);

	num_verts = 0;
	for (i = 0; i < pList->NumFaces; i++)
	{
		curnum_verts = Face_GetNumPoints(pList->Faces[i]);
		for (j = 0; j < curnum_verts - 2; j++)
		{
			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Face_Data[FaceIndex].a = num_verts;
			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Face_Data[FaceIndex].b = num_verts + 2 + j;
			App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->Face_Data[FaceIndex].c = num_verts + 1 + j;

			FaceIndex++;
		}

		num_verts += curnum_verts;
	}

	// -----------------------------------  Texture IDs
	App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->TextID_Data.resize(500);

	for (i = 0; i < pList->NumFaces; i++)
	{
		if (!matf[i])
		{
			matf[i] = 1;

			int curnum_faces = (Face_GetNumPoints(pList->Faces[i]) - 2);

			for (j = i + 1; j < pList->NumFaces; j++)
			{
				if (strcmp(Face_GetTextureName(pList->Faces[i]), Face_GetTextureName(pList->Faces[j])) == 0)
				{
					curnum_faces += (Face_GetNumPoints(pList->Faces[j]) - 2);
				}
			}

			strncpy(matname, Face_GetTextureName(pList->Faces[i]), 11);

			// Material Name
			int DibId = Get_Adjusted_Index(Face_GetTextureDibId(pList->Faces[i]));
			
			// write face numbers
			curnum_faces = 0;
			for (j = 0; j < i; j++)
			{
				curnum_faces += (Face_GetNumPoints(pList->Faces[j]) - 2);
			}

			curnum_verts = Face_GetNumPoints(pList->Faces[i]);
			for (j = 0; j < curnum_verts - 2; j++)
			{
				int DibId2 = Get_Adjusted_Index(Face_GetTextureDibId(pList->Faces[i]));
				App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->TextID_Data[curnum_faces + j].ID = DibId2;
			}

			curnum_faces += (curnum_verts - 2);

			for (j = i + 1; j < pList->NumFaces; j++)
			{
				curnum_verts = Face_GetNumPoints(pList->Faces[j]);
				if (strcmp(Face_GetTextureName(pList->Faces[i]), Face_GetTextureName(pList->Faces[j])) == 0)
				{
					matf[j] = 1;
					for (k = 0; k < curnum_verts - 2; k++)
					{
						int DibId2 = Get_Adjusted_Index(Face_GetTextureDibId(pList->Faces[i]));
						App->CLSB_Model->B_XBrush[BrushCount]->B_Brush[SubBrushChange]->TextID_Data[curnum_faces + k].ID = DibId2;
					}
				}

				curnum_faces += (curnum_verts - 2);
			}
		}
	}

	free(matf);

	SubBrushChange++;
	return GE_TRUE;
}

// *************************************************************************
// *							Get_Adjusted_Index						   *
// *************************************************************************
int SB_Scene_Data::Get_Adjusted_Index(int RealIndex)
{
	int Count = 0;
	while (Count < 500)
	{
		if (AdjusedIndex_Store[Count] == RealIndex)
		{
			return Count;
		}

		Count++;
	}

	return -1;
}

// *************************************************************************
// *			AddTexture_GL:- Terry and Hazel Flanigan 2023		  	   *
// *************************************************************************
bool SB_Scene_Data::AddTexture_GL(geVFile* BaseFile, const char* TextureName, int GroupIndex)
{
	App->Get_Current_Document();

	//Debug
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

			char TempTextureFile_BMP[MAX_PATH];
			strcpy(TempTextureFile_BMP, App->WorldEditor_Directory);
			strcat(TempTextureFile_BMP, "\\");
			strcat(TempTextureFile_BMP, "TextureLoad.bmp");

			App->CLSB_Textures->Genesis_WriteToBmp(Bitmap, TempTextureFile_BMP);

			App->CLSB_Textures->Soil_Load_Texture(App->CLSB_Ogre_Setup->RenderListener->g_BrushTexture, TempTextureFile_BMP, GroupIndex);

			DeleteFile((LPCTSTR)TempTextureFile_BMP);
		}
	}

	return TRUE;
}

// *************************************************************************
// *		Go_FullScreen_Mode:- Terry and Hazel Flanigan 2022		  	   *
// *************************************************************************
void SB_Scene_Data::Go_FullScreen_Mode(void)
{
	FullScreenMode_Flag = 1;

	//App->SBC_Scene->CurrentCamMode = App->CL_Ogre->OgreListener->GD_CameraMode;

	//App->FullScreen = 1;
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(App->ViewGLhWnd, HWND_TOP, 0, 0, cx, cy, NULL);

	SetParent(App->ViewGLhWnd, NULL);

	App->CLSB_Ogre_Setup->mWindow->resize(cx, cy);

	App->CLSB_Ogre_Setup->mWindow->windowMovedOrResized();
	App->CLSB_Ogre_Setup->mCamera->setAspectRatio((Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getWidth() / (Ogre::Real)App->CLSB_Ogre_Setup->mWindow->getHeight());

	Root::getSingletonPtr()->renderOneFrame();
}

// *************************************************************************
// *		Create_Resources_Group:- Terry and Hazel Flanigan 2022		   *
// *************************************************************************
bool SB_Scene_Data::Create_Resources_Group()
{
	if (Project_Resources_Created == 0)
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(Project_Resource_Group);
		Project_Resources_Created = 1;
	}

	return 1;
}

// *************************************************************************
// *		Delete_Resources_Group:- Terry and Hazel Flanigan 2022 		   *
// *************************************************************************
bool SB_Scene_Data::Delete_Resources_Group()
{

	if (Project_Resources_Created == 1)
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(Project_Resource_Group);
		Project_Resources_Created = 0;
	}

	return 1;
}

// *************************************************************************
// *	Add_Resource_Location_Project:- Terry and Hazel Flanigan 2022	   *
// *************************************************************************
bool SB_Scene_Data::Add_Resource_Location_Project(char* Resource_Location)
{
	bool Test = Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(Resource_Location, Project_Resource_Group);

	if (Test == 0)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Resource_Location, "FileSystem", Project_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(Project_Resource_Group);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(Project_Resource_Group);
	}

	return 1;
}

// *************************************************************************
// *	  Get_Adjusted_Counters_Count:- Terry and Hazel Flanigan 2024	   *
// *************************************************************************
int SB_Scene_Data::Get_Adjusted_Counters_Count(void)
{
	int New_Count = 0;
	int Count = 0;
	int Total = Counters_Count;

	while (Count < Total)
	{
		/*if (App->SBC_Scene->B_Counter[Count]->Deleted == 0)
		{
			New_Count++;
		}*/

		Count++;
	}

	return New_Count;
}

