/****************************************************************************************/
/*  ChildFrm.cpp                                                                        */
/*                                                                                      */
/*  Author:       Jim Mischel, Ken Baird                                                */
/*  Description:  Frame and splitter window code                                        */
/*                                                                                      */
/*  The contents of this file are subject to the Genesis3D Public License               */
/*  Version 1.01 (the "License"); you may not use this file except in                   */
/*  compliance with the License. You may obtain a copy of the License at                */
/*  http://www.genesis3d.com                                                            */
/*                                                                                      */
/*  Software distributed under the License is distributed on an "AS IS"                 */
/*  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See                */
/*  the License for the specific language governing rights and limitations              */
/*  under the License.                                                                  */
/*                                                                                      */
/*  The Original Code is Genesis3D, released March 25, 1999.                            */
/*  Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved           */
/*                                                                                      */
/****************************************************************************************/
#include "stdafx.h"
#include "ChildFrm.h"

#include "FUSIONView.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFixedSplitterWnd::CFixedSplitterWnd()
{
	m_hwndAct = NULL;
}

CFixedSplitterWnd::~CFixedSplitterWnd()
{
}

// WM_MOUSEWHEEL isn't defined in earlier versions of MFC, which
// causes us a problem when compiling with VC 4.  This should take
// care of the problem...
	BEGIN_MESSAGE_MAP(CFixedSplitterWnd, CSplitterWnd)
#if (_MFC_VER == 0x0421)
		//{{AFX_MSG_MAP(CFixedSplitterWnd)
		ON_WM_MOUSEWHEEL()
		//}}AFX_MSG_MAP
#endif
 	END_MESSAGE_MAP()

#pragma warning (disable: 4100)
BOOL CFixedSplitterWnd::OnMouseWheel (UINT nFlags, short zDelta, CPoint pt)
{
	CMDIChildWnd * pParent = (CMDIChildWnd*)GetParent() ;
	CFusionView * cv = (CFusionView *)pParent->GetActiveView() ;
	if( cv != NULL )
	{
		if( zDelta > 0 )
		{
			cv->DoZoom (0.1f);
		}
		else
		{
			cv->DoZoom (-0.1f);
		}
	}
	return FALSE;
}
#pragma warning (default: 4100)

void CFixedSplitterWnd::RefreshSplitBars() // hgtterrry RefreshSplitBars
{
	if (App->BR_True3D_Mode_Active == 1)
	{
		App->CLSB_BR_Render->BR_3D_Resize();
	}
	
	CRect rectInside;
	GetInsideRect(rectInside);
	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);

}

void CFixedSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	
	CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
	CWnd *pAct = GetActivePane();
	if(pDC && pAct && !pAct->IsKindOf( RUNTIME_CLASS( CSplitterWnd)) && nType == splitBorder)
	{	
		CRect rect;
		pAct->GetWindowRect(&rect);
		pDC->GetWindow()->ScreenToClient(&rect);
		if(rectArg.PtInRect(rect.CenterPoint()))
		{
			ASSERT_VALID(pDC);
			rect = rectArg;
			pDC->Draw3dRect(rect, GetSysColor( COLOR_ACTIVECAPTION),GetSysColor(COLOR_ACTIVECAPTION));
			rect.InflateRect(-GetSystemMetrics(SM_CXBORDER), -GetSystemMetrics(SM_CYBORDER));
			pDC->Draw3dRect(rect, GetSysColor( COLOR_ACTIVECAPTION),GetSysColor(COLOR_ACTIVECAPTION));
		}
		
	}
	
}

BOOL CFixedSplitterWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	SetHighlight();
	return CSplitterWnd::OnNotify(wParam, lParam, pResult);
}

void CFixedSplitterWnd::SetHighlight()
{
	CWnd *pAct = GetActivePane();
	if(pAct && m_hwndAct != pAct->m_hWnd )
	{
		m_hwndAct = pAct->m_hWnd;
		RedrawWindow();
	}
}

BOOL CFixedSplitterWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	SetHighlight();
	return CSplitterWnd::OnCommand(wParam, lParam);
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	App->Debug_Message("CChildFrame",1);
	InitDone=FALSE;
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//Create maximized
	cs.style |= WS_MAXIMIZE|WS_VISIBLE;
	return CMDIChildWnd::PreCreateWindow(cs);
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT, CCreateContext* pContext)
{

	int x, y;
	int x2, y2;
	RECT r;

	if (!m_wndSplitter.CreateStatic(this, 2, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	GetClientRect(&r);
	x=(r.right>>2);
	y=(r.bottom>>2);
	x2=x*2;
	y2=y*2;

	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CFusionView), CSize(x2, y2), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CFusionView), CSize(x, y2), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(CFusionView), CSize(x, y), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	if (!m_wndSplitter.CreateView(1, 1,
		RUNTIME_CLASS(CFusionView), CSize(x2, y), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	//set the view types
	((CFusionView *)m_wndSplitter.GetPane(0,0))->OnViewType(ID_VIEW_TOPVIEW);
	((CFusionView *)m_wndSplitter.GetPane(0,1))->OnViewType(ID_VIEW_SIDEVIEW);
	((CFusionView *)m_wndSplitter.GetPane(1,0))->OnViewType(ID_VIEW_FRONTVIEW);
	((CFusionView *)m_wndSplitter.GetPane(1,1))->OnViewType(ID_VIEW_3DWIREFRAME); //ID_VIEW_TEXTUREVIEW);

	App->WE_3DView_Hwnd = m_wndSplitter.GetPane(1, 1)->GetSafeHwnd();

	// activate the top left view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));

	InitDone=TRUE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
						
#pragma warning (disable: 4100)
LRESULT CChildFrame::OnSetText(WPARAM wParam, LPARAM lParam)
{
//	const char *Text = (const char *)lParam;
//	if( Text[0] == '-' ) {
		return Default();
//	}
//	return 1;
}
#pragma warning (default: 4100)


void CChildFrame::OnSize(UINT nType, int cx, int cy) // hgtterry
{
	//App->Debug_Message("CChildFrame::OnSize", 1);
	//center the panes on a size
	if(InitDone &&(m_wndSplitter.GetRowCount()==2 && m_wndSplitter.GetColumnCount()==2))
	{
		m_wndSplitter.SetColumnInfo(0, (cx>>2)*2, 0);
		m_wndSplitter.SetColumnInfo(1, cx>>3, 0);
		m_wndSplitter.SetRowInfo(0, (cy>>2)*2, 0);
		m_wndSplitter.SetRowInfo(1, cy>>2, 0);
	}

	CMDIChildWnd::OnSize(nType, cx, cy);
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	NMHDR hdr;
	CWnd *pParent;

	CMDIChildWnd::OnSetFocus(pOldWnd);

	// notify main frame window that we've been given the focus
	hdr.hwndFrom = this->m_hWnd;
	hdr.idFrom = 0;
	hdr.code = WM_SETFOCUS;
	pParent = this->GetMDIFrame ();
	if (pParent != NULL)
	{
		pParent->SendMessage (WM_NOTIFY, 0, (LPARAM)&hdr);
	}

	
}

// *************************************************************************
// *	  						MaximizeUpperLeftPane					   *
// *************************************************************************
bool CChildFrame::MaximizeUpperLeftPane()
{
	HWND hWnd = NULL;
	hWnd = m_wndSplitter.GetSafeHwnd();
	if (hWnd == NULL || !::IsWindow(hWnd))
		return false;

	CRect	rect;
	GetClientRect(&rect);
	CSize size = rect.Size();

	m_wndSplitter.SetColumnInfo(0, size.cx, 0);
	m_wndSplitter.SetColumnInfo(1, 0, 0);
	m_wndSplitter.SetRowInfo(0, size.cy, 0);
	m_wndSplitter.SetRowInfo(1, 0, 0);

	m_wndSplitter.RecalcLayout();

	return true;
}

// *************************************************************************
// *	  						MaximizeUpperRightPane					   *
// *************************************************************************
bool CChildFrame::MaximizeUpperRightPane()
{
	HWND hWnd = NULL;
	hWnd = m_wndSplitter.GetSafeHwnd();
	if (hWnd == NULL || !::IsWindow(hWnd))
		return false;

	CRect	rect;
	GetClientRect(&rect);
	CSize size = rect.Size();

	m_wndSplitter.SetColumnInfo(0, 0, 0);
	m_wndSplitter.SetColumnInfo(1, size.cx, 0);
	m_wndSplitter.SetRowInfo(0, size.cy, 0);
	m_wndSplitter.SetRowInfo(1, 0, 0);

	m_wndSplitter.RecalcLayout();

	return true;
}

// *************************************************************************
// *	  						MaximizeLowerLeftPane					   *
// *************************************************************************
bool CChildFrame::MaximizeLowerLeftPane()
{
	HWND hWnd = NULL;
	hWnd = m_wndSplitter.GetSafeHwnd();
	if (hWnd == NULL || !::IsWindow(hWnd))
		return false;

	CRect	rect;
	GetClientRect(&rect);
	CSize size = rect.Size();

	m_wndSplitter.SetColumnInfo(0, size.cx, 0);
	m_wndSplitter.SetColumnInfo(1, 0, 0);
	m_wndSplitter.SetRowInfo(0, 0, 0);
	m_wndSplitter.SetRowInfo(1, size.cy, 0);

	m_wndSplitter.RecalcLayout();

	return true;
}

// *************************************************************************
// *	  						MaximizeLowerRightPane					   *
// *************************************************************************
bool CChildFrame::MaximizeLowerRightPane()
{
	HWND hWnd = NULL;
	hWnd = m_wndSplitter.GetSafeHwnd();
	if (hWnd == NULL || !::IsWindow(hWnd))
		return false;

	CRect	rect;
	GetClientRect(&rect);
	CSize size = rect.Size();

	m_wndSplitter.SetColumnInfo(0, 0, 0);
	m_wndSplitter.SetColumnInfo(1, size.cx, 0);
	m_wndSplitter.SetRowInfo(0, 0, 0);
	m_wndSplitter.SetRowInfo(1, size.cy, 0);

	m_wndSplitter.RecalcLayout();

	return true;
}

// *************************************************************************
// *	  						RestoreAllPanes							   *
// *************************************************************************
bool CChildFrame::RestoreAllPanes()
{
	HWND hWnd = NULL;
	hWnd = m_wndSplitter.GetSafeHwnd();
	if (hWnd == NULL || !::IsWindow(hWnd))
		return false;

	int correctedHeight;
	CRect	propPanelRect, childFrameRect;

	GetClientRect(&childFrameRect);

	correctedHeight = childFrameRect.Height();

	m_wndSplitter.SetColumnInfo(0, childFrameRect.Width() / 2, 0);
	m_wndSplitter.SetColumnInfo(1, childFrameRect.Width() / 2, 0);
	m_wndSplitter.SetRowInfo(0, correctedHeight / 2, 0);
	m_wndSplitter.SetRowInfo(1, correctedHeight / 2, 0);

	m_wndSplitter.RecalcLayout();

	return true;
}
