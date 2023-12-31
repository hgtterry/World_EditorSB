/*
Copyright (c) 2022 GameDirectorSB and EquityME -- HGTInflanite Software W.T.Flanigan H.C.Flanigan

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
class SB_Visuals
{
public:
	SB_Visuals(void);
	~SB_Visuals(void);

	void Load_PickSight(void); // BERNIE_GUN_SIGHT
	bool MarkerBB_Setup(void);
	bool MarkerBB_Update(float Depth,float Height,float Width);
	void MarkerBB_Addjust(int Index);

	Ogre::Overlay*	mPickSight;

	ManualObject*	BoxManual;
	SceneNode*		BoxNode;
};

