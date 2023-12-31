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
class SB_Com_Area
{
public:

	SB_Com_Area();
	~SB_Com_Area();

	void Reset_Class(void);

	Ogre::Vector3 Get_BoundingBox_World_Centre(int Object_Index);
	void UpDate_Physics_And_Visuals(int Index);
	void Set_Physics(int Index);
	void Set_Area_Defaults(int Index);
	bool Add_New_Area();
	void Add_Aera_To_Project(int Index, char* FileName, char* Resource_Location);

protected:

	btBvhTriangleMeshShape* Create_Area_Trimesh(int Index, Base_Area* Object);
};

