// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	XmlEditor.h
// File mark:   
// File summary:显示、编辑XML
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2017-5-5
// ----------------------------------------------------------------
#pragma once

/// <summary>
///		subclass Scintilla wnd
/// </summary>
class XmlEditor : public DMHWnd
{
	DECLARE_MESSAGE_MAP()	
public:
	XmlEditor();

public:// 消息处理
	void Destroy();
	LRESULT OnImeChar(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);


public:// 对外接口
	HWND Create(HWND hWndParent);

public:// http://www.naughter.com/scintilla.html,封装Scintilla的调用
	void InitXml();
	LRESULT Call(UINT message,WPARAM wParam=0,LPARAM lParam=0,bool bDirect=true);
	LRESULT AddText(int iLen, LPCSTR lpszText, bool bDirect = true);									///< 在pos位置后增加文本
	LRESULT Cancel(bool bDirect = true);
	LRESULT ClearAll(bool bDirect = true);																///< 清空所有文本
	LRESULT EmptyUndoBuffer(bool bDirect = true);
	LRESULT GotoPos(int iPos, bool bDirect = true);														///< 跳转到指定pos
	int GetCurrentPos(bool bDirect = true);																///< 获得当前所在的pos(0开始)
	int GetCurLine(int iLen, LPCSTR lpszText, bool bDirect = true);
	LRESULT SetUndoCollection(bool bCollectUndo, bool bDirect = true);
	LRESULT SetSavePoint(bool bDirect = true);
	bool GetReadOnly(bool bDirect = true);																///< 是否为ReadOnly状态
	LRESULT SetReadOnly(bool bReadOnly, bool bDirect = true);											///< ReadOnly状态时，可折叠，可显示光标，但不能编辑

public:
	LRESULT				m_DirectFunction;
	LRESULT				m_DirectPointer;
};


/// <summary>
///		DUI的RealWnd接口
/// </summary>
class DUIRealXml : public IDUIRealWnd, public AttrBase
{
	DMDECLARE_CLASS_NAME(DUIWindow,"xmleditor",DMREG_Window);
public:
	DUIRealXml();

public:
	HWND OnCreateRealWnd(LPCWSTR atom,HWND hWndParent);
	DMCode OnDestoryRealWnd();
	DMCode OnShowRealWnd(BOOL bShow, UINT nStatus){return DM_ECODE_NOTIMPL;};
	DMCode OnMoveRealWnd(LPRECT lpRect);
	DMCode OnIsPopupWnd(){return DM_ECODE_FAIL;}

public:
	DMCode UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel);
	DMCode UpdateLock(HDMTREEITEM hItem);
	DMCode OnUpdateXml(DMEventArgs* pEvt);			
	
public:
	DMSmartPtrT<XmlEditor>         m_pXmlEditor;   
	HWND                           m_hParent;
};
