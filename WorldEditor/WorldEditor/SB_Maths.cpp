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

#include "stdafx.h"
#include "AB_App.h"
#include "SB_Maths.h"

SB_Maths::SB_Maths(void)
{
}

SB_Maths::~SB_Maths(void)
{
}

// *************************************************************************
// *		Vector3_Subtract:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void SB_Maths::Vector3_Subtract(const Ogre::Vector3* V1, const Ogre::Vector3* V2, Ogre::Vector3* V1MinusV2)
{

	if (V1MinusV2 == NULL)
	{
		return;
	}

	V1MinusV2->x = V1->x - V2->x;
	V1MinusV2->y = V1->y - V2->y;
	V1MinusV2->z = V1->z - V2->z;
}

// *************************************************************************
// *			Vector3_Scale:- Terry and Hazel Flanigan 2024		 	   *
// *************************************************************************
void SB_Maths::Vector3_Scale(const Ogre::Vector3* VSrc, float Scale, Ogre::Vector3* VDst)
{
	VDst->x = VSrc->x * Scale;
	VDst->y = VSrc->y * Scale;
	VDst->z = VSrc->z * Scale;
}
