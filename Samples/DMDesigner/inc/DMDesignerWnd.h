// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMDesignerWnd.h
// File mark:   
// File summary:设计器主窗口
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-7
// ----------------------------------------------------------------
#pragma once

class DMDesignerWnd : public DMHWnd,public IDMAccelHandler
{
public:
	DMDesignerWnd();
	~DMDesignerWnd();

	DECLARE_MESSAGE_MAP()						
	DECLARE_EVENT_MAP()		

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);
	void OnDestroy();
	void OnLButtonDown(UINT nFlags,CPoint pt);
	void OnLButtonDbClick(UINT nFlags,CPoint pt);
	void OnDropFiles(HDROP hDropInfo);
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);	
	void OnTimer(UINT_PTR idEvent);

	DMCode OnClose();
	DMCode OnMaximize();
	DMCode OnRestore();
	DMCode OnMinimize();	
	DMCode OnPreview(DMEventArgs *pEvt);	

	bool OnAccelPressed(const DUIAccel& Accel);

	DMCode OnParseRes(CStringW strResDir);									///< 读取资源
	
protected:
	// Menu项
	DMCode OnGlobalMenuBtn(int idFrom);
	DMCode HandleGlobalMenu(int nID);
	DMCode InitFileMenu(DMXmlNode& XmlNode,int idFrom);
	DMCode InitEditMenu(DMXmlNode& XmlNode, int idFrom);
	DMCode InitViewMenu(DMXmlNode& XmlNode,int idFrom);
	DMCode InitModeMenu(DMXmlNode& XmlNode,int idFrom);
	DMCode InitHelpMenu(DMXmlNode& XmlNode,int idFrom);

public:
	DMCode OpenNewProj();
	DMCode CloseProj();
	DMCode OptionObjProj();
	DMCode ReloadProj();
public:
	ObjXml*							m_pDesignerXml;   
	ActionSlotMgr<ActionSlot>		m_ActionSlotMgr;
};