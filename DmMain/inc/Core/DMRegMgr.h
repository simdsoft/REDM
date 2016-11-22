//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMRegMgr.h 
// File Des:注册类统一管理
// File Summary: Mgr为Manager缩写
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{	
	/// <summary>
	///		<see cref="DefRegName"/>可用于定义默认注册类型类名，
	///     比如xml解析实现如果有几套注册类，这时就需要通过它来指定当前使用哪一套！
	/// </summary>
	class DMRegTypeItem:public DMArrayT<IDMRegPtr>
	{
	public:
		DMRegTypeItem();
		virtual ~DMRegTypeItem();

	public:// 辅助重载函数

		void PreArrayObjRemove(const IDMRegPtr &obj);
	    bool EqualArrayObj(const IDMRegPtr &objsrc, const IDMRegPtr &objdest);

	public:
		CStringW                    m_DefRegName;   ///< 定义默认注册类型类名
	};

	/// <summary>
	///		按<see cref="DMREGTYPE"/>分类管理所有的注册类
	/// </summary>
	class DMRegMgr
	{
	public:
		DMRegMgr();
		~DMRegMgr();
	public:
		DMCode Register(IDMReg &RegObj, bool bReplace=false);
		DMCode CreateRegObj(void** ppObj, LPCWSTR lpszClassName,int RegType);
		DMCode UnRegister(LPCWSTR lpszClassName,int RegType);
		DMCode SetDefRegObj(LPCWSTR lpszClassName,int RegType);
		DMCode GetDefRegObj(CStringW &szName,int RegType);

	public:// 辅助
		DMCode RegisterByType(DMRegTypeItem &RtItem, IDMReg &RegObj, bool bReplace);
		DMCode CreateRegObjByType(DMRegTypeItem &RtItem, void** ppObj, LPCWSTR lpszClassName);
		DMCode UnRegisterByType(DMRegTypeItem &RtItem, LPCWSTR lpszClassName);
		DMCode SetDefRegObjByType(DMRegTypeItem &RtItem, LPCWSTR lpszClassName);

	public:
		DMRegTypeItem						m_RtSkin;
		DMRegTypeItem                       m_RtStyle;
		DMRegTypeItem                       m_RtAttribute;
		DMRegTypeItem                       m_RtWindow;
		DMRegTypeItem                       m_RtLayout;
		DMRegTypeItem                       m_RtImgDecoder;
		DMRegTypeItem                       m_RtRes;
		DMRegTypeItem                       m_RtRender;
		DMRegTypeItem                       m_RtDraw;
		DMRegTypeItem                       m_RtLog;
		DMRegTypeItem                       m_RtFlowLayout;
		DMRegTypeItem                       m_RtAnimate;
		DMRegTypeItem                       m_RtToolTip;
		DMRegTypeItem                       m_RtScript;
		DMRegTypeItem                       m_RtTrans;

	};

}//namespace DM