//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMAccel.h 
// File Des: Accel接口
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-4	1.0			
//-------------------------------------------------------
#pragma once

#ifndef NOHOTKEY
#define HOTKEYF_SHIFT           0x01
#define HOTKEYF_CONTROL         0x02
#define HOTKEYF_ALT             0x04
#endif 

namespace DM
{
	/// <summary>
	///		加速键转换
	/// </summary>
    class DM_EXPORT DUIAccel
    {
    public:
        DUIAccel(DWORD dwAccel);							///< dwAccel加速键值
        DUIAccel(UINT vKey=0,bool bCtrl=false,bool bAlt=false,bool bShift=false);///< UINT vKey与Ctrl组合、与Alt组合、与Shilft组合
        ~DUIAccel(void);

        CStringW GetKeyName(WORD vk);					    ///< 将键盘值转换为对应的字符串
        CStringW FormatHotkey();						    ///< 获得当前加速键的字符串形式

        WORD GetModifier() const {return m_wModifier;}	    ///< 获得加速键的修饰位
        WORD GetKey() const {return m_wVK;}				    ///< 获得加速键的主键
	
        static DWORD TranslateAccelKey(LPCWSTR lpszAccelKey);	///< 解析与字符串对应的加速键值

    protected:
        WORD					 m_wModifier;
        WORD					 m_wVK;
    };


	/// <summary>
	///		键盘加速键的事件处理抽象接口
	/// </summary>
    class DM_EXPORT IDMAccelHandler
    {
	public:
		/// -------------------------------------------------
		/// @brief obj是否存在
		/// @param[in]		 Accel    按下的加速键
		/// @return 加速键被处理返回true
        virtual bool OnAccelPressed(const DUIAccel& Accel) = 0;
    };

	/// <summary>
	///		加速键管理抽象接口
	/// </summary>
    class DM_EXPORT IDMAccelMgr
    {
	public:
        virtual void RegisterAccel(const DUIAccel& Accel, IDMAccelHandler* pHandler) = 0;	///< 注册加速键以及事件处理接口对象
        virtual void UnregisterAccel(const DUIAccel& Accel,IDMAccelHandler* pHandler) = 0;	///< 反注册加速键以及事件处理接口对象
        virtual void UnregisterAccels(IDMAccelHandler* pHandler) = 0;						///< 反注册事件处理接口对象的所有加速键
    };

}//namespace DM