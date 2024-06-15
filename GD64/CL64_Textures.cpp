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
#include "CL64_Textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CL64_Textures::CL64_Textures(void)
{
	TextureFileName[0] = 0;
}

CL64_Textures::~CL64_Textures(void)
{
}

// *************************************************************************
// *		Load_Textures_Assimp:- Terry and Hazel Flanigan 2024	  	   *
// *************************************************************************
void CL64_Textures::Load_Textures_Assimp()
{
	int v = 0;
	int Count = 0;
	bool DummyCreated = 0;

	int mGroupCount = App->CL_Scene->GroupCount;

	while (Count < mGroupCount)
	{
		char JustFileName[MAX_PATH];
		GetFileTitleA(App->CL_Scene->Group[Count]->Text_FileName, JustFileName, MAX_PATH);
		strcpy(App->CL_Scene->Group[Count]->Text_FileName, JustFileName);

		int Test = strcmp(JustFileName, "No_Texture");
		if (Test != 0) // not a match
		{

			char ImageFullPath[MAX_PATH];
			strcpy(ImageFullPath, App->CL_Scene->Texture_FolderPath);
			strcat(ImageFullPath, App->CL_Scene->Group[Count]->Text_FileName);

			strcpy(App->CL_Scene->Group[v]->Texture_PathFileName, ImageFullPath);

			strcpy(TextureFileName, ImageFullPath);

			//Windows_Preview_FullPath(v, ImageFullPath);

			bool test = Load_OpenGL_Textures(App->CL_Scene->Group[Count]->MaterialIndex);
			if (test == 0)
			{
				//App->Error_ToFile("Loading Dummy Texture Instead");
				App->CL_Textures->Create_DummyTexture(App->CL_Scene->Texture_FolderPath);

				char buf[MAX_PATH];
				strcpy(buf, App->CL_Scene->Texture_FolderPath);
				strcat(buf, "TTemp.bmp");
				UINT* Texture_List = g_Texture;
				Soil_Load_Texture(Texture_List, buf, App->CL_Scene->Group[Count]->MaterialIndex);

				//App->CL_Loader->LoadError = 1;
				DummyCreated = 1;
			}
			v++;
		}
		else
		{
			//App->Error_ToFile("No Texture in File");
			//App->Error_ToFile("Loading Dummy Texture Instead");
			App->CL_Textures->Create_DummyTexture(App->CL_Scene->Texture_FolderPath);

			char buf[MAX_PATH];
			strcpy(buf, App->CL_Scene->Texture_FolderPath);
			strcat(buf, "TTemp.bmp");
			UINT* Texture_List = g_Texture;
			Soil_Load_Texture(Texture_List, buf, App->CL_Scene->Group[Count]->MaterialIndex);

			//App->CL_Loader->LoadError = 1;
			DummyCreated = 1;

			v++;
		}

		Count++;
	}

	if (DummyCreated == 1)
	{
		char buf[MAX_PATH];
		strcpy(buf, App->CL_Scene->Texture_FolderPath);
		strcat(buf, "TTemp.bmp");
		remove(buf);
	}
}

// *************************************************************************
// *		Create_DummyTexture:- Terry and Hazel Flanigan 2024    	 	   *
// *************************************************************************
bool CL64_Textures::Create_DummyTexture(char* Folder)
{
	//HBITMAP hbmpTemp;
	//hbmpTemp = LoadBitmap(App->hInst, MAKEINTRESOURCE(IDB_DUMMY));

	char OutFile[MAX_PATH];
	strcpy(OutFile, Folder);
	strcat(OutFile, "TTemp.bmp");

	//HBITMAP_TO_BmpFile(hbmpTemp, OutFile, "");
	return 1;
}

// *************************************************************************
// *		Load_OpenGL_Textures:- Terry and Hazel Flanigan 2024   	  	   *
// *************************************************************************
bool CL64_Textures::Load_OpenGL_Textures(int TextureID)
{
	int Index = 0;
	int Dont = 0;
	int jpg = 0;
	int DontAdd = 0;
	int AddDummy = 0;

	char buf[1024];
	strcpy(buf, App->CL_Scene->Texture_FolderPath);
	strcat(buf, "Etemp.bmp");

	UINT* Texture_List = g_Texture;

	// ----------------------------------- Bitmap
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".BMP") == 0)
	{
		bool test = Soil_Load_Texture(Texture_List, TextureFileName, TextureID);
		if (test == 0)
		{
			return 0;
		}

		return 1;
	}
	// ------------------------------------ JPEG
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".JPG") == 0)
	{
		bool test = Soil_Load_Texture(Texture_List, TextureFileName, TextureID);

		if (test == 0)
		{
			//App->Error_ToFile("Failed to load JPG", TextureFileName);
			return 0;
		}

		return 1;
	}
	// ------------------------------------ TGA
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".TGA") == 0)
	{
		Soil_Load_Texture(Texture_List, TextureFileName, TextureID);
		return 1;
	}
	//// ------------------------------------ DDS
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".DDS") == 0)
	{
		Soil_Load_Texture(Texture_List, TextureFileName, TextureID);
		return 1;
	}
	//// ------------------------------------ PNG
	if (_stricmp(TextureFileName + strlen(TextureFileName) - 4, ".PNG") == 0)
	{
		Soil_Load_Texture(Texture_List, TextureFileName, TextureID);
		return 1;
	}

	/*CreateDummyTexture();
	Soil_Load_Texture(g_Texture, buf, TextureID);
	remove(buf);*/

	return 1;
}

// *************************************************************************
// *		 Soil_Load_Texture:- Terry and Hazel Flanigan 2024		  	   *
// *************************************************************************
bool CL64_Textures::Soil_Load_Texture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	int image_width = 0;
	int image_height = 0;
	int channels = 0;
	int force_channels = 4;

	unsigned char* image_data = stbi_load(strFileName, &image_width, &image_height, &channels, force_channels);
	if (image_data == NULL)
	{
		//App->Say("Cant Create Texture");
		return 0;
	}

	if ((force_channels >= 1) && (force_channels <= 4))
	{
		channels = force_channels;
	}

	bool INVERT_Y = 1;
	if (INVERT_Y == 1)
	{
		int i, j;
		for (j = 0; j * 2 < image_height; ++j)
		{
			int index1 = j * image_width * channels;
			int index2 = (image_height - 1 - j) * image_width * channels;
			for (i = image_width * channels; i > 0; --i)
			{
				unsigned char temp = image_data[index1];
				image_data[index1] = image_data[index2];
				image_data[index2] = temp;
				++index1;
				++index2;
			}
		}
	}

	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	textureArray[textureID] = image_texture;

	stbi_image_free(image_data);

	if (textureArray[textureID] == 0) // Fall back attemp to convert and load or Bail
	{
		//Texture_To_Bmp(strFileName);
		remove("Etemp.bmp");
	}

	return 1;
}
