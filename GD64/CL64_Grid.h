#pragma once
class CL64_Grid
{

public:
	CL64_Grid(void);
	~CL64_Grid(void);

	void Grid_Update(bool Create);
	void Grid_SetVisible(bool Option);

	void Hair_Update(bool Create);
	void Hair_SetVisible(bool Option);

	Ogre::ManualObject* GridManual;
	Ogre::SceneNode* GridNode;

	Ogre::ManualObject* HairManual;
	Ogre::SceneNode* HairNode;

	int XAxis_min;
	int	XAxis_max;
	int YAxis_min;
	int	YAxis_max;
	int ZAxis_min;
	int	ZAxis_max;

	int Division_X;
	int Division_Y;
	int Division_Z;

	int Scale_X;
	int Scale_Y;
	int Scale_Z;

	Ogre::ColourValue ColourMain;
	Ogre::ColourValue ColourDivision;

	int HairExtend;
	Ogre::ColourValue ColourHairZ;
	Ogre::ColourValue ColourHairX;
	Ogre::ColourValue ColourHairY;

	bool ShowGridFlag;
	bool ShowDivisions;
	bool ShowHair;
};

