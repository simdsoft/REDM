//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMLog.h 
// File Des:
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-10	1.0			
//-------------------------------------------------------
#pragma once
namespace DM
{
	enum DMLOGLEVEL///LOG类型
	{
		DMLOG_TRACE = 0,		///<跟踪类型
		DMLOG_DEBUG = 1,		///<Debug类型
		DMLOG_INFO  = 2,		///<正常类型
		DMLOG_WARN  = 3,		///<警告类型
		DMLOG_ERR   = 4,		///<错误类型
		DMLOG_FATAL = 5,		///<极其严重错误
	};

	/// <summary>
	///		LOG打印对外扩展接口,classtype=<see cref="DMREG_Log"/>
	/// </summary>
	/// <remarks>
	///     可以输出，或保存成log文件
	/// </remarks>
	class IDMLog:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMLog,L"IDMLog",DMREG_Log);
	public:
		/// @brief 打印LOG接口
		/// @param[in]		 iLevel			LOG类型<see cref="DMLOGLEVEL"/>
		/// @param[in]		 lpszFuncName	函数名
		/// @param[in]		 lpszFileName	文件名
		/// @param[in]		 iLine			行号
		/// @param[in]		 szLogBuf		输出的LOG
		/// @remark			
		/// @return DM_ECODE_OK为成功
		virtual DMCode LogW(DMLOGLEVEL iLevel,LPCWSTR lpszFuncName, LPCWSTR lpszFileName, int iLine,LPCWSTR szLogBuf) = 0;
	};
}
