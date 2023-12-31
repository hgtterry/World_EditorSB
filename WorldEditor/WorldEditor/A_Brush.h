#pragma once

#include "brush.h"
#include "basetype.h"
#include "box3d.h"


class A_Brush
{
public:
	A_Brush(void);
	~A_Brush(void);

	const char* Brush_GetName(const Brush *b);
	const BrushList* Brush_GetBrushList(const Brush *b);
	int Get_Brush_Count(void);
	Brush* Get_By_Name(char* BrushName);
	BrushList* BrushList_Create(void);
	Brush* Get_By_Index(int Index);
	bool Brush_Create_From_Equity();
	bool Check_if_Name_Exist(char* Name);
};



