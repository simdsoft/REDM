//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMMsgLoop.h 
// File Des:
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-12	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		窗口消息分发，基于atlapp.CMessageLoop
	/// </summary>
	/// <remarks>
	///		基于smart指针控制，因为一个线程可能多个主窗口调用Run
	/// </remarks>
	class DM_EXPORT DMMsgLoop:public DMRefNum
	{
	public:
		DMMsgLoop();
		~DMMsgLoop();
	public:
		/// -------------------------------------------------
		/// @brief			开始消息循环
		/// @return			Msg.Wparam
		virtual int Run();

		/// -------------------------------------------------
		/// @brief			增加预处理消息
		/// @param[in]		pMessageFilter		基于<see cref="IDMMessageFilter"/>
		/// @return			TRUE:成功,False:失败
		BOOL AddMessageFilter(IDMMessageFilter* pMessageFilter);

		/// -------------------------------------------------
		/// @brief			移除预处理消息
		/// @param[in]		pMessageFilter		基于<see cref="IDMMessageFilter"/>
		/// @return			TRUE:成功,False:失败
		BOOL RemoveMessageFilter(IDMMessageFilter* pMessageFilter);

		/// -------------------------------------------------
		/// @brief			增加空闲处理消息
		/// @param[in]		pIdleHandler		基于<see cref="IDMIdleHandler"/>
		/// @return			TRUE:成功,False:失败
		BOOL AddIdleHandler(IDMIdleHandler* pIdleHandler);

		/// -------------------------------------------------
		/// @brief			移除空闲处理消息
		/// @param[in]		pIdleHandler		基于<see cref="IDMIdleHandler"/>
		/// @return			TRUE:成功,False:失败
		BOOL RemoveIdleHandler(IDMIdleHandler* pIdleHandler);

	public:
		/// -------------------------------------------------
		/// @brief			消息预过滤函数
		/// @remark         可重载，从而改变消息过滤
		/// @param[in]		pMsg		基于<see cref="MSG"/>
		/// @return			TRUE:成功,False:失败
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		/// -------------------------------------------------
		/// @brief			空闲预处理函数
		/// @remark         可重载，从而改变消息过滤
		/// @return			TRUE:成功,False:失败
		virtual BOOL OnIdle(int /*nIdleCount*/);

	public:
		static BOOL IsIdleMessage(MSG* pMsg);

	public:
		typedef CList<IDMMessageFilterPtr>	    IDMMessageFilterList;
		typedef CList<IDMIdleHandlerPtr>        IDMIdleHandlerList;
		IDMMessageFilterList					m_MsgFilterList;				
		IDMIdleHandlerList						m_IdleHandlerList;
		
		MSG										m_msg;
		DMLock*								    m_pMsgLock;
		DMLock*                                 m_pIdleLock;

	};

	/// <summary>
	///		管理<see cref="DMMsgLoop"/>对象,基于atlapp.CAppModule
	/// </summary>
	/// <remarks>
	///		传入的<see cref="DMMsgLoop"/>对象，请使用new 分配，smart指针会在引用计数为0时自动释放
	/// </remarks>
	class DM_EXPORT DMMsgLoopTool
		:public DMMapT<DWORD, DMMsgLoop*>
	{
	public:
		DMMsgLoopTool();
		virtual~DMMsgLoopTool();

		/// -------------------------------------------------
		/// @brief			增加Loop对象
		/// @remark         必须使用new分配，因为DMMsgLoop是基于智能指针控制
		/// @return			true:成功,false:失败
		bool AddMessageLoop(DMMsgLoop* pMsgLoop);		
		bool RemoveMessageLoop();											 ///< 移除当前线程loop对象
		DMMsgLoop* GetMessageLoop(DWORD dwThreadID = ::GetCurrentThreadId());///< 获取当前线程loop对象
		void Term();

	protected:
		DWORD							m_dwMainThreadID;					///< 主线程ID
		DMLock							m_Lock;

	};

}//namespace DM