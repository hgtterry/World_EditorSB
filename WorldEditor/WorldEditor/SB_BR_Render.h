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

class SB_BR_Render
{
public:
	SB_BR_Render();
	~SB_BR_Render();

	void Start_RB_Window();

	void Resize_3DView();

	void Update_Scene();

	void Start_BR_Mode(void);
	void Go_BR_Mode(void);
	void BR_Resize(void);
	bool BR_Mode_Active;
	bool RB_Render_Started;

	Ogre::SceneNode* CamNode;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	HWND MeshView_3D_hWnd;

private:
	
};

