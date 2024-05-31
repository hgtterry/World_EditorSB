#pragma once
class CL64_Grid
{

public:
	CL64_Grid(void);
	~CL64_Grid(void);

	void Grid_Update(bool Create);

	ManualObject* GridManual;
	SceneNode* GridNode;

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

	bool ShowGridFlag;
	bool ShowDivisions;

};

