#include "DmMainAfx.h"
#include "DMLogImpl.h"

namespace DM
{
	static PWCHAR pszLogPrix[] = 
	{
		L"DMLOG_INFO",	//INFO_LOG = 0
		L"DMLOG_ERR",	//ERROR_LOG = 1
	};
	DMCode DMLogImpl::LogW(DMLOGLEVEL iLevel,LPCWSTR lpszFuncName, LPCWSTR lpszFileName, int iLine,LPCWSTR szLogBuf)
	{
		DM::CStringW szLog;
		szLog.Format(L"[SECCLI:DuiMagic][%s][%s][line:%d]%s",pszLogPrix[iLevel],lpszFuncName,iLine,szLogBuf);
		OutputDebugStringW(szLog);
		return DM_ECODE_OK;
	}

}//namespace DM