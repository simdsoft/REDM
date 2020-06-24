//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMHWndHelper.h 
// File Des: ��������DMHWnd.h�ĸ�������
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-2	1.0			
//-------------------------------------------------------
#pragma once

typedef struct _MARGINS MARGINS;

namespace DMAttr
{ 
	/// <summary>
	///		<see cref="DM::DMHWnd"/>��xml���Զ���
	/// </summary>
	class DMHWndAttr
	{
	public:
		static wchar_t* SIZE_initsize;                                    ///< �����������ʱ�����sizeΪ0����ʹ�����������ڳ�ʼ��С, ʾ��:initsize="800,572"
		static wchar_t* SIZE_minsize;                                     ///< ��С��С, ʾ��:minsize="600,500"
		static wchar_t* RECT_maxinset;                                    ///< ȫ��ʱ���ܱ�Ԥ����С,�����ø�ֵ ʾ��:maxinset="50,50,50,50"
		static wchar_t* SIZE_maxsize;                                     ///< ����С��ʾ��:maxsize="800,800"��-1��ʾ��������
		static wchar_t* STRING_title;                                     ///< ���ڱ��⣬Ĭ�ϲ�����ʾ���� ʾ��:title="dmmagic"
		static wchar_t* STRING_regtip;                                    ///< ʹ�õ�Ĭ��tipע������,ΪNULLʹ��Ĭ�� ʾ��:regtip="dui_tip"
		static wchar_t* STRING_regdraw;                                   ///< ʹ�õ�Ĭ��drawע������,ΪNULLʹ��Ĭ�� ʾ��:regdraw="dui_draw"
		static wchar_t* STRING_transid;                                   ///< ʹ�õ�transid,ΪNULLʹ��Ĭ��custom ʾ��:transid="custom"
		static wchar_t* bool_bresize;                                     ///< ���ڿɸı��С,ʾ��:bresize="1"
		static wchar_t* bool_btranslucent;                                ///< ����ʹ��UpdateLayeredWindow����,ʾ��:btranslucent="1"
		static wchar_t* BYTE_alpha;                                        ///< ����͸����,͸������������,���http://hgy413.com/1865.html ʾ��:alpha="0xc0"
		static wchar_t* INT_h;                                            ///< hsl�任��h,ֵ����Ϊ[-180,180],��������������ܸ�֪����ɫ��Χ,0ΪĬ��ֵ��h="-10"
		static wchar_t* INT_s;											  ///< hsl�任��s,ֵ����Ϊ[0,200],ɫ�ʵı��Ͷȣ�����0%��100%��ֵ��������ͬɫ�ࡢ������ɫ�ʴ��ȵı仯����ֵԽ����ɫ�еĻ�ɫԽ�٣���ɫԽ����,ʾ��:s="10"
		static wchar_t* INT_l;										      ///< hsl�任��l,ֵ����Ϊ[0,200],ָ����ɫ�ʵ�����,100ΪĬ��ֵ����ֵԽ��Խ�ӽ��ڰ�ɫ,ʾ��:l="10"
	};
	DMAttrValueInit(DMHWndAttr,SIZE_initsize)DMAttrValueInit(DMHWndAttr,SIZE_minsize)DMAttrValueInit(DMHWndAttr,RECT_maxinset)DMAttrValueInit(DMHWndAttr,SIZE_maxsize)
	DMAttrValueInit(DMHWndAttr,STRING_title)DMAttrValueInit(DMHWndAttr,STRING_regtip)DMAttrValueInit(DMHWndAttr,STRING_regdraw)DMAttrValueInit(DMHWndAttr,STRING_transid)DMAttrValueInit(DMHWndAttr,bool_bresize)DMAttrValueInit(DMHWndAttr,bool_btranslucent)
	DMAttrValueInit(DMHWndAttr,BYTE_alpha)DMAttrValueInit(DMHWndAttr,INT_h)DMAttrValueInit(DMHWndAttr,INT_s)DMAttrValueInit(DMHWndAttr,INT_l)
}

namespace DM
{
	/// <summary>
	///		<see cref="DMHWnd"/>�ĸ�������
	/// </summary>
	class DM_EXPORT DMHWnd_Data
	{
	public:
		DMHWnd_Data();
		~DMHWnd_Data();
	public:
		LPCWSTR							m_lpszXmlId;        ///< ʹ�õ�XML id    
		bool                            m_bTrackFlag;       ///< ����ƶ����ٱ�־
	};

	class DMHWnd;
	/// <summary>
	///		<see cref="DMHWnd"/>��xml����,xml����Ĭ�ϲ����ִ�Сд
	/// </summary>
	class DM_EXPORT DMHWnd_XmlInfo:public DMDataBase
	{
	public:
		DMHWnd_XmlInfo(DMHWnd* pOwner);
		~DMHWnd_XmlInfo();
		void ResetXmlInfo();			///< ����XML����
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_SIZE_ATTRIBUTE(DMAttr::DMHWndAttr::SIZE_initsize,m_szInit,DM_ECODE_OK)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::SIZE_minsize,OnAttributeMinSize)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::SIZE_maxsize,OnAttributeMaxSize)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::bool_bresize,OnAttributeResize)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::bool_btranslucent,OnAttributeTranslucent)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::BYTE_alpha,OnAttributeAlpha)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::INT_h,OnAttributeH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::INT_s,OnAttributeS)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::INT_l,OnAttributeL)
			DM_RECT_ATTRIBUTE(DMAttr::DMHWndAttr::RECT_maxinset,m_rcMaxInset,DM_ECODE_OK)
			DM_STRING_ATTRIBUTE(DMAttr::DMHWndAttr::STRING_title,m_strTitle,DM_ECODE_OK)
			DM_STRING_ATTRIBUTE(DMAttr::DMHWndAttr::STRING_regtip,m_strRegTip,DM_ECODE_OK)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::STRING_regdraw,OnAttributeRegDraw)
			DM_STRING_ATTRIBUTE(DMAttr::DMHWndAttr::STRING_transid,m_strTransId,DM_ECODE_OK)
			
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeMinSize(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeMaxSize(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeResize(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeTranslucent(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeAlpha(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeH(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeS(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeL(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeRegDraw(LPCWSTR lpszValue, bool bLoadXml);

	public:
		CSize							m_szInit;			 ///< ��ʼ��С
		CSize							m_szMin;			 ///< ��С��С
		CSize                           m_szMax;             ///< ����С
		CRect							m_rcMaxInset;		 ///< ȫ��ʱ���ܱ�Ԥ����С

		bool							m_bResizable;		 ///< �Ƿ���϶��ı��С
		bool							m_bTranslucent;		 ///< �Ƿ�ʹ��UpdateLayeredWindow������͸������
		BYTE						    m_byAlpha;           ///< ����͸����
		int                             m_H;				 ///< HSL
		int                             m_S;				 ///< HSL
		int                             m_L;				 ///< HSL

		CStringW						m_strTitle;			 ///< ���ڱ���
		CStringW                        m_strRegTip;         ///< tipע����
		CStringW                        m_strRegDraw;        ///< drawע����
		CStringW                        m_strTransId;        ///< ���԰���Node��ʶ
		DMHWnd*                         m_pOwner;
	};

	class DMHWnd;
	/// <summary>
	///		<see cref="DMHWnd"/>��͸�������ɰ�
	/// </summary>
	class DMDummyWnd: public DMCWnd
	{
	public:
		DMDummyWnd(DMHWnd* pOwner);
		DECLARE_MSG_MAP()// ��MFC��Ϣӳ���

	public:

		void OnPaint(HDC hdc);
	public:
		DMHWnd					 *m_pOwner;
	};

	/// <summary>
	///		��Ӧϵͳ��UPDATELAYEREDWINDOWINFO 
	/// </summary>
	typedef struct tagDMUPDATELAYEREDWINDOWINFO 
	{
		DWORD				 cbSize;
		HDC					 hdcDst;
		const POINT*		 pptDst;
		const SIZE*			 size;
		HDC					 hdcSrc;
		const POINT*		 pptSrc;
		COLORREF			 crKey;
		const BLENDFUNCTION* pblend;
		DWORD				 dwFlags;
		const RECT*			 prcDirty;
	}DMUPDATELAYEREDWINDOWINFO, *PDMUPDATELAYEREDWINDOWINFO;

	// ----------------------------------------------------
	// ͸�����ڸ���
	typedef BOOL (WINAPI *fun_UpdateLayeredWindowIndirect)(HWND hwnd, const DMUPDATELAYEREDWINDOWINFO *pULWInfo);

	// ----------------------------------------------------
	// DWM��Ӱ����֧��
	typedef HRESULT(WINAPI* fun_DwmIsCompositionEnabled)(
		BOOL* pfEnabled
		);
	typedef HRESULT(WINAPI* fun_DwmSetWindowAttribute)(
		HWND    hwnd,
		DWORD   dwAttribute,
		LPCVOID pvAttribute,
		DWORD   cbAttribute
	);
	typedef HRESULT(WINAPI* fun_DwmExtendFrameIntoClientArea)(
		HWND          hWnd,
		const MARGINS* pMarInset
	);
}