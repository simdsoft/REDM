#include "DmMainAfx.h"
#include "DUIComboBoxHelper.h"

namespace DM
{

	// --------------------------------------------------------------DUIComboBox
	DUIComboEdit::DUIComboEdit(DUIComboBoxBase *pComBoxBase)
	{
		DM_SetOwnerWnd(pComBoxBase); // 
	}

	DUIComboEdit::~DUIComboEdit()
	{
	}
	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	void DUIComboEdit::OnMouseHover(WPARAM wParam, CPoint ptPos)
	{
		__super::OnMouseHover(wParam,ptPos);
		DM_GetWindow(GDW_OWNER)->DM_SendMessage(WM_MOUSEHOVER,wParam,MAKELPARAM(ptPos.x,ptPos.y));
	}

	void DUIComboEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		__super::OnChar(nChar,nRepCnt,nFlags);
		DUIWindow* pOwner = DM_GetWindow(GDW_OWNER);
		if (pOwner)
		{
			pOwner->DM_SendMessage(WM_CHAR, nChar, MAKELONG(nFlags, nRepCnt));
		}
	}

	void DUIComboEdit::OnMouseLeave()
	{
		__super::OnMouseLeave();
		DM_GetWindow(GDW_OWNER)->DM_SendMessage(WM_MOUSELEAVE);
	}

	void DUIComboEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		DUIWindow* pOwner = DM_GetWindow(GDW_OWNER);
		if (pOwner&& (VK_DOWN == nChar||VK_ESCAPE == nChar))
		{
			pOwner->DM_SendMessage(WM_KEYDOWN, nChar, MAKELONG(nFlags, nRepCnt));
			return;
		}

		SetMsgHandled(FALSE);
	}

	DMCode DUIComboEdit::DV_FireEvent(DMEventArgs &Evt)
	{
		if (DMEVT_RENOTIFY == Evt.GetEventID())
		{// 转发richedit的txNotify消息
			Evt.m_IdFrom     = DM_GetWindow(GDW_OWNER)->GetID();
			Evt.m_szNameFrom = DM_GetWindow(GDW_OWNER)->GetName();
		}
		return DUIEdit::DV_FireEvent(Evt);
	}

	// --------------------------------------------------------------DUIComboBoxBase
	DUIComboBoxBase::DUIComboBoxBase(void)
	{
		Init();
	}

	DUIComboBoxBase::~DUIComboBoxBase()
	{
		UnInit();
	}

	void DUIComboBoxBase::Init()
	{
		m_pSkinBtn		= NULL;
		m_pEdit			= NULL;
		m_bHideEdit		= true;
		m_bDropTranslucent = false;
		m_nDropHeight   = 200;
		m_dwBtnState    = DUIWNDSTATE_Normal;
		m_iAnimTime     = 200;
		m_pDropDownWnd  = NULL;
		m_iInitSel      = -1;
		m_nTextOffset	= 0;
		m_BtnSize.SetSize(-1,-1);

		//
		m_pDUIXmlInfo->m_bFocusable       = true;
		m_pDUIXmlInfo->m_pStyle->SetAttribute(L"align",L"left",false);    ///Align_Left
		m_pDUIXmlInfo->m_pStyle->SetAttribute(L"valign",L"middle",false); ///VAlign_Middle
	}

	void DUIComboBoxBase::UnInit()
	{
	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	void DUIComboBoxBase::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas, DrawEnviron);

			// 绘制文字
			if (m_bHideEdit&&-1!=GetCurSel())
			{
				CStringW strText = GetLBText(GetCurSel());
				if (!strText.IsEmpty())
				{
					CRect rcTextTmp;
					DV_GetTextRect(rcTextTmp);
					CRect rcText = rcTextTmp;
					rcText.left = rcText.left+m_nTextOffset;
					UINT uAlign = 0;
					m_pDUIXmlInfo->m_pStyle->GetTextAlign(uAlign);
					CStringW strTrans = DMTR(strText);
					DV_DrawText(pCanvas, strTrans, strTrans.GetLength(), rcText, uAlign);
					//DV_DrawText(pCanvas,strText,strText.GetLength(),rcText,uAlign);
				}
			}

			if (m_pDUIXmlInfo->m_bShowDrawFocus)
			{
				if (DM_IsFocusWnd())
				{
					// 绘制焦点框
					DV_DrawDefFocus(pCanvas);
				}
			}

			DV_PopDrawEnviron(pCanvas, DrawEnviron);
			// 绘制下拉按钮状态
			if (m_pSkinBtn)
			{
				CRect rcBtn = CalcBtnRect();
				m_pSkinBtn->Draw(pCanvas,rcBtn,IIF_STATE4(m_dwBtnState,0,1,2,3));
			}
		} while (false);
	}

	void DUIComboBoxBase::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		DV_SetFocusWnd();
		DropDown();
	}

	void DUIComboBoxBase::OnMouseMove(UINT nFlags,CPoint pt)
	{
		do 
		{
			if (DUIWNDSTATE_PushDown == m_dwBtnState)
			{
				break;
			}
			__super::OnMouseHover(nFlags,pt);

			CRect rcBtn = CalcBtnRect();
			if (rcBtn.PtInRect(pt))
			{
				m_dwBtnState = DUIWNDSTATE_Hover;
				DM_InvalidateRect(rcBtn);
			}
			else if(DUIWNDSTATE_Hover == m_dwBtnState)
			{
				m_dwBtnState = DUIWNDSTATE_Normal;
				DM_InvalidateRect(rcBtn);
			}
		} while (false);
	}

	void DUIComboBoxBase::OnMouseLeave()
	{
		do 
		{
			if (DUIWNDSTATE_PushDown == m_dwBtnState)
			{
				break;
			}

			if (m_dwDUIState&DUIWNDSTATE_Hover)
			{
				__super::OnMouseLeave();
			}

			if (DUIWNDSTATE_Hover == m_dwBtnState)
			{
				m_dwBtnState=DUIWNDSTATE_Normal;
				CRect rcBtn = CalcBtnRect();
				DM_InvalidateRect(rcBtn);
			}
		} while (false);
	}

	void DUIComboBoxBase::OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags)
	{
		if (VK_DOWN == nChar)
		{
			DropDown();
		}
	}

	void DUIComboBoxBase::OnDestroy()
	{
		if (m_pDropDownWnd&&m_pDropDownWnd->IsWindow())
		{// 仍交由父窗口释放时自动释放
			//m_pDropDownWnd->PostMessage(WM_DESTROY);// 如果一定要释放,这里必须为Post消息
			m_pDropDownWnd->m_pOwner = NULL;
			m_pDropDownWnd = NULL;
		}
		__super::OnDestroy();
	}

	//---------------------------------------------------
	// Function Des: IDMDropDownOwner实现
	DUIWindow* DUIComboBoxBase::GetOwnerWindow()
	{
		return this;
	}

	DMCode DUIComboBoxBase::OnDropDown(DMDropWnd *pDropDown)
	{
		m_dwBtnState = DUIWNDSTATE_PushDown;
		CRect rcBtn = CalcBtnRect();
		DM_InvalidateRect(rcBtn);
		return DM_ECODE_OK;
	}

	DMCode DUIComboBoxBase::OnCloseUp(DMDropWnd *pDropDown,UINT uCode)
	{
		if (!m_bHideEdit 
			&& m_pEdit)
		{
			if (GetFocus() == GetContainer()->OnGetHWnd())
			{
				m_pEdit->DV_SetFocusWnd();
				m_pEdit->SetSel(MAKELONG(0,-1));
			}
		}

		// 恢复下拉按钮状态
		m_dwBtnState  = DUIWNDSTATE_Normal;
		CRect rcBtn   = CalcBtnRect();
		DM_InvalidateRect(rcBtn);

		DM_ModifyState(0,DUIWNDSTATE_Hover,true);
		CPoint pt;
		::GetCursorPos(&pt);
		::ScreenToClient(GetContainer()->OnGetHWnd(),&pt);
		::PostMessage(GetContainer()->OnGetHWnd(),WM_MOUSEMOVE,0,MAKELPARAM(pt.x,pt.y));

		if (IDOK == uCode)
		{// 改变选中项
			OnSelChanged();
		}
		return DM_ECODE_OK;
	}

	// 重载
	DMCode DUIComboBoxBase::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		bool bRet = false;
		do 
		{
			if (!m_bHideEdit)
			{// 创建Edit
				CSize szBtn;// 此时rcClient还为空
				if (CSize(-1,-1)==m_BtnSize)
				{//1.(0,0) 2.(使用skinbtn的sz)
					if (m_pSkinBtn)
					{
						m_pSkinBtn->GetStateSize(szBtn);
					}
				}
				else
				{//3.(m_BtnSize)
					szBtn = m_BtnSize;
				}

				m_pEdit = new DUIComboEdit(this);

				DM_InsertChild(m_pEdit);

				// 此处对DUIComboEdit做XML初始化
				DMXmlNode XmlEditStyle = XmlNode.FirstChild(DMAttr::DUIComboBoxBaseAttr::ITEM_subedit);
				DMXmlNode XmlBase =  m_EditDoc.Base();
				XmlBase.InsertCopyChildNode(&XmlEditStyle);

				m_pEdit->m_EventMgr.SetMuted(true);
				if (XmlEditStyle.IsValid())
				{
					m_pEdit->InitDMData(XmlEditStyle);
				}
				else
				{
					m_pEdit->DM_SendMessage(WM_CREATE);
				}
				m_pEdit->m_EventMgr.SetMuted(false);

				// 重设位置
				CStringW strPos;
				strPos.Format(L"0,0,-%d,-0",szBtn.cx);
				m_pEdit->SetAttribute(L"pos",strPos,true);
				m_pEdit->SetID(CB_EDIT_ID);
				m_pEdit->SetEventMask(ENM_CHANGE|m_pEdit->GetEventMask());
			}

			bRet = CreateListBox(XmlNode);// 创建List
		} while (false);
		return bRet;
	}

	DMCode DUIComboBoxBase::DV_GetTextRect(LPRECT lpRect)
	{
		DV_GetClientRect(lpRect);
		CRect rcBtn  = CalcBtnRect();
		lpRect->right -= rcBtn.Width();// 去掉BTN的宽度
		return DM_ECODE_OK;
	}

	DMCode DUIComboBoxBase::DV_SetFocusWnd()
	{
		if (m_pEdit) 
		{
			return m_pEdit->DV_SetFocusWnd();
		}
		else
		{
			return __super::DV_SetFocusWnd();
		}
	}

	DMCode DUIComboBoxBase::DV_FireEvent(DMEventArgs &Evt)
	{
		if (Evt.GetEventID() == DMEventRENotifyArgs::EventID)
		{
			DMEventRENotifyArgs *pEvt = (DMEventRENotifyArgs*)(&Evt);
			if (EN_CHANGE == pEvt->m_iNotify 
				&& !m_pEdit->m_EventMgr.IsMuted())
			{
				m_pEdit->m_EventMgr.SetMuted(true); // 开启事件过滤
				SetCurSel(-1);
				m_pEdit->m_EventMgr.SetMuted(false);
			}
		}
		return DUIWindow::DV_FireEvent(Evt);
	}

	UINT DUIComboBoxBase::DV_OnGetDlgCode()
	{        
		return DMDLGC_WANTARROWS;
	}

	bool DUIComboBoxBase::DV_IsFocusable()
	{
		if (m_bHideEdit 
			&&m_pDUIXmlInfo->m_bFocusable)
		{
			return true;
		}
		return false;
	}

	bool DUIComboBoxBase::CalcPopupRect(int nHeight,CRect& rcPopup)
	{
		bool bRet = false;
		do 
		{
			CRect rcWnd;
			DV_GetWindowRect(rcWnd);
			GetContainer()->OnClientToScreen(rcWnd);

			HMONITOR hMonitor = ::MonitorFromWindow(GetContainer()->OnGetHWnd(), MONITOR_DEFAULTTONULL);
			CRect rcMonitor;
			if (hMonitor)
			{
				MONITORINFO mi = {sizeof(MONITORINFO)};
				::GetMonitorInfo(hMonitor, &mi);
				rcMonitor = mi.rcMonitor;
			}
			else
			{
				rcMonitor.right   =   GetSystemMetrics(SM_CXSCREEN);   
				rcMonitor.bottom  =   GetSystemMetrics(SM_CYSCREEN);
			}

			if (rcWnd.bottom+nHeight<=rcMonitor.bottom)
			{
				rcPopup = CRect(rcWnd.left,rcWnd.bottom,rcWnd.right,rcWnd.bottom+nHeight);
				bRet = true;
			}
			else
			{// 向上
				rcPopup = CRect(rcWnd.left,rcWnd.top-nHeight,rcWnd.right,rcWnd.top);
				bRet = false;
			}

		} while (false);
		return bRet;
	}

	CRect DUIComboBoxBase::CalcBtnRect()
	{
		CRect rcBtn;
		CRect rcClient;
		DV_GetClientRect(rcClient);
		do 
		{
			CSize sz;
			if (CSize(-1,-1)==m_BtnSize)
			{//1.(0,0) 2.(使用skinbtn的sz)
				if (m_pSkinBtn)
				{
					m_pSkinBtn->GetStateSize(sz);
				}
			}
			else
			{//3.(m_BtnSize)
				sz = m_BtnSize;
			}
			sz.cx = sz.cx>rcClient.Width()?rcClient.Width():sz.cx;
			sz.cy = sz.cy>rcClient.Height()?rcClient.Height():sz.cy;
			if (sz.cx<0||sz.cy<0)
			{// 错误数据
				break;
			}

			rcBtn		 = rcClient;
			rcBtn.left	 = rcBtn.right-sz.cx;
			rcBtn.top	 = rcClient.top+(rcClient.Height()-sz.cy)/2;
			rcBtn.bottom = rcBtn.top+sz.cy;
		} while (false);
		return rcBtn;
	}

	void DUIComboBoxBase::OnSelChanged()
	{
		DMEventCBSelChangeArgs Evt(this, GetCurSel());
		DV_FireEvent(Evt);
	}

	CStringW DUIComboBoxBase::GetCBText()
	{
		CStringW strText;
		if (m_bHideEdit)
		{
			strText = GetLBText(GetCurSel());
		}
		else 
		{
			if (m_pEdit)
			{
				strText = m_pEdit->GetWindowText();
			}
		}
		return strText;
	}

	// 辅助
	void DUIComboBoxBase::DropDown()
	{
		do 
		{
			if (DUIWNDSTATE_PushDown == m_dwBtnState)
			{
				break;
			}

			CRect rcPopup;
			bool bDown = CalcPopupRect(GetListBoxHeight(),rcPopup);
			if(rcPopup.Height() <= 0)
			{
				break;
			}

			if (NULL == m_pDropDownWnd)
			{
				m_pDropDownWnd = new DMDropWnd(this);
			}

			m_pDropDownWnd->Show(rcPopup);
			if (0 == m_iAnimTime)
			{
				m_pDropDownWnd->ShowWindow(SW_SHOW);
			}
			else
			{
				m_pDropDownWnd->DM_AnimateWindow(m_iAnimTime,AW_SLIDE|(bDown?AW_VER_POSITIVE:AW_VER_NEGATIVE));
				m_pDropDownWnd->RedrawAll();
			}
		} while (false);
	}

	void DUIComboBoxBase::CloseUp()
	{
		if (m_pDropDownWnd)
		{
			m_pDropDownWnd->Hide(IDOK);
		}
	}

	DMCode DUIComboBoxBase::OnAttributeHideEdit(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			bool bHideEdit = m_bHideEdit;
			CStringW strText;
			if (!bLoadXml)
			{
				strText = GetCBText();
			}
			dm_parsebool(lpszValue,m_bHideEdit);
			if (!bLoadXml)
			{
				if (bHideEdit&&!m_bHideEdit)// 原来隐藏，现在显示
				{
					if (NULL == m_pEdit)
					{
						CRect rcBtn = CalcBtnRect();
						m_pEdit = new DUIComboEdit(this);
						DM_InsertChild(m_pEdit);

						// 此处对DUIComboEdit做XML初始化
						DMXmlNode XmlEditStyle = m_EditDoc.Root();
						m_pEdit->m_EventMgr.SetMuted(true);
						if (XmlEditStyle.IsValid())
						{
							m_pEdit->InitDMData(XmlEditStyle);
						}
						else
						{
							m_pEdit->DM_SendMessage(WM_CREATE);
						}
						m_pEdit->m_EventMgr.SetMuted(false);

						// 重设位置
						CStringW strPos;
						strPos.Format(L"0,0,-%d,-0",rcBtn.Width());
						m_pEdit->SetAttribute(L"pos",strPos,true);
						m_pEdit->SetID(CB_EDIT_ID);
						m_pEdit->SetEventMask(ENM_CHANGE|m_pEdit->GetEventMask());
					}
					m_pEdit->SetWindowText(strText);
					m_pEdit->DM_SetVisible(true,true);
				}
				else if (!bHideEdit&&m_bHideEdit)// 原来出现，现在隐藏
				{
					if (m_pEdit)
					{
						m_pEdit->DM_SetVisible(false,true);
					}
					DM_Invalidate();
				}
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIComboBoxBase::OnAttributeCurSel(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!bLoadXml)
			{
				int iSelItem = -1;
				dm_parseint(lpszValue,iSelItem);
				SetCurSel(iSelItem);
			}
			else
			{
				dm_parseint(lpszValue,m_iInitSel);// 此时listbox未初始化,在初始化时再设置
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIComboBoxBase::OnAttributeBtnSize(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CSize size;
			dm_parsesize(lpszValue,size);
			if (size==m_BtnSize)
			{
				break;
			}
			m_BtnSize = size;
			if (!bLoadXml)
			{
				if (m_pEdit)
				{
					CStringW strPos;
					CRect rcBtn = CalcBtnRect();
					strPos.Format(L"0,0,-%d,-0",rcBtn.Width());
					m_pEdit->SetAttribute(L"pos",strPos,true);
				}
				DM_Invalidate();
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

}//namespace DM