/*
Copyright (c) 2021 EquitySB and EquityME -- Inflanite Software W.T.Flanigan H.C.Flanigan

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

#pragma once
class SB_LookUps
{
public:

	SB_LookUps(void);
	~SB_LookUps(void);

	int Get_Adjusted_Object_Count(void);
	int Get_Adjusted_Counters_Count(void);

	int CheckNames_Objects(char* Name);

	bool Get_Usage(int Index);

	bool Show_Entities(bool YesNo);
	bool Is_Meshes_Used(char* Name);

	bool Update_Types();
	
	char Chr_Type[32];
	char Chr_Shape[32];
	char Chr_Usage[32];

protected:

	bool Get_Type(int Index);
	bool Get_Shape(int Index);
	

};

