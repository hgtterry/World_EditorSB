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
#include "SB_Level.h"

struct tag_BrushList
{
	Brush* First;
	Brush* Last;
};

SB_Level::SB_Level(void)
{
}

SB_Level::~SB_Level(void)
{
}

//// *************************************************************************
//// *	  Check_For_Centre_Texture:- Terry and Hazel Flanigan 2023		   *
//// *************************************************************************
//bool SB_Level::Check_For_Centre_Texture()
//{
//	App->Get_Current_Document();
//
//	WadFileEntry* pbmp;
//	pbmp = NULL;
//	pbmp = Level_GetWadBitmap(App->CLSB_Doc->pLevel, "Dummy");
//	if (pbmp == NULL)
//	{
//		//App->Say("Not Found");
//
//		bool test = App->CL_TextureDialog->Open_TXL_File(App->CL_World->mCurrent_TXL_FilePath);
//		if (test == 0)
//		{
//			App->Say(App->CL_World->mCurrent_TXL_FilePath);
//		}
//
//		char Path2[MAX_PATH];
//		strcpy(Path2, App->WorldEditor_Directory);
//		strcat(Path2, "Data");
//		strcat(Path2, "\\");
//		strcat(Path2, "Dummy.bmp");
//
//		HBITMAP Base_Bitmap = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_DUMMY));
//		App->CLSB_Textures->HBITMAP_TO_BmpFile(Base_Bitmap, Path2, "");
//
//		App->CL_TextureDialog->AddTexture(NULL, Path2);
//
//		App->CL_TextureDialog->Save(App->CL_World->mCurrent_TXL_FilePath);
//
//		Level_SetWadPath(App->CLSB_Doc->pLevel, Level_GetWadPath(App->CLSB_Doc->pLevel));
//
//		App->CL_World->Set_Current_TxlPath();
//		App->CLSB_Doc->UpdateAfterWadChange();
//
//	}
//	else
//	{
//		//App->Say("Found");
//	}
//
//	return 1;
//}
//
//// *************************************************************************
//// *	  Check_For_Centre_Brush:- Terry and Hazel Flanigan 2023		   *
//// *************************************************************************
//bool SB_Level::Check_For_Centre_Brush()
//{
//	Level* pLevel = App->CLSB_Doc->pLevel;
//	BrushList* pList = Level_GetBrushes(App->CLSB_Doc->pLevel);
//
//	int Result = 1;
//	bool GotIt = 0;
//	Brush* b;
//
//	b = pList->First;
//	while (b != NULL)
//	{
//		Result = strcmp(b->Name, "XYZ");
//		if (Result == 0)
//		{
//			GotIt = 1;
//			break;
//		}
//
//		b = b->Next;
//	}
//
//	if (GotIt == 0)
//	{
//		//App->Say("Create XYZ");
//
//		App->m_pDoc->OnToolsTemplate();
//
//		BrushTemplate_Box* pBoxTemplate;
//		pBoxTemplate = Level_GetBoxTemplate(App->CLSB_Doc->pLevel);
//
//		pBoxTemplate->Solid = 0;
//		pBoxTemplate->XSizeBot = 2;
//		pBoxTemplate->XSizeTop = 2;
//		pBoxTemplate->ZSizeBot = 2;
//		pBoxTemplate->ZSizeTop = 2;
//		pBoxTemplate->YSize = 2;
//
//		Brush* pCube;
//		pCube = BrushTemplate_CreateBox(pBoxTemplate);
//		if (pCube != NULL)
//		{
//			App->m_pDoc->LastTemplateTypeName = "XYZ";
//
//			App->CLSB_File_WE->CreateNewTemplateBrush(pCube);
//			App->CLSB_Doc->AddBrushToWorld();
//
//		}
//	}
//
//	return 1;
//}
//
//// *************************************************************************
//// *	  Change_Centre_Brush_Texture:- Terry and Hazel Flanigan 2023	   *
//// *************************************************************************
//bool SB_Level::Change_Centre_Brush_Texture()
//{
//	App->Get_Current_Document();
//
//	App->CLSB_Doc->ResetAllSelections();
//	App->CLSB_Doc->UpdateSelected();
//	
//	int Index = App->CL_World->Get_Brush_Index_By_Name("XYZ");
//	if (Index == -1)
//	{
//		App->Say("Error");
//		return 0;
//	}
//
//	Brush* Selected_Brush = NULL; 
//	Selected_Brush = App->CL_World->Get_Brush_ByIndex(Index);
//	if (Selected_Brush == NULL)
//	{
//		App->Say("Error");
//		return 0;
//	}
//
//	SelBrushList_Add(App->CLSB_Doc->pSelBrushes, Selected_Brush);
//	
//	App->CLSB_Doc->UpdateSelected();
//
//	App->m_pDoc->SelectAllFacesInBrushes();
//
//	App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);
//
//	const int NumFaces = Brush_GetNumFaces(Selected_Brush);
//	
//	//copy face TexInfos
//	int			i;
//	for (i = 0; i < NumFaces; i++)
//	{
//		Face* f = Brush_GetFace(Selected_Brush, i);
//		WadFileEntry* pbmp;
//
//		Face_SetTextureName(f, "Dummy");
//		Face_SetTextureDibId(f, Level_GetDibId(App->CLSB_Doc->pLevel, "Dummy"));
//		pbmp = Level_GetWadBitmap(App->CLSB_Doc->pLevel, "Dummy");
//		if (pbmp != NULL)
//		{
//			Face_SetTextureSize(f, pbmp->Width, pbmp->Height);
//		}
//
//	}
//
//	App->CLSB_Doc->ResetAllSelections();
//	App->CLSB_Doc->UpdateSelected();
//
//	Brush_SetVisible(Selected_Brush, GE_FALSE);
//
//	Brush_UpdateChildFaces(Selected_Brush);
//	Brush_SetFaceListDirty(Selected_Brush);
//	App->CLSB_Doc->UpdateAllViews(UAV_ALL3DVIEWS, NULL);
//	App->m_pDoc->SetModifiedFlag();
//
//	return 1;
//}
