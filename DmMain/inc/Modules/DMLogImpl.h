//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMLogImpl.h 
// File Des: 
// File Summary: 
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
	class DMLogImpl:public IDMLog
	{
		DMDECLARE_CLASS_NAME(DMLogImpl,L"DMLogImpl",DMREG_Log);
	public:
		virtual DMCode LogW(DMLOGLEVEL iLevel,LPCWSTR lpszFuncName, LPCWSTR lpszFileName, int iLine,LPCWSTR szLogBuf);
	};

}//namespace DM