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
#include "DUIEditHelper.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIButton"/>的xml属性定义
	/// </summary>
	class DUIRichEditAttr:public DUIScrollBaseAttr
	{
	public:
		static wchar_t* INT_editstyle;								    ///< 通过值来设置richedit属性，如ES_MULTILINE=0x4,示例:editstyle="0x4"
		static wchar_t* INT_maxbuf;										///< 设置edit最大字符数，示例:maxbuf="100"
		static wchar_t* bool_bautosel;									///< 每次进入，默认选中所有内容，示例:bautosel="1"
		static wchar_t* bool_brichtext;									///< RichEdit模式，示例:brichtext="1"
		static wchar_t* bool_bwordwrap;									///< 在多行状态下支持自动换行，示例:bwordwrap="1"
		static wchar_t* bool_ballowbeep;								///< If TRUE, beeping is enabled，示例:ballowbeep="1"
		static wchar_t* bool_bautowordsel;								///< 在选中文本时，如为true,在结尾自动选中到行头，示例:bautowordsel="1"
		static wchar_t* bool_bsinglevcenter;                            ///< 单行居中模式,仅在bmultilines="0"且brichtext="0"的状态下生效，示例:bsinglevcenter="1"
		static wchar_t* bool_bmultilines;								///< 多行模式.示例:bmultilines="1"
		static wchar_t* RECT_rcinsertmargin;                            ///< 字符串显示区四边框值.示例:rcinsertmargin="2,2,2,2"
		static wchar_t* COLOR_clrtext;									///< 默认文本颜色，和style的clrtext相同作用.示例:clrtext="pbgra(00,00,00,ff)"
		static wchar_t* FONT_font;										///< 默认文本字体,和style的font相同作用.示例:font="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		static wchar_t* OPTION_align;									///< 段落排布，和style的align相同作用,默认left,可设置center,right,示例:align="left"
		static wchar_t* bool_bhscroll;									///< 显示水平滚动条,示例:bhscroll="1"
		static wchar_t* bool_bvscroll;									///< 显示竖直滚动条,示例:bvscroll="1"
		static wchar_t* bool_bautohscroll;								///< 支持水平滚动条自动滚动,示例:bautohscroll="1"
		static wchar_t* bool_bautovscroll;								///< 支持竖直滚动条自动滚动,示例:bautovscroll="1"
		static wchar_t* bool_breadonly;							     	///< 只读，此时插入符仍会出现,示例:breadonly="1"
		static wchar_t* bool_bwantreturn;							    ///< 回车换行，不然如果外部对它处理了，可能是exe退出,示例:bwantreturn="1"
		static wchar_t* bool_bpassword;								    ///< 加密显示，默认都以*显示,示例:bpassword="1"
		static wchar_t* CHAR_passwordchar;								///< 配合加密，加密显示字符，取第一个字符，如为空，则使用默认*,示例:passwordchar="#"
		static wchar_t* bool_bnumber;								    ///< 仅支持数字输入，此时无滚动条显示,示例:bnumber="1"
		static wchar_t* bool_benabledrag;								///< 支持内容拖拽，默认不支持,示例:benabledrag="1"

		// 光标
		static wchar_t* COLOR_clrcaret;                                 ///< 光标颜色,示例:clrcaret="pbgra(ff,ff,ff,ff)"
		static wchar_t* INI_caretanimatecount;                          ///< 光标渐隐渐显次数，1次为正常状态，默认为6次,示例:caretanimatecount="6"
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
		static wchar_t* SKIN_surfaceskin;                               ///< 表层背景皮肤，示例:surfaceskin="editskin"
		static wchar_t* STRING_surfacetext;                             ///< 表层文字，当真实文字为空时，表层文字出现，示例:surfacetext="this is edit"
		static wchar_t* COLOR_surfacetextclr;                           ///< 表层文字背景，示例:surfacetextclr="pbgra(00,00,ff,ff)"  
		static wchar_t* FONT_surfacefont;                               ///< 表层文字字体，示例:surfacefont="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1"
		static wchar_t* OPTION_surfacealign;							///< 表层文字字体左中右, 默认居左, 示例:surfacealign="right"
	};
	DMAttrValueInit(DUIEditAttr,SKIN_surfaceskin)DMAttrValueInit(DUIEditAttr,STRING_surfacetext)DMAttrValueInit(DUIEditAttr,COLOR_surfacetextclr)DMAttrValueInit(DUIEditAttr,FONT_surfacefont)DMAttrValueInit(DUIEditAttr,OPTION_surfacealign)

}

namespace DM
{
	/// <summary>
	///			DUIRichEdit的内部实现，属性：<see cref="DMAttr::DUIRichEditAttr"/>
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
		// Function Des: 对外接口
		//---------------------------------------------------
		void SetText(const CStringW& text) override;
		CStringW GetText() const override;

		// [deprecated] 参看afxcmn.inl，和CRichEditCtrl保持一致
		void SetWindowText(LPCWSTR lpszText) { SetText(lpszText); }
		CStringW GetWindowText();
		int GetWindowText(LPWSTR lpString, int nMaxCount);
		int GetWindowTextLength();

		DWORD GetEventMask();										
		DWORD SetEventMask(DWORD dwEventMask);						///< 设置需要接收的事件类型 SetEventMask(ENM_OBJECTPOSITIONS | ENM_PROTECTED | ENM_DROPFILES | ENM_CHANGE | ENM_LINK | ENM_SELCHANGE | ENM_DRAGDROPDONE);

		int LineFromChar(LONG nIndex);								///< 参看MSDN：返回字符所在行号,-1返回当前选中第一个字符所在的行, 未选中则返回光标所在行
		int LineIndex(int nLine = -1);								///< 参看MSDN：返回行起始字符的Index,-1表示当前光标所在行
		int CharFromPos(POINT pt);									///< 参看MSDN：返回pt(客户端坐标)最近的字符的Index，如果pt超过了最后字符范围,返回最后字符的Index
		POINT PosFromChar(LONG nChar);								///< 参看MSDN：nChar为字符Index,pt为字符所在的pt(客户端坐标),和CharFromPos相对应
		int GetFirstVisibleLine();									///< 参看MSDN：返回最上面可见行的Index
		int GetLineCount();											///< 参看MSDN：返回行数,注意,行可以为空行,可以使用
		int LineLength(int nLine = -1);								///< 注意这里是直接传入行号,原始的是传入字符Index,内部做了封装
		CStringW GetLineText(int nLine = -1);						///< 得到指定行的字符串

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
		// Function Des: DUI的消息分发系列函数
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
		LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);  ///< 控制滚动条显示或者隐藏
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
		// Function Des: 重载
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual UINT DV_OnGetDlgCode();   
		virtual DMCode DV_OnSetCursor(const CPoint &pt);                                  // 设置当前的光标为I型
		virtual bool OnScroll(bool bVert,UINT uCode,int nPos);                              // 
		virtual BOOL DV_WndProc(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& lResult);  // 此函数最终处于所有的对外接口请求，如GetWindowTextLength
		virtual int GetScrollLineSize(bool bVert){return m_nFontHeight;}                    // 点下箭头滚动一行吧
	public:// 辅助
		HRESULT InitDefaultCharFormat(CHARFORMAT2W* pcf,IDMFont *pFt=NULL);
		HRESULT InitDefaultParaFormat(PARAFORMAT2* ppf);

		void OnEnableDragDrop(bool bEnable); // 注册或反注册拖拽
		HRESULT OnTxSendCurMessage();        // 向ITextServices发送当前消息

		void ShowCaret(bool bShow);
		void InitCaret(int w, int h);
		bool ResetCaret(CPoint point);

	public:
		HRESULT OnTxNotify(DWORD iNotify,LPVOID pv);// 消息分发

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
		DMCode OnAttrTextColor(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrTextFont(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrAlign(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrText(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrCustomEx(LPCWSTR lpszAttribute, LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttrCuretClr(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrCuretAnimateCount(LPCWSTR pszValue, bool bLoadXml);

	public:
		void InitTxPropertyBits(DWORD &dwBit,DWORD &dwMask);

		CHARFORMAT2W                 m_cfDef;             
		PARAFORMAT2					 m_pfDef;            
		DWORD						 m_cchTextMost;   
		TCHAR						 m_chPasswordChar;    
		CRect						 m_rcInsetMargin;       

		// 提供给ITextHost,HME指代MM_HIMETRIC，单位0.01毫米，坐标系X轴右增加，Y轴上增加           
		SIZEL						 m_HMEsizelExtent;     // 逻辑坐标    
		CRect						 m_HMErcInsetMargin;   // 逻辑坐标,标识文本所在区域离整个区域四边的边距   

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
		bool						 m_bScrollPending;                     // 滚动条处理中
		bool						 m_bEnableDragDrop;   
		bool						 m_bAutoSel;
		HIMC						 m_hCurIMC;	
		DUITextHost					 *m_pTxtHost;

		DUICaret*					 m_pCaret;
		DMColor                      m_clrCaret;
		int						     m_iCaretAniCount;
	};

	/// <summary>
	///			DUIEdit的内部实现，属性：<see cref="DMAttr::DUIEditAttr"/>
	/// </summary>
	class DM_EXPORT DUIEdit : public DUIRichEdit
	{
		DMDECLARE_CLASS_NAME(DUIEdit, DUINAME_Edit, DMREG_Window)
	public:
		DUIEdit();

	public:
		int GetSurfaceText(LPTSTR lpString,int nMaxCount);
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
			DM_STRING_ATTRIBUTE(DMAttr::DUIEditAttr::STRING_surfacetext,m_strSurfaceText,DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIEditAttr::FONT_surfacefont,m_pSurfaceFont,DM_ECODE_NOXMLLOADREFRESH)
			DM_ENUM_BEGIN(DMAttr::DUIEditAttr::OPTION_surfacealign, UINT, DM_ECODE_NOXMLLOADREFRESH)
				DM_ENUM_VALUE(L"left", EditAlign_Left)
				DM_ENUM_VALUE(L"center", EditAlign_Center)
				DM_ENUM_VALUE(L"right", EditAlign_Right)
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