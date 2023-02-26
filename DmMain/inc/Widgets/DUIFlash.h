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
//      guoyou		2014-11-17	1.0			
//--------------------------------------------------------
#pragma once
#include "Widgets/DUIActiveX.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIFlash"/>��xml���Զ���
	/// </summary>
	class DUIFlashAttr
	{
	public:
		static const char* STRING_url;									   ///< flash����Դ,֧�����·�����ػ�Res��ʽ����,ʾ��:url="http://www.hgy413.com/swf/2.swf"
	};
	DMAttrValueInit(DUIFlashAttr,STRING_url)
}

namespace DM
{
	/// <summary>
	///		 DUIFlash������ʵ��
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
		// Function Des: ���ſ���
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
		DMCode OnAttrUrl(LPCSTR pszValue, bool bLoadXml);

	public:
		long                                               m_curFrame;                                        
		CStringW										   m_strUrl;
	};

}//namespace DM