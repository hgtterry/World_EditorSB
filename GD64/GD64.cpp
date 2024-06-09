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

#include "pch.h"
#include "framework.h"
#include "GD64.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void StartOgre();
void Close_App();
LRESULT CALLBACK Ogre3D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int Block_Call = 0;

CL64_App* App = NULL;

// *************************************************************************
// *				WinMain:- Terry and Hazel Flanigan 2024		  		   *
// *************************************************************************
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    InitCommonControls();

    App = new CL64_App();

	char* a_cwd = _getcwd(App->GD_Directory_FullPath, MAX_PATH);

    App->InitApp();

	App->CL_Preferences->Read_Preferences();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_GD64, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GD64));

    App->SetMainWinCentre();


    App->CL_SplitterViews->Init_Views();
    App->CL_SplitterViews->Create_Top_Left_Win();
    App->CL_SplitterViews->Create_Top_Right_Win();
    App->CL_SplitterViews->Create_Bottom_Left_Window();

    App->CL_SplitterViews->Resize_Windows(App->Fdlg, App->CL_SplitterViews->nleftWnd_width, App->CL_SplitterViews->nleftWnd_Depth);

	

    SetTimer(App->MainHwnd, 1, 1, NULL);

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

// *************************************************************************
// *			MyRegisterClass:- Terry and Hazel Flanigan 2024	  	 	   *
// *************************************************************************
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = 0;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GD64));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_GD64);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

// *************************************************************************
// *			InitInstance:- Terry and Hazel Flanigan 2024			   *
// *************************************************************************
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   App->hInst = hInstance; // Store instance handle in our global variable

   App->MainHwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       0, 0, 1200, 770, nullptr, nullptr, App->hInst, nullptr);

   App->Fdlg = CreateDialog(App->hInst, (LPCTSTR)IDD_FILEVIEW, App->MainHwnd, (DLGPROC)App->CL_SplitterViews->ViewerMain_Proc);

   int cx = GetSystemMetrics(SM_CXSCREEN);
   int cy = GetSystemMetrics(SM_CYSCREEN);
   MoveWindow(App->Fdlg, 0, 0, cx, cy, TRUE);

   App->ViewGLhWnd = CreateDialog(App->hInst, (LPCTSTR)IDD_VIEWER3D, App->Fdlg, (DLGPROC)Ogre3D_Proc);

   App->CL_Ogre->RenderHwnd = App->ViewGLhWnd;

   if (!App->MainHwnd)
   {
      return FALSE;
   }

   if (App->CL_Preferences->Start_FullScreen == 1)
   {
	   ShowWindow(App->MainHwnd, SW_SHOWMAXIMIZED);
   }
   else
   {
	   ShowWindow(App->MainHwnd, nCmdShow);
   }
  
   UpdateWindow(App->MainHwnd);

   return TRUE;
}

// *************************************************************************
// *				WndProc:- Terry and Hazel Flanigan 2024	  			   *
// *************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {

		// Debug -------------------------------------------------------
		if (LOWORD(wParam) == ID_DEBUG_TESTPREFERANCE)
		{
			App->CL_Preferences->Read_Preferences();
			return TRUE;
		}

		if (LOWORD(wParam) == ID_DEBUG_HIDEOGRETESTCUBE)
		{
			if (App->CL_Ogre->flag_Hide_Test_Cube == 1)
			{
				App->CL_Ogre->flag_Hide_Test_Cube = 0;
			}
			else
			{
				App->CL_Ogre->flag_Hide_Test_Cube = 1;
			}

			App->CL_Ogre->Hide_Test_Cube();

			return TRUE;
		}
		
		if (LOWORD(wParam) == ID_IMGUI_IMGUIDEMO)
		{
			if (App->CL_ImGui->Show_ImGui_Demo == 1)
			{
				App->CL_ImGui->Show_ImGui_Demo = 0;
			}
			else
			{
				App->CL_ImGui->Show_ImGui_Demo = 1;
			}

			return TRUE;
		}
		
		// Import -------------------------------------------------------
		if (LOWORD(wParam) == ID_IMPORT_WAVEFRONTOBJ)
		{
			App->CL_Assimp->SelectedPreset = 8 + 8388608 + 64 + aiProcess_PreTransformVertices;
			App->CL_Importers->Assimp_Loader("Wavefront OBJ   *.obj\0*.obj\0", "Wavefront OBJ");
			return TRUE;
		}
		
		if (LOWORD(wParam) == ID_OPTIONS_PREFERENCES)
		{
			App->CL_Preferences->Start_Preferences_Dlg();
			return TRUE;
		}
		
		if (LOWORD(wParam) == ID_VIEW_RESET)
		{
			App->CL_Camera->Reset_View();
				return TRUE;
		}

		if (LOWORD(wParam) == ID_VIEW_MAX3DVIEW)
		{
			App->CL_SplitterViews->Max_3D_win();
			return TRUE;
		}
		
		
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            
            case IDM_EXIT:

                if (App->CL_Ogre->OgreListener->StopOgre == 0)
                {
                    App->CL_Ogre->OgreListener->StopOgre = 1;
                }

                PostQuitMessage(0);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	//case WM_PAINT:
	//{
	//	//App->Flash_Window();
	//	//PAINTSTRUCT ps;
	//	//HDC hdc = BeginPaint(hWnd, &ps);
	//	//// TODO: Add any drawing code that uses hdc here...
	//	//EndPaint(hWnd, &ps);
	//}

    case WM_SIZE:
    {
		if (App->CL_SplitterViews->Max_Window == 0)
		{
			App->CL_SplitterViews->Init_Views();
		}
       

        App->CL_SplitterViews->Resize_Fldg();
        App->CL_SplitterViews->Resize_OgreWin();

        return 0;
    }

    case WM_WINDOWPOSCHANGED:
    {
       // App->Flash_Window();
        App->CL_SplitterViews->Init_Views();
        App->CL_SplitterViews->Resize_Fldg();
        App->CL_SplitterViews->Resize_OgreWin();
        return 0;
    }
        //break;
    case WM_CLOSE:
    {

        if (App->CL_Ogre->OgreListener->StopOgre == 0)
        {
            App->CL_Ogre->OgreListener->StopOgre = 1;
        }

        PostQuitMessage(0);
        break;
    }

    case WM_TIMER:
        if (wParam == 1)
        {
            if (App->OgreStarted == 0)
            {
                if (Block_Call == 0)
                {
                    Block_Call = 1;
                    StartOgre();
                }
            }

            /*if (App->RenderBackGround == 1 && App->OgreStarted == 1)
            {
                Ogre::Root::getSingletonPtr()->renderOneFrame();
            }*/

            break;
        }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// *************************************************************************
// *			Ogre3D_Proc:- Terry and Hazel Flanigan 2024				   *
// *************************************************************************
LRESULT CALLBACK Ogre3D_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_INITDIALOG:
	{
		//App->ViewPLeaseWait = CreateDialog(App->hInst, (LPCTSTR)IDD_PLEASEWAIT, App->Fdlg, (DLGPROC)PleaseWait_Proc);
		return TRUE;
	}

	case WM_CTLCOLORDLG:
	{
		if (App->OgreStarted == 0)
		{
			return (LONG)App->BlackBrush;
		}

		return false;
	}
	case WM_MOUSEWHEEL:
	{
		/*ImGuiIO& io = ImGui::GetIO();
		io.*/
		//if (App->FullScreen == 1)
		if (App->CL_Ogre->OgreListener->Pl_LeftMouseDown == 0)
		{
			int zDelta = (short)HIWORD(wParam);    // wheel rotation

			if (zDelta > 0)
			{
				App->CL_Ogre->OgreListener->Wheel = -1;
			}
			else if (zDelta < 0)
			{
				App->CL_Ogre->OgreListener->Wheel = 1;
			}
			return 1;

		}
	}

	case WM_MOUSEMOVE: // ok up and running and we have a loop for mouse
	{
		POINT p;

		ImGuiIO& io = ImGui::GetIO();

		POINT pos;

		GetCursorPos(&pos);
		ScreenToClient(App->ViewGLhWnd, &pos);
		io.MousePos.x = static_cast<float>(pos.x);
		io.MousePos.y = static_cast<float>(pos.y);
		//App->CL_Ogre->m_imgui.mouseMoved();


		if (GetCursorPos(&p) && App->OgreStarted == 1)// && App->CL10_Dimensions->Mouse_Move_Mode == Enums::Edit_Mouse_None)
		{
			if (ScreenToClient(App->ViewGLhWnd, &p))
			{
				RECT rc;
				GetClientRect(App->ViewGLhWnd, &rc);
				int width = rc.right - rc.left;
				int height = rc.bottom - rc.top;

				float tx = ((float)width / 2) - (float)p.x;
			}
		}

		SetFocus(App->ViewGLhWnd);

		break;
	}

	// Left Mouse Button
	case WM_LBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = true;

		if (!ImGui::GetIO().WantCaptureMouse)
		{

			{
				if (App->OgreStarted == 1)
				{
					if (App->Block_Mouse_Buttons == 0)
					{
						//if (!ImGui::GetIO().WantCaptureMouse)
						{
							POINT p;
							GetCursorPos(&p);
							App->CursorPosX = p.x;
							App->CursorPosY = p.y;
							App->CL_Ogre->OgreListener->Pl_Cent500X = p.x;
							App->CL_Ogre->OgreListener->Pl_Cent500Y = p.y;

							SetCapture(App->ViewGLhWnd);// Bernie
							SetCursorPos(App->CursorPosX, App->CursorPosY);
							App->CL_Ogre->OgreListener->Pl_LeftMouseDown = 1;
							App->CUR = SetCursor(NULL);
						}
						//else
						//{
							//App->CL_Ogre->OgreListener->Pl_LeftMouseDown = 1;
						//}
					}

					return 1;
				}
			}
		}

		return 1;
	}

	case WM_LBUTTONUP:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = false;

		if (App->OgreStarted == 1)
		{
			ReleaseCapture();
			App->CL_Ogre->OgreListener->Pl_LeftMouseDown = 0;
			SetCursor(App->CUR);
			return 1;
		}

		return 1;
	}

	// Right Mouse Button
	case WM_RBUTTONDOWN: // BERNIE_HEAR_FIRE 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = true;
		
		//if (ImGui::GetIO().WantCaptureMouse)
		//{
			//App->Cl_FileView_V2->RightMouseDown = 1;
		//}

		if (!ImGui::GetIO().WantCaptureMouse)
		{

			//if (App->SBC_Scene->GameMode_Running_Flag == 0)
			{
				if (App->OgreStarted == 1)
				{
					POINT p;
					GetCursorPos(&p);
					App->CursorPosX = p.x;
					App->CursorPosY = p.y;
					App->CL_Ogre->OgreListener->Pl_Cent500X = p.x;
					App->CL_Ogre->OgreListener->Pl_Cent500Y = p.y;
					SetCapture(App->ViewGLhWnd);// Bernie
					SetCursorPos(App->CursorPosX, App->CursorPosY);
					App->CL_Ogre->OgreListener->Pl_RightMouseDown = 1;
					App->CUR = SetCursor(NULL);
					return 1;
				}
			}
		}
		return 1;
	}

	case WM_RBUTTONUP:
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = false;
		//App->CL_Ogre->m_imgui.mouseReleased();

		if (App->OgreStarted == 1)
		{
			ReleaseCapture();
			App->CL_Ogre->OgreListener->Pl_RightMouseDown = 0;
			SetCursor(App->CUR);
			return 1;
		}

		return 1;
	}

	case WM_KEYUP:
	{
		
		ImGuiIO& io = ImGui::GetIO();
		//io.wWantCaptureKeyboard();

		//io.AddKeyEvent(ImGuiKey(ImGuiKey_None), false);
		/*io.KeysDown[VK_BACK] = false;
		io.KeysDown[VK_LEFT] = false;
		io.KeysDown[VK_RIGHT] = false;*/
		return 1;
	}

	case WM_CHAR:
	{
		if (ImGui::GetIO().WantCaptureKeyboard)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.AddInputCharacter((unsigned short)wParam);
		}
		return 1;
	}

	case WM_KEYDOWN:
	{
		ImGuiIO& io = ImGui::GetIO();
		
		if (wParam == VK_BACK)
		{
			//io.AddKeyEvent(ImGuiKey(ImGuiKey_LeftArrow),true);
			//io.AddKeyEvent(ImGuiKey(ImGuiKey_None), false);
			//io.AddInputCharacter((unsigned short)ImGuiKey_Delete);
			//io.KeysDown[VK_BACK] = true;
		}

		if (wParam == VK_RIGHT)
		{
			//io.AddKeyEvent(ImGuiKey(ImGuiKey_RightArrow), true);
			//io.KeysDown[VK_RIGHT] = true;
		}

		if (wParam == VK_LEFT)
		{
			//io.AddKeyEvent(ImGuiKey(ImGuiKey_LeftArrow), true);
			//io.KeysDown[VK_LEFT] = true;
		}

		return 1;
	}

	}

	return FALSE;
}

// *************************************************************************
// *						StartOgre Terry Bernie			  	 		   *
// *************************************************************************
void StartOgre()
{
    App->CL_Ogre->InitOgre();

    Ogre::Root::getSingletonPtr()->renderOneFrame();
    //EndDialog(App->ViewPLeaseWait, LOWORD(0));

    App->OgreStarted = 1;

    KillTimer(App->MainHwnd, 1);

	if (App->CL_Preferences->Start_Full_3DWin == 1)
	{
		App->CL_SplitterViews->Max_3D_win();
	}

    App->CL_Ogre->Ogre_Render_Loop();

    Close_App();
    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, TRUE);
    PostQuitMessage(0);
}

// *************************************************************************
// *						Close_App Terry Bernie			  	 		   *
// *************************************************************************
void Close_App()
{
	if (App->CL_Ini_File)
	{
		App->CL_Ini_File->Clean_Up();

		delete App->CL_Ini_File;
		App->CL_Ini_File = nullptr;
	}

	if (App->CL_Preferences)
	{
		App->CL_Preferences->Clean_Up();

		delete App->CL_Preferences;
		App->CL_Preferences = nullptr;
	}

	// And Finaly
    if (App->CL_Ogre->mRoot)
    {
        delete App->CL_Ogre->mRoot;
        App->CL_Ogre->mRoot = NULL;
    }

   // ImGui::DestroyContext();
}

// *************************************************************************
// *			About:- Terry and Hazel Flanigan 2024			  	 	   *
// *************************************************************************
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
