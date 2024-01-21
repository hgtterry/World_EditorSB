#pragma once
class SB_Equity
{
public:
	SB_Equity(void);
	~SB_Equity(void);

	void Start_Equity_Dialog_New();

	bool Auto_Load_File();
	void Hide_Equity_Dialog();

	void Set_Mode_Equity();
	void Go_Equity();
	void Set_Camera();

	void Do_Preview_Selected();

	void Resize_3DView();

	bool mAutoLoad;
	bool First_Run;
	bool EquitySB_Dialog_Created;
	bool EquitySB_Dialog_Visible;
	int Equity_Render_Mode;

	bool Saved_Camera_Mode;

	HWND OgreView_3D_hWnd;
	
	bool Close_Equity_Flag;

	Ogre::Vector3 Position_Offsets;

	static LRESULT CALLBACK Ogre3D_New_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:

	static LRESULT CALLBACK Equity_Dialog_New_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	void Get_CurrentDocument();
	void Set_Title_Bar(char* Title);
	void Clear_Model();

	CFusionDoc* m_pDoc;
};

