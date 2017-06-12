// StackPanelCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/StackPanelCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStackPanelCtrl

IMPLEMENT_DYNAMIC(CStackPanelCtrl, CWnd)

CStackPanelCtrl::CStackPanelCtrl()
	: m_bIsCreated(FALSE)
	, m_szCtrlSize(0, 0)
	, m_szCurrentSize(0, 0)
	, m_pnScrollPos(0, 0)
{

}

CStackPanelCtrl::~CStackPanelCtrl()
{
	
	while (m_listPanles.GetCount()) {
		auto info = m_listPanles.RemoveTail();
		delete info;
	}
}

void CStackPanelCtrl::SetCtrlWidth(int newSize)
{
	m_szCtrlSize.cx = newSize;
	Invalidate();
}

int CStackPanelCtrl::GetCtrlHeight()const
{
	return m_szCtrlSize.cy;
}

int CStackPanelCtrl::GetCtrlWidth()const
{
	return m_szCtrlSize.cx;
}

BOOL CStackPanelCtrl::Create(CWnd * pParentWnd, RECT const& rect, UINT nStyle, UINT nID)
{
	return CreateEx(pParentWnd, rect, 0, nStyle, nID);
}

BOOL CStackPanelCtrl::CreateEx(CWnd * pParentWnd, RECT const& rect, UINT nExStyle, UINT nStyle, UINT nID)
{
	if (!CWnd::CreateEx(nExStyle, NULL, NULL, nStyle, rect, pParentWnd, nID))
		return FALSE;

	m_szCurrentSize.cx = rect.right - rect.left;
	m_szCurrentSize.cy = rect.bottom - rect.top;
	m_szCtrlSize.cx = m_szCurrentSize.cx;
	m_szCtrlSize.cy = 18;
	return TRUE;
}

void CStackPanelCtrl::AdjustLayout()
{
	if (!m_bIsCreated)
		return;

	SCROLLINFO si;
	LONG cx = m_szCurrentSize.cx;
	LONG cy = m_szCurrentSize.cy;

	GetScrollInfo(SB_VERT, &si);


	int nVScrollMax, nHScrollMax;
	if (cy < m_szCtrlSize.cy)
	{
		nVScrollMax = m_szCtrlSize.cy - cy;
	}
	else
		nVScrollMax = 0;

	if (cx < m_szCtrlSize.cx)
	{
		nHScrollMax = m_szCtrlSize.cx - cx;
	}
	else
		nHScrollMax = 0;

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; // SIF_ALL = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = nVScrollMax;
	si.nPage = si.nMax / 10;
	si.nPos = 0;

	SetScrollInfo(SB_VERT, &si, TRUE);

	si.nMax = nHScrollMax;
	si.nPage = si.nMax / 10;

	SetScrollInfo(SB_HORZ, &si, TRUE);

	ScrollWindow(m_pnScrollPos.x, m_pnScrollPos.y);
	m_pnScrollPos.x = 0;
	m_pnScrollPos.y = 0;

	for (auto pos = m_listPanles.GetHeadPosition(); pos != NULL; )
	{
		auto info = m_listPanles.GetNext(pos);
		CRect rc;
		info->wndPanel->GetClientRect(rc);
		info->wndPanel->SetWindowPos(NULL, 0, 0, max(m_szCtrlSize.cx, cx) - 30, rc.Height(), SWP_NOMOVE | SWP_NOACTIVATE);
	}
}

void CStackPanelCtrl::AddPanel(LPCTSTR displayText, int minWidth, BOOL widthAutoAdjust, CWnd *wndPanel)
{
	if (wndPanel == NULL || wndPanel->GetSafeHwnd() == NULL)
		return;

	CRect rc;
	wndPanel->GetClientRect(rc);

	wndPanel->SetParent(this);

	if (minWidth <= 0)
		minWidth = rc.Width();

	if (minWidth > (m_szCtrlSize.cx - 30))
		m_szCtrlSize.cx = minWidth + 30;

	if (widthAutoAdjust)
	{
		wndPanel->SetWindowPos(NULL, 15, m_szCtrlSize.cy, m_szCtrlSize.cx - 30, rc.Height(), SWP_NOACTIVATE);
	}
	else
	{
		wndPanel->SetWindowPos(NULL, 15, m_szCtrlSize.cy, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	}
	m_szCtrlSize.cy += 15 + rc.Height();
	m_listPanles.AddTail(new STACKPANELINFO{ displayText , TRUE, minWidth ,widthAutoAdjust, wndPanel });
}

void CStackPanelCtrl::ShowPanel(int idx, BOOL show)
{
	CRect rc;
	int total = 15;
	auto pos = m_listPanles.FindIndex(idx);
	if (pos == NULL)
		return;
	auto info = m_listPanles.GetAt(pos);
	if (info->bIsVisible != show)
	{
		info->bIsVisible = show ? TRUE : FALSE;
		info->wndPanel->GetWindowRect(rc);
		ScreenToClient(rc);
		total += rc.top + (show ? rc.Height() : 15);
		info->wndPanel->GetClientRect(rc);
		info->wndPanel->ShowWindow(show ? SW_SHOW : SW_HIDE);

		m_listPanles.GetNext(pos);
		while (pos != NULL)
		{
			info = m_listPanles.GetNext(pos);
			info->wndPanel->SetWindowPos(NULL, 15, total, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);

			if (info->bIsVisible)
			{
				info->wndPanel->GetClientRect(rc);
				total += rc.Height();
			}
			else
			{
				total += 15;
			}

			total += 15;
		}

		if (m_szCtrlSize.cy != total)
		{
			m_szCtrlSize.cy = total;
			OnSize(0, m_szCurrentSize.cx, m_szCurrentSize.cy);
		}


		Invalidate();
	}
}

BEGIN_MESSAGE_MAP(CStackPanelCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CStackPanelCtrl message handlers

int CStackPanelCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bIsCreated = TRUE;
	AdjustLayout();
	return 0;
}

BOOL CStackPanelCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode >= 0 && nCode <= 10)
	{
		GetParent()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	else switch (nCode)
	{
	case -1:
	case 0X100:
	case 0X200:
	case 0X300:
	case 0X400:
	case 0X500:
	case 0X501:
	case 0X601:
	case 0X602:
		GetParent()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		break;
	}
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

HBRUSH CStackPanelCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	struct AFX_CTLCOLOR
	{
		HWND hWnd;
		HDC hDC;
		UINT nCtlType;
	}ctrlColor;

	ctrlColor.hDC = pDC->GetSafeHdc();
	ctrlColor.hWnd = pWnd->GetSafeHwnd();
	ctrlColor.nCtlType = nCtlColor;
	return (HBRUSH)GetParent()->SendMessage(WM_CTLCOLOR,(WPARAM)pDC->GetSafeHdc(), (LPARAM)&ctrlColor);
}

BOOL CStackPanelCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, VisualStyleHelper::s_clrBackground);
	return TRUE;
}

void CStackPanelCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	m_szCurrentSize.cx = cx;
	m_szCurrentSize.cy = cy;
	AdjustLayout();
}

void CStackPanelCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_szCtrlSize.cy - m_szCurrentSize.cy;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_pnScrollPos.y >= nMaxPos)
			return;
		nDelta = min(nMaxPos / 100, nMaxPos - m_pnScrollPos.y);
		break;

	case SB_LINEUP:
		if (m_pnScrollPos.y <= 0)
			return;
		nDelta = -min(nMaxPos / 100, m_pnScrollPos.y);
		break;

	case SB_PAGEDOWN:
		if (m_pnScrollPos.y >= nMaxPos)
			return;
		nDelta = min(nMaxPos / 10, nMaxPos - m_pnScrollPos.y);
		break;

	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_pnScrollPos.y;
		break;

	case SB_PAGEUP:
		if (m_pnScrollPos.y <= 0)
			return;
		nDelta = -min(nMaxPos / 10, m_pnScrollPos.y);
		break;

	default:
		return;
	}
	m_pnScrollPos.y += nDelta;
	SetScrollPos(SB_VERT, m_pnScrollPos.y, TRUE);
	ScrollWindow(0, -nDelta);
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CStackPanelCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_szCtrlSize.cx - m_szCurrentSize.cx;

	switch (nSBCode)
	{
	case SB_LINERIGHT:
		if (m_pnScrollPos.x >= nMaxPos)
			return;
		nDelta = min(nMaxPos / 100, nMaxPos - m_pnScrollPos.x);
		break;

	case SB_LINELEFT:
		if (m_pnScrollPos.x <= 0)
			return;
		nDelta = -min(nMaxPos / 100, m_pnScrollPos.x);
		break;

	case SB_PAGERIGHT:
		if (m_pnScrollPos.x >= nMaxPos)
			return;
		nDelta = min(nMaxPos / 10, nMaxPos - m_pnScrollPos.x);
		break;

	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_pnScrollPos.x;
		break;

	case SB_PAGELEFT:
		if (m_pnScrollPos.x <= 0)
			return;
		nDelta = -min(nMaxPos / 10, m_pnScrollPos.x);
		break;

	default:
		return;
	}
	m_pnScrollPos.x += nDelta;
	SetScrollPos(SB_HORZ, m_pnScrollPos.x, TRUE);
	ScrollWindow(-nDelta, 0);
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CStackPanelCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	CSize sz(max(m_szCtrlSize.cx, m_szCurrentSize.cx), max(m_szCtrlSize.cy, m_szCurrentSize.cy));
	CMFCVisualManager* vm = CMFCVisualManager::GetInstance();

	CMFCToolBarEditBoxButton dummy0;

	GetClientRect(rc);
	//dc.FillSolidRect(rc, GetSysColor(COLOR_3DFACE));

	rc = CRect(-m_pnScrollPos, sz);

	int h = 0, total = 15;

	for (auto pos = m_listPanles.GetHeadPosition(); pos != NULL; )
	{
		auto info = m_listPanles.GetNext(pos);
		if (info->bIsVisible)
		{
			info->wndPanel->GetClientRect(rc);
			h = rc.Height();
		}
		else
		{
			h = 15;
		}

	

		rc = CRect(-m_pnScrollPos.x + 5, total - m_pnScrollPos.y - 5, sz.cx - m_pnScrollPos.x - 5, total - m_pnScrollPos.y + h + 11);

		//vm->OnDrawEditBorder(&dc, rc, FALSE, TRUE, &dummy0);
		dc.Draw3dRect(rc, VisualStyleHelper::s_clrBorderLight, VisualStyleHelper::s_clrBorderLight);
		dc.Draw3dRect(-m_pnScrollPos.x + 1, total - m_pnScrollPos.y - 9, 9, 9, VisualStyleHelper::s_clrBorderDark, VisualStyleHelper::s_clrBorderDark);


		dc.Draw3dRect(-m_pnScrollPos.x + 3, total - m_pnScrollPos.y - 5, 5, 1, VisualStyleHelper::s_clrBorderDark, VisualStyleHelper::s_clrBorderDark);
		if (!info->bIsVisible)
			dc.Draw3dRect(-m_pnScrollPos.x + 5, total - m_pnScrollPos.y - 7, 1, 5, VisualStyleHelper::s_clrBorderDark, VisualStyleHelper::s_clrBorderDark);

		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(ncm);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
		LOGFONT lfDlgFont = ncm.lfMessageFont;
		CFont font;
		font.CreateFontIndirect(&lfDlgFont);
		auto oldFont = dc.SelectObject(&font);
		dc.SetBkColor(VisualStyleHelper::s_clrBackground);
		rc = CRect(CPoint(-m_pnScrollPos.x + 11, total - m_pnScrollPos.y - 13), CSize(200, 30));
		dc.DrawTextW(info->strDisplayText, rc, DT_LEFT | DT_BOTTOM);
		dc.SelectObject(oldFont);
		total += h + 15;
	}
}

void CStackPanelCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rc;
	CPoint p(point.x + m_pnScrollPos.x, point.y + m_pnScrollPos.y);
	//CPoint p(point);
	if (p.x < 1 || p.x > 9)
		return;

	int h = 0, total = 6;
	auto pos = m_listPanles.GetHeadPosition();
	if (pos == nullptr)
		return;
	int i = 0;
	do
	{
		auto info = m_listPanles.GetNext(pos);

		if ((p.y - total) > 0 && (p.y - total) < 8)
		{
			ShowPanel(i, !info->bIsVisible);
			return;
		}
		else if ((p.y - total) < 0)
			return;

		if (info->bIsVisible)
		{
			info->wndPanel->GetClientRect(rc);
			h = rc.Height();
		}
		else
		{
			h = 15;
		}
		total += h + 15;
		i++;
	} while (pos);

	CWnd::OnLButtonDown(nFlags, point);
}

void CStackPanelCtrl::OnClose()
{
	while (m_listPanles.GetCount() > 0)
		delete m_listPanles.RemoveTail();
	CWnd::OnClose();
}
