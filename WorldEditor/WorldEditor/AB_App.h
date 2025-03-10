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

#if !defined(AFX_SB_APP_H__F1CE337E_1C46_4F80_9639_566F8F7DFBD6__INCLUDED_)
#define AFX_SB_APP_H__F1CE337E_1C46_4F80_9639_566F8F7DFBD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FUSIONDoc.h"
//#include "FUSIONView.h"

#include "AB_Export_RFW.h"
#include "A_FileIO.h"
#include "A_Dialogs.h"
#include "A_Scene.h"
#include "A_Render_App.h"
#include "A_TxlEditor.h"
#include "SB_Tabs_Texture_Dlg.h"
#include "A_FaceDialog.h"

#include "SB_Tabs_Control.h"
#include "SB_Tabs_Brushes_Dlg.h"
#include "SB_Tabs_Templates_Dlg.h"
#include "SB_Tabs_Game_Dlg.h"

#include "A_CreateArchDialog.h"
#include "A_CreateBoxDialog.h"
#include "A_CreateConeDialog.h"
#include "A_CreateCylDialog.h"
#include "A_CreateSpheroidDialog.h"
#include "A_CreateStaircaseDialog.h"
#include "A_Camera.h"
#include "A_TopTabs.h"
#include "SB_File_WE.h"
#include "A_Brush.h"

#include "A_MainView.h"

#include "SB_Export_World.h"
#include "F_View.h"

// Scene Builder 
#include "SB_Ogre_Setup.h"
#include "SB_Grid.h"
#include "SB_Equity.h"
#include "SB_Keyboard.h"
#include "SB_Camera.h"
#include "SB_Model.h"
#include "SB_Loader.h"
#include "SB_Ini.h"

#include "SB_Ogre3D.h"
#include "SB_Export_Object.h"
#include "SB_Export_3DS.h"
#include "SB_Export_Milkshape.h"
#include "SB_Exporter.h"

#include "SB_Textures.h"
#include "SB_Dialogs.h"
#include "SB_Brushes.h"
#include "SB_RecentFiles.h"
#include "SB_TopTabs_Equity.h"
#include "SB_Preferences.h"
#include "SB_Doc.h"
#include "SB_Panels.h"
#include "SB_Level.h"
#include "SB_Scene.h"
#include "SB_ViewMgrDlg.h"

#include "SB_Player.h"

#include "SB_Object.h"
#include "SB_Project.h"
#include "SB_Objects_Create.h"
#include "SB_Physics.h"

#include "SB_Picking.h"

#include "SB_Mesh_Mgr.h"

#include "SB_PB.h"
#include "SB_BR_Render.h"
#include "Base_Faces.h"

#include "SB_Game_Editor.h"
#include "SB_Project_Create.h"
#include "SB_Test_View.h"
#include "SB_Maths.h"

class SB_App
{
public:
	SB_App();
	~SB_App();

	//std::stack::size();

	AB_Export_RFW*				ABC_Export_RFW;
	SB_FileIO*					CLSB_FileIO;
	A_Dialogs*					CL_Dialogs;
	A_World*					CL_World;
	A_Render_App*				CL_Render_App;
	A_TxlEditor*				CL_TxlEditor;
	SB_Tabs_Texture_Dlg*		CLSB_TextureDialog;
	A_FaceDialog*				CL_FaceDialog;
	SB_Tabs_Control*			CLSB_TabsControl;
	SB_Tabs_Brushes_Dlg*		CL_TabsGroups_Dlg;
	SB_Tabs_Templates_Dlg*		CLSB_Tabs_Templates_Dlg;
	SB_Tabs_Game_Dlg*			CLSB_Tabs_True3D_Dlg;

	A_CreateArchDialog*			CL_CreateArchDialog;
	A_CreateBoxDialog*			CL_CreateBoxDialog;
	A_CreateConeDialog*			CL_CreateConeDialog;
	A_CreateCylDialog*			CL_CreateCylDialog;
	A_CreateSpheroidDialog*		CL_CreateSpheroidDialog;
	A_CreateStaircaseDialog*	CL_CreateStaircaseDialog;
	SB_Camera_WE*				CLSB_Camera_WE;
	SB_TopTabs*					CLSB_TopTabs;
	SB_File_WE*					CLSB_File_WE;
	A_Brush*					CL_Brush;

	SB_Export_World*			CLSB_Export_World;

	A_MainView*					CL_Main_View;

	F_View*						CL_F_View;

	// Scene Builder 
	SB_Ogre_Setup*				CLSB_Ogre_Setup;
	SB_Grid*					CLSB_Grid;
	SB_Equity*					CLSB_Equity;
	SB_Keyboard*				CLSB_Keyboard;
	SB_Camera_EQ*				CLSB_Camera_EQ;
	SB_Model*					CLSB_Model;
	SB_Loader*					CLSB_Loader;
	SB_Ini*						CLSB_Ini;

	SB_Export_3DS*				CLSB_Export_3DS;
	SB_Ogre3D*					CLSB_Ogre3D;
	SB_Export_Object*			CLSB_Export_Object;
	SB_Export_Milkshape*		CLSB_Export_Milkshape;
	SB_Exporter*				CLSB_Exporter;

	SB_Textures*				CLSB_Textures;
	SB_Dialogs*					CLSB_Dialogs;
	SB_Brushes*					CLSB_Brushes;
	SB_RecentFiles*				CLSB_RecentFiles;
	SB_TopTabs_Equity*			CLSB_TopTabs_Equity;
	SB_Preferences*				CLSB_Preferences;
	SB_Doc*						CLSB_Doc;
	SB_Panels*					CLSB_Panels;
	SB_Level*					CLSB_Level;
	SB_Scene_Data*				CLSB_Scene_Data;
	SB_ViewMgrDlg*				CLSB_ViewMgrDlg;

	SB_Player*					CLSB_Player;

	SB_Object*					CLSB_Object;
	SB_Project*					CLSB_Project;
	SB_Objects_Create*			CLSB_Objects_Create;
	SB_Physics*					CLSB_Physics;
	
	SB_Picking*					CLSB_Picking;
	
	SB_Mesh_Mgr*				CLSB_Mesh_Mgr;

	SB_PB*						CLSB_PB;
	SB_BR_Render*				CLSB_BR_Render;
	Base_Faces*					CLSB_BaseFaces;
	SB_Game_Editor*				CLSB_Game_Editor;
	SB_Project_Create*			CLSB_Project_Create;
	SB_Test_View*				CL_Test_View;
	SB_Maths*					CL_Maths;


	bool InitApp(void);
	void InitMFC(void);

	void SetBrushes_Fonts(void);
	void LoadProgramResource(void);

	void Start_Dialogs();
	void Enable_Dialogs(bool Enable);

	void Beep_Win();
	void Say(const char* Message, char* Message2 = NULL);
	void Say_Int(int Value);
	void Say_Float(float Value);
	void Say_Vector3(Ogre::Vector3 Vector);
	void Message_ToFile(char* Message, char* Message2 = NULL);
	void Error_ToFile(char* Message, char* Message2 = NULL);
	void Clear_ErrorLog();
	void Close_WorldEditor();

	void RedrawWindow_Dlg(HWND hDlg);

	void Flash_Window();
	bool IsProcessRunning(char* executableName);
	uint64_t Get_Stack(void);

	bool Custom_Button_Greyed(LPNMCUSTOMDRAW item);
	bool Custom_Button_Normal(LPNMCUSTOMDRAW item);
	bool Custom_Button_Toggle(LPNMCUSTOMDRAW item, bool Toggle);
	bool Custom_Button_Toggle_Tabs(LPNMCUSTOMDRAW item, bool Toggle);
	
	void Debug_Set(void);
	void Debug_Close(void);
	void Debug_Message(char* Message, bool NewLine);
	void Debug_Int(int Value, bool NewLine);
	void Debug_Float(float Value, bool NewLine);
	bool Check_World_Loaded(void);
	
	HBRUSH CreateGradientBrush(COLORREF top, COLORREF bottom, LPNMCUSTOMDRAW item);

	char WorldEditor_Directory[MAX_PATH];

	HBRUSH AppBackground;	
	HBRUSH BlackBrush;		
	HBRUSH Brush_White;		
	HBRUSH Brush_Green;		

	HBRUSH Brush_But_Pressed;
	HBRUSH Brush_But_Normal;
	HBRUSH Brush_But_Hover;
	HBRUSH Brush_Tabs_UnSelected;
	HBRUSH Brush_Tabs;

	HFONT Font_CB15;
	HFONT Font_CB18;
	HFONT Font_Arial20;
	HFONT Font_Banner;

	HANDLE Hnd_HollowBox_Bmp;
	HANDLE Hnd_SolidBox_Bmp;

	HANDLE Hnd_HollowSpheroid_Bmp;
	HANDLE Hnd_SolidSpheroid_Bmp;

	HANDLE Hnd_GridOn_Bmp;
	HANDLE Hnd_GridOff_Bmp;
	HANDLE Hnd_ModelInfo_Bmp;		// Bitmap for button Model Info Screen
	HANDLE Hnd_ModelInfoOn_Bmp;
	HANDLE Hnd_BBOn_Bmp;
	HANDLE Hnd_BBOff_Bmp;
	HANDLE Hnd_MeshOn_Bmp;
	HANDLE Hnd_MeshOff_Bmp;
	HANDLE Hnd_HairOn_Bmp;
	HANDLE Hnd_HairOff_Bmp;
	HANDLE Hnd_TexturesOn_Bmp;
	HANDLE Hnd_TexturesOff_Bmp;
	HANDLE Hnd_MeshPointsOn_Bmp;
	HANDLE Hnd_MeshPointsOff_Bmp;
	HANDLE Hnd_BonesOn_Bmp;
	HANDLE Hnd_BonesOff_Bmp;
	HANDLE Hnd_NormalsOn_Bmp;
	HANDLE Hnd_NormalsOff_Bmp;
	HANDLE Hnd_LightsOn_Bmp;
	HANDLE Hnd_LightsOff_Bmp;
	HANDLE Hnd_FullScreen_Bmp;

	HANDLE Hnd_Arch_Icon;
	HANDLE Hnd_Cone_Icon;
	HANDLE Hnd_Cyl_Icon;
	HANDLE Hnd_Spheriod_Icon;
	HANDLE Hnd_Box_Icon;
	HANDLE Hnd_Stairs_Icon;

	bool Debug_App;
	bool Debug_File;
	bool Just_Equity;
	bool Centre_Debug;

	bool File_Loaded_Flag;
	bool Block_RB_Actions;
	bool BR_True3D_Mode_Active;
	bool Create_User_Mesh;
	HCURSOR CUR;

	int CursorPosX;
	int CursorPosY;

	HINSTANCE hInst;
	HWND MainHwnd;
	HWND ViewGLhWnd;
	HWND ListPanel;
	HMENU hMenu_WE;
	HWND WE_3DView_Hwnd;

	HWND Equity_Dlg_hWnd;

	//      MFC
	CMainFrame *m_pMainFrame;
	CFusionDoc *pCFusionDoc;

	FILE *Write_DebugFile;

	void Get_Current_Document();

	CFusionDoc* m_pDoc;
};

extern SB_App* App;

#endif // !defined(AFX_SB_APP_H__F1CE337E_1C46_4F80_9639_566F8F7DFBD6__INCLUDED_)
