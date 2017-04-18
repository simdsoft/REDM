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
//      guoyou		2014-11-17	1.0			
//--------------------------------------------------------
#pragma once
#include "DUIActiveX.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIFlash"/>的xml属性定义
	/// </summary>
	class DUIFlashAttr
	{
	public:
		static wchar_t* STRING_url;									   ///< flash的资源,支持相对路径加载或Res方式加载,示例:url="http://www.hgy413.com/swf/2.swf"
	};
	DMAttrValueInit(DUIFlashAttr,STRING_url)
}

namespace DM
{
	/// <summary>
	///		 DUIFlash的内置实现
	/// </summary>
	class DM_EXPORT DUIFlash :public DUIActiveX
	{
		DMDECLARE_CLASS_NAME(DUIFlash, DUINAME_Flash,DMREG_Window)
	public:
		DUIFlash();

	public:
		DMCode DV_OnAxActivate(IUnknown *pUnknwn);

	public:
		//---------------------------------------------------
		// Function Des: 播放控制
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		bool Play(void* pBuf, DWORD dwSize);
		bool Play(const LPCWSTR pszUrl);
		DMComPtr<ShockwaveFlashObjects::IShockwaveFlash> Ptr();

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_SHOWWINDOW(OnShowWindow)
		DM_END_MSG_MAP()
	public:
		void OnShowWindow(BOOL bShow, UINT nStatus);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIFlashAttr::STRING_url,OnAttrUrl)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttrUrl(LPCWSTR pszValue, bool bLoadXml);

	public:
		long                                               m_curFrame;                                        
		CStringW										   m_strUrl;
	};

}//namespace DM