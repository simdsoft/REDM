#include "DmMainAfx.h"
#include "DMDispatch.h"
#include <stdarg.h>

namespace DM
{
	/// DMAttributeDispatch------------------------------
	IDMAttribute *DMAttributeDispatch::m_pAttributeObj=NULL;
	void DMAttributeDispatch::SetAttributeDispatch(IDMAttribute* pAttributeObj)
	{
		DMASSERT_EXPR(NULL!=pAttributeObj, L"m_pAttributeObj为空!");
		m_pAttributeObj = pAttributeObj;
	}

	DMCode DMAttributeDispatch::ParseInt(LPCWSTR lpszValue,int&Obj)
	{
		DMASSERT_EXPR(NULL!=m_pAttributeObj, L"m_pAttributeObj未初始化!");
		return m_pAttributeObj->ParseInt(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParseBool(LPCWSTR lpszValue,bool&Obj)
	{
		DMASSERT_EXPR(NULL!=m_pAttributeObj, L"m_pAttributeObj未初始化!");
		return m_pAttributeObj->ParseBool(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParseColor(LPCWSTR lpszValue,DMColor&Obj)
	{
		DMASSERT_EXPR(NULL!=m_pAttributeObj, L"m_pAttributeObj未初始化!");
		return m_pAttributeObj->ParseColor(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParseSize(LPCWSTR lpszValue,DM::CSize&Obj)
	{
		DMASSERT_EXPR(NULL!=m_pAttributeObj, L"m_pAttributeObj未初始化!");
		return m_pAttributeObj->ParseSize(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParsePoint(LPCWSTR lpszValue,DM::CPoint&Obj)
	{
		DMASSERT_EXPR(NULL!=m_pAttributeObj, L"m_pAttributeObj未初始化!");
		return m_pAttributeObj->ParsePoint(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParseRect(LPCWSTR lpszValue,DM::CRect&Obj)
	{
		DMASSERT_EXPR(NULL!=m_pAttributeObj, L"m_pAttributeObj未初始化!");
		return m_pAttributeObj->ParseRect(lpszValue, Obj);
	}

	// DMLogDispatch------------------------------------
	IDMLog *DMLogDispatch::m_pLogObj = NULL;
	DMLock  DMLogDispatch::m_Lock;
	void DMLogDispatch::SetLogDispatch(IDMLog* pLogObj)
	{
		DMAutoLock autolock(&m_Lock);
		m_pLogObj = pLogObj;
	}
	DMCode DMLogDispatch::LogW(DMLOGLEVEL iLevel,LPCWSTR lpszFuncName, LPCWSTR lpszFileName, int iLine,LPCWSTR szFmt,...)
	{
		DMAutoLock autolock(&m_Lock);
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (NULL == m_pLogObj)
			{
				iErr = DM_ECODE_FAIL;
				break;
			}
			va_list arg;
			va_start(arg, szFmt);
			wchar_t szBuf[200] = {0};
			_vsnwprintf_s(szBuf, 200, sizeof(szBuf) / sizeof(wchar_t) - 1, szFmt, arg);
			va_end(arg);
			iErr = m_pLogObj->LogW(iLevel, lpszFuncName, lpszFileName, iLine, szBuf);
		} while (false);

		return iErr;
	}
}//namespace DM

namespace DMAttr
{
	// ----------------------------------------------------
	// 内部强制规定XML属性字符串定义方式
#if defined(_DEBUG)|| defined(_DMDesigner_)
	DM::DMLazyT<DM::DMXmlDocument>  DMInitAttrDispatch::m_pDoc;
#endif
	wchar_t* DMInitAttrDispatch::GetAttrValue(wchar_t* cls,wchar_t *pBuf)
	{  
#if defined(_DEBUG)|| defined(_DMDesigner_)
		if (!m_pDoc->Root().IsValid())///< 自动调用DMXmlDocument构造初始化
		{
			m_pDoc->Base().InsertChildNode(L"root");
		}
		DM::DMXmlNode XmlChild = m_pDoc->Root().FirstChild();
		while (XmlChild.IsValid())
		{
			if (0 == _wcsicmp(XmlChild.GetName(),cls))
			{
				break;
			}
			XmlChild = XmlChild.NextSibling();
		}
		if (!XmlChild.IsValid())// 未找到就插入
		{
			XmlChild = m_pDoc->Root().InsertChildNode(cls);
		}
		XmlChild.SetAttribute(pBuf,L"");
#endif
		wchar_t*pLow = wcsstr(pBuf,L"_");
		if (!pLow){DMASSERT_EXPR(0,L"请使用类型_定义方式,如INT_ihei");}
		pLow++;
		return pLow;
	}
}