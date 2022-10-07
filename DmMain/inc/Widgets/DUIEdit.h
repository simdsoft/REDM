//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: http://www.cppblog.com/wlwlxj/archive/2012/07/01/180982.html
// File Des: http://blog.csdn.net/skilledprogrammer/article/details/6223650
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-11-24	1.0			
//--------------------------------------------------------
#pragma once
#include "Widgets/DUIEditHelper.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIButton"/>��xml���Զ���
	/// </summary>
	class DUIRichEditAttr:public DUIScrollBaseAttr
	{
	public:
		static const char* INT_editstyle;								    ///< ͨ��ֵ������richedit���ԣ���ES_MULTILINE=0x4,ʾ��:editstyle="0x4"
		static const char* INT_maxbuf;										///< ����edit����ַ�����ʾ��:maxbuf="100"
		static const char* bool_bautosel;									///< ÿ�ν��룬Ĭ��ѡ���������ݣ�ʾ��:bautosel="1"
		static const char* bool_brichtext;									///< RichEditģʽ��ʾ��:brichtext="1"
		static const char* bool_bwordwrap;									///< �ڶ���״̬��֧���Զ����У�ʾ��:bwordwrap="1"
		static const char* bool_ballowbeep;								///< If TRUE, beeping is enabled��ʾ��:ballowbeep="1"
		static const char* bool_bautowordsel;								///< ��ѡ���ı�ʱ����Ϊtrue,�ڽ�β�Զ�ѡ�е���ͷ��ʾ��:bautowordsel="1"
		static const char* bool_bsinglevcenter;                            ///< ���о���ģʽ,����bmultilines="0"��brichtext="0"��״̬����Ч��ʾ��:bsinglevcenter="1"
		static const char* bool_bmultilines;								///< ����ģʽ.ʾ��:bmultilines="1"
		static const char* RECT_rcinsertmargin;                            ///< �ַ�����ʾ���ı߿�ֵ.ʾ��:rcinsertmargin="2,2,2,2"
		static const char* COLOR_clrtext;									///< Ĭ���ı���ɫ����style��clrtext��ͬ����.ʾ��:clrtext="pbgra(00,00,00,ff)"
		static const char* FONT_font;										///< Ĭ���ı�����,��style��font��ͬ����.ʾ��:font="face:����,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:��weight:���λ������:������1λ,����ո�!�ڲ����˿ո�ȥ������
		static const char* OPTION_align;									///< �����Ų�����style��align��ͬ����,Ĭ��left,������center,right,ʾ��:align="left"
		static const char* bool_bhscroll;									///< ��ʾˮƽ������,ʾ��:bhscroll="1"
		static const char* bool_bvscroll;									///< ��ʾ��ֱ������,ʾ��:bvscroll="1"
		static const char* bool_bautohscroll;								///< ֧��ˮƽ�������Զ�����,ʾ��:bautohscroll="1"
		static const char* bool_bautovscroll;								///< ֧����ֱ�������Զ�����,ʾ��:bautovscroll="1"
		static const char* bool_breadonly;							     	///< ֻ������ʱ������Ի����,ʾ��:breadonly="1"
		static const char* bool_bwantreturn;							    ///< �س����У���Ȼ����ⲿ���������ˣ�������exe�˳�,ʾ��:bwantreturn="1"
		static const char* bool_bpassword;								    ///< ������ʾ��Ĭ�϶���*��ʾ,ʾ��:bpassword="1"
		static const char* CHAR_passwordchar;								///< ��ϼ��ܣ�������ʾ�ַ���ȡ��һ���ַ�����Ϊ�գ���ʹ��Ĭ��*,ʾ��:passwordchar="#"
		static const char* bool_bnumber;								    ///< ��֧���������룬��ʱ�޹�������ʾ,ʾ��:bnumber="1"
		static const char* bool_benabledrag;								///< ֧��������ק��Ĭ�ϲ�֧��,ʾ��:benabledrag="1"

		// ���
		static const char* COLOR_clrcaret;                                 ///< �����ɫ,ʾ��:clrcaret="pbgra(ff,ff,ff,ff)"
		static const char* INI_caretanimatecount;                          ///< ��꽥�����Դ�����1��Ϊ����״̬��Ĭ��Ϊ6��,ʾ��:caretanimatecount="6"
	};
	DMAttrValueInit(DUIRichEditAttr,INT_editstyle)DMAttrValueInit(DUIRichEditAttr,INT_maxbuf)DMAttrValueInit(DUIRichEditAttr,bool_bautosel)
	DMAttrValueInit(DUIRichEditAttr,bool_brichtext)DMAttrValueInit(DUIRichEditAttr,bool_bwordwrap)DMAttrValueInit(DUIRichEditAttr,bool_ballowbeep)
	DMAttrValueInit(DUIRichEditAttr,bool_bautowordsel)DMAttrValueInit(DUIRichEditAttr,bool_bsinglevcenter)DMAttrValueInit(DUIRichEditAttr,bool_bmultilines)
	DMAttrValueInit(DUIRichEditAttr,RECT_rcinsertmargin)DMAttrValueInit(DUIRichEditAttr,COLOR_clrtext)DMAttrValueInit(DUIRichEditAttr,OPTION_align)
	DMAttrValueInit(DUIRichEditAttr,FONT_font)DMAttrValueInit(DUIRichEditAttr,bool_bhscroll)DMAttrValueInit(DUIRichEditAttr,bool_bvscroll)
	DMAttrValueInit(DUIRichEditAttr,bool_bautohscroll)DMAttrValueInit(DUIRichEditAttr,bool_bautovscroll)
	DMAttrValueInit(DUIRichEditAttr,bool_breadonly)DMAttrValueInit(DUIRichEditAttr,bool_bwantreturn)DMAttrValueInit(DUIRichEditAttr,bool_bpassword)
	DMAttrValueInit(DUIRichEditAttr,CHAR_passwordchar)DMAttrValueInit(DUIRichEditAttr,bool_bnumber)DMAttrValueInit(DUIRichEditAttr,bool_benabledrag)
	DMAttrValueInit(DUIRichEditAttr,COLOR_clrcaret)DMAttrValueInit(DUIRichEditAttr,INI_caretanimatecount)

	class DUIEditAttr:public DUIRichEditAttr
	{
	public:
		static const char* SKIN_surfaceskin;                               ///< ��㱳��Ƥ����ʾ��:surfaceskin="editskin"
		static const char* STRING_surfacetext;                             ///< ������֣�����ʵ����Ϊ��ʱ��������ֳ��֣�ʾ��:surfacetext="this is edit"
		static const char* COLOR_surfacetextclr;                           ///< ������ֱ�����ʾ��:surfacetextclr="pbgra(00,00,ff,ff)"  
		static const char* FONT_surfacefont;                               ///< ����������壬ʾ��:surfacefont="face:����,size:0,weight:400,charset:0,underline:1,italic:1,strike:1"
		static const char* OPTION_surfacealign;							///< �����������������, Ĭ�Ͼ���, ʾ��:surfacealign="right"
	};
	DMAttrValueInit(DUIEditAttr,SKIN_surfaceskin)DMAttrValueInit(DUIEditAttr,STRING_surfacetext)DMAttrValueInit(DUIEditAttr,COLOR_surfacetextclr)DMAttrValueInit(DUIEditAttr,FONT_surfacefont)DMAttrValueInit(DUIEditAttr,OPTION_surfacealign)

}

namespace DM
{
	/// <summary>
	///			DUIRichEdit���ڲ�ʵ�֣����ԣ�<see cref="DMAttr::DUIRichEditAttr"/>
	/// </summary>
	class DM_EXPORT DUIRichEdit :public DUIScrollBase
	{
		DMDECLARE_CLASS_NAME(DUIRichEdit, DUINAME_RichEdit,DMREG_Window)
	public:
		DUIRichEdit();
		virtual ~DUIRichEdit();
		void Init();
		void UnInit();

	public:
		//---------------------------------------------------
		// Function Des: ����ӿ�
		//---------------------------------------------------
		void SetText(const CStringW& text) override;
		CStringW GetText() const override;

		// [deprecated] �ο�afxcmn.inl����CRichEditCtrl����һ��
		void SetWindowText(LPCWSTR lpszText) { SetText(lpszText); }
		CStringW GetWindowText();
		int GetWindowText(LPWSTR lpString, int nMaxCount);
		int GetWindowTextLength();

		DWORD GetEventMask();										
		DWORD SetEventMask(DWORD dwEventMask);						///< ������Ҫ���յ��¼����� SetEventMask(ENM_OBJECTPOSITIONS | ENM_PROTECTED | ENM_DROPFILES | ENM_CHANGE | ENM_LINK | ENM_SELCHANGE | ENM_DRAGDROPDONE);

		int LineFromChar(LONG nIndex);								///< �ο�MSDN�������ַ������к�,-1���ص�ǰѡ�е�һ���ַ����ڵ���, δѡ���򷵻ع��������
		int LineIndex(int nLine = -1);								///< �ο�MSDN����������ʼ�ַ���Index,-1��ʾ��ǰ���������
		int CharFromPos(POINT pt);									///< �ο�MSDN������pt(�ͻ�������)������ַ���Index�����pt����������ַ���Χ,��������ַ���Index
		POINT PosFromChar(LONG nChar);								///< �ο�MSDN��nCharΪ�ַ�Index,ptΪ�ַ����ڵ�pt(�ͻ�������),��CharFromPos���Ӧ
		int GetFirstVisibleLine();									///< �ο�MSDN������������ɼ��е�Index
		int GetLineCount();											///< �ο�MSDN����������,ע��,�п���Ϊ����,����ʹ��
		int LineLength(int nLine = -1);								///< ע��������ֱ�Ӵ����к�,ԭʼ���Ǵ����ַ�Index,�ڲ����˷�װ
		CStringW GetLineText(int nLine = -1);						///< �õ�ָ���е��ַ���

		void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE);
		void ReplaceSel(LPWSTR LPCWSTR,BOOL bCanUndo = TRUE);
		
		void SetWordWrap(bool bWordWrap);
		bool GetWordWrap(void);

		BOOL GetReadOnly();
		BOOL SetReadOnly(bool bReadOnly);

		LONG GetLimitText();
		BOOL SetLimitText(int nLength);

		WORD GetDefaultAlign();
		void SetDefaultAlign(WORD wNewAlign);

		bool GetRichTextFlag();
		void SetRichTextFlag(bool bRich);

		LONG GetDefaultLeftIndent();
		void SetDefaultLeftIndent(LONG lNewIndent);

		bool SetSaveSelection(bool bSaveSelection);
		DMColor SetDefaultTextColor(DMColor Clr);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
			DM_MSG_WM_PAINT(DM_OnPaint)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_RBUTTONDOWN(OnRButtonDown)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_CHAR(OnChar)
			DM_MSG_WM_SETFONT(OnSetFont)
			MSG_WM_NCCALCSIZE(OnNcCalcSize)
			MESSAGE_HANDLER_EX(WM_IME_COMPOSITION, OnImeChar)
			MESSAGE_HANDLER_EX(WM_SETTEXT,OnSetText)
			MESSAGE_HANDLER_EX(EM_SETPARAFORMAT,OnSetParaFormat)
			MESSAGE_HANDLER_EX(EM_SETCHARFORMAT,OnSetCharFormat)
			MESSAGE_HANDLER_EX(EM_SETREADONLY,OnSetReadOnly)
			MESSAGE_HANDLER_EX(EM_EXLIMITTEXT,OnSetLimitText)
			MESSAGE_HANDLER_EX(WM_LBUTTONDBLCLK,OnButtonClick)
			MESSAGE_HANDLER_EX(WM_LBUTTONUP,OnButtonClick)
			DM_MSG_WM_THREADTIMER(DM_OnTimer)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		int OnCreate(LPVOID);
		void OnDestroy();
		void DM_OnPaint(IDMCanvas* pCanvas);
		void DM_OnSetFocus();
		void DM_OnKillFocus();
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnRButtonDown(UINT nFlags, CPoint point);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnSetFont(IDMFont* font, BOOL bRedraw);
		LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);  ///< ���ƹ�������ʾ��������
		LRESULT OnSetText(UINT uMsg,WPARAM wparam,LPARAM lparam);
		LRESULT OnSetCharFormat(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnSetParaFormat(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnSetReadOnly(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnSetLimitText(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnButtonClick(UINT uMsg,WPARAM wParam,LPARAM lParam);
		LRESULT OnImeChar(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void DM_OnTimer(UINT_PTR idEvent);

	public:
		//---------------------------------------------------
		// Function Des: ����
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
		virtual UINT DV_OnGetDlgCode();   
		virtual DMCode DV_OnSetCursor(const CPoint &pt);                                  // ���õ�ǰ�Ĺ��ΪI��
		virtual bool OnScroll(bool bVert,UINT uCode,int nPos);                              // 
		virtual BOOL DV_WndProc(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& lResult);  // �˺������մ������еĶ���ӿ�������GetWindowTextLength
		virtual int GetScrollLineSize(bool bVert){return m_nFontHeight;}                    // ���¼�ͷ����һ�а�
	public:// ����
		HRESULT InitDefaultCharFormat(CHARFORMAT2W* pcf,IDMFont *pFt=NULL);
		HRESULT InitDefaultParaFormat(PARAFORMAT2* ppf);

		void OnEnableDragDrop(bool bEnable); // ע���ע����ק
		HRESULT OnTxSendCurMessage();        // ��ITextServices���͵�ǰ��Ϣ

		void ShowCaret(bool bShow);
		void InitCaret(int w, int h);
		bool ResetCaret(CPoint point);

	public:
		HRESULT OnTxNotify(DWORD iNotify,LPVOID pv);// ��Ϣ�ַ�

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::INT_editstyle,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::INT_maxbuf,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_brichtext,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bwordwrap,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_ballowbeep,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bautowordsel,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bmultilines,OnAttrCustomEx)

			DM_RECT_ATTRIBUTE(DMAttr::DUIRichEditAttr::RECT_rcinsertmargin,m_rcInsetMargin,DM_ECODE_NOXMLRELAYOUT)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRichEditAttr::COLOR_clrtext,OnAttrTextColor)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRichEditAttr::FONT_font,OnAttrTextFont)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRichEditAttr::OPTION_align,OnAttrAlign)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIWindowAttr::STRING_text,OnAttrText)
			
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bhscroll,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bvscroll,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bautohscroll,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bautovscroll,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_breadonly,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bwantreturn,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bpassword,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::CHAR_passwordchar,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bnumber,OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_benabledrag,OnAttrCustomEx)

			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRichEditAttr::COLOR_clrcaret,OnAttrCuretClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRichEditAttr::INI_caretanimatecount,OnAttrCuretAnimateCount)

			DM_bool_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bautosel,m_bAutoSel,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIRichEditAttr::bool_bsinglevcenter,m_bSingleLineVCenter,DM_ECODE_NOXMLRELAYOUT)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttrTextColor(LPCSTR pszValue, bool bLoadXml);
		DMCode OnAttrTextFont(LPCSTR pszValue, bool bLoadXml);
		DMCode OnAttrAlign(LPCSTR pszValue, bool bLoadXml);
		DMCode OnAttrText(LPCSTR pszValue, bool bLoadXml);
		DMCode OnAttrCustomEx(LPCSTR lpszAttribute, LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttrCuretClr(LPCSTR pszValue, bool bLoadXml);
		DMCode OnAttrCuretAnimateCount(LPCSTR pszValue, bool bLoadXml);

	public:
		void InitTxPropertyBits(DWORD &dwBit,DWORD &dwMask);

		CHARFORMAT2W                 m_cfDef;             
		PARAFORMAT2					 m_pfDef;            
		DWORD						 m_cchTextMost;   
		TCHAR						 m_chPasswordChar;    
		CRect						 m_rcInsetMargin;       

		// �ṩ��ITextHost,HMEָ��MM_HIMETRIC����λ0.01���ף�����ϵX�������ӣ�Y��������           
		SIZEL						 m_HMEsizelExtent;     // �߼�����    
		CRect						 m_HMErcInsetMargin;   // �߼�����,��ʶ�ı��������������������ıߵı߾�   

		LONG						 m_lAccelPos;          

		int							 m_nFontHeight;         
		DWORD						 m_dwEditStyle;

		bool						 m_bAutoWordSel;
		bool						 m_bWordWrap;          
		bool						 m_bRichText;             
		bool						 m_bSaveSelection; 
		bool						 m_bAllowBeep;        
		bool						 m_bWantTab;          
		bool						 m_bSingleLineVCenter;    
		bool						 m_bScrollPending;                     // ������������
		bool						 m_bEnableDragDrop;   
		bool						 m_bAutoSel;
		HIMC						 m_hCurIMC;	
		DUITextHost					 *m_pTxtHost;

		DUICaret*					 m_pCaret;
		DMColor                      m_clrCaret;
		int						     m_iCaretAniCount;
	};

	/// <summary>
	///			DUIEdit���ڲ�ʵ�֣����ԣ�<see cref="DMAttr::DUIEditAttr"/>
	/// </summary>
	class DM_EXPORT DUIEdit : public DUIRichEdit
	{
		DMDECLARE_CLASS_NAME(DUIEdit, DUINAME_Edit, DMREG_Window)
	public:
		DUIEdit();

	public:
		int GetSurfaceText(LPWSTR lpString,int nMaxCount);
		int GetSurfaceTextLength();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_ERASEBKGND(DM_OnEraseBkgnd)
			DM_MSG_WM_PAINT(DM_OnPaint)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
		DM_END_MSG_MAP()
	protected:
		BOOL DM_OnEraseBkgnd(IDMCanvas* pCanvas);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void DM_OnSetFocus();
		void DM_OnKillFocus();
	public:
		enum{EditAlign_Left,EditAlign_Center,EditAlign_Right};
		DM_BEGIN_ATTRIBUTES()
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIEditAttr::SKIN_surfaceskin,m_pSurfaceSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIEditAttr::COLOR_surfacetextclr,m_SurfaceTextClr,DM_ECODE_NOXMLLOADREFRESH)
			DM_WSTRING_ATTRIBUTE(DMAttr::DUIEditAttr::STRING_surfacetext,m_strSurfaceText,DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIEditAttr::FONT_surfacefont,m_pSurfaceFont,DM_ECODE_NOXMLLOADREFRESH)
			DM_ENUM_BEGIN(DMAttr::DUIEditAttr::OPTION_surfacealign, UINT, DM_ECODE_NOXMLLOADREFRESH)
				DM_ENUM_VALUE("left", EditAlign_Left)
				DM_ENUM_VALUE("center", EditAlign_Center)
				DM_ENUM_VALUE("right", EditAlign_Right)
			DM_ENUM_END(m_uSurfaceAlign)
		DM_END_ATTRIBUTES()

	public:
		DMColor					m_SurfaceTextClr;
		CStringW				m_strSurfaceText;
		DMSmartPtrT<IDMFont>	m_pSurfaceFont;
		DMSmartPtrT<IDMSkin>    m_pSurfaceSkin;
		UINT					m_uSurfaceAlign;	

	};
}//namespace DM