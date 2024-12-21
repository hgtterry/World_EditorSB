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

class A_MainView
{
public:
	A_MainView(void);
	~A_MainView(void);

	void RenderOrthoView(ViewVars *v, CDC *pDC);

	CFusionDoc			*m_pDoc;

private:

	static geBoolean BrushDraw( Brush *pBrush, void *lParam);
	void Render_RenderBrushFacesOrtho( const ViewVars *Cam, Brush *b, HDC ViewDC);
	POINT Render_OrthoWorldToView(const ViewVars *v, geVec3d const *wp);

};
