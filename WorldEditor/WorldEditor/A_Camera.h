#pragma once

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

class SB_Camera_WE
{
public:
	SB_Camera_WE(void);
	~SB_Camera_WE(void);

	void Start_Move_Camera();
	geVec3d Get_Camera_Position();

	CEntity* FindCameraEntity();
	void Zero_Camera();
	void Reset_Camera_Position();
	void Reset_Camera_Angles();

	void Move_Camera_Forward(float Step);
	void Move_Camera_Back(float Step);

	void Move_Camera_Down(float Step);
	void Move_Camera_Up(float Step);

	void Move_Camera_Right(float Step);
	void Move_Camera_Left(float Step);

	bool KeyBeingPresed_Flag;

	CEntity* pCameraEntity;
	geVec3d CameraPosition;
	geVec3d Angles;

private:

	static LRESULT CALLBACK Move_Camera_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Update_Dlg(HWND hDlg);
	void Get_Values_Dlg(HWND hDlg);
};
