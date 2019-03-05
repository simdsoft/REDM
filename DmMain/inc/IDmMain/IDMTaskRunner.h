// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	IDMTaskRunner.h
// File mark:   
// File summary:实现类似于QT的跨线程事件绑定
// 这是一种委托监听方式,0. DMSolt函数的原型为bool fun(const DMEventSender& sender, const DMBundle& args), fun可以为全局或类成员函数
//                      1. 用户通过ConnectEvent,ConnectUiEvent,ConnectAsyncEvent预先绑定lpszEventName----DMSolt函数
//						2. 用户通过FireEvent传入lpszEventName来触发DMSolt函数调用
//                      3. DMSolt函数的调用场景可以是同步或异步或在UI线程,这由1的绑定方式来决定

// 示例:类A,IDMTaskRunner的变量g_taskrunner(全局)
//			  class A{
//						A(){g_taskrunner.ConnectEvent(DMTT_ASYN_UI,"123",DMSolt(&A::fun, this));}
//						bool fun(const DMEventSender& sender, const DMBundle& args);}
//            上面代码在A的构造函数中预先绑定了DMSolt,之后,在任意代码处调用event.FireEvent("123")，都会触发在UI线程中异步调用fun函数
//             
//      类B，有成员函数void UiFun(int a, CStringW b, double c);
//            g_taskrunner.PostTask(DMTT_ASYN_UI,DMNewRunnableMethod(&B::UiFun, this, 10, str , 5.67)),就会在UI线程中触发异步调用UiFun
//            g_taskrunner.PostTask(DMTT_ASYN_WORK,DMNewRunnableMethod(&B::UiFun, this, 10, str , 5.67)),就会在UI线程中触发异步调用UiFun
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-03-02
// ----------------------------------------------------------------
#pragma once
#include "DMTaskT.h"

namespace DM
{
	enum DMTaskType
	{
		DMTT_SYNC,		///< 同步触发Task											
		DMTT_ASYN_UI,	///< 在UI线程中异步触发Task
		DMTT_ASYN_WORK,	///< 在工作线程中异步触发Task
	};

	/// <summary>
	///		跨线程事件绑定对外扩展接口,classtype=<see cref="DMREG_Event"/>
	/// </summary>
	class IDMTaskRunner : public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMTaskRunner,L"IDMEvent",DMREG_TaskRunner);
	public:
		virtual DMCode InitEvent(DWORD dwUIThreadId) = 0;											     ///< 初始化并指定UI线程ID,所有的UI TASK都会Post到此线程处理,此函数要求在UI线程中进行初始化
		virtual DMCode UninstallEvent() = 0;															 ///< 反初始化
		virtual bool   IsEventExists(LPCWSTR lpszEventName) = 0;										 ///< 事件是否已存在
		virtual DMCode RemoveEvent(LPCWSTR lpszEventName) = 0;											 ///< 移除某个事件对应的事件槽
		virtual DMCode RemoveEventIfThis(void* pThis) = 0;												 ///< 移除某个对象的所有关联事件槽(不包括静态成员函数)
		virtual DMCode RemoveAllEvents() = 0;															 ///< 移除所有事件槽
		virtual DMCode ConnectEvent(DMTaskType tty, LPCWSTR lpszEventName, const DMSlot& slot, int group = 100) = 0;///< 绑定一个事件槽,此事件槽在FireEvent时(同步,异步UI线程,异步工作线程)触发,同一个lpszEventName,group越大越迟调用(优先级越低,group内部会转成正值)
		virtual DMCode FireEvent(LPCWSTR lpszEventName) = 0;											  ///< 触发绑定的事件槽
		virtual DMCode FireEvent(LPCWSTR lpszEventName, DMBundle& args) = 0;							  ///< 传入参数,触发绑定的事件槽

		virtual DMCode PostTask(DMTaskType tty, IDMTaskPtr pTask) = 0;                                    ///< 无需绑定，直接把一个函数Post到(同步,异步UI线程,异步工作线程)触发
	};

}//namespace DM