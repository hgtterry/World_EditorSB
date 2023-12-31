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

class A_CreateSpheroidDialog
{
public:
	A_CreateSpheroidDialog(void);
	~A_CreateSpheroidDialog(void);

	void Start_CreateSpheroid_Dlg();

	BrushTemplate_Spheroid *pSpheroidTemplate;

	int		m_HorizontalBands;
	int		m_VerticalBands;
	float	m_YSize;
	int		m_Solid;
	float	m_Thickness;
	bool	m_TCut;

	bool	m_UseCamPos;

private:

	static LRESULT CALLBACK CreateSpheroid_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Set_Members();
	void Set_DLG_Members(HWND hDlg);
	void Get_DLG_Members(HWND hDlg);
	void Set_SpheroidTemplate();

	void Set_Defaults(HWND hDlg);

	void CreateSpheroid();
	void CreateNewTemplateBrush(Brush *pBrush);

	CFusionDoc			*m_pDoc;

	char SpheroidName[MAX_PATH];
};
