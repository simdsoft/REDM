//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMToolTip.h 
// File Des: IDMToolTip是对tooltip的抽象类
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-3	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	typedef struct stDMToolTipInfo
	{
		stDMToolTipInfo()
		{
			hDUIWnd = 0;iDelayTime = 500;iSpanTime = 5000;dwReserve = 0;rcPosFlags.SetRect(-1,0,0,0);
		}
		DUIWND		hDUIWnd;		 ///< 拥有tooltip的DUI窗口句柄
		int         iDelayTime;      ///< 延时显示
		int         iSpanTime;       ///< 延时消失
		CRect		rcTarget;		 ///< tooltip感应区
		CStringW	strTip;			 ///< tooltip字符串
		CStringW	strXmlId;        ///< tooltip的xml
		CRect       rcPosFlags;		 ///< 类似TrackPopupMenu的参数，前三项分别代表UINT uFlags, int x, int y,第四项大于表示强制固定，为0表示不强制固定，到屏幕边上时，自动内移
		CRect       rcScreenTarget;  ///< rcTarget为相对于其窗口的客户区域，rcScreenTarget为屏幕区域
		
		DWORD       dwReserve;       ///< 保留
	}DMToolTipInfo,*PDMToolTipInfo;

	/// <summary>
	///		ToolTip对外扩展接口,classtype=<see cref="DMREG_ToolTip"/>
	/// </summary>
	class DM_EXPORT IDMToolTip:public DMBase,public IDMMessageFilter
	{
		DMDECLARE_CLASS_NAME(IDMToolTip,L"IDMToolTip",DMREG_ToolTip);
	public:
		virtual DMCode Update(PDMToolTipInfo pInfo) = 0;					///< 更新并启用显示定时器
		virtual DMCode Hide() = 0;											///< 直接隐藏
		virtual DMCode Clear() = 0;											///< 最后清除
		virtual DMCode RelayEvent(const MSG *pMsg) = 0;                     ///< 转发消息
	};
}
