//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIWindow.h 
// File Des: ��WPF�� Visual, ����Ϊģ���Ӵ��ڵ�HWND
// File Summary: http://msdn.microsoft.com/zh-cn/library/ms748373(v=vs.110).aspx
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-30	1.0			
//--------------------------------------------------------
#pragma once
#include "DUIWindowHelper.h"

namespace DM
{
	#define DMTR(str)									DV_GetTransText(str)          
	class DUIWindow;
	typedef DUIWindow* DUIWindowPtr;
	/// <summary>
	///		DUI���ڵĻ��࣬����DUI��ͨ������̳У���DV_��ͷ��Ϊ�麯�������������,���ԣ�<see cref="DMAttr::DUIWindowAttr"/>
	/// </summary>
	/// <remarks>
	///		1.DV_��ͷ��ʾ�麯��������ɼ̳� 
	///     2.DM_��ͷ��ʾ����DUI����,��������DMCWndBase.h�еķ�װ
	///     3.ͨ�õ�DM_Is�жϺ���ʹ��bool����ֵ,DM_Get/DV_Getֱ�ӷ���,����ʹ��DMCode
	/// </remarks>
	class DM_EXPORT DUIWindow:public DMBase
							 ,public DUIMsgHandler
	{
		DMDECLARE_CLASS_NAME(DUIWindow,L"window",DMREG_Window);
	public:
		DUIWindow();
		virtual~DUIWindow();

	public:
		//---------------------------------------------------
		// Function Des:��������
		LPCWSTR GetName();																///< ʹ��������ΪΨһ��ʶ
		int GetID();																	///< ʹ��������ΪΨһ��ʶ
		DMCode SetID(int id);															///< ����id

		DUIWindow* FindChildByName(LPCWSTR lpszName,bool bPanelFind = false);			///< ͨ�����ֲ���DUI����
		DUIWindow* FindChildById(int ID,bool bPanelFind = false);						///< ͨ��ID����DUI����
		DUIWindow* FindPanelChildByName(LPCWSTR lpszName,bool bPanelFind = false);		///< ͨ�����ִ���panel���в���DUI����
		DUIWindow* FindPanelChildById(int ID,bool bPanelFind = false);					///< ͨ��ID����panel���в���DUI����


		virtual DUIWND HitTestPoint(CPoint pt,bool bFindNoMsg = false);					///< ͨ��point��ȷ�����ĸ������ϣ�����ģ����Ϣ�ַ�
		DUIWND GetDUIWnd(){return m_hDUIWnd;};											///< ���ģ�ⴰ�ھ��

		//---------------------------------------------------
		// �û�����
		DMCode SetData(CStringW strKey, CStringW strValue, bool bReplace = true);       ///< CStringWҲ����ָ��ĳ��ָ�루ָ����ַ�����ת��,strValue���ձ�ʾ���strKey
		CStringW GetData(CStringW strKey);
		
		//---------------------------------------------------
		// Function Des:��ʼ��
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);						    ///< 1.��ʼ������Ϊ���ؼ�XmlNode 2.��̬�����ӿؼ��б�(s),����Child1,Child2��ʽӦΪ:<A><Child1 ../><Child2 .../></A>,(A��Ϊ������)
		DMCode InitDMData(DMXmlNode &XmlNode);											///< ����XML
		
		//---------------------------------------------------
		// Function Des:����
		IDMContainerPtr GetContainer();													///< ��ȡ����
		DMCode SetContainer(IDMContainerPtr pContainer);								///< ��������
		DMCode DM_AddChildPanel(DUIWindowPtr pWnd);										///< ����������(��DUIListBoxEx),�����ڲ���panel�Ŀؼ�,pWndͬʱҲΪIDMContainerPtr
		DMCode DM_RemoveChildPanel(DUIWindowPtr pWnd);									///< �Ƴ�������(��DUIListBoxEx),�����ڲ���panel�Ŀؼ�,pWndͬʱҲΪIDMContainerPtr
		DMCode DM_RemoveAllChildPanel();												///< �Ƴ�����������											

		//---------------------------------------------------
		// Function Des: ����
		virtual DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);								///< ����,lp��ʾ�軻��������

		//---------------------------------------------------
		// Function Des: tooltip
		virtual DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);			///< ����tooltip��ʾ��Ϣ
		
		//---------------------------------------------------
		// Function Des: ActiveX
		virtual DMCode DV_OnAxActivate(IUnknown *pUnknwn);								///< ��com���뵽DUIWindow

		//---------------------------------------------------
		// Function Des:�¼�
		virtual DMCode DV_FireEvent(DMEventArgs &Evt);									///< �����ص��¼�ִ֪ͨ��, Ҳ��Event��Ϣ����Դ
		virtual DMCode DV_DispatchEvent(DMEventArgs &Evt);                              ///< Ⱥ���¼���Ϣ��������DUI����
		DMEventMgrPtr GetEventMgrPtr(){return &m_EventMgr;};							///< ��ȡ�¼�������,�����ⲿע���¼�

		//---------------------------------------------------
		// Function Des:ͨ�ò���
		virtual DMCode DV_UpdateChildLayout();											///< ���������Ӵ���λ��
		virtual DMCode DV_GetChildMeasureLayout(LPRECT lpRect);							///< ����Ӵ��ڵĲ��ֿռ��С,Ĭ��Ϊ�ͻ���������tab�����Ŀؼ���һ��
		virtual DMCode DV_GetDesiredSize(LPRECT pRcContainer,SIZE &sz);					///< ��û��ָ�����ڴ�Сʱ��ͨ����Ƥ�����������ݼ��㴰�ڵ�������С(����ʹ���ı�������ı�Ϊ��,��ʹ��skin��С)

		bool DM_IsLayoutFinished();														///< �����Ƿ����
		bool DM_IsParentFlowLayout();													///< �������Ƿ�ʹ����ʽ����
		DMCode DM_FloatLayout(LPRECT lpRect);											///< �Ӿ��Բ��ֿ�ʼ�Ӳ���
		DMCode DM_UpdateLayout(LPRECT lpRect);											///< ����������

		//---------------------------------------------------
		//  Function Des:��ʽ����
		virtual DMCode DV_Measure(CSize& AvailableSize);								///< ������ô�С				
		virtual DMCode DV_Arrange(CRect& FinalRect);									///< ������ô�С

		//---------------------------------------------------
		// Function Des:״̬
		virtual DMCode DV_GetState(int& iState);										///< ��ȡ��ǰ����״̬
		DWORD DM_ModifyState(DWORD dwAdd, DWORD dwRemove,bool bUpdate=false);			///< ����״̬

		//---------------------------------------------------
		// Function Des:���԰�
		virtual CStringW DV_GetTransText(CStringW strSrc);								///< ���ת�����ַ���

		//---------------------------------------------------
		// Function Des:����,����rect�������������m_hWnd�Ĵ�������

		/// ��ȡ����
		virtual DMCode DV_GetTextRect(LPRECT lpRect);									///< ����ı�������
		virtual DMCode DV_GetWindowRect(LPRECT lpRect);									///< ��ô��ڵ������д�С,�粻ռλ,����ʾʱΪ��
		virtual DMCode DV_GetClientRect(LPRECT lpRect);									///< ��ô��ڵĿͻ���
	
		/// �������
		virtual DMCode DV_SetFocusWnd();												///< ���ý���,���ý�������½��㴰�ڷ���WM_SETFOCUS��Ϣ��ͬʱ��ɽ��㴰�ڷ���WM_KILLFOCUS��Ϣ
		virtual DMCode DV_KillFocusWnd();												///< ȥ������,����Ϊ�ѽ������õ�һ����DUI������
				bool   DM_IsFocusWnd();                                                 ///< �Ƿ�Ϊ���㴰��
		virtual	bool DV_IsFocusable();													///< �Ƿ��н�������,�Ӵ��ڿ�ͨ�������Ծ����Ƿ����������߿����

		virtual DMCode DV_OnNcHitTest(CPoint pt){return DM_ECODE_FAIL;};				///< ģ��ǿͻ���,�ַ���Ϣʱ��Ϊtrue���Զ�ת���ɷǿͻ�����Ϣ,����scroll
		virtual DMCode DV_OnSetCursor(const CPoint &pt);								///< ���õ�ǰ�Ĺ��
		virtual UINT DV_OnGetDlgCode(){return 0;};										///< ��ʾ��Ҫĳ���,�ַ�ʱ�Դ��������ֱ�ӹ���,http://msdn.microsoft.com/zh-tw/library/t83978eh.aspx

		/// ˢ�����
		DMCode DM_Invalidate();															///< ˢ�������ͻ�������Ҫˢ�·ǿͻ�����ʹ��DM_InvalidateRect(m_rcWindow)
		DMCode DM_InvalidateRect(LPRECT lpRect);										///< ˢ��ָ�������ڲ������ϲ�������ʵˢ����
		DMCode DM_LockUpdate();															///< ����ˢ��				
		DMCode DM_UnlockUpdate();														///< ����ˢ��
		bool DM_IsUpdateLocked();														///< �Ƿ�����ˢ��

		/// Capture���
		DUIWND DM_GetCapture();															///< ��ȡ��ǰcapture��DUI����
		DUIWND DM_SetCapture();															///< ����capture��ע��һ���ڵ���DM_ReleaseCapture���ͷ�
		DMCode DM_ReleaseCapture();														///< �ͷ�capture

		/// ���ӻ����
		bool DM_IsVisible(bool bCheckParent = false);								    ///< �Ƿ�ɼ�
		DMCode DM_SetVisible(bool bVisible, bool bUpdate = false);						///< ���ÿɼ�
		bool DM_IsDisable(bool bCheckParent = false);									///< �Ƿ����	
		DMCode DM_EnableWindow(BOOL bEnable,bool bUpdate=false);						///< enable����

		/// ������Ϣ
		bool DM_IsMsgNoHandle();														///< Ϊtrueʱ,HitTestPointʱ��˿ؼ��ᱻ����,�Ӷ���Ϣ�ᱻ���͸�ͬһ�������һ���ؼ�

		/// checkbox
		bool DM_IsChecked();														    ///< �Ƿ�Ϊchecked״̬
		DMCode DM_SetCheck(bool bCheck);											    ///< ����checked״̬

		/// RadioButton
		virtual bool DV_IsSiblingsAutoGroup(){return false;}							///< �Ƿ��Զ����ӿؼ�����һ��,һ����Ĭ��ֻ��Ӧcheck״̬�Ľ��㣬����ĺ���
		virtual DUIWindow* DV_GetSelSiblingInGroup(){return NULL;}					    ///< ������ڵ�ѡ���ӿؼ�
	
		//---------------------------------------------------
		// Function Des:���滭��
		virtual bool DV_IsPreCmpSizeDiff();												///< ��size����ʱ��Ӧ�ÿ��ǲ���������, �����أ���ֹ����ؼ��ĸ����ڱ����任
		DMCode DM_MarkCacheDirty(bool bDirty);											///< ���滭������Ϊ��																		
		bool DM_IsCacheDirty();															///< ���滭���Ƿ�Ϊ��
		bool DM_IsDrawToCache();														///< �Ƿ���Ƶ����滭��
		IDMCanvas* DM_GetCacheCanvas(){return m_pCacheCanvas;}							///< ��ȡ���滭��ָ��

		//---------------------------------------------------
		// Function Des: ��ʱ��
		DMCode DM_SetTimer(char id, UINT uElapse);										///< ���ô�����ض�ʱ�� id��֧��0-127,�����ڲ��������1,2
		DMCode DM_KillTimer(char id);													///< kill������ض�ʱ��
		DMCode DM_SetThreadTimer(UINT_PTR id, UINT uElapse);							///< �����̻߳ص���ʱ��
		DMCode DM_KillThreadTimer(UINT_PTR id);											///< kill�̻߳ص���ʱ��

		//---------------------------------------------------
		// Function Des: �������ӿ�		
		inline int DM_GetChildCount(){return m_Node.m_nChildrenCount;}					///< ��ȡ�����Ӵ�����Ŀ
		static DUIWindow* DM_GetNextVisibleWnd(DUIWindow* pWnd,const CRect& rcDraw);    ///< ��ȡrcDraw��pWnd����һ���ɼ�����
		DMCode DM_InsertChild(DUIWindow* pNewChild, DUIWindow* pInsertAfter=NULL);		///< �����Ӵ���
		DMCode DM_RemoveChildWnd(DUIWindow* pChild);									///< �Ƴ��Ӵ���
		DMCode DM_DestroyChildWnd(DUIWindow* pChild);									///< ����WM_DESTROY,�Ƴ����ͷ��Ӵ���(�Ӵ���һ��Ϊnew obj,�ⲿ����Ҫdelete)
		DMCode DM_SetWndToTop();														///< ��Ϊ�Ӵ���,���������firstchildλ��
		DMCode DM_SetOwnerWnd(DUIWindow* pWnd);											///< ����ӵ���ߴ���(��������)
		DUIWindow* DM_GetWindow(int iCode);												///< ��ȡָ���Ĵ���
		DUIWindow* DM_GetTopParentWnd();												///< ��ȡ���㸸����
		

		//---------------------------------------------------
		// Function Des:����
		virtual bool DV_IsOnlyDrawClient();												///< �Ƿ�ֻ���ƿͻ���
		virtual bool DV_IsStateChangeRedraw();											///< ������״̬�仯ʱ���Ƿ��ػ�
		virtual bool DV_IsNeedDrawEnviron(){return true;};								///< �Ƿ���Ҫ׼�����ƻ���
		virtual DMCode DV_OnStateChanged(DWORD dwOldState,DWORD dwNewState);		    ///< ״̬�ı�ʱ����

		/// �������
		virtual DMCode DV_SetWindowText(LPCWSTR lpszText);							    ///< �����ı�
		virtual const CStringW& DV_GetWindowText() const;
		virtual DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat);						///< ��������,��WM_PAINT�д���
		virtual DMCode DV_DrawMultText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat,int nLineInter);   ///< ʾ������,����xml�е��ַ���\R\N����
		
		/// Ĭ�Ͻ����
		virtual DMCode DV_DrawDefFocus(IDMCanvas* pCanvas);								///< ���ƽ����
		virtual DMCode DV_DrawDefFocusRect(IDMCanvas* pCanvas,CRect rcFocus);			///< ���ƽ����

		/// Ĭ�ϻ��ƻ���
		virtual DMCode DV_PushDrawEnviron(IDMCanvas* pCanvas, DUIDrawEnviron&old);     ///< ���õ�ǰ�������ɫ�Ȼ���
		virtual DMCode DV_PopDrawEnviron(IDMCanvas* pCanvas, DUIDrawEnviron&old);      ///< �ָ���ǰ�������ɫ�Ȼ���
		DMCode DV_SetDrawEnvironEx(IDMCanvas* pCanvas);								   ///< ����㸸���ڿ�ʼ,һ������û���

		/// ��Ļ�������(��Ҫ��������)
		IDMCanvas* DM_GetCanvas(LPRECT lpRect,DWORD dcFlags,bool bClientDC=true);	   ///< ��ȡ�������������ṩ:1.Ĭ�ϻ���(Ĭ������)2.���ƴ��ڻ��滭������3.���Ʊ���ɫ��
		DMCode DM_ReleaseCanvas(IDMCanvas* pCanvas);								   ///< ���»��������ڣ��������ṩ:�ڲ��Ǵ���Ĭ�ϻ���״̬�£�
		DMCode DM_DrawBackground(IDMCanvas* pCanvas,LPRECT lpRect);					   ///< ���Ʊ����ڵı���������,�Ӷ��㸸���ڻ��Ƶ������ڽ���
		DMCode DM_DrawForeground(IDMCanvas* pCanvas,LPRECT lpRect);					   ///< ���Ʊ�����ǰ����������ǰ��ָ��������������ɼ����ڣ����Ǳ����ڵ��Ӵ��ڣ�
		DMCode DM_RedrawRegion(IDMCanvas* pCanvas, IDMRegion* lpRgn);				   ///< ��lpRgn�����������(�������пɼ��Ӵ���)������
	
		//---------------------------------------------------
		// Function Des:��Ϣ
	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			DM_MSG_WM_NCPAINT(DM_OnNcPaint)
			DM_MSG_WM_ERASEBKGND(DM_OnEraseBkgnd)
			DM_MSG_WM_ENABLE(DM_OnEnable)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
			DM_MSG_WM_WINPOSCHANGED(OnWindowPosChanged)
			
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_SIZE(OnSize)
			MSG_WM_NCCALCSIZE(OnNcCalcSize)
			MSG_WM_SHOWWINDOW(OnShowWindow)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSEHOVER(OnMouseHover)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
		DM_END_MSG_INBASE()// ����ʹ��DM_END_MSG_INBASE����������ʹ��DM_END_MSG_MAP
	public:
		LRESULT DM_SendMessage(UINT uMsg, WPARAM wp = 0, LPARAM lp = 0,BOOL *pbMsgHandled=NULL);
		LRESULT DM_DispatchMessage(MSG* pMsg, BOOL *pbMsgHandled=NULL);
		LRESULT OnWindowPosChanged(LPRECT lpRcContainer);		  // 0��ʾ�ɹ�
		LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
		int OnCreate(LPVOID);
		BOOL DM_OnEraseBkgnd(IDMCanvas* pCanvas);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void DM_OnNcPaint(IDMCanvas* pCanvas);
		void DM_OnSetFocus();
		void DM_OnKillFocus();
		void DM_OnEnable(BOOL bEnable,UINT nStatus);
		void OnDestroy();
		void OnSize(UINT nType, CSize size);
		void OnShowWindow(BOOL bShow, UINT nStatus);         
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseHover(WPARAM wParam,CPoint ptPos);
		void OnMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

		//---------------------------------------------------
		// Function Des: ģ��
		template<class T> 
		T* FindChildByNameT(LPCWSTR lpszName,bool bPanelFind = false)						//< ����չ�ӿ�<see cref="FindChildByName"/>ȡ��һ����ǿ��ת��ģ��
		{	
			DUIWindow *pFindWnd = FindChildByName(lpszName,bPanelFind);
			if (NULL == pFindWnd) return NULL;
			return dynamic_cast<T*>(pFindWnd);
		}
		template<class T> 
		T* FindChildByIdT(int ID,bool bPanelFind = false)									//< ����չ�ӿ�<see cref="FindChildById"/>ȡ��һ����ǿ��ת��ģ��
		{
			DUIWindow *pFindWnd = FindChildById(ID,bPanelFind);
			if (NULL == pFindWnd)return NULL;
			return dynamic_cast<T*>(pFindWnd);
		}

	public:
		virtual BOOL DV_WndProc(UINT uMsg,WPARAM wParam,LPARAM lParam, LRESULT& lResult);	///< DUI��Ϣ�������
		virtual DMCode OnAttributeFinished(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml,DMCode iErr);
	
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CHAINFUN_ATTRIBUTTE(DM_Layout)
			DM_CHAIN_ATTRIBUTTE(m_pDUIXmlInfo)
		DM_END_ATTRIBUTES()
	
	public:
		DMCode DM_Layout(LPCWSTR lpszAttribute,LPCWSTR lpszValue,bool bLoadXml);			///< ���ڷǳ�ʼ��ʱ,ͨ�����Ե�������

	public:
		DUIWND                                  m_hDUIWnd;									///< ����m_hWnd��DUI���ڵ�Ψһ��ʶ    
		CRect                                   m_rcWindow;									///< �����������е�λ��
		DWORD									m_dwDUIState;								///< ����״̬
		
		DMSmartPtrT<IDMCanvas>					m_pCacheCanvas;								///< ���滭��
		DUIWindowNode                           m_Node;										///< DUI�����δ����б�

		DMSmartPtrT<DUIWindow_XmlInfo>          m_pDUIXmlInfo;								///< �Դ���XML����
		DUIWindow_Data							m_DUIData;

		DMEventMgr								m_EventMgr;									///< �¼������
		IDMContainerPtr                         m_pContainer;								///< ��������һ��Ϊ����

		DMSmartPtrT<IDMLayout>                  m_pLayout;									///< ê�㲼�ֶ���
		bool                                    m_bFloatLayout;								///< ��������

		CMap<CStringW,CStringW>					m_StrDataMap;
		CArray<DUIWindowPtr>					m_ChildPanelArray;							///< �ؼ��а������������б�
	
		//-----------------------------------------------------------------------------------------------
		CStringW								m_strXml;									///< ��debug����Ч,���ṩ��spy++��ʾ
		DMXmlNode                               m_XmlNode;                                  ///< ������_DMDesigner_��ʱ��Ч,design��xmldocһֱ����Ч�ģ��������Ķ���Ҳ��һֱ��Ч
	};
}//namespace DM
