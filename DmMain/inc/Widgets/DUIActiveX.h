//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des: 
// File Summary:��ActiveX�£����ڲ�ͣ��ˢ�±���������ӿؼ��п�����ʾ�����ؼ���һ������ʹ��cache����
// ��Ȼ����ˢ��ʱ���ӿؼ��п��ܲ�����£�����ʹ���ϱ���
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-5	1.0			
//--------------------------------------------------------
#pragma once
#include "Common/ActiveX/DMActiveXBase.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIActiveX"/>��xml���Զ���
	/// </summary>
	class DUIActiveXAttr
	{
	public:
		static const char* STRING_clsid;                                  ///< ָ��CLSID,ʾ��:clsid=""
		static const char* INT_clsctx;									   ///< ָ��CLSCTX,Ĭ��ΪCLSCTX_ALL,ʾ��:clsctx=""
		static const char* bool_bdelayinit;							   ///< ������ʾʱ�ż���,Ĭ��Ϊtrue,ʾ��:bdelayinit="1"
		static const char* bool_bsupportmulthread;						   ///< ACTIVEX�Ƿ�֧�ֶ��߳�,Ĭ��Ϊfalse,ע��,�������ݲ�֧�ֶ�̬�ı�,ʾ��:bsupportmulthread="0"   
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
		DMCode OnAttrClsid(LPCSTR pszValue, bool bLoadXml);

	public:// ����
		bool InitActiveX();
		
	public:
		DMAxContainerImpl			*m_pAxContainer;
		CLSID						 m_ClsId;
		DWORD						 m_ClsCtx;
		bool						 m_bDelayInit;										///< ������ʾʱ�ż���
		bool                         m_bInit;
		bool                         m_bSupportMultThread;
	};
}// namespace DM
