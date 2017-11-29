//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIFontPool.h 
// File Des:// font="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-30	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	typedef IDMFont*  IDMFontPtr;
	/// <summary>
	///		管理字体的池
	/// </summary>
	/// <remarks>
	///		导出仅为了方便gtest测试
	/// </remarks>
	class DM_EXPORT DUIFontPool:public DMMapT<CStringW,IDMFontPtr>
	{
	public:
		DUIFontPool();
		~DUIFontPool();

	public:
		bool SetDefaultFont(const CStringW& strFont);				///< 设置默认字体
		IDMFontPtr GetFont(const CStringW& strFont);				///< 获取字体,如strFont为NULL表示获取默认字体
		
	public:// 辅助
		virtual void PreMapKeyRemove(const IDMFontPtr &obj);

	protected:
		CStringW GetFontKey(const LPLOGFONTW lpLogFont);
		bool GetLogFont(const CStringW& strFont,LPLOGFONTW lpLogFont);

	protected:
		LOGFONTW				m_lfDefault;					   ///< 默认字体
		wchar_t					m_szDefFontFace[LF_FACESIZE];      ///< 默认字体名称
	};

}//namespace DM