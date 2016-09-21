//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMMsgThunk.h 
// File Des:此代码来自atl:atlstdthunk.h，对应原型_stdcallthunk
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-11	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	#pragma pack(push, 1)
	/// <summary>
	///		thunk操作，把hwnd偷换成this指针，从而实现全局函数切换到类成员函数,x64代码暂未移过来
	/// </summary>
	class DM_EXPORT DMMsgThunk
	{
	public:
		void Init(DWORD_PTR proc, void* pThis);	
		void* GetCodeAddress();
	public:
		DWORD			m_mov;		// 4个字节
		DWORD			m_this;
		BYTE			m_jmp;
		DWORD			m_relproc;
	};
	#pragma pack(pop)

	/// <summary>
	///		thunk工具,要创建多个主窗口，所以不能为单例模式
	/// </summary>
	class DM_EXPORT DMMsgThunkTool
	{
	public:
		DMMsgThunkTool();
		virtual~DMMsgThunkTool();
	public:
		bool MsgThunkInit();										///< thunk初始化,分配thunk堆
		bool MsgThunkRun(HWND hWnd, DWORD_PTR proc, PVOID pThis);	///< 执行thunk
		void MsgThunkUnInit();					
		PVOID MsgThunkGetCodeAddress();

	protected:
		HANDLE                m_hHeap;
		DMMsgThunk            *m_pThunk;
	};	

}//namespace DM