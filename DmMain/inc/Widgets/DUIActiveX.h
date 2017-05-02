//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des: 
// File Summary:在ActiveX下，由于不停的刷新背景，如果子控件中可能显示出父控件，一定不能使用cache属性
// 不然背景刷新时，子控件有可能不会更新，而是使用老背景
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-5	1.0			
//--------------------------------------------------------
#pragma once
#include "DMActiveXBase.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIActiveX"/>的xml属性定义
	/// </summary>
	class DUIActiveXAttr
	{
	public:
		static wchar_t* STRING_clsid;                                  ///< 指定CLSID,示例:clsid=""
		static wchar_t* INT_clsctx;									   ///< 指定CLSCTX,默认为CLSCTX_ALL,示例:clsctx=""
		static wchar_t* bool_bdelayinit;							   ///< 窗口显示时才加载,默认为true,示例:bdelayinit="1"
		static wchar_t* bool_bsupportmulthread;						   ///< ACTIVEX是否支持多线程,默认为false,注意,此属性暂不支持动态改变,示例:bsupportmulthread="0"   
	};
	DMAttrValueInit(DUIActiveXAttr,STRING_clsid)DMAttrValueInit(DUIActiveXAttr,INT_clsctx)DMAttrValueInit(DUIActiveXAttr,bool_bdelayinit)DMAttrValueInit(DUIActiveXAttr,bool_bsupportmulthread)
}

namespace DM
{
	class DM_EXPORT DUIActiveX : public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIActiveX, DUINAME_ActiveX,DMREG_Window)
	public:
		explicit DUIActiveX();
		virtual ~DUIActiveX();

	public:
		bool SetActiveXVisible(bool bVisible,bool bFocus = false);						///< For windowless objects, this method invalid
		bool SetActiveXRect(CRect rcPos);
		
	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST,WM_MOUSELAST,OnMouseEvent)
			MESSAGE_RANGE_HANDLER_EX(WM_KEYFIRST,WM_KEYLAST,OnKeyEvent)
			MSG_WM_SIZE(OnSize)
			MSG_WM_SHOWWINDOW(OnShowWindow)
		DM_END_MSG_MAP()

	public:
		void OnSize(UINT nType, CSize size);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnShowWindow(BOOL bShow, UINT nStatus);
		LRESULT OnMouseEvent(UINT uMsg,WPARAM wp,LPARAM lp);
		LRESULT OnKeyEvent(UINT uMsg,WPARAM wp,LPARAM lp);

	public:
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTALLKEYS;}

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIActiveXAttr::STRING_clsid,OnAttrClsid)
			DM_DWORD_ATTRIBUTE(DMAttr::DUIActiveXAttr::INT_clsctx,m_ClsCtx,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIActiveXAttr::bool_bdelayinit,m_bDelayInit,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIActiveXAttr::bool_bsupportmulthread,m_bSupportMultThread,DM_ECODE_OK)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttrClsid(LPCWSTR pszValue, bool bLoadXml);

	public:// 辅助
		bool InitActiveX();
		
	public:
		DMAxContainerImpl			*m_pAxContainer;
		CLSID						 m_ClsId;
		DWORD						 m_ClsCtx;
		bool						 m_bDelayInit;										///< 窗口显示时才加载
		bool                         m_bInit;
		bool                         m_bSupportMultThread;
	};
}// namespace DM
