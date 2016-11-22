// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMTransImpl.h
// File mark:   
// File summary:翻译类的内置实现
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-11-21
// ----------------------------------------------------------------
#pragma once
#include "DMTransHelper.h"

namespace DM
{
	/// <summary>
	///		内置语言包的处理示例
	/// </summary>
	class DMTransImpl : public IDMTrans,public DMArrayT<DMLanguageItemPtr>
	{
		DMDECLARE_CLASS_NAME(DMTransImpl,L"DMTransImpl",DMREG_Trans);
	public:
		DMTransImpl();
		~DMTransImpl();

		DMCode LoadTrans(WPARAM wp, LPARAM lp);
		DMCode SetTrans(CStringW strLanguageName);
		DMCode AddTransItem(CStringW strSrc, CStringW strTrans,CStringW strNodeName = L"",CStringW strLanguageName = L"");
		CStringW GetTrans(CStringW strSrc,CStringW strNodeName = L"",CStringW strLanguageName = L"");

	public:// 辅助
		DMLanguageItemPtr FindLanguageItemPtr(CStringW strLanguageName);

	protected:
		virtual void PreArrayObjRemove(const DMLanguageItemPtr &obj);
		virtual bool EqualArrayObj(const DMLanguageItemPtr &objsrc, const DMLanguageItemPtr &objdest);

	public:
		DMLanguageItemPtr			      m_pCurLanguage;			  ///< 当前使用的语言包
		CStringW                          m_strCurLanguageName;       ///< 当前使用的theme Key
	};
}//namespace DM