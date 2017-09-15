//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMApp.h 
// File Des: 主入口
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
	/// 简易宏定义-------------------------------------------
	#define  g_pDMApp                                   DMApp::getSingletonPtr()

	/// <summary>
	///		此为最重要主Data类，一个应用程序应该只有一个此类的全局对象
	/// </summary>
	/// <remarks>
	///		实现了线程、资源、插件注册等定义、延迟加载
	/// </remarks>
	class DM_EXPORT DMApp:public DMSingletonT<DMApp>
	{
	public:
		DMApp(HINSTANCE hInst = GetModuleHandle(NULL));
		~DMApp();
	public:
		/// -------------------------------------------------
		/// @brief			获取内部Class对象
		/// @param[in]		bShadow          是否为阴影类
		/// @return         ATOM
		ATOM   GetClassAtom(bool bShadow=false);

		/// -------------------------------------------------
		/// @brief			加载全局的skin、style、默认字体等
		/// @param[in]		lpszXmlId		 在资源中的名字
		/// @remark         应该在资源加载后再调用
		/// @return			DMCode
		DMCode InitGlobal(LPCWSTR lpszXmlId=L"global");

		/// -------------------------------------------------
		/// @brief			运行消息循环
		/// @param[in]		hMainWnd          窗口句柄
		/// @param[in]		bEnableActive     是否强制enalbe窗口(用于DoModal)
		/// @remark         在窗口创建后，运行消息循环
		/// @return			DMCode
		DMCode Run(HWND hMainWnd,bool bEnableActive=false);

		/// -------------------------------------------------
		/// @brief			此窗口是否为运行消息循环的主窗口   
		/// @param			hWnd			  窗口句柄		
		/// @remark         如为运行消息循环的主窗口,在退出时，应使用WM_QUIT退出消息循环
		/// @return			DMCode
		DMCode IsRun(HWND hWnd);

		//----------------------------------------------------------------------
		// Function Des: 注册类相关,为保证安全,请在创建主窗口前或销毁主窗口后使用它们!
		//-----------------------------------------------------------------------

		/// -------------------------------------------------
		/// @brief			注册
		/// @param[in]		RegObj			  注册类，使用DMRegHelperT<Obj>()方式传入
		/// @param[in]		bReplace		  true:如内部已注册,强制替换,false:如内部已注册,返回<see cref="DMREGMGR_REGISTERBYTYPE_REG_EXIST"/>
		/// @remark         注册函数为关键函数,为外部扩展类、插件的调用入口
		/// @return			DMCode
		DMCode Register(IDMReg &RegObj, bool bReplace=false);

		/// -------------------------------------------------
		/// @brief			从注册类中获得类对象指针(内部new)
		/// @param[out]		ppObj			  注册类基类双指针:如<see cref="IDMRes"/>、<see cref="IDMLog"/>、等
		/// @param[in]		lpszClassName	  注册类名
		/// @param[in]		RegType			  注册类型<see cref="DMREGTYPE"/>
		/// @remark         这个函数用于内部根据xml指定创建对象，开放仅方便gtest测试,由于是内部分配对象,不建议外部获取
		/// @return			DMCode
		DMCode CreateRegObj(void** ppObj, LPCWSTR lpszClassName,int RegType);

		/// -------------------------------------------------
		/// @brief			反注册
		/// @param[in]		lpszClassName	  注册类名
		/// @param[in]		RegType			  注册类型<see cref="DMREGTYPE"/>
		/// @return			DMCode
		DMCode UnRegister(LPCWSTR lpszClassName,int RegType);

		/// -------------------------------------------------
		/// @brief			对于内部仅只能选择使用一种的注册类，指定一种（如render、log、atrribute）
		/// @param[in]		lpszClassName	  注册类名
		/// @param[in]		RegType			  注册类型<see cref="DMREGTYPE"/>
		/// @return			DMCode
		DMCode SetDefRegObj(LPCWSTR lpszClassName,int RegType);

		/// -------------------------------------------------
		/// @brief			取得内部仅只能选择使用一种的注册类当前使用对象，指定一种（如render、log、atrribute）
		/// @param[in]		ppObj			  传入参数
		/// @param[in]		RegType			  注册类型<see cref="DMREGTYPE"/>
		/// @remark         在获取时内部会增加引用计数,这意味着外部最好使用智能指针
		/// @return			DMCode
		DMCode GetDefRegObj(void** ppObj,int RegType);
		
		/// -------------------------------------------------
		/// @brief			取得注册类当前使用的默认类名，指定一种（如render、log、atrribute）
		/// @param[in]		RegType			  注册类型<see cref="DMREGTYPE"/>
		/// @return			LPCWSTR
		LPCWSTR GetDefRegObj(int RegType);

		//----------------------------------------------------------------------
		// Function Des: 插件相关,为保证安全,请在创建主窗口前或销毁主窗口后使用它们!
		//----------------------------------------------------------------------
		/// -------------------------------------------------
		/// @brief			通过cfg来加载plugin-dll列表
		/// @param[in]		lpszPluginsCfg	  plugin-dll列表的cfg路径(相对于exe路径)
		/// @return			DMCode
		DMCode LoadPlugins(LPCWSTR lpszPluginsCfg = L"dmplugins.cfg");

		/// -------------------------------------------------
		/// @brief			直接加载plugin-dll
		/// @param[in]		lpszPluginName	  plugin-dll路径(相对于exe路径)
		/// @return			DMCode
		DMCode LoadPlugin(LPCWSTR lpszPluginName);
		
		/// -------------------------------------------------
		/// @brief			安装plugin
		/// @param[in]		plugin			  继承于<see cref="IDMPlugin"/>
		/// @return			DMCode
		DMCode InstallPlugin(IDMPlugin* plugin);

		/// -------------------------------------------------
		/// @brief			卸载插件
		/// @param[in]		plugin			  继承于<see cref="IDMPlugin"/>
		/// @return			DMCode
		DMCode UninstallPlugin(IDMPlugin* plugin);	

		/// -------------------------------------------------
		/// @brief			调用全部插件的初始化
		/// @param[in]		plugin			  继承于<see cref="IDMPlugin"/>
		/// @return			DMCode
		DMCode InitialisePlugins();

		/// -------------------------------------------------
		/// @brief			调用全部插件的反初始化
		/// @param[in]		plugin			  继承于<see cref="IDMPlugin"/>
		/// @return			DMCode
		DMCode ShutdownPlugins();

		//----------------------------------------------------------------------
		// Function Des: 解析资源Res
		//----------------------------------------------------------------------
		/// -------------------------------------------------
		/// @brief			解析资源Res
		/// @param[in]		wp				  传入参数
		/// @param[in]		lp				  传入参数
		/// @param[in]		lpszClassName	  指定默认的方式（如为NULL,则使用系统内置方式）
		/// @remark         不同的解析方式使用不同的传入参数,具体参看demo
		/// @return			DMCode
		DMCode LoadResPack(WPARAM wp, LPARAM lp,LPCWSTR lpszClassName);

		//----------------------------------------------------------------------
		// Function Des: 属性相关
		//----------------------------------------------------------------------
		/// -------------------------------------------------
		/// @brief			加入一个skin池，如skin池已存在,则加入解析的项
		/// @param[in]      XmlNode		      xml节点
		/// @return  DMCode
		DMCode AddSkinPoolItem(DMXmlNode &XmlNode);

		/// -------------------------------------------------
		/// @brief			移除指定的Skin队列
		/// @param[in]		lpszName	      对应skin的name属性
		/// @param[in]		DMCode
		DMCode RemoveSkinPoolItem(LPCWSTR lpszName);

		/// -------------------------------------------------
		/// @brief			删除所有的skin池，除了指定的key以外
		/// @param[in]		lpszName		  被排除的skin池的名称
		/// @return			DMCode
		DMCode RemoveAllSkinPoolItemExcept(LPCWSTR lpszName);

		/// -------------------------------------------------
		/// @brief  加入一个style池，如style池已存在,则加入解析的项
		/// @param[in]		XmlNode			  xml节点
		/// @return			DMCode
		DMCode AddStylePoolItem(DMXmlNode &XmlNode);

		/// -------------------------------------------------
		/// @brief			移除指定的Style队列
		/// @param[in]		lpszName	      对应style的name属性
		/// @param[in]		DMCode
		DMCode RemoveStylePoolItem(LPCWSTR lpszName);

		/// -------------------------------------------------
		/// @brief			移除所有的style池
		/// @return			DMCode
		DMCode RemoveAllStylePoolItem();

		/// -------------------------------------------------
		/// @brief			获取指定字体
		/// @param[in]		lpszFont	      字体对应的str
		IDMFont* GetFont(LPCWSTR lpszFont);

		/// -------------------------------------------------
		/// @brief			获取指定Skin,从所有skin池中
		/// @param[in]		lpszSkin	      Skin对应的str
		IDMSkin* GetSkin(LPCWSTR lpszSkin);

		/// -------------------------------------------------
		/// @brief			把指定buf设置到skin池中
		/// @param[in]		pBuf			  图片内存
		/// @param[in]		bufLen			  图片内存大小
		/// @param[in]		pszType			  图片类型，一般默认为png
		/// @param[in]		lpszXml			  描述skin的xml，xml中就不需要定义src了
		/// @param[in]		lpszPoolName	  skin池名称
		DMCode AddSkin(void *pBuf,size_t bufLen,LPCWSTR pszType,
			LPCWSTR lpszXml,LPCWSTR lpszPoolName=NULL);

		/// -------------------------------------------------
		/// @brief			移除skin
		/// @param[in]		lpszKey			  用于查找skin的key
		/// @param[in]		lpszName		  用于查找skin池的name
		/// @remark			默认先在lpszName的skin池中移除，如果找不到,而且bLoopFind为真，就全部遍历找到并移除
		/// @return			DMCode,从skin池中移除skin,并将引用计数减1
		DMCode RemoveSkin(LPCWSTR lpszKey,LPCWSTR lpszName,bool bLoopFind = true);

		/// -------------------------------------------------
		/// @brief			获取指定style,从所有style池中
		/// @param[in]		lpszSkin		  Skin对应的str
		/// @return			DMCode
		IDMStyle* GetStyle(LPCWSTR lpszStyle);

		/// -------------------------------------------------
		/// @brief			换肤
		/// @param[in]		wp				  默认为str:name列表字符串
		/// @param[in]		lp				  字符串长度
		/// @return			DMCode
		DMCode UpdateSkin(WPARAM wp, LPARAM lp);

		/// -------------------------------------------------
		/// @brief			刷新所有主窗口
		/// @return			DMCode
		DMCode RedrawAll();

		/// -------------------------------------------------
		/// @brief			换肤
		/// @param[in]		pSkin	         查找的skin
		/// @param[in]		type	         类型
		/// @return			bool
		bool IsNeedUpdateSkin(IDMSkinPtr pSkin,int type=DMREG_Skin);

		/// -------------------------------------------------
		/// @brief			HSLA变化，调整全局的HSLA(所有主窗口)
		/// @param[in]		ivalue			 HSLA的值
		/// @param[in]		iType			 0，1，2，3分别指HSLA的相应调整
		/// @return			DMCode
		DMCode AdjustHSLA(int ivalue,int iType);

		/// -------------------------------------------------
		/// @brief			增加空闲处理消息
		/// @param[in]		pIdleHandler	 基于<see cref="IDMIdleHandler"/>
		/// @return			DMCode
		DMCode AddIdleHandler(IDMIdleHandler* pIdleHandler);

		/// -------------------------------------------------
		/// @brief			移除空闲处理消息
		/// @param[in]		pIdleHandler	 基于<see cref="IDMIdleHandler"/>
		/// @return			DMCode
		DMCode RemoveIdleHandler(IDMIdleHandler* pIdleHandler);

		/// -------------------------------------------------
		/// @brief			增加预处理消息
		/// @param[in]		pMessageFilter	 基于<see cref="IDMMessageFilter"/>
		/// @return			DMCode
		DMCode AddMessageFilter(IDMMessageFilter* pMessageFilter);

		/// -------------------------------------------------
		/// @brief			移除预处理消息
		/// @param[in]		pMessageFilter	 基于<see cref="IDMMessageFilter"/>
		/// @return			DMCode
		DMCode RemoveMessageFilter(IDMMessageFilter* pMessageFilter);

		/// -------------------------------------------------
		/// @brief			增加线程消息循环
		/// @param[in]		pMessageFilter	 基于<see cref="IDMMessageFilter"/>
		/// @return			DMCode
		bool AddMessageLoop(DMMsgLoop* pMsgLoop);

		/// -------------------------------------------------
		/// @brief			初始化DMXmlDocument
		/// @param[inout]	XmlDoc			 需要初始化的xml对象
		/// @param[in]		lpszType		 类型
		/// @param[in]		lpszResName		 名称
		/// @return			DMCode
		DMCode InitDMXmlDocument(DMXmlDocument &XmlDoc, LPCWSTR lpszType,LPCWSTR lpszResName);	

		/// -------------------------------------------------
		/// @brief			 找到DUI窗口
		/// @param[in]		 hDUIWnd		  DUI窗口的唯一句柄
		/// @return DUIWindowPtr，失败为NULL
		DUIWindowPtr FindDUIWnd(DUIWND hDUIWnd);

		/// -------------------------------------------------
		/// @brief			 允许外部指定sub xml的回调函数,外部传入xml句柄
		/// @return			 DMCode
		DMCode SetSubXmlDocCallBack(DMXmlDocument* (*fun_cbGetSubXmlDoc)(LPCWSTR,LPCWSTR));

		/// -------------------------------------------------
		/// @brief 加载翻译包扩展接口
		/// @param[in]		 wp					传入内部使用的参数1，如翻译XML路径等，和自己扩展实现相关，内置为xml的ld或xml全路径
		/// @param[in]		 lp					传入内部使用的参数2，如翻译XML等，和自己扩展实现相关,内置0表示xml id方式,1表示xml全路径方式
		/// @return DM_ECODE_OK为成功
		DMCode LoadTrans(WPARAM wp, LPARAM lp = 0);

		/// -------------------------------------------------
		/// @brief 设置默认语言包
		// @param[in]		 strLanguageName	对应的语言包唯一标识，如果传NULL,则不使用翻译 
		/// @return DM_ECODE_OK为成功
		DMCode SetTrans(CStringW strLanguageName);

		/// -------------------------------------------------
		/// @brief 新加翻译字符串
		/// @param[in]		 strSrc				XML中配置的字符串
		/// @param[in]		 strTrans			被翻译的字符串
		/// @param[in]		 strNodeName		是否指定为特定的配置,为NULL表示从通用的配置中查找
		/// @param[in]		 strLanguageName	对应的语言包唯一标识,为NULL使用当前SetTrans指定的语言包
		/// @return DM_ECODE_OK为成功
		DMCode AddTransItem(CStringW strSrc, CStringW strTrans,CStringW strNodeName = L"",CStringW strLanguageName = L"");

		/// -------------------------------------------------
		/// @brief 获得翻译字符串
		/// @param[in]		 strSrc				XML中配置的字符串
		/// @param[in]		 strNodeName		是否指定为特定的配置
		/// @param[in]		 strLanguageName	对应的语言包唯一标识,为NULL使用当前SetTrans指定的语言包
		/// @return DM_ECODE_OK为成功
		CStringW GetTrans(CStringW strSrc,CStringW strNodeName = L"",CStringW strLanguageName = L"");
	};

}//namespace DM