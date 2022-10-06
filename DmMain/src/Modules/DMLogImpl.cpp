#include "DmMainAfx.h"
#include "DMLogImpl.h"

namespace DM
{
	static const wchar_t* pszLogPrix[] = 
	{
		L"DMLOG_TRACE",		///<��������
		L"DMLOG_DEBUG",		///<Debug����
		L"DMLOG_INFO",		///<��������
		L"DMLOG_WARN",		///<��������
		L"DMLOG_ERR",		///<��������
		L"DMLOG_FATAL",     ///<�������ش���
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