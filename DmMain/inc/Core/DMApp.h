//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMApp.h 
// File Des: �����
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
	/// ���׺궨��-------------------------------------------
	#define  g_pDMApp                                   DMApp::getSingletonPtr()

	/// �ⲿxmlDoc
	typedef DMXmlDocument* (*fun_cbGetSubXmlDoc)(LPCSTR /*pszType*/, LPCSTR /*pszName*/);

	typedef HBITMAP(*DM_CUSTOM_IMAGELOADER)(DWORD fourccID, LPCWSTR fileName);

	/// <summary>
	///		��Ϊ����Ҫ��Data�࣬һ��Ӧ�ó���Ӧ��ֻ��һ�������ȫ�ֶ���
	/// </summary>
	/// <remarks>
	///		ʵ�����̡߳���Դ�����ע��ȶ��塢�ӳټ���
	/// </remarks>
	class DM_EXPORT DMApp:public DMSingletonT<DMApp>
	{
	public:
		DMApp(HINSTANCE hInst = GetModuleHandle(NULL));
		~DMApp();
	public:
		/// -------------------------------------------------
		/// @brief			��ȡ�ڲ�Class����
		/// @param[in]		bShadow          �Ƿ�Ϊ��Ӱ��
		/// @return         ATOM
		ATOM   GetClassAtom(bool bShadow=false);

		/// -------------------------------------------------
		/// @brief			����ȫ�ֵ�skin��style��Ĭ�������
		/// @param[in]		lpszXmlId		 ����Դ�е�����
		/// @remark         Ӧ������Դ���غ��ٵ���
		/// @return			DMCode
		DMCode InitGlobal(LPCSTR lpszXmlId="global");

		void AddCustomImageLoader(DWORD fourccID, DM_CUSTOM_IMAGELOADER loaderFn);

		/// -------------------------------------------------
		/// @brief			������Ϣѭ��
		/// @param[in]		hMainWnd          ���ھ��
		/// @remark         �ڴ��ڴ�����������Ϣѭ��
		/// @return			DMCode
		DMCode Run(HWND hWnd);

		/// -------------------------------------------------
		/// @brief			�˴����Ƿ�Ϊ������Ϣѭ����������   
		/// @param			hWnd			  ���ھ��		
		/// @remark         ��Ϊ������Ϣѭ����������,���˳�ʱ��Ӧʹ��WM_QUIT�˳���Ϣѭ��
		/// @return			DMCode
		DMCode IsRun(HWND hWnd);

		//----------------------------------------------------------------------
		// Function Des: ע�������,Ϊ��֤��ȫ,���ڴ���������ǰ�����������ں�ʹ������!
		//-----------------------------------------------------------------------

		/// -------------------------------------------------
		/// @brief			ע��
		/// @param[in]		RegObj			  ע���࣬ʹ��DMRegHelperT<Obj>()��ʽ����
		/// @param[in]		bReplace		  true:���ڲ���ע��,ǿ���滻,false:���ڲ���ע��,����<see cref="DMREGMGR_REGISTERBYTYPE_REG_EXIST"/>
		/// @remark         ע�ắ��Ϊ�ؼ�����,Ϊ�ⲿ��չ�ࡢ����ĵ������
		/// @return			DMCode
		DMCode Register(const IDMReg &RegObj, bool bReplace=false);

		/// -------------------------------------------------
		/// @brief			��ע�����л�������ָ��(�ڲ�new)
		/// @param[out]		ppObj			  ע�������˫ָ��:��<see cref="IDMRes"/>��<see cref="IDMLog"/>����
		/// @param[in]		lpszClassName	  ע������
		/// @param[in]		RegType			  ע������<see cref="DMREGTYPE"/>
		/// @remark         ������������ڲ�����xmlָ���������󣬿��Ž�����gtest����,�������ڲ��������,�������ⲿ��ȡ
		/// @return			DMCode
		DMCode CreateRegObj(void** ppObj, LPCSTR lpszClassName,int RegType);

		/// -------------------------------------------------
		/// @brief			��ע��
		/// @param[in]		lpszClassName	  ע������
		/// @param[in]		RegType			  ע������<see cref="DMREGTYPE"/>
		/// @return			DMCode
		DMCode UnRegister(LPCSTR lpszClassName,int RegType);

		/// -------------------------------------------------
		/// @brief			�����ڲ���ֻ��ѡ��ʹ��һ�ֵ�ע���ָ࣬��һ�֣���render��log��atrribute��
		/// @param[in]		lpszClassName	  ע������
		/// @param[in]		RegType			  ע������<see cref="DMREGTYPE"/>
		/// @return			DMCode
		DMCode SetDefRegObj(LPCSTR lpszClassName,int RegType);

		/// -------------------------------------------------
		/// @brief			ȡ���ڲ���ֻ��ѡ��ʹ��һ�ֵ�ע���൱ǰʹ�ö���ָ��һ�֣���render��log��atrribute��
		/// @param[in]		ppObj			  �������
		/// @param[in]		RegType			  ע������<see cref="DMREGTYPE"/>
		/// @remark         �ڻ�ȡʱ�ڲ����������ü���,����ζ���ⲿ���ʹ������ָ��
		/// @return			DMCode
		DMCode GetDefRegObj(void** ppObj,int RegType);
		
		/// -------------------------------------------------
		/// @brief			ȡ��ע���൱ǰʹ�õ�Ĭ��������ָ��һ�֣���render��log��atrribute��
		/// @param[in]		RegType			  ע������<see cref="DMREGTYPE"/>
		/// @return			LPCWSTR
		CStringA GetDefRegObj(int RegType);

		//----------------------------------------------------------------------
		// Function Des: ������,Ϊ��֤��ȫ,���ڴ���������ǰ�����������ں�ʹ������!
		//----------------------------------------------------------------------
		/// -------------------------------------------------
		/// @brief			ͨ��cfg������plugin-dll�б�
		/// @param[in]		lpszPluginsCfg	  plugin-dll�б��cfg·��(�����exe·��)
		/// @return			DMCode
		DMCode LoadPlugins(LPCWSTR lpszPluginsCfg = L"dmplugins.cfg");

		/// -------------------------------------------------
		/// @brief			ֱ�Ӽ���plugin-dll
		/// @param[in]		lpszPluginName	  plugin-dll·��(�����exe·��)
		/// @return			DMCode
		DMCode LoadPlugin(LPCWSTR lpszPluginName);
		
		/// -------------------------------------------------
		/// @brief			��װplugin
		/// @param[in]		plugin			  �̳���<see cref="IDMPlugin"/>
		/// @return			DMCode
		DMCode InstallPlugin(IDMPlugin* plugin);

		/// -------------------------------------------------
		/// @brief			ж�ز��
		/// @param[in]		plugin			  �̳���<see cref="IDMPlugin"/>
		/// @return			DMCode
		DMCode UninstallPlugin(IDMPlugin* plugin);	

		/// -------------------------------------------------
		/// @brief			����ȫ������ĳ�ʼ��
		/// @param[in]		plugin			  �̳���<see cref="IDMPlugin"/>
		/// @return			DMCode
		DMCode InitialisePlugins();

		/// -------------------------------------------------
		/// @brief			����ȫ������ķ���ʼ��
		/// @param[in]		plugin			  �̳���<see cref="IDMPlugin"/>
		/// @return			DMCode
		DMCode ShutdownPlugins();

		//----------------------------------------------------------------------
		// Function Des: ������ԴRes
		//----------------------------------------------------------------------
		/// -------------------------------------------------
		/// @brief			������ԴRes
		/// @param[in]		wp				  �������
		/// @param[in]		lp				  �������
		/// @param[in]		lpszClassName	  ָ��Ĭ�ϵķ�ʽ����ΪNULL,��ʹ��ϵͳ���÷�ʽ��
		/// @remark         ��ͬ�Ľ�����ʽʹ�ò�ͬ�Ĵ������,����ο�demo
		/// @return			DMCode
		DMCode LoadResPack(WPARAM wp, LPARAM lp, LPCSTR lpszClassName);

		//----------------------------------------------------------------------
		// Function Des: �������
		//----------------------------------------------------------------------
		/// -------------------------------------------------
		/// @brief			����һ��skin�أ���skin���Ѵ���,������������
		/// @param[in]      XmlNode		      xml�ڵ�
		/// @return  DMCode
		DMCode AddSkinPoolItem(DMXmlNode &XmlNode);

		/// -------------------------------------------------
		/// @brief			�Ƴ�ָ����Skin����
		/// @param[in]		lpszName	      ��Ӧskin��name����
		/// @param[in]		DMCode
		DMCode RemoveSkinPoolItem(LPCSTR lpszName);

		/// -------------------------------------------------
		/// @brief			ɾ�����е�skin�أ�����ָ����key����
		/// @param[in]		lpszName		  ���ų���skin�ص�����
		/// @return			DMCode
		DMCode RemoveAllSkinPoolItemExcept(LPCSTR lpszName);

		/// -------------------------------------------------
		/// @brief  ����һ��style�أ���style���Ѵ���,������������
		/// @param[in]		XmlNode			  xml�ڵ�
		/// @return			DMCode
		DMCode AddStylePoolItem(DMXmlNode &XmlNode);

		/// -------------------------------------------------
		/// @brief			�Ƴ�ָ����Style����
		/// @param[in]		lpszName	      ��Ӧstyle��name����
		/// @param[in]		DMCode
		DMCode RemoveStylePoolItem(LPCSTR lpszName);

		/// -------------------------------------------------
		/// @brief			�Ƴ����е�style��
		/// @return			DMCode
		DMCode RemoveAllStylePoolItem();

		/// -------------------------------------------------
		/// @brief			��ȡָ������
		/// @param[in]		lpszFont	      �����Ӧ��str
		IDMFont* GetFont(LPCSTR lpszFont);

		/// -------------------------------------------------
		/// @brief			��ȡָ��Skin,������skin����
		/// @param[in]		lpszSkin	      Skin��Ӧ��str
		IDMSkin* GetSkin(LPCSTR lpszSkin);

		/// -------------------------------------------------
		/// @brief			��ָ��buf���õ�skin����
		/// @param[in]		pBuf			  ͼƬ�ڴ�
		/// @param[in]		bufLen			  ͼƬ�ڴ��С
		/// @param[in]		pszType			  ͼƬ���ͣ�һ��Ĭ��Ϊpng
		/// @param[in]		lpszXml			  ����skin��xml��xml�оͲ���Ҫ����src��
		/// @param[in]		lpszPoolName	  skin������
		DMCode AddSkin(void *pBuf,size_t bufLen, LPCSTR pszType,
			LPCSTR lpszXml, LPCSTR lpszPoolName=NULL);

		/// -------------------------------------------------
		/// @brief			�Ƴ�skin
		/// @param[in]		lpszKey			  ���ڲ���skin��key
		/// @param[in]		lpszName		  ���ڲ���skin�ص�name
		/// @remark			Ĭ������lpszName��skin�����Ƴ�������Ҳ���,����bLoopFindΪ�棬��ȫ�������ҵ����Ƴ�
		/// @return			DMCode,��skin�����Ƴ�skin,�������ü�����1
		DMCode RemoveSkin(LPCSTR lpszKey, LPCSTR lpszName,bool bLoopFind = true);

		/// -------------------------------------------------
		/// @brief			��ȡָ��style,������style����
		/// @param[in]		lpszSkin		  Skin��Ӧ��str
		/// @return			DMCode
		DMXmlNode GetStyle(LPCSTR lpszStyle);

		/// -------------------------------------------------
		/// @brief			����
		/// @param[in]		wp				  Ĭ��Ϊstr:name�б��ַ���
		/// @param[in]		lp				  �ַ�������
		/// @return			DMCode
		DMCode UpdateSkin(WPARAM wp, LPARAM lp);

		/// -------------------------------------------------
		/// @brief			ˢ������������
		/// @return			DMCode
		DMCode RedrawAll();

		/// -------------------------------------------------
		/// @brief			����
		/// @param[in]		pSkin	         ���ҵ�skin
		/// @param[in]		type	         ����
		/// @return			bool
		bool IsNeedUpdateSkin(IDMSkinPtr pSkin,int type=DMREG_Skin);

		/// -------------------------------------------------
		/// @brief			HSLA�仯������ȫ�ֵ�HSLA(����������)
		/// @param[in]		ivalue			 HSLA��ֵ
		/// @param[in]		iType			 0��1��2��3�ֱ�ָHSLA����Ӧ����
		/// @return			DMCode
		DMCode AdjustHSLA(int ivalue,int iType);

		/// -------------------------------------------------
		/// @brief			���ӿ��д�����Ϣ
		/// @param[in]		pIdleHandler	 ����<see cref="IDMIdleHandler"/>
		/// @return			DMCode
		DMCode AddIdleHandler(IDMIdleHandler* pIdleHandler);

		/// -------------------------------------------------
		/// @brief			�Ƴ����д�����Ϣ
		/// @param[in]		pIdleHandler	 ����<see cref="IDMIdleHandler"/>
		/// @return			DMCode
		DMCode RemoveIdleHandler(IDMIdleHandler* pIdleHandler);

		/// -------------------------------------------------
		/// @brief			����Ԥ������Ϣ
		/// @param[in]		pMessageFilter	 ����<see cref="IDMMessageFilter"/>
		/// @return			DMCode
		DMCode AddMessageFilter(IDMMessageFilter* pMessageFilter);

		/// -------------------------------------------------
		/// @brief			�Ƴ�Ԥ������Ϣ
		/// @param[in]		pMessageFilter	 ����<see cref="IDMMessageFilter"/>
		/// @return			DMCode
		DMCode RemoveMessageFilter(IDMMessageFilter* pMessageFilter);

		/// -------------------------------------------------
		/// @brief			�����߳���Ϣѭ��
		/// @param[in]		pMessageFilter	 ����<see cref="IDMMessageFilter"/>
		/// @return			DMCode
		bool AddMessageLoop(DMMsgLoop* pMsgLoop);

		/// -------------------------------------------------
		/// @brief			��ʼ��DMXmlDocument
		/// @param[inout]	XmlDoc			 ��Ҫ��ʼ����xml����
		/// @param[in]		lpszType		 ����
		/// @param[in]		lpszResName		 ����
		/// @return			DMCode
		DMCode InitDMXmlDocument(DMXmlDocument &XmlDoc, LPCSTR lpszType,LPCSTR lpszResName);	

		/// -------------------------------------------------
		/// @brief			 �ҵ�DUI����
		/// @param[in]		 hDUIWnd		  DUI���ڵ�Ψһ���
		/// @return DUIWindowPtr��ʧ��ΪNULL
		DUIWindowPtr FindDUIWnd(DUIWND hDUIWnd);

		/// -------------------------------------------------
		/// @brief			 �����ⲿָ��sub xml�Ļص�����,�ⲿ����xml���
		/// @return			 DMCode
		DMCode SetSubXmlDocCallBack(fun_cbGetSubXmlDoc);

		// TODO: unify LoadXXX(WPARAM wp, LPARAM lp) LPCSTR or LPCWSTR
		/// -------------------------------------------------
		/// @brief ���ط������չ�ӿ�
		/// @param[in]		 wp					�����ڲ�ʹ�õĲ���1���緭��XML·���ȣ����Լ���չʵ����أ�����Ϊxml��ld��xmlȫ·��
		/// @param[in]		 lp					�����ڲ�ʹ�õĲ���2���緭��XML�ȣ����Լ���չʵ�����,����0��ʾxml id��ʽ,1��ʾxmlȫ·����ʽ
		/// @return DM_ECODE_OKΪ�ɹ�
		DMCode LoadTrans(WPARAM wp, LPARAM lp = 0);

		/// -------------------------------------------------
		/// @brief ����Ĭ�����԰�
		// @param[in]		 strLanguageName	��Ӧ�����԰�Ψһ��ʶ�������NULL,��ʹ�÷��� 
		/// @return DM_ECODE_OKΪ�ɹ�
		DMCode SetTrans(CStringW strLanguageName);

		/// -------------------------------------------------
		/// @brief �¼ӷ����ַ���
		/// @param[in]		 strSrc				XML�����õ��ַ���
		/// @param[in]		 strTrans			��������ַ���
		/// @param[in]		 strNodeName		�Ƿ�ָ��Ϊ�ض�������,ΪNULL��ʾ��ͨ�õ������в���
		/// @param[in]		 strLanguageName	��Ӧ�����԰�Ψһ��ʶ,ΪNULLʹ�õ�ǰSetTransָ�������԰�
		/// @return DM_ECODE_OKΪ�ɹ�
		DMCode AddTransItem(CStringW strSrc, CStringW strTrans,CStringW strNodeName = L"",CStringW strLanguageName = L"");

		/// -------------------------------------------------
		/// @brief ��÷����ַ���
		/// @param[in]		 strSrc				XML�����õ��ַ���
		/// @param[in]		 strNodeName		�Ƿ�ָ��Ϊ�ض�������
		/// @param[in]		 strLanguageName	��Ӧ�����԰�Ψһ��ʶ,ΪNULLʹ�õ�ǰSetTransָ�������԰�
		/// @return DM_ECODE_OKΪ�ɹ�
		CStringW GetTrans(CStringW strSrc,CStringW strNodeName = L"",CStringW strLanguageName = L"");
	};

}//namespace DM