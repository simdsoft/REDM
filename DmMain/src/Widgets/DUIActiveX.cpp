#include "DmMainAfx.h"
#include "DUIActiveX.h"

namespace DM
{
	DUIActiveX::DUIActiveX() 
		: m_axContainer(new DMAxContainerImpl(this))
	{
		m_clsid		 = CLSID_NULL;
		m_clsCtx	 = CLSCTX_ALL;
		m_bDelayInit = true;
		m_bInit		 = false;
		m_pDUIXmlInfo->m_bFocusable = true;
	}

	DUIActiveX::~DUIActiveX() 
	{
		delete m_axContainer;
	}

	BOOL DUIActiveX::InitActiveX()
	{
		BOOL bRet = m_axContainer->CreateControl(m_rcWindow,m_clsid,m_clsCtx);
		if (bRet)
		{
			CRect rcClient;
			DV_GetClientRect(rcClient);
			m_axContainer->ActivateAx(NULL);
			SetActiveXVisible(DM_IsVisible(TRUE));
			m_axContainer->OnPosRectChange(m_rcWindow);
		}
		return bRet;
	}

	void DUIActiveX::OnSize(UINT nType, CSize size)
	{
		__super::OnSize(nType,size);
		if (m_axContainer->ActiveXControl())
		{
			m_axContainer->OnPosRectChange(m_rcWindow);        
		}
	}

	void DUIActiveX::DM_OnPaint(IDMCanvas* pCanvas)
	{
		HDC hdc = pCanvas->GetDC();
		m_axContainer->ProcessPaint(hdc, m_rcWindow);
		pCanvas->ReleaseDC(hdc);
	}

	void DUIActiveX::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		__super::OnShowWindow(bShow, nStatus);

		if (m_bDelayInit)
		{
			if (bShow&&false == m_bInit)// 窗口显示时才初始化
			{
				InitActiveX();
				m_bInit = true;
			}
		}
		else
		{
			if (false == m_bInit)
			{
				InitActiveX();
				m_bInit = true;
			}
		}

		SetActiveXVisible(bShow);
	}

	LRESULT DUIActiveX::OnMouseEvent(UINT uMsg,WPARAM wp,LPARAM lp)
	{
		LRESULT lResult = 0;
		do 
		{
			if (!m_axContainer->ActiveXControl())
			{
				break;
			}

			if (WM_LBUTTONDOWN == uMsg)
			{
				DV_SetFocusWnd();
			}
			lResult = m_axContainer->OnWindowMessage(uMsg, wp, lp);
		} while (false);
		return lResult;
	}

	LRESULT DUIActiveX::OnKeyEvent(UINT uMsg,WPARAM wp,LPARAM lp)
	{
		if (!m_axContainer->ActiveXControl()) 
			return 0;
		return m_axContainer->OnWindowMessage(uMsg, wp, lp);
	}

	DMCode DUIActiveX::OnAttrClsid(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			if (NULL == pszValue||0 == wcslen(pszValue))
			{
				break;
			}
			OLECHAR szCLSID[100] = {0};
			wcscpy_s(szCLSID,pszValue);

			if (szCLSID[0] == L'{') 
			{
				::CLSIDFromString(szCLSID, &m_clsid);
			}
			else
			{
				::CLSIDFromProgID(szCLSID, &m_clsid);
			}
		} while (false);
		return DM_ECODE_OK;
	}

	void DUIActiveX::SetActiveXVisible(BOOL bVisible)
	{
		do 
		{
			if (!m_axContainer->ActiveXControl())
			{
				break;
			}
			DMComQIPtr<IOleWindow> ole_window = m_axContainer->ActiveXControl();
			if (!ole_window)
			{
				break;
			}
			HWND window = NULL;
			ole_window->GetWindow(&window);//For windowless objects, this method should always fail and return E_FAIL
			if(!window)
			{
				break;
			}

			HWND hWnd = NULL;

			if (!bVisible)
			{
				HWND hFocusWnd = ::GetFocus();
				hWnd = hFocusWnd;
				while (hWnd && hWnd != window)
				{
					hWnd = ::GetParent(hWnd);
				}
			}
			ShowWindow(window, bVisible?SW_SHOW:SW_HIDE);
			if (hWnd == window) // 避免主窗口失去焦点
			{
				::SetFocus(GetContainer()->OnGetHWnd());
			}
		} while (false);
	}

	IUnknown * DUIActiveX::GetIUnknow()
	{
		if (!m_axContainer)
		{
			return NULL;
		}
		return m_axContainer->ActiveXControl();
	}
}// namespace DM

