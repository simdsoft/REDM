//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSpyTool.h 
// File Des: Spy的相关封装
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-2	1.0		
//-------------------------------------------------------
#pragma once

namespace DM
{
	enum DMSPYCMD
	{
		DMSPY_INIT = 1314,  													    ///< 校验初始化消息
		DMSPY_ROOT,																    ///< 通过hWnd取得起始DUI
		DMSPY_ENUM,		                                                            ///< 取得窗口	
		DMSPY_INFO,                                                                 ///< 取得数据
		DMSPY_POINT,                                                                ///< 通过pt取得窗口
	};

#define  DMSPY_NAME_LEN					260
#define  DMSPY_CLASSNAME_LEN            260
#define  DMSPY_XML_LEN                  10000
#define  DMSPY_ENUM_BEGIN               -1
#define  DMSPY_ENUM_END                 -2
#define  DMSPY_SHAREMEMORY             L"DMSPY_SHAREMEMORY" 
#define  DMSPY_SHAREMEMORYSIZE         L"DMSPY_SHAREMEMORYSIZE" 
	struct DMSpyInfo
	{
		bool               bVisible;
		DUIWND             hDUIWnd;
		int                iId;
		CRect              rcWindow;
		CRect              rcClient;
		wchar_t            szName[DMSPY_NAME_LEN];
		wchar_t			   szClassName[DMSPY_CLASSNAME_LEN];
		wchar_t            szXml[DMSPY_XML_LEN];
	};
	typedef struct stDMSpyEnum
	{
		DUIWND              hDUIWnd;
		DUIWND              hNextDUIWnd;
		int                 iCode;
		int                 iPanelIndex;// 记录panel当前索引（以1开始,不是panel,则为0）
		bool                bPanel;
		CPoint              pt;    // 使用坐标定位
		DMSpyInfo           info;

	}DMSpyEnum,*PDMSpyEnum;

	/// <summary>
	///		封装Spy的相关
	/// </summary>
	class DM_EXPORT DMSpyTool
	{
	public:
		DMSpyTool();
		~DMSpyTool();
	public:
		LRESULT OnSpy(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void EnumDUITree(LPCWSTR lpszPath);
		void EnumTreeItem(DUIWindow* pWnd,DMXmlNode XmlNode,int iLevel,bool bPanel); //iLevel指定XmlNode的下一层，也即pWnd所在的层
		void EnumPanelTreeItem(DUIWindow* pWnd,DMXmlNode XmlNode,int iLevel);// 查找pwnd的子panel列，XmlNode为pwnd所在的Node,每层只查一次
		void EnumInfo(DUIWindow*pWnd,PDMSpyEnum pSpyEnum);

	public:// 共享内存操作封装
		static bool CreateSharedMemory(UINT nSize);
		static bool GetShareMemorySize(UINT& nSize);
		static bool WriteShareMemory(void *pDate, UINT nSize);
		static bool ReadShareMemory(void *pDate, UINT nSize);
		static bool ReleaseSharedMemory();

	public:
		DUIWND                    m_hMainDUIWnd;  // 起始的DUI窗口
		HWND                      m_hSpyWnd;
		PDMSpyEnum                m_pSpyEnum;
		DMXmlNode				  m_XmlBase;
		CMap<int,bool>			  m_PanelParseMap; // 仅验证算法使用
	};

}//namespace DM