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

#include "Ogre.h"

class CL64_OGL_Listener : public Ogre::RenderQueueListener
{
public:
	CL64_OGL_Listener(void);
	~CL64_OGL_Listener(void);

	float RX;
	float RZ;

private:

	virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& skipThisInvocation);

	virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation,
		bool& repeatThisInvocation);

	void PreRender();
	void PostRender();
	void Render_Loop();
	void Translate(void);

	bool Assimp_Render_Textures(void);
	bool Assimp_Textured_Parts(int Count);

	bool Assimp_Render_Faces(void);
	bool Assimp_Face_Parts(int Count);

	void RenderCrossHair(void);

	float Hair_1PosX;
	float Hair_1PosY;
	float Hair_1PosZ;

	float Hair_1RotX;
	float Hair_1RotY;
	float Hair_1RotZ;

	bool ShowFaces;
	bool Light_Activated;
};

