//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMScript.h 
// File Des:
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-25	1.0			
//-------------------------------------------------------
#pragma once
namespace DM
{
	/// <summary>
	///		扩展接口，lasstype=<see cref="DMREG_Script"/>
	/// </summary>
	class IDMScript : public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMAnimate,L"IDMScript",DMREG_Script);
	public:
		virtual DMCode InitScript() = 0;                            ///< 用于初始化,(也可以方便绑定运行时才能绑定的全局变量）
		virtual DMCode RunString(LPCSTR lpszString) = 0;			///< 通过文本运行脚本
		virtual DMCode RunBuf(LPCSTR pBuf, size_t len) = 0;			///< 通过Buf运行脚本
		virtual DMCode RunFile(LPCSTR lpszFilePath) = 0;			///< 通过文件运行脚本
		virtual DMCode RunEvent(DMEventArgs* pEvt) = 0;				///< 通过事件控制运行脚本
	};

}//namespace DM