#include "DmMainAfx.h"
#include "DMLogImpl.h"

namespace DM
{
	static PWCHAR pszLogPrix[] = 
	{
		L"DMLOG_TRACE",		///<跟踪类型
		L"DMLOG_DEBUG",		///<Debug类型
		L"DMLOG_INFO",		///<正常类型
		L"DMLOG_WARN",		///<警告类型
		L"DMLOG_ERR",		///<错误类型
		L"DMLOG_FATAL",     ///<极其严重错误
	};
	DMCode DMLogImpl::LogW(DMLOGLEVEL iLevel,LPCWSTR lpszFuncName, LPCWSTR lpszFileName, int iLine,LPCWSTR szLogBuf)
	{
#ifndef _DEBUG
		if (DMLOG_DEBUG==iLevel)
		{
			return DM_ECODE_FAIL;
		}
#endif
		DM::CStringW szLog;
		szLog.Format(L"[SECCLI:DuiMagic][%s][%s][line:%d]%s",pszLogPrix[iLevel],lpszFuncName,iLine,szLogBuf);
		OutputDebugStringW(szLog);
		return DM_ECODE_OK;
	}

}//namespace DM