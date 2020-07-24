#include "DmMainAfx.h"
#include "DMDispatch.h"
#include <stdarg.h>

namespace DM
{
	/// DMAttributeDispatch------------------------------
	IDMAttribute *DMAttributeDispatch::m_pAttributeObj=NULL;
	void DMAttributeDispatch::SetAttributeDispatch(IDMAttribute* pAttributeObj)
	{
		DMASSERT(NULL!=pAttributeObj);
		m_pAttributeObj = pAttributeObj;
	}

	DMCode DMAttributeDispatch::ParseInt(LPCSTR lpszValue,int&Obj)
	{
		DMASSERT(NULL!=m_pAttributeObj);
		return m_pAttributeObj->ParseInt(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParseBool(LPCSTR lpszValue,bool&Obj)
	{
		DMASSERT(NULL!=m_pAttributeObj);
		return m_pAttributeObj->ParseBool(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParseColor(LPCSTR lpszValue,DMColor&Obj)
	{
		DMASSERT(NULL!=m_pAttributeObj);
		return m_pAttributeObj->ParseColor(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParseSize(LPCSTR lpszValue,DM::CSize&Obj)
	{
		DMASSERT(NULL!=m_pAttributeObj);
		return m_pAttributeObj->ParseSize(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParsePoint(LPCSTR lpszValue,DM::CPoint&Obj)
	{
		DMASSERT(NULL!=m_pAttributeObj);
		return m_pAttributeObj->ParsePoint(lpszValue, Obj);
	}

	DMCode DMAttributeDispatch::ParseRect(LPCSTR lpszValue,DM::CRect&Obj)
	{
		DMASSERT(NULL!=m_pAttributeObj);
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
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (NULL == m_pLogObj)
			{
				iErr = DM_ECODE_FAIL;
				break;
			}
			DMAutoLock autolock(&m_Lock);
			va_list arg;
			va_start(arg, szFmt);
			int len = _vscwprintf(szFmt, arg);
			DMBufT<wchar_t> pBuf;pBuf.Allocate(len+1);
			vswprintf_s(pBuf, (len+1), szFmt, arg);
			va_end(arg);
			iErr = m_pLogObj->LogW(iLevel, lpszFuncName, lpszFileName, iLine, pBuf);
		} while (false);

		return iErr;
	}
}//namespace DM

namespace DMAttr
{
	// ----------------------------------------------------
	// 内部强制规定XML属性字符串定义方式
	char* DMInitAttrDispatch::GetAttrValue(char* cls, char*pBuf)
	{  
		char*pLow = strstr(pBuf,"_");
		DMASSERT_EXPRA(!!pLow, "type_method,INT_ihei");
		pLow++;
		return pLow;
	}
}