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

#include "Base_Object.h"

class SB_Game_Editor
{
public:
	SB_Game_Editor(void);
	~SB_Game_Editor(void);

	void Go_Game_Editor();
	
	void Hide_Game_Editor_Dialog();
	void Show_Objects(bool Flag);
	void Set_Camera();
	void Reset_Camera();

	bool Project_Loaded;

	HMENU GE_Menu;

	std::vector<Base_Object*> V_Object;
};

