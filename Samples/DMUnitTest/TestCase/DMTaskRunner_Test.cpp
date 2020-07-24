//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMPlugin_Test.cpp 
// File Des: 测试跨线程事件绑定的测试用例
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-16	1.0			
//-------------------------------------------------------
#pragma once
#include "DMUintTestAfx.h"

enum DMEventNameId
{
	EVENTID_SYNC1,
	EVENTID_SYNC2,

	EVENTID_UI1,
	EVENTID_UI2,

	EVENTID_AYNC1,
	EVENTID_AYNC2,
};

class TaskRunnerTest:public::testing::Test,public DMRefNum
{
public:
	DMSmartPtrT<IDMTaskRunner> m_pTaskRunner;
	//DMDISABLE_RUNNABLE_METHOD_REFCOUNT(TaskRunnerTest);//强制禁用引用计数控制生命周期,如需要,应该放在在.h文件中，基于DMRefNum的类,不需要此句，默认DM库都基于DMRefNum

public:
	virtual void SetUp()
	{
		//1. 获取对象,两种方式------------------------------------------------
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&m_pTaskRunner,DMREG_TaskRunner)),true);// 使用内部已初始化完成的TaskRunner对象。
		//EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&m_pTaskRunner,NULL, DMREG_TaskRunner)),true);// 自己创建一个TaskRunner对象，但要在UI线程中调用InitEvent初始化UI环境。
		//m_pTaskRunner->InitEvent(::GetCurrentThreadId());

		//2. 提前绑定，只要在FireEvent之前绑定即可---------------------------
		InitBind();
	}

	virtual void TearDown(){}
	virtual void OnFinalRelease(){}	
	~TaskRunnerTest(){m_RefCount = 0;}

public:// 辅助
	void InitBind()
	{
		if (m_pTaskRunner)
		{
			// 同步绑定，在FireEvent时同步触发
			m_pTaskRunner->ConnectEvent(DMTT_SYNC, ID2NAME(EVENTID_SYNC1),DMSlot(&TaskRunnerTest::s_slotSyncEvent1));			// 绑定全局函数
			m_pTaskRunner->ConnectEvent(DMTT_SYNC, ID2NAME(EVENTID_SYNC1),DMSlot(&TaskRunnerTest::slotSyncEvent1, this));
			m_pTaskRunner->ConnectEvent(DMTT_SYNC, ID2NAME(EVENTID_SYNC1),DMSlot(&TaskRunnerTest::slotSyncEvent1, this));		// 多次绑定，后一次被忽略
			m_pTaskRunner->ConnectEvent(DMTT_SYNC, ID2NAME(EVENTID_SYNC1),DMSlot(&TaskRunnerTest::slotSyncEvent2, this),99);	// 后绑定，但先调用，因为99比默认100小
			m_pTaskRunner->ConnectEvent(DMTT_SYNC, ID2NAME(EVENTID_SYNC2),DMSlot(&TaskRunnerTest::slotSyncEvent2, this),99);	// 后绑定，但先调用，因为99比默认100小
			//m_pTaskRunner->RemoveEvent(ID2NAME(EVENTID_SYNC1));	// 移除测试	
			//m_pTaskRunner->RemoveEventIfThis(this);				// 移除这个类的所有绑定
			//m_pTaskRunner->RemoveAllEvents();						// 移除所有绑定

			// UI绑定，在FireEvent时异步到UI线程触发
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_UI,ID2NAME(EVENTID_UI1),DMSlot(&TaskRunnerTest::s_slotUIEvent1));				// 绑定全局函数
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_UI,ID2NAME(EVENTID_UI1),DMSlot(&TaskRunnerTest::slotUIEvent1, this));
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_UI,ID2NAME(EVENTID_UI1),DMSlot(&TaskRunnerTest::slotUIEvent1, this));			// 多次绑定，后一次被忽略
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_UI,ID2NAME(EVENTID_UI1),DMSlot(&TaskRunnerTest::slotUIEvent2, this),99);		// 后绑定，但先调用，因为99比默认100小
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_UI,ID2NAME(EVENTID_UI2),DMSlot(&TaskRunnerTest::slotUIEvent2, this),99);		// 后绑定，但先调用，因为99比默认100小

			// 工作线程绑定,在FireEvent时异步到内部的工作线程触发
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_WORK,ID2NAME(EVENTID_AYNC1),DMSlot(&TaskRunnerTest::s_slotAyncEvent1));				// 绑定全局函数
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_WORK,ID2NAME(EVENTID_AYNC1),DMSlot(&TaskRunnerTest::slotAyncEvent1, this));
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_WORK,ID2NAME(EVENTID_AYNC1),DMSlot(&TaskRunnerTest::slotAyncEvent1, this));		// 多次绑定，后一次被忽略
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_WORK,ID2NAME(EVENTID_AYNC1),DMSlot(&TaskRunnerTest::slotAyncEvent2, this),99);	// 后绑定，但先调用，因为99比默认100小
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_WORK,ID2NAME(EVENTID_AYNC2),DMSlot(&TaskRunnerTest::slotAyncEvent2, this),99);	// 后绑定，但先调用，因为99比默认100小
			m_pTaskRunner->ConnectEvent(DMTT_ASYN_WORK,ID2NAME(EVENTID_AYNC2),DMSlot(&TaskRunnerTest::s_slotAyncEvent1));			// 绑定全局函数
		}
	}

	void FireBind()
	{
		if (m_pTaskRunner)
		{
			// 无需FireEvent，直接把一个函数Post到(同步,异步UI线程,异步工作线程)触发
			CStringA str = "abc";
			m_pTaskRunner->PostTask(DMTT_SYNC,DMNewRunnableMethod(&TaskRunnerTest::UiFun, this, 10, str , 5.67));
			m_pTaskRunner->PostTask(DMTT_ASYN_UI,DMNewRunnableMethod(&TaskRunnerTest::UiFun, this, 10, str , 5.67));
			m_pTaskRunner->PostTask(DMTT_ASYN_WORK,DMNewRunnableMethod(&TaskRunnerTest::UiFun, this, 10, str , 5.67));


			// 使用参数，触发调用绑定的函数
			DMBundle bundle = InitBundle();
			m_pTaskRunner->FireEvent(ID2NAME(EVENTID_SYNC1),bundle);
			m_pTaskRunner->FireEvent(ID2NAME(EVENTID_SYNC2),bundle);

			m_pTaskRunner->FireEvent(ID2NAME(EVENTID_UI1),bundle);
			m_pTaskRunner->FireEvent(ID2NAME(EVENTID_UI2),bundle);

			m_pTaskRunner->FireEvent(ID2NAME(EVENTID_AYNC1),bundle);
			m_pTaskRunner->FireEvent(ID2NAME(EVENTID_AYNC2),bundle);
		}
	}

	// 使用DMBundle设置绑定数据
	DMBundle InitBundle()
	{
		DMBundle bundle;
		bundle.PutBool(L"1", true);
		bundle.PutByte(L"2", 'a');
		bundle.PutChar(L"3", 'b');
		bundle.PutDword(L"4", 450);
		bundle.PutDouble(L"5", 500.122);
		bundle.PutFloat(L"6", 510.2f);
		bundle.PutInt(L"7", 54546);
		bundle.PutInt64(L"8", 0xffffffffffff);
		bundle.PutShort(L"9", 0xfffff);
		bundle.PutString(L"10", "127758857");
		bundle.PutStringW(L"11", L"234565");
		bundle.PutSize(L"12", CSize(10,9));
		bundle.PutRect(L"13", CRect(1,2,3,4));
		bundle.PutVoidPtr(L"14", (void*)0x12233);
		LiteralString strXml = "<imglist id=\"1\" src=\"png:1\" states=\"1\" />";
		DMXmlDocument doc;
		doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
		DMXmlNode XmlNode = doc.Root();
		XmlNode.SetAttribute("name1","value");
		XmlNode.SetAttributeInt("name2",222);
		bundle.PutXmlNode(L"15", XmlNode);

		return bundle;
	}

public:// 测试
	static bool TaskRunnerTest::s_slotSyncEvent1(const DMEventSender& sender, const DMBundle& args)
	{
		DMXmlNode ret15 = args.GetXmlNode(L"15");
		return true;
	}

	bool TaskRunnerTest::slotSyncEvent1(const DMEventSender& sender, const DMBundle& args)
	{
		DMXmlNode ret15 = args.GetXmlNode(L"15");
		return true;
	}


	bool TaskRunnerTest::slotSyncEvent2(const DMEventSender& sender, const DMBundle& args)
	{
		DMXmlNode ret15 = args.GetXmlNode(L"15");
		return true;
	}

	static bool TaskRunnerTest::s_slotUIEvent1(const DMEventSender& sender, const DMBundle& args)
	{
		DMXmlNode ret15 = args.GetXmlNode(L"15");
		return true;
	}

	bool TaskRunnerTest::slotUIEvent1(const DMEventSender& sender, const DMBundle& args)
	{
		DMXmlNode ret15 = args.GetXmlNode(L"15");
		return true;
	}

	bool TaskRunnerTest::slotUIEvent2(const DMEventSender& sender, const DMBundle& args)
	{
		DMXmlNode ret15 = args.GetXmlNode(L"15");
		return true;
	}

	static bool TaskRunnerTest::s_slotAyncEvent1(const DMEventSender& sender, const DMBundle& args)
	{
		DMXmlNode ret15 = args.GetXmlNode(L"15");
		return true;
	}

	bool TaskRunnerTest::slotAyncEvent1(const DMEventSender& sender, const DMBundle& args)
	{
		DMXmlNode ret15 = args.GetXmlNode(L"15");
		return true;
	}

	bool TaskRunnerTest::slotAyncEvent2(const DMEventSender& sender, const DMBundle& args)
	{
		DMXmlNode ret15 = args.GetXmlNode(L"15");
		return true;
	}

	void TaskRunnerTest::UiFun(int a, CStringA b, double c)
	{
		int cd; 
	}
};


TEST_F(TaskRunnerTest,测试)
{
	//3. 触发调用,可以在任意函数处调用------------------------------------------------------------------------------------ 
	FireBind();
}
