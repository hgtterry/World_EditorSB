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

#pragma once
class CL64_ImGui
{
public:

	CL64_ImGui(void);
	~CL64_ImGui(void);

	// Setup -----------------------------------
	void Init_ImGui(void);
	void ImGui_Set_Colours(void);
	void Load_Font(void);

	float PosX;
	float PosY;

	// -----------------------------------------

	void ImGui_Render_Loop(void);
	void ImGui_FPS(void);
	void Camera_Data_GUI(void);
	void Model_Data_GUI(void);

	bool StartPos;
	bool Show_FPS;
	bool Show_ImGui_Demo;
	bool Show_Camera_Data_F;
	bool Show_Model_Data_F;

	float Camera_Data_PosX;
	float Camera_Data_Posy;

	float Model_Data_PosX;
	float Model_Data_PosY;
};

