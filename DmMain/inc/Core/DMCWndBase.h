//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMCWndBase.h 
// File Des: 实现CWnd封装
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-13	1.0			
//-------------------------------------------------------
#pragma once
namespace DM
{
	/// <summary>
	///		基于atlwin.CWindow,模拟CWnd
	/// </summary>
	/// <remarks>
	///		CWnd封装的函数,此类基本都封装了,函数太多,不一一列出,请直接使用
	/// </remarks>
	class DM_EXPORT DMCWndBase
	{
	public:
		DMCWndBase(HWND hWnd = NULL) throw();
		virtual~DMCWndBase();
		DMCWndBase& operator=(HWND hWnd) throw();
		void Attach(HWND hWndNew) throw();
		HWND Detach() throw();
		BOOL DestroyWindow() throw();

		// -------------------------------------------------
		// Attributes
		operator HWND() const throw();
		DWORD GetStyle() const throw();
		DWORD GetExStyle() const throw();

		LONG GetWindowLong(int nIndex) const throw();
		LONG SetWindowLong(int nIndex, LONG dwNewLong) throw();

#ifdef _WIN64
		LONG_PTR GetWindowLongPtr(int nIndex) const throw();//32位下同于GetWindowLong,WinUser.h中有宏定义
		LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) throw();//32位下同于SetWindowLong,WinUser.h中有宏定义
#endif

		WORD GetWindowWord(int nIndex) const throw();
		WORD SetWindowWord(int nIndex, WORD wNewWord) throw();

		// -------------------------------------------------
		// Message Functions
		LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) throw();
		BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) throw();
		BOOL SendNotifyMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0) throw();

		// support for C style macros
		static LRESULT SendMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) throw();

		// -------------------------------------------------
		// Window Text Functions
		BOOL SetWindowText(LPCTSTR lpszString) throw();
		int GetWindowText(_Out_z_cap_post_count_(nMaxCount, return + 1) LPTSTR lpszStringBuf, _In_ int nMaxCount) const throw();
		int GetWindowText(CStringT& strText) const;//CStringT代替CSimpleString
		int GetWindowTextLength() const throw();

		// -------------------------------------------------
		// Font Functions
		void SetFont(HFONT hFont, BOOL bRedraw = TRUE) throw();
		HFONT GetFont() const throw();

		// -------------------------------------------------
		// Menu Functions (non-child windows only)
		HMENU GetMenu() const throw();
		BOOL SetMenu(HMENU hMenu) throw();
		BOOL DrawMenuBar() throw();
		HMENU GetSystemMenu(BOOL bRevert) const throw();
		BOOL HiliteMenuItem(HMENU hMenu, UINT uItemHilite, UINT uHilite) throw();

		// -------------------------------------------------
		// Window Size and Position Functions
		BOOL IsIconic() const throw();
		BOOL IsZoomed() const throw();
		BOOL MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE) throw();
		BOOL MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE) throw();
		BOOL SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags) throw();
		BOOL SetWindowPos(HWND hWndInsertAfter, LPCRECT lpRect, UINT nFlags) throw();
		UINT ArrangeIconicWindows() throw();
		BOOL BringWindowToTop() throw();
		BOOL GetWindowRect(LPRECT lpRect) const throw();
		BOOL GetClientRect(LPRECT lpRect) const throw();
		BOOL GetWindowPlacement(WINDOWPLACEMENT FAR* lpwndpl) const throw();
		BOOL SetWindowPlacement(const WINDOWPLACEMENT FAR* lpwndpl) throw();

		// -------------------------------------------------
		// Coordinate Mapping Functions
		BOOL ClientToScreen(LPPOINT lpPoint) const throw();
		BOOL ClientToScreen(LPRECT lpRect) const throw();
		BOOL ScreenToClient(LPPOINT lpPoint) const throw();
		BOOL ScreenToClient(LPRECT lpRect) const throw();
		int MapWindowPoints(HWND hWndTo, LPPOINT lpPoint, UINT nCount) const throw();
		int MapWindowPoints(HWND hWndTo, LPRECT lpRect) const throw();

		// -------------------------------------------------
		// Update and Painting Functions
		HDC BeginPaint(LPPAINTSTRUCT lpPaint) throw();
		void EndPaint(LPPAINTSTRUCT lpPaint) throw();
		HDC GetDC() throw();
		HDC GetWindowDC() throw();
		int ReleaseDC(HDC hDC) throw();
		void Print(HDC hDC, DWORD dwFlags) const throw();
		void PrintClient(HDC hDC, DWORD dwFlags) const throw();
		BOOL UpdateWindow() throw();
		void SetRedraw(BOOL bRedraw = TRUE) throw();
		BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE) throw();
		int GetUpdateRgn(HRGN hRgn, BOOL bErase = FALSE) throw();
		BOOL Invalidate(BOOL bErase = TRUE) throw();
		BOOL InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE) throw();
		BOOL ValidateRect(LPCRECT lpRect) throw();
		void InvalidateRgn(HRGN hRgn, BOOL bErase = TRUE) throw();
		BOOL ValidateRgn(HRGN hRgn) throw();
		BOOL ShowWindow(int nCmdShow) throw();
		BOOL IsWindowVisible() const throw();
		BOOL ShowOwnedPopups(BOOL bShow = TRUE) throw();
		HDC GetDCEx(HRGN hRgnClip, DWORD flags) throw();
		BOOL LockWindowUpdate(BOOL bLock = TRUE) throw();
		BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL, HRGN hRgnUpdate = NULL, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE) throw();
		HWND GetSafeHwnd(void) throw();

		// -------------------------------------------------
		// Timer Functions
		UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse, void (CALLBACK* lpfnTimer)(HWND, UINT, UINT_PTR, DWORD) = NULL) throw();
		BOOL KillTimer(UINT_PTR nIDEvent) throw();

		// -------------------------------------------------
		// Window State Functions
		BOOL IsWindowEnabled() const throw();
		BOOL EnableWindow(BOOL bEnable = TRUE) throw();
		HWND SetActiveWindow() throw();
		HWND SetCapture() throw();
		HWND SetFocus() throw();

		// -------------------------------------------------
		// Dialog-Box Item Functions
		BOOL CheckDlgButton(int nIDButton, UINT nCheck) throw();
		BOOL CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) throw();
		int DlgDirList(_Inout_z_ LPTSTR lpPathSpec, _In_ int nIDListBox, _In_ int nIDStaticPath, _In_ UINT nFileType) throw();
		int DlgDirListComboBox(_Inout_z_ LPTSTR lpPathSpec, _In_ int nIDComboBox, _In_ int nIDStaticPath, _In_ UINT nFileType) throw();
		BOOL DlgDirSelect(_Out_z_cap_(nCount) LPTSTR lpString, _In_ int nCount, _In_ int nIDListBox) throw();
		BOOL DlgDirSelectComboBox(_Out_z_cap_(nCount) LPTSTR lpString, _In_ int nCount, _In_ int nIDComboBox) throw();
		UINT GetDlgItemInt(int nID, BOOL* lpTrans = NULL, BOOL bSigned = TRUE) const throw();
		UINT GetDlgItemText(_In_ int nID, _Out_z_cap_post_count_(nMaxCount, return + 1) LPTSTR lpStr, _In_ int nMaxCount) const throw();
#ifdef _OLEAUTO_H_
		BOOL GetDlgItemText(int nID, BSTR& bstrText) const throw();
#endif // _OLEAUTO_H_
		DMCWndBase GetNextDlgGroupItem(HWND hWndCtl, BOOL bPrevious = FALSE) const throw();
		DMCWndBase GetNextDlgTabItem(HWND hWndCtl, BOOL bPrevious = FALSE) const throw();
		UINT IsDlgButtonChecked(int nIDButton) const throw();
		LRESULT SendDlgItemMessage(int nID, UINT message, WPARAM wParam = 0, LPARAM lParam = 0) throw();
		BOOL SetDlgItemInt(int nID, UINT nValue, BOOL bSigned = TRUE) throw();
		BOOL SetDlgItemText(int nID, LPCTSTR lpszString) throw();

		//HRESULT GetDlgControl(int nID, REFIID iid, void** ppCtrl) throw();
		//HRESULT GetDlgHost(int nID, REFIID iid, void** ppHost) throw();

		// -------------------------------------------------
		// Scrolling Functions
		int GetScrollPos(int nBar) const throw();
		BOOL GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const throw();
		BOOL ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect = NULL, LPCRECT lpClipRect = NULL) throw();
		int ScrollWindowEx(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, HRGN hRgnUpdate, LPRECT lpRectUpdate, UINT uFlags) throw();
		int ScrollWindowEx(int dx, int dy, UINT uFlags, LPCRECT lpRectScroll = NULL, LPCRECT lpRectClip = NULL, HRGN hRgnUpdate = NULL, LPRECT lpRectUpdate = NULL) throw();
		int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE) throw();
		BOOL SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw = TRUE) throw();
		BOOL ShowScrollBar(UINT nBar, BOOL bShow = TRUE) throw();
		BOOL EnableScrollBar(UINT uSBFlags, UINT uArrowFlags = ESB_ENABLE_BOTH) throw();

		// -------------------------------------------------
		// Window Access Functions
		DMCWndBase ChildWindowFromPoint(POINT point) const throw();
		DMCWndBase ChildWindowFromPointEx(POINT point, UINT uFlags) const throw();
		DMCWndBase GetTopWindow() const throw();
		DMCWndBase GetWindow(UINT nCmd) const throw();
		DMCWndBase GetLastActivePopup() const throw();
		BOOL IsChild(HWND hWnd) const throw();
		DMCWndBase GetParent() const throw();
		DMCWndBase SetParent(HWND hWndNewParent) throw();

		// -------------------------------------------------
		// Window Tree Access
		int GetDlgCtrlID() const throw();
		int SetDlgCtrlID(int nID) throw();
		DMCWndBase GetDlgItem(int nID) const throw();

		// -------------------------------------------------
		// Alert Functions
		BOOL FlashWindow(BOOL bInvert) throw();
		int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = _T(""), UINT nType = MB_OK) throw();

		// -------------------------------------------------
		// Clipboard Functions
		BOOL ChangeClipboardChain(HWND hWndNewNext) throw();
		HWND SetClipboardViewer() throw();
		BOOL OpenClipboard() throw();

		// -------------------------------------------------
		// Caret Functions
		BOOL CreateCaret(HBITMAP hBitmap) throw();
		BOOL CreateSolidCaret(int nWidth, int nHeight) throw();
		BOOL CreateGrayCaret(int nWidth, int nHeight) throw();
		BOOL HideCaret() throw();
		BOOL ShowCaret() throw();

#ifdef _INC_SHELLAPI
		// Drag-Drop Functions
		void DragAcceptFiles(BOOL bAccept = TRUE) throw();
#endif

		// -------------------------------------------------
		// Icon Functions
		HICON SetIcon(HICON hIcon, BOOL bBigIcon = TRUE) throw();
		HICON GetIcon(BOOL bBigIcon = TRUE) const throw();

		// -------------------------------------------------
		// Help Functions
		BOOL WinHelp(LPCTSTR lpszHelp, UINT nCmd = HELP_CONTEXT, DWORD dwData = 0) throw();
		BOOL SetWindowContextHelpId(DWORD dwContextHelpId) throw();
		DWORD GetWindowContextHelpId() const throw();

		// -------------------------------------------------
		// Hot Key Functions
		int SetHotKey(WORD wVirtualKeyCode, WORD wModifiers) throw();
		DWORD GetHotKey() const throw();

		// Misc. Operations

		// -------------------------------------------------
		//N new
		BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo) throw();
		int SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE) throw();
		BOOL IsDialogMessage(LPMSG lpMsg) throw();
		void NextDlgCtrl() const throw();
		void PrevDlgCtrl() const throw();
		void GotoDlgCtrl(HWND hWndCtrl) const throw();
		BOOL ResizeClient(int nWidth, int nHeight, BOOL bRedraw = TRUE) throw();
		int GetWindowRgn(HRGN hRgn) throw();
		int SetWindowRgn(HRGN hRgn, BOOL bRedraw = FALSE) throw();
		HDWP DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) throw();
		DWORD GetWindowThreadID() throw();
		DWORD GetWindowProcessID() throw();
		BOOL IsWindow() const throw();
		BOOL IsWindowUnicode() const throw();
		BOOL IsParentDialog() throw();
		BOOL ShowWindowAsync(int nCmdShow) throw();
		DMCWndBase GetDescendantWindow(int nID) const throw();
		void SendMessageToDescendants(UINT message, WPARAM wParam = 0, LPARAM lParam = 0, BOOL bDeep = TRUE) throw();
		BOOL CenterWindow(HWND hWndCenter = NULL) throw();
		BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0) throw();
		BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0) throw();
#ifdef _OLEAUTO_H_
		//BOOL GetWindowText(BSTR* pbstrText) throw();
		//BOOL GetWindowText(BSTR& bstrText) throw();
#endif // _OLEAUTO_H_
		DMCWndBase GetTopLevelParent() const throw();
		DMCWndBase GetTopLevelWindow() const throw();

		// -------------------------------------------------
		// layout
		BOOL SetLayeredWindowAttributes(COLORREF crKey,BYTE bAlpha,DWORD dwFlags);
		BOOL UpdateLayeredWindow(HDC hdcDst, POINT *pptDst, SIZE *psize, HDC hdcSrc, POINT *pptSrc,COLORREF crKey, BLENDFUNCTION *pblend,DWORD dwFlags);

	public:
		HWND					m_hWnd;///<窗口句柄
	};
}// namespace DM