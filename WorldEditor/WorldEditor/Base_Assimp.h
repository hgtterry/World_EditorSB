#pragma once

/*
Copyright (c) 2022 - 2024 Reality Factory Scene Builder -- HGT Software W.T.Flanigan H.C.Flanigan

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

typedef struct vertex_type_Assimp
{
	float x, y, z;
}vertex_type_Assimp;

typedef struct polygon_type_Assimp { int a, b, c; }polygon_type_Assimp;
typedef struct normal_type_Assimp { float x, y, z; }normal_type_Assimp;
typedef struct mapcoord_type_Assimp { float u, v; }mapcoord_type_Assimp;
typedef struct int_type_Assimp { int Index; }int_type_Assimp;
typedef struct float_type_Assimp { float Float1; }float_type_Assimp;

class Base_Assimp
{
public:
	Base_Assimp();
	~Base_Assimp();

	char GroupName[255];
	char MaterialName[255];
	char Text_FileName[255];			// Just Texture File Name ie Texture.bmp

	char Equity_Text_FileName[255];			// Just Texture File Name ie Texture.bmp

	char Texture_FolderPath[MAX_PATH];
	char Texture_PathFileName[MAX_PATH];

	int GroupVertCount;	// Group Vertice Count
	int GroupFaceCount;
	int IndicesCount; // Ogre
	int MaterialIndex;
	int Soil_TextureIndex;

	int Depth;
	long Height;
	long Width;

	bool Bitmap_Loaded;

	bool Dont_Use;

	std::vector<vertex_type_Assimp> vertex_Data;		// XYZ
	std::vector<polygon_type_Assimp> Face_Data;		// ABC
	std::vector<normal_type_Assimp> Normal_Data;		// XYZ
	std::vector<mapcoord_type_Assimp> MapCord_Data;	// UV
	std::vector<int_type_Assimp> FaceIndex_Data;

	std::vector<int_type_Assimp> BoneIndex_Data; // Ogre
	std::vector<int_type_Assimp> BA_BoneIndex_Data; // Ogre
	std::vector<int_type_Assimp> BA_BoneVertexIndex_Data; // Ogre
	std::vector<float_type_Assimp> BA_Weight_Data; // Ogre
	int BoneAssignMentCount; // Ogre

	// Bounding Box
	vertex_type_Assimp BB_Max;
	vertex_type_Assimp BB_Min;
	vertex_type_Assimp Size;
	vertex_type_Assimp Centre;
	float radius;

	HBITMAP Base_Bitmap;
	geBitmap* RF_Bitmap;
};

