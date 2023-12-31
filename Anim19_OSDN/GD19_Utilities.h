/*
Copyright (c) GameDirector 2019 Inflanite Software W.T.Flanigan H.C.Flanigan B.Parkin

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
class GD19_Utilities
{
public:
	GD19_Utilities(void);
	~GD19_Utilities(void);

	bool TestForBadCharactors(char* buf);
	bool ReverseBackSlash(char* buf);

	bool OpenHTML(char* HelpTitle);
	bool OpenHTML_WEB(char* HelpTitle);

	float floatPercent(int Val);

	float DegreesToRadians(float Degrees);
	float RadiansToDegrees(float Radians);

	std::string Get_FileName_From_Path(char* pString);

	char Return_Chr[255];
};

