//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMAppData.h 
// File Des: ȫ������
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-14	1.0		
//-------------------------------------------------------
#pragma once
#include "Core/DMRegMgr.h"
#include "Core/DMDispatch.h"
#include "Common/Plugins/DMPluginsTool.h"
#include "Modules/DMPluginImpl.h"
#include "Core/Dui/DUIWndPool.h"
#include "Core/Dui/DUIFontPool.h"
#include "Core/Dui/DUISkinPool.h"
#include "Core/Dui/DUIStylePool.h"
#include "Core/Msg/DMMsgLoop.h"
#include "Core/Dui/DUITimer.h"
#include "Common/DMSpyTool.h"

namespace DMAttr
{
	/// <summary>
	///		ȫ�ֵ�xml���Զ���
	/// </summary>
	/// <remarks>
	///		ȫ�ֵ�Ĭ�����塢Ƥ�������Զ���
	/// </remarks>
	class DMGlobalAttr
	{
	public:
		static const char* XMLNODE_global;                              ///< ȫ��xmlָ����Ĭ��NODE����ʾ��:..global font="face:����,size:14,weight:100"..(..���������)
		static const char* XMLATTR_font;                                ///< ȫ��xml��Ĭ���������ԣ���������ã�����Ĭ������Ϊ���壬�ָ�14���������Ժ�DEFAULT_GUI_FONTһ�£�ʾ��:font="face:����,size:15"
		static const char* XMLNODE_skin;                                ///< ȫ��xmlָ��ȫ��Ƥ��NODE����֧�ֶ��,ʾ��:..skin..(..���������)
		static const char* XMLNODE_style;                               ///< ȫ��xmlָ��ȫ������NODE����֧�ֶ��,ʾ��:..style..(..���������)
	};
	DMAttrValueInit(DMGlobalAttr,XMLNODE_global)DMAttrValueInit(DMGlobalAttr,XMLATTR_font)DMAttrValueInit(DMGlobalAttr,XMLNODE_skin)DMAttrValueInit(DMGlobalAttr,XMLNODE_style)
}

namespace DM
{
	// ���׺궨��,����DMMain�ڲ�ʹ�ã��ⲿ��ʹ��GetDefRegObj��ȡ!!
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
	#define  g_pDMTaskRunner                             g_pDMAppData->m_pTaskRunnerObj
	
	/// <summary>
	///		��װ�������е�����,��������ڲ�ʹ��,��ĳ�����迪�Žӿ�,�����ӵ�DMApp��ת��
	/// </summary>
	class DMAppData:public DMSingletonT<DMAppData>
	{
	public:
		DMAppData(HINSTANCE hInst = GetModuleHandle(NULL));
		virtual~DMAppData();

		DMCode InitGlobal(LPCSTR lpszXmlId);
		DMCode Run(HWND hWnd);
		DMCode IsRun(HWND hWnd);
		//---------------------------------------------------
		// Function Des: ע����
		//---------------------------------------------------
		DMCode Register(const IDMReg &RegObj, bool bReplace=false);
		DMCode CreateRegObj(void** ppObj, LPCSTR lpszClassName,int RegType);
		DMCode UnRegister(LPCSTR lpszClassName,int RegType);
		DMCode SetDefRegObj(LPCSTR lpszClassName,int RegType);
		DMCode GetDefRegObj(void** ppObj,int RegType);
		DMCode GetDefRegObj(CStringA &szName,int RegType);

		//---------------------------------------------------
		// Function Des: ����Զ���ͼƬ����
		//---------------------------------------------------
		void AddCustomImageLoader(DWORD fourccID, DM_CUSTOM_IMAGELOADER loaderFn);
		DM_CUSTOM_IMAGELOADER GetCustomImageLoader(DWORD fourccID);

		//---------------------------------------------------
		// Function Des: ע����
		//---------------------------------------------------
		DMCode LoadPlugins(LPCWSTR lpszPluginsCfg);
		DMCode LoadPlugin(LPCWSTR lpszPluginName);
		DMCode InstallPlugin(IDMPlugin* plugin);
		DMCode UninstallPlugin(IDMPlugin* plugin);
		DMCode InitialisePlugins();
		DMCode ShutdownPlugins();

		//---------------------------------------------------
		// Function Des: ������ԴRes
		//---------------------------------------------------
		DMCode LoadResPack(WPARAM wp, LPARAM lp, LPCSTR lpszClassName);

		//---------------------------------------------------
		// Function Des: �ڲ�����
		//---------------------------------------------------
		DMCode InitDMXmlDocument(DMXmlDocument &XmlDoc, LPCSTR lpszType,LPCSTR lpszResName);	
		bool   IsNeedUpdateSkin(IDMSkinPtr pSkin,int type=DMREG_Skin);  ///< ��ǰskinָ���Ƿ���Ҫ����
		DMCode ClearUpdateSkinArray();									///< �������µ�skin�б� 

	protected:
		ATOM RegisterClassEx(HINSTANCE hInst, LPCWSTR lpClassName, bool bShadow=false);

	public:
		HINSTANCE					m_hInst;									///< ������
		ATOM						m_Atom;										///< ��ͨ������atom����
		ATOM						m_shadowAtom;                               ///< Shadow������atom����

	public:// ��Ϣ����
		DMLazyT<DMMsgLoopTool>      m_MsgLoopTool;                              ///< ��Ϣѭ��

	public:
		DMLazyT<DMRegMgr>			m_RegMgr;									///< ע�������
		DMLazyT<DMPluginsTool>		m_PluginsTool;								///< ע��Plugin��Plugin-dll����
#if !defined(DM_EXCLUDE_SPY)
		DMLazyT<DMSpyTool>          m_SpyTool;                                  ///< ����Spy����
#endif
		
	public:// DUI���ڸ���
		DMLazyT<DUIWndPool>			m_DUIWndPool;                               ///< DUIWindow-�����ӳ���ϵ�����
		DMLazyT<DUIFontPool>        m_FontPool;									///< �����                  
		DMLazyT<DUISkinPool>		m_SkinPool;									///< Skin��
		DMLazyT<DUIStylePool>       m_StylePool;								///< Style��

		DMLazyT<DUITimer>           m_Timer;									///< ��ʱ��  

	public: // Ψһ�Զ���
		IDMPlugin                   *m_pPlugin;                                 ///< ���ò������
		DMSmartPtrT<IDMAttribute>   m_pAttributeObj;							///< ��ǰ��ʹ�õ�Attribute����
		DMSmartPtrT<IDMLog>         m_pLogObj;                                  ///< ��ǰ��ʹ�õ�Log����
		DMSmartPtrT<IDMRes>         m_pResObj;                                  ///< ��ǰ������������Res����
		DMSmartPtrT<IDMRender>      m_pRenderObj;                               ///< ��ǰ������������Render����
		DMSmartPtrT<IDMTrans>       m_pTransObj;                                ///< ��ǰ�������������԰���Trans����
		DMSmartPtrT<IDMTaskRunner>  m_pTaskRunnerObj;                           ///< ��ǰ�����������¼��󶨵�Event����
  
	public:// ȫ�ֳ�ʼ��
		fun_UpdateLayeredWindowIndirect m_fun_UpdateLayeredWindowIndirect;

		/// <summary>
		///  DWM windows vista only
		/// </summary>
		fun_DwmIsCompositionEnabled m_fun_DwmIsCompositionEnabled;
		fun_DwmSetWindowAttribute m_fun_DwmSetWindowAttribute;
		fun_DwmExtendFrameIntoClientArea m_fun_DwmExtendFrameIntoClientArea;

		HMODULE m_hModuleDWM;

		float m_dpiScale;

	public:
		DM::CArray<HWND>            m_RunhWndArray;                             ///< ����Run������hWnd����

		DMMapT<FOURCC, DM_CUSTOM_IMAGELOADER> m_customImageLoaders;
	};
}//namespace DM