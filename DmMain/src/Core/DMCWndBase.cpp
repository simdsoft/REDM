#include "DmMainAfx.h"
#include "DMCWndBase.h"

namespace DM
{
	DMCWndBase::DMCWndBase(HWND hWnd /*= NULL*/) throw() 
		:m_hWnd(hWnd)
	{
	}

	DMCWndBase::~DMCWndBase()
	{	
	}

	DMCWndBase& DMCWndBase::operator=(HWND hWnd) throw()
	{
		m_hWnd = hWnd;
		return *this;
	}

	void DMCWndBase::Attach(HWND hWndNew) throw()
	{
		DMASSERT(m_hWnd == NULL);
		DMASSERT((hWndNew == NULL) || ::IsWindow(hWndNew));
		m_hWnd = hWndNew;
	}

	HWND DMCWndBase::Detach() throw()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

	BOOL DMCWndBase::DestroyWindow() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::DestroyWindow(m_hWnd);// 此处不能把m_hWnd赋为0，因为消息循环还在跑
		//if (!::DestroyWindow(m_hWnd))
		//	return FALSE;

		//m_hWnd = NULL;
		//return TRUE;
	}

	DMCWndBase::operator HWND() const throw()
	{ 
		return m_hWnd; 
	}

	DWORD DMCWndBase::GetStyle() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (DWORD)::GetWindowLong(m_hWnd, GWL_STYLE);
	}

	DWORD DMCWndBase::GetExStyle() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (DWORD)::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	}

	LONG DMCWndBase::GetWindowLong(int nIndex) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowLong(m_hWnd, nIndex);
	}

	LONG DMCWndBase::SetWindowLong(int nIndex, LONG dwNewLong) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetWindowLong(m_hWnd, nIndex, dwNewLong);
	}

#ifdef _WIN64
	LONG_PTR DMCWndBase::SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetWindowLongPtr(m_hWnd, nIndex, dwNewLong);
	}

	LONG_PTR DMCWndBase::GetWindowLongPtr(int nIndex) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowLongPtr(m_hWnd, nIndex);
	}
#endif//_WIN64
	WORD DMCWndBase::GetWindowWord(int nIndex) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowWord(m_hWnd, nIndex);
	}

	WORD DMCWndBase::SetWindowWord(int nIndex, WORD wNewWord) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetWindowWord(m_hWnd, nIndex, wNewWord);
	}

	// Message Functions
	LRESULT DMCWndBase::SendMessage(UINT message, WPARAM wParam/* = 0*/, LPARAM lParam /*= 0*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd,message,wParam,lParam);
	}

	BOOL DMCWndBase::PostMessage(UINT message, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::PostMessage(m_hWnd,message,wParam,lParam);
	}

	BOOL DMCWndBase::SendNotifyMessage(UINT message, WPARAM wParam /*= 0*/, LPARAM lParam/* = 0*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SendNotifyMessage(m_hWnd, message, wParam, lParam);
	}

	// support for C style macros
	LRESULT DMCWndBase::SendMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) throw()
	{
		DMASSERT(::IsWindow(hWnd));
		return ::SendMessage(hWnd, message, wParam, lParam);
	}

	// Window Text Functions
	BOOL DMCWndBase::SetWindowText(LPCTSTR lpszString) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetWindowText(m_hWnd, lpszString);
	}

	int DMCWndBase::GetWindowText(_Out_z_cap_post_count_(nMaxCount, return + 1) LPTSTR lpszStringBuf, _In_ int nMaxCount) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowText(m_hWnd, lpszStringBuf, nMaxCount);
	}

	int DMCWndBase::GetWindowText(CStringT& strText) const
	{
		int nLength;
		LPTSTR pszText;

		nLength = GetWindowTextLength();
		pszText = strText.GetBuffer(nLength+1);
		nLength = GetWindowText(pszText, nLength+1);
		strText.ReleaseBuffer(nLength);

		return nLength;
	}

	int DMCWndBase::GetWindowTextLength() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowTextLength(m_hWnd);
	}

	// Font Functions
	void DMCWndBase::SetFont(HFONT hFont, BOOL bRedraw/* = TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(bRedraw, 0));
	}

	HFONT DMCWndBase::GetFont() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0);
	}


	// Menu Functions (non-child windows only)
	HMENU DMCWndBase::GetMenu() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (HMENU)::GetMenu(m_hWnd);
	}

	BOOL DMCWndBase::SetMenu(HMENU hMenu) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetMenu(m_hWnd, hMenu);
	}

	BOOL DMCWndBase::DrawMenuBar() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::DrawMenuBar(m_hWnd);
	}

	HMENU DMCWndBase::GetSystemMenu(BOOL bRevert) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (HMENU)::GetSystemMenu(m_hWnd, bRevert);
	}

	BOOL DMCWndBase::HiliteMenuItem(HMENU hMenu, UINT uItemHilite, UINT uHilite) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::HiliteMenuItem(m_hWnd, hMenu, uItemHilite, uHilite);
	}

	// Window Size and Position Functions

	BOOL DMCWndBase::IsIconic() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::IsIconic(m_hWnd);
	}

	BOOL DMCWndBase::IsZoomed() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::IsZoomed(m_hWnd);
	}

	BOOL DMCWndBase::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint/* = TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint);
	}

	BOOL DMCWndBase::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
	}

	BOOL DMCWndBase::SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, nFlags);
	}

	BOOL DMCWndBase::SetWindowPos(HWND hWndInsertAfter, LPCRECT lpRect, UINT nFlags) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, nFlags);
	}

	UINT DMCWndBase::ArrangeIconicWindows() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ArrangeIconicWindows(m_hWnd);
	}

	BOOL DMCWndBase::BringWindowToTop() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::BringWindowToTop(m_hWnd);
	}

	BOOL DMCWndBase::GetWindowRect(LPRECT lpRect) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowRect(m_hWnd, lpRect);
	}

	BOOL DMCWndBase::GetClientRect(LPRECT lpRect) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetClientRect(m_hWnd, lpRect);
	}

	BOOL DMCWndBase::GetWindowPlacement(WINDOWPLACEMENT FAR* lpwndpl) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowPlacement(m_hWnd, lpwndpl);
	}

	BOOL DMCWndBase::SetWindowPlacement(const WINDOWPLACEMENT FAR* lpwndpl) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetWindowPlacement(m_hWnd, lpwndpl);
	}

	// Coordinate Mapping Functions
	BOOL DMCWndBase::ClientToScreen(LPPOINT lpPoint) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ClientToScreen(m_hWnd, lpPoint);
	}

	BOOL DMCWndBase::ClientToScreen(LPRECT lpRect) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		if(!::ClientToScreen(m_hWnd, (LPPOINT)lpRect))
			return FALSE;
		return ::ClientToScreen(m_hWnd, ((LPPOINT)lpRect)+1);
	}

	BOOL DMCWndBase::ScreenToClient(LPPOINT lpPoint) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ScreenToClient(m_hWnd, lpPoint);
	}

	BOOL DMCWndBase::ScreenToClient(LPRECT lpRect) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		if(!::ScreenToClient(m_hWnd, (LPPOINT)lpRect))
			return FALSE;
		return ::ScreenToClient(m_hWnd, ((LPPOINT)lpRect)+1);
	}

	int DMCWndBase::MapWindowPoints(HWND hWndTo, LPPOINT lpPoint, UINT nCount) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::MapWindowPoints(m_hWnd, hWndTo, lpPoint, nCount);
	}

	int DMCWndBase::MapWindowPoints(HWND hWndTo, LPRECT lpRect) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::MapWindowPoints(m_hWnd, hWndTo, (LPPOINT)lpRect, 2);
	}

	// Update and Painting Functions

	HDC DMCWndBase::BeginPaint(LPPAINTSTRUCT lpPaint) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::BeginPaint(m_hWnd, lpPaint);
	}

	void DMCWndBase::EndPaint(LPPAINTSTRUCT lpPaint) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		::EndPaint(m_hWnd, lpPaint);
	}

	HDC DMCWndBase::GetDC() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetDC(m_hWnd);
	}

	HDC DMCWndBase::GetWindowDC() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowDC(m_hWnd);
	}

	int DMCWndBase::ReleaseDC(HDC hDC) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ReleaseDC(m_hWnd, hDC);
	}

	void DMCWndBase::Print(HDC hDC, DWORD dwFlags) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_PRINT, (WPARAM)hDC, dwFlags);
	}

	void DMCWndBase::PrintClient(HDC hDC, DWORD dwFlags) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_PRINTCLIENT, (WPARAM)hDC, dwFlags);
	}

	BOOL DMCWndBase::UpdateWindow() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::UpdateWindow(m_hWnd);
	}

	void DMCWndBase::SetRedraw(BOOL bRedraw /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)bRedraw, 0);
	}

	BOOL DMCWndBase::GetUpdateRect(LPRECT lpRect, BOOL bErase/* = FALSE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetUpdateRect(m_hWnd, lpRect, bErase);
	}

	int DMCWndBase::GetUpdateRgn(HRGN hRgn, BOOL bErase/* = FALSE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetUpdateRgn(m_hWnd, hRgn, bErase);
	}

	BOOL DMCWndBase::Invalidate(BOOL bErase /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::InvalidateRect(m_hWnd, NULL, bErase);
	}

	BOOL DMCWndBase::InvalidateRect(LPCRECT lpRect, BOOL bErase/* = TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::InvalidateRect(m_hWnd, lpRect, bErase);
	}

	BOOL DMCWndBase::ValidateRect(LPCRECT lpRect) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ValidateRect(m_hWnd, lpRect);
	}

	void DMCWndBase::InvalidateRgn(HRGN hRgn, BOOL bErase /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		::InvalidateRgn(m_hWnd, hRgn, bErase);
	}

	BOOL DMCWndBase::ValidateRgn(HRGN hRgn) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ValidateRgn(m_hWnd, hRgn);
	}

	BOOL DMCWndBase::ShowWindow(int nCmdShow) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	BOOL DMCWndBase::IsWindowVisible() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::IsWindowVisible(m_hWnd);
	}

	BOOL DMCWndBase::ShowOwnedPopups(BOOL bShow/* = TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ShowOwnedPopups(m_hWnd, bShow);
	}

	HDC DMCWndBase::GetDCEx(HRGN hRgnClip, DWORD flags) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetDCEx(m_hWnd, hRgnClip, flags);
	}

	BOOL DMCWndBase::LockWindowUpdate(BOOL bLock /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::LockWindowUpdate(bLock ? m_hWnd : NULL);
	}

	BOOL DMCWndBase::RedrawWindow(LPCRECT lpRectUpdate /*= NULL*/, HRGN hRgnUpdate/* = NULL*/, UINT flags /*= RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::RedrawWindow(m_hWnd, lpRectUpdate, hRgnUpdate, flags);
	}

	HWND DMCWndBase::GetSafeHwnd(void) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return m_hWnd;
	}

	// Timer Functions

	UINT_PTR DMCWndBase::SetTimer(UINT_PTR nIDEvent, UINT nElapse, void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD) /*= NULL*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetTimer(m_hWnd, nIDEvent, nElapse, (TIMERPROC)lpfnTimer);
	}

	BOOL DMCWndBase::KillTimer(UINT_PTR nIDEvent) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::KillTimer(m_hWnd, nIDEvent);
	}

	// Window State Functions

	BOOL DMCWndBase::IsWindowEnabled() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::IsWindowEnabled(m_hWnd);
	}

	BOOL DMCWndBase::EnableWindow(BOOL bEnable/* = TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::EnableWindow(m_hWnd, bEnable);
	}

	HWND DMCWndBase::SetActiveWindow() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetActiveWindow(m_hWnd);
	}

	HWND DMCWndBase::SetCapture() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetCapture(m_hWnd);
	}

	HWND DMCWndBase::SetFocus() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetFocus(m_hWnd);
	}

	// Dialog-Box Item Functions

	BOOL DMCWndBase::CheckDlgButton(int nIDButton, UINT nCheck) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::CheckDlgButton(m_hWnd, nIDButton, nCheck);
	}

	BOOL DMCWndBase::CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::CheckRadioButton(m_hWnd, nIDFirstButton, nIDLastButton, nIDCheckButton);
	}

	int DMCWndBase::DlgDirList(_Inout_z_ LPTSTR lpPathSpec, _In_ int nIDListBox, _In_ int nIDStaticPath, _In_ UINT nFileType) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::DlgDirList(m_hWnd, lpPathSpec, nIDListBox, nIDStaticPath, nFileType);
	}

	int DMCWndBase::DlgDirListComboBox(_Inout_z_ LPTSTR lpPathSpec, _In_ int nIDComboBox, _In_ int nIDStaticPath, _In_ UINT nFileType) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::DlgDirListComboBox(m_hWnd, lpPathSpec, nIDComboBox, nIDStaticPath, nFileType);
	}

	BOOL DMCWndBase::DlgDirSelect(_Out_z_cap_(nCount) LPTSTR lpString, _In_ int nCount, _In_ int nIDListBox) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::DlgDirSelectEx(m_hWnd, lpString, nCount, nIDListBox);
	}

	BOOL DMCWndBase::DlgDirSelectComboBox(_Out_z_cap_(nCount) LPTSTR lpString, _In_ int nCount, _In_ int nIDComboBox) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::DlgDirSelectComboBoxEx(m_hWnd, lpString, nCount, nIDComboBox);
	}

	UINT DMCWndBase::GetDlgItemInt(int nID, BOOL* lpTrans /*= NULL*/, BOOL bSigned/* = TRUE*/) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetDlgItemInt(m_hWnd, nID, lpTrans, bSigned);
	}

	UINT DMCWndBase::GetDlgItemText(_In_ int nID, _Out_z_cap_post_count_(nMaxCount, return + 1) LPTSTR lpStr, _In_ int nMaxCount) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetDlgItemText(m_hWnd, nID, lpStr, nMaxCount);
	}

#ifdef _OLEAUTO_H_
	//BOOL DMCWndBase::GetDlgItemText(int nID, BSTR& bstrText) const throw()
	//{
	//	DMASSERT(::IsWindow(m_hWnd));

	//	HWND hWndCtl = GetDlgItem(nID);
	//	if (hWndCtl == NULL)
	//		return FALSE;

	//	return DMCWndBase(hWndCtl).GetWindowText(bstrText);
	//}
#endif // _OLEAUTO_H_

	DMCWndBase DMCWndBase::GetNextDlgGroupItem(HWND hWndCtl, BOOL bPrevious/* = FALSE*/) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::GetNextDlgGroupItem(m_hWnd, hWndCtl, bPrevious));
	}

	DMCWndBase DMCWndBase::GetNextDlgTabItem(HWND hWndCtl, BOOL bPrevious /*= FALSE*/) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::GetNextDlgTabItem(m_hWnd, hWndCtl, bPrevious));
	}

	UINT DMCWndBase::IsDlgButtonChecked(int nIDButton) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::IsDlgButtonChecked(m_hWnd, nIDButton);
	}

	LRESULT DMCWndBase::SendDlgItemMessage(int nID, UINT message, WPARAM wParam/* = 0*/, LPARAM lParam /*= 0*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SendDlgItemMessage(m_hWnd, nID, message, wParam, lParam);
	}

	BOOL DMCWndBase::SetDlgItemInt(int nID, UINT nValue, BOOL bSigned /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetDlgItemInt(m_hWnd, nID, nValue, bSigned);
	}

	BOOL DMCWndBase::SetDlgItemText(int nID, LPCTSTR lpszString) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetDlgItemText(m_hWnd, nID, lpszString);
	}

	//HRESULT  DMCWndBase::GetDlgControl(int nID, REFIID iid, void** ppCtrl) throw()
	//{
	//	DMASSERT(::IsWindow(m_hWnd));
	//	DMASSERT(ppCtrl != NULL);
	//	if (ppCtrl == NULL)
	//		return E_POINTER;
	//	*ppCtrl = NULL;
	//	HRESULT hr = HRESULT_FROM_WIN32(ERROR_CONTROL_ID_NOT_FOUND);
	//	HWND hWndCtrl = GetDlgItem(nID);
	//	if (hWndCtrl != NULL)
	//	{
	//		*ppCtrl = NULL;
	//		CComPtr<IUnknown> spUnk;
	//		hr = AtlAxGetControl(hWndCtrl, &spUnk);
	//		if (SUCCEEDED(hr))
	//			hr = spUnk->QueryInterface(iid, ppCtrl);
	//	}
	//	return hr;
	//}

	//HRESULT  DMCWndBase::GetDlgHost(int nID, REFIID iid, void** ppHost) throw()
	//{
	//	DMASSERT(::IsWindow(m_hWnd));
	//	DMASSERT(ppHost != NULL);
	//	if (ppHost == NULL)
	//		return E_POINTER;
	//	*ppHost = NULL;
	//	HRESULT hr = HRESULT_FROM_WIN32(ERROR_CONTROL_ID_NOT_FOUND);
	//	HWND hWndCtrl = GetDlgItem(nID);
	//	if (hWndCtrl != NULL)
	//	{
	//		CComPtr<IUnknown> spUnk;
	//		hr = AtlAxGetHost(hWndCtrl, &spUnk);
	//		if (SUCCEEDED(hr))
	//			hr = spUnk->QueryInterface(iid, ppHost);
	//	}
	//	return hr;
	//}

	// Scrolling Functions

	int DMCWndBase::GetScrollPos(int nBar) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetScrollPos(m_hWnd, nBar);
	}

	BOOL DMCWndBase::GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetScrollRange(m_hWnd, nBar, lpMinPos, lpMaxPos);
	}

	BOOL DMCWndBase::ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect/* = NULL*/, LPCRECT lpClipRect/* = NULL*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ScrollWindow(m_hWnd, xAmount, yAmount, lpRect, lpClipRect);
	}

	int DMCWndBase::ScrollWindowEx(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate, UINT uFlags) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ScrollWindowEx(m_hWnd, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate, uFlags);
	}

	int DMCWndBase::ScrollWindowEx(int dx, int dy, UINT uFlags, LPCRECT lpRectScroll/* = NULL*/, LPCRECT lpRectClip /*= NULL*/, HRGN hRgnUpdate /*= NULL*/, LPRECT lpRectUpdate /*= NULL*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ScrollWindowEx(m_hWnd, dx, dy, lpRectScroll, lpRectClip, hRgnUpdate, lpRectUpdate, uFlags);
	}

	int DMCWndBase::SetScrollPos(int nBar, int nPos, BOOL bRedraw /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
	}

	BOOL DMCWndBase::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
	}

	BOOL DMCWndBase::ShowScrollBar(UINT nBar, BOOL bShow /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ShowScrollBar(m_hWnd, nBar, bShow);
	}

	BOOL DMCWndBase::EnableScrollBar(UINT uSBFlags, UINT uArrowFlags /*= ESB_ENABLE_BOTH*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::EnableScrollBar(m_hWnd, uSBFlags, uArrowFlags);
	}

	// Window Access Functions

	DMCWndBase DMCWndBase::ChildWindowFromPoint(POINT point) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::ChildWindowFromPoint(m_hWnd, point));
	}

	DMCWndBase DMCWndBase::ChildWindowFromPointEx(POINT point, UINT uFlags) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::ChildWindowFromPointEx(m_hWnd, point, uFlags));
	}

	DMCWndBase DMCWndBase::GetTopWindow() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::GetTopWindow(m_hWnd));
	}

	DMCWndBase DMCWndBase::GetWindow(UINT nCmd) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::GetWindow(m_hWnd, nCmd));
	}

	DMCWndBase DMCWndBase::GetLastActivePopup() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::GetLastActivePopup(m_hWnd));
	}

	BOOL DMCWndBase::IsChild(HWND hWnd) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::IsChild(m_hWnd, hWnd);
	}

	DMCWndBase DMCWndBase::GetParent() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::GetParent(m_hWnd));
	}

	DMCWndBase DMCWndBase::SetParent(HWND hWndNewParent) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::SetParent(m_hWnd, hWndNewParent));
	}


	// Window Tree Access

	int DMCWndBase::GetDlgCtrlID() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetDlgCtrlID(m_hWnd);
	}

	int DMCWndBase::SetDlgCtrlID(int nID) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (int)::SetWindowLong(m_hWnd, GWL_ID, nID);
	}

	DMCWndBase DMCWndBase::GetDlgItem(int nID) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return DMCWndBase(::GetDlgItem(m_hWnd, nID));
	}

	// Alert Functions

	BOOL DMCWndBase::FlashWindow(BOOL bInvert) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::FlashWindow(m_hWnd, bInvert);
	}

	int DMCWndBase::MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption/* = _T("")*/, UINT nType /*= MB_OK*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
	}

	// Clipboard Functions

	BOOL DMCWndBase::ChangeClipboardChain(HWND hWndNewNext) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ChangeClipboardChain(m_hWnd, hWndNewNext);
	}

	HWND DMCWndBase::SetClipboardViewer() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetClipboardViewer(m_hWnd);
	}

	BOOL DMCWndBase::OpenClipboard() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::OpenClipboard(m_hWnd);
	}


	// Caret Functions

	BOOL DMCWndBase::CreateCaret(HBITMAP hBitmap) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::CreateCaret(m_hWnd, hBitmap, 0, 0);
	}

	BOOL DMCWndBase::CreateSolidCaret(int nWidth, int nHeight) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::CreateCaret(m_hWnd, (HBITMAP)0, nWidth, nHeight);
	}

	BOOL DMCWndBase::CreateGrayCaret(int nWidth, int nHeight) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::CreateCaret(m_hWnd, (HBITMAP)1, nWidth, nHeight);
	}

	BOOL DMCWndBase::HideCaret() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::HideCaret(m_hWnd);
	}

	BOOL DMCWndBase::ShowCaret() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ShowCaret(m_hWnd);
	}

#ifdef _INC_SHELLAPI
	// Drag-Drop Functions
	void DMCWndBase::DragAcceptFiles(BOOL bAccept /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd)); ::DragAcceptFiles(m_hWnd, bAccept);
	}
#endif

	// Icon Functions

	HICON DMCWndBase::SetIcon(HICON hIcon, BOOL bBigIcon /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, WM_SETICON, bBigIcon, (LPARAM)hIcon);
	}

	HICON DMCWndBase::GetIcon(BOOL bBigIcon /*= TRUE*/) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (HICON)::SendMessage(m_hWnd, WM_GETICON, bBigIcon, 0);
	}


	// Help Functions

	BOOL DMCWndBase::WinHelp(LPCTSTR lpszHelp, UINT nCmd /*= HELP_CONTEXT*/, DWORD dwData/* = 0*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::WinHelp(m_hWnd, lpszHelp, nCmd, dwData);
	}

	BOOL DMCWndBase::SetWindowContextHelpId(DWORD dwContextHelpId) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetWindowContextHelpId(m_hWnd, dwContextHelpId);
	}

	DWORD DMCWndBase::GetWindowContextHelpId() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowContextHelpId(m_hWnd);
	}

	// Hot Key Functions

	int DMCWndBase::SetHotKey(WORD wVirtualKeyCode, WORD wModifiers) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, WM_SETHOTKEY, MAKEWORD(wVirtualKeyCode, wModifiers), 0);
	}

	DWORD DMCWndBase::GetHotKey() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd, WM_GETHOTKEY, 0, 0);
	}


	// Misc. Operations

	//N new
	BOOL DMCWndBase::GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetScrollInfo(m_hWnd, nBar, lpScrollInfo);
	}
	int DMCWndBase::SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetScrollInfo(m_hWnd, nBar, lpScrollInfo, bRedraw);
	}
	BOOL DMCWndBase::IsDialogMessage(LPMSG lpMsg) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::IsDialogMessage(m_hWnd, lpMsg);
	}

	void DMCWndBase::NextDlgCtrl() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_NEXTDLGCTL, 0, 0L);
	}

	void DMCWndBase::PrevDlgCtrl() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_NEXTDLGCTL, 1, 0L);
	}
	void DMCWndBase::GotoDlgCtrl(HWND hWndCtrl) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)hWndCtrl, 1L);
	}

	BOOL DMCWndBase::ResizeClient(int nWidth, int nHeight, BOOL bRedraw /*= TRUE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));

		RECT rcWnd;
		if(!GetClientRect(&rcWnd))
			return FALSE;

		if(nWidth != -1)
			rcWnd.right = nWidth;
		if(nHeight != -1)
			rcWnd.bottom = nHeight;

		if(!::AdjustWindowRectEx(&rcWnd, GetStyle(), (!(GetStyle() & WS_CHILD) && (GetMenu() != NULL)), GetExStyle()))
			return FALSE;

		UINT uFlags = SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE;
		if(!bRedraw)
			uFlags |= SWP_NOREDRAW;

		return SetWindowPos(NULL, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, uFlags);
	}

	int DMCWndBase::GetWindowRgn(HRGN hRgn) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowRgn(m_hWnd, hRgn);
	}

	int DMCWndBase::SetWindowRgn(HRGN hRgn, BOOL bRedraw /*= FALSE*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetWindowRgn(m_hWnd, hRgn, bRedraw);
	}

	HDWP DMCWndBase::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	DWORD DMCWndBase::GetWindowThreadID() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::GetWindowThreadProcessId(m_hWnd, NULL);
	}

	DWORD DMCWndBase::GetWindowProcessID() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		DWORD dwProcessID;
		::GetWindowThreadProcessId(m_hWnd, &dwProcessID);
		return dwProcessID;
	}

	BOOL DMCWndBase::IsWindow() const throw()
	{
		return ::IsWindow(m_hWnd);
	}

	BOOL DMCWndBase::IsWindowUnicode() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::IsWindowUnicode(m_hWnd);
	}

	BOOL DMCWndBase::IsParentDialog() throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		TCHAR szBuf[8]; // "#32770" + NUL character
		if (GetClassName(GetParent(), szBuf, sizeof(szBuf)/sizeof(szBuf[0])) == 0)
			return FALSE;
		return lstrcmp(szBuf, _T("#32770")) == 0;
	}

	BOOL DMCWndBase::ShowWindowAsync(int nCmdShow) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::ShowWindowAsync(m_hWnd, nCmdShow);
	}

	DMCWndBase  DMCWndBase::GetDescendantWindow(int nID) const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));

		// GetDlgItem recursive (return first found)
		// breadth-first for 1 level, then depth-first for next level

		// use GetDlgItem since it is a fast USER function
		HWND hWndChild, hWndTmp;
		if((hWndChild = ::GetDlgItem(m_hWnd, nID)) != NULL)
		{
			if(::GetTopWindow(hWndChild) != NULL)
			{
				// children with the same ID as their parent have priority
				DMCWndBase wnd(hWndChild);
				hWndTmp = wnd.GetDescendantWindow(nID);
				if(hWndTmp != NULL)
					return DMCWndBase(hWndTmp);
			}
			return DMCWndBase(hWndChild);
		}

		// walk each child
		for(hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
		{
			DMCWndBase wnd(hWndChild);
			hWndTmp = wnd.GetDescendantWindow(nID);
			if(hWndTmp != NULL)
				return DMCWndBase(hWndTmp);
		}

		return DMCWndBase(NULL);    // not found
	}

	void  DMCWndBase::SendMessageToDescendants(UINT message, WPARAM wParam/* = 0*/, LPARAM lParam /*= 0*/, BOOL bDeep /*= TRUE*/) throw()
	{
		for(HWND hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
		{
			::SendMessage(hWndChild, message, wParam, lParam);

			if(bDeep && ::GetTopWindow(hWndChild) != NULL)
			{
				// send to child windows after parent
				DMCWndBase wnd(hWndChild);
				wnd.SendMessageToDescendants(message, wParam, lParam, bDeep);
			}
		}
	}

	BOOL  DMCWndBase::CenterWindow(HWND hWndCenter/* = NULL*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));

		// determine owner window to center against
		DWORD dwStyle = GetStyle();
		if(hWndCenter == NULL)
		{
			if(dwStyle & WS_CHILD)
				hWndCenter = ::GetParent(m_hWnd);
			else
				hWndCenter = ::GetWindow(m_hWnd, GW_OWNER);
		}

		// get coordinates of the window relative to its parent
		RECT rcDlg;
		::GetWindowRect(m_hWnd, &rcDlg);
		RECT rcArea;
		RECT rcCenter;
		HWND hWndParent;
		if(!(dwStyle & WS_CHILD))
		{
			// don't center against invisible or minimized windows
			if(hWndCenter != NULL)
			{
				DWORD dwStyleCenter = ::GetWindowLong(hWndCenter, GWL_STYLE);
				if(!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
					hWndCenter = NULL;
			}

			// center within screen coordinates
#if WINVER < 0x0500
			::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
#else
			HMONITOR hMonitor = NULL;
			if(hWndCenter != NULL)
			{
				hMonitor = ::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST);
			}
			else
			{
				hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
			}

			/* Used inside COM methods that do not want to throw */
#ifndef DMENSURE_RETURN_VAL
#define DMENSURE_RETURN_VAL(expr, val)        \
	do {                                      \
	int __atl_condVal=!!(expr);               \
	DMASSERT(__atl_condVal);                    \
	if(!(__atl_condVal)) return val;          \
	} while (0) 
#endif // DMENSURE_RETURN_VAL
			DMENSURE_RETURN_VAL(hMonitor != NULL, FALSE);

			MONITORINFO minfo;
			minfo.cbSize = sizeof(MONITORINFO);
			BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);
			DMENSURE_RETURN_VAL(bResult, FALSE);

			rcArea = minfo.rcWork;
#endif
			if(hWndCenter == NULL)
				rcCenter = rcArea;
			else
				::GetWindowRect(hWndCenter, &rcCenter);
		}
		else
		{
			// center within parent client coordinates
			hWndParent = ::GetParent(m_hWnd);
			DMASSERT(::IsWindow(hWndParent));

			::GetClientRect(hWndParent, &rcArea);
			DMASSERT(::IsWindow(hWndCenter));
			::GetClientRect(hWndCenter, &rcCenter);
			::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
		}

		int DlgWidth = rcDlg.right - rcDlg.left;
		int DlgHeight = rcDlg.bottom - rcDlg.top;

		// find dialog's upper left based on rcCenter
		int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
		int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

		// if the dialog is outside the screen, move it inside
		if(xLeft + DlgWidth > rcArea.right)
			xLeft = rcArea.right - DlgWidth;
		if(xLeft < rcArea.left)
			xLeft = rcArea.left;

		if(yTop + DlgHeight > rcArea.bottom)
			yTop = rcArea.bottom - DlgHeight;
		if(yTop < rcArea.top)
			yTop = rcArea.top;

		// map screen coordinates to child coordinates
		return ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL  DMCWndBase::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags/* = 0*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));

		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if(dwStyle == dwNewStyle)
			return FALSE;

		::SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);
		if(nFlags != 0)
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}

		return TRUE;
	}

	BOOL DMCWndBase::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags /*= 0*/) throw()
	{
		DMASSERT(::IsWindow(m_hWnd));

		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if(dwStyle == dwNewStyle)
			return FALSE;

		::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewStyle);
		if(nFlags != 0)
		{
			::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}

		return TRUE;
	}

#ifdef _OLEAUTO_H_
	//BOOL DMCWndBase::GetWindowText(BSTR* pbstrText) throw()
	//{
	//	return GetWindowText(*pbstrText);
	//}

	//	BOOL DMCWndBase::GetWindowText(BSTR& bstrText) throw()
	//	{
	//		USES_CONVERSION_EX;
	//		DMASSERT(::IsWindow(m_hWnd));
	//		::SysFreeString(bstrText);
	//		bstrText = NULL;
	//
	//		int nLen = ::GetWindowTextLength(m_hWnd);
	//
	//		CTempBuffer<TCHAR> lpszText;
	//		if(nLen>0)
	//		{
	//			ATLTRY(lpszText.Allocate(nLen+1));
	//			if (lpszText == NULL)
	//			{
	//				return FALSE;
	//			}
	//
	//			if(!::GetWindowText(m_hWnd, lpszText, nLen+1))
	//			{
	//				return FALSE;
	//			}
	//		}
	//
	//		bstrText = ::SysAllocString(T2OLE_EX_DEF(lpszText));
	//#pragma warning(push)
	//#pragma warning(disable:4068)
	//#pragma prefast(push)
	//#pragma prefast(disable:325, "We are deliberately checking if this has already been allocated")
	//		return nLen==0 ? TRUE : ((bstrText != NULL) ? TRUE : FALSE);
	//#pragma prefast(pop)
	//#pragma warning(pop)
	//	}
#endif // _OLEAUTO_H_
	DMCWndBase DMCWndBase::GetTopLevelParent() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));

		HWND hWndParent = m_hWnd;
		HWND hWndTmp;
		while((hWndTmp = ::GetParent(hWndParent)) != NULL)
			hWndParent = hWndTmp;

		return DMCWndBase(hWndParent);
	}

	DMCWndBase DMCWndBase::GetTopLevelWindow() const throw()
	{
		DMASSERT(::IsWindow(m_hWnd));

		HWND hWndParent;
		HWND hWndTmp = m_hWnd;

		do
		{
			hWndParent = hWndTmp;
			hWndTmp = (::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD) ? ::GetParent(hWndParent) : ::GetWindow(hWndParent, GW_OWNER);
		}
		while(hWndTmp != NULL);

		return DMCWndBase(hWndParent);
	}

	BOOL DMCWndBase::SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,DWORD dwFlags)
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::SetLayeredWindowAttributes(m_hWnd, crKey, bAlpha, dwFlags);
	}

	BOOL DMCWndBase::UpdateLayeredWindow(HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc,
		POINT *pptSrc,COLORREF crKey, BLENDFUNCTION *pblend,DWORD dwFlags)
	{
		DMASSERT(::IsWindow(m_hWnd));
		return ::UpdateLayeredWindow(m_hWnd,hdcDst,pptDst,psize,hdcSrc,pptSrc,crKey,pblend,dwFlags);
	}

}//namespace DM