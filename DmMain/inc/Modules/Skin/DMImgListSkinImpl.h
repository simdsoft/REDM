//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMImgListSkinImpl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-28	1.0			
//-------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DMImgListSkinImpl"/>的xml属性定义
	/// </summary>
	class DMImgListSkinImplAttr
	{
	public:
		static wchar_t* STRING_src;                            ///< 状态连图源，可以通过xml加载
		static wchar_t* STRING_id;                             ///< 唯一标识skin类对象，示例：id="channel_vz"
		static wchar_t* bool_btitle;                           ///< 是否平铺绘制，示例：btitle="1"
		static wchar_t* bool_bvert;                            ///< 状态连图是否为竖直分布的，示例：bvert="1"
		static wchar_t* INT_states;                            ///< 状态数，示例：states="1"

	};
	DMAttrValueInit(DMImgListSkinImplAttr,STRING_src)DMAttrValueInit(DMImgListSkinImplAttr,STRING_id)
	DMAttrValueInit(DMImgListSkinImplAttr,bool_btitle)DMAttrValueInit(DMImgListSkinImplAttr,bool_bvert)
	DMAttrValueInit(DMImgListSkinImplAttr,INT_states)
}

namespace DM
{
	/// <summary>
	///		状态连图内置实现，属性：<see cref="DMAttr::DMImgListSkinImplAttr"/>
	/// </summary>
	class DMImgListSkinImpl:public IDMSkin
	{
		DMDECLARE_CLASS_NAME(DMImgListSkinImpl,L"imglist",DMREG_Skin);
	public:
		DMImgListSkinImpl();
	public:
		DMCode GetID(wchar_t* lpszId, int iSize);
		DMCode Draw(IDMCanvas *pCanvas, LPCRECT lpRectDraw, int iState,BYTE alpha=0xFF); 
		DMCode GetStateSize(SIZE &sz, int iState=0);                                        
		DMCode GetStates(int &iStates);	
		DMCode GetBitmap(IDMBitmap** ppBitmap);
		DMCode SetBitmap(LPBYTE pBuf,size_t szLen,LPCWSTR pszType);
		DMCode UpdateSkin(WPARAM wp, LPARAM lp);
		DMCode IsValid();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::STRING_src, OnAttributeGetImage)
			DM_STRING_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::STRING_id, m_strID, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::bool_btitle, m_bTile,  DM_ECODE_OK)	   
			DM_bool_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::bool_bvert, m_bVert, DM_ECODE_OK)	 
			DM_INT_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::INT_states,m_nStates,DM_ECODE_OK)   
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeGetImage(LPCWSTR pszValue, bool bLoadXml); // 初始化m_pBitmap
			
	protected:
		DMSmartPtrT<IDMBitmap>						m_pBitmap;      ///< 状态图         
		int											m_nStates;		///< 有多少种状态
		bool										m_bTile;		///< 是否平铺绘制，flase表示拉伸绘制
		bool										m_bVert;        ///< 子图是否垂直排列，0--水平排列(默认), 其它--垂直排列
		CStringW                                    m_strID;        ///< 唯一标识
		CStringW                                    m_strRes;       ///< 资源的唯一标识，用于换肤确认
	};
}//namespace DM