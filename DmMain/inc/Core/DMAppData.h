//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMAppData.h 
// File Des: 全局数据
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-14	1.0		
//-------------------------------------------------------
#pragma once
#include "DMRegMgr.h"
#include "DMDispatch.h"
#include "DMPluginsTool.h"
#include "DMPluginImpl.h"
#include "DUIWndPool.h"
#include "DUIFontPool.h"
#include "DUISkinPool.h"
#include "DUIStylePool.h"
#include "DMMsgLoop.h"
#include "DUITimer.h"
#include "DMSpyTool.h"

namespace DMAttr
{
	/// <summary>
	///		全局的xml属性定义
	/// </summary>
	/// <remarks>
	///		全局的默认字体、皮肤、属性定义
	/// </remarks>
	class DMGlobalAttr
	{
	public:
		static wchar_t* XMLNODE_global;                              ///< 全局xml指定的默认NODE名，示例:..global font="face:仿宋,size:14,weight:100"..(..代表尖括号)
		static wchar_t* XMLATTR_font;                                ///< 全局xml的默认字体属性，如果不设置，内置默认字体为宋体，字高14，其余属性和DEFAULT_GUI_FONT一致，示例:font="face:宋体,size:15"
		static wchar_t* XMLNODE_skin;                                ///< 全局xml指定全局皮肤NODE名，支持多份,示例:..skin..(..代表尖括号)
		static wchar_t* XMLNODE_style;                               ///< 全局xml指定全局属性NODE名，支持多份,示例:..style..(..代表尖括号)
	};
	DMAttrValueInit(DMGlobalAttr,XMLNODE_global)DMAttrValueInit(DMGlobalAttr,XMLATTR_font)DMAttrValueInit(DMGlobalAttr,XMLNODE_skin)DMAttrValueInit(DMGlobalAttr,XMLNODE_style)
}

namespace DM
{
	// 简易宏定义,仅供DMMain内部使用，外部请使用GetDefRegObj获取!!
	#define  g_pDMAppData                                DMAppData::getSingletonPtr()
	#define  g_pDMPluginTool                             g_pDMAppData->m_PluginsTool
	#define  g_pDMSpyTool                                g_pDMAppData->m_SpyTool
	#define  g_pDMMsgLoopTool                            g_pDMAppData->m_MsgLoopTool
	#define  g_pDMDWndPool                               g_pDMAppData->m_DUIWndPool
	#define  g_pDMSkinPool                               g_pDMAppData->m_SkinPool
	#define  g_pDMFontPool                               g_pDMAppData->m_FontPool
	#define  g_pDMStylePool                              g_pDMAppData->m_StylePool
	#define  g_pDMRender                                 g_pDMAppData->m_pRenderObj
	#define  g_pDMThreadActiveWndTool                    g_pDMAppData->m_ThreadActiveWndTool
	#define  g_pDMTrans                                  g_pDMAppData->m_pTransObj
	

	/// <summary>
	///		封装管理所有的数据,建议仅供内部使用,如某功能需开放接口,请增加到DMApp中转发
	/// </summary>
	class DMAppData:public DMSingletonT<DMAppData>
	{
	public:
		DMAppData(HINSTANCE hInst = GetModuleHandle(NULL));
		virtual~DMAppData();

		DMCode InitGlobal(LPCWSTR lpszXmlId);
		DMCode Run(HWND hMainWnd,bool bEnableActive);
		DMCode IsRun(HWND hWnd);
		//---------------------------------------------------
		// Function Des: 注册类
		//---------------------------------------------------
		DMCode Register(IDMReg &RegObj, bool bReplace=false);
		DMCode CreateRegObj(void** ppObj, LPCWSTR lpszClassName,int RegType);
		DMCode UnRegister(LPCWSTR lpszClassName,int RegType);
		DMCode SetDefRegObj(LPCWSTR lpszClassName,int RegType); 
		DMCode GetDefRegObj(void** ppObj,int RegType);
		DMCode GetDefRegObj(CStringW &szName,int RegType);

		//---------------------------------------------------
		// Function Des: 注册插件
		//---------------------------------------------------
		DMCode LoadPlugins(LPCWSTR lpszPluginsCfg);
		DMCode LoadPlugin(LPCWSTR lpszPluginName);
		DMCode InstallPlugin(IDMPlugin* plugin);
		DMCode UninstallPlugin(IDMPlugin* plugin);
		DMCode InitialisePlugins();
		DMCode ShutdownPlugins();

		//---------------------------------------------------
		// Function Des: 解析资源Res
		//---------------------------------------------------
		DMCode LoadResPack(WPARAM wp, LPARAM lp,LPCWSTR lpszClassName);

		//---------------------------------------------------
		// Function Des: 内部辅助
		//---------------------------------------------------
		DMCode InitDMXmlDocument(DMXmlDocument &XmlDoc, LPCWSTR lpszType,LPCWSTR lpszResName);	
		bool   IsNeedUpdateSkin(IDMSkinPtr pSkin,int type=DMREG_Skin);  ///< 当前skin指针是否需要更新
		DMCode ClearUpdateSkinArray();									///< 清空需更新的skin列表 

	protected:
		ATOM RegisterClassEx(HINSTANCE hInst, LPCWSTR lpClassName, bool bShadow=false);

	public:
		HINSTANCE					m_hInst;									///< 程序句柄
		ATOM						m_Atom;										///< 普通窗口类atom对象
		ATOM						m_shadowAtom;                               ///< Shadow窗口类atom对象

	public:// 消息处理
		DMLazyT<DMMsgLoopTool>      m_MsgLoopTool;                              ///< 消息循环

	public:
		DMLazyT<DMRegMgr>			m_RegMgr;									///< 注册类管理
		DMLazyT<DMPluginsTool>		m_PluginsTool;								///< 注册Plugin与Plugin-dll管理
		DMLazyT<DMSpyTool>          m_SpyTool;                                  ///< 辅助Spy工具
		
	public:// DUI窗口辅助
		DMLazyT<DUIWndPool>			m_DUIWndPool;                               ///< DUIWindow-句柄的映射关系管理池
		DMLazyT<DUIFontPool>        m_FontPool;									///< 字体池                  
		DMLazyT<DUISkinPool>		m_SkinPool;									///< Skin池
		DMLazyT<DUIStylePool>       m_StylePool;								///< Style池

		DMLazyT<DUITimer>           m_Timer;									///< 定时器  

	public: // 唯一性对象
		IDMPlugin                   *m_pPlugin;                                 ///< 内置插件对象
		DMSmartPtrT<IDMAttribute>   m_pAttributeObj;							///< 当前被使用的Attribute对象
		DMSmartPtrT<IDMLog>         m_pLogObj;                                  ///< 当前被使用的Log对象
		DMSmartPtrT<IDMRes>         m_pResObj;                                  ///< 当前被用来解析的Res对象
		DMSmartPtrT<IDMRender>      m_pRenderObj;                               ///< 当前被用来解析的Render对象
		DMSmartPtrT<IDMTrans>       m_pTransObj;                                ///< 当前被用来管理语言包的Trans对象

	public:// 全局初始化
		fun_UpdateLayeredWindowIndirect m_fun_UpdateLayeredWindowIndirect;

	public:
		DM::CArray<HWND>            m_RunhWndArray;                             ///< 运行Run函数的hWnd队列


	};
}//namespace DM