#include "DmMainAfx.h"
#include "DUIActiveX.h"

namespace DM
{
	DUIActiveX::DUIActiveX() 
		: m_pAxContainer(new DMAxContainerImpl(this))
	{
		m_ClsId		 = CLSID_NULL;
		m_ClsCtx	 = CLSCTX_ALL;
		m_bDelayInit = true;
		m_bInit		 = false;
		m_bSupportMultThread = false;
		m_pDUIXmlInfo->m_bFocusable = true;
	}

	DUIActiveX::~DUIActiveX() 
	{
		DM_DELETE(m_pAxContainer);
	}

	bool DUIActiveX::SetActiveXVisible(bool bVisible,bool bFocus)
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pAxContainer)
			{
				break;
			}

			DMComPtr<IUnknown> pUnknown = m_pAxContainer->ActiveXControl();
			if (NULL == pUnknown)
			{
				break;
			}
			DMComQIPtr<IOleWindow> pOleWnd = pUnknown;
			if (NULL == pOleWnd)
			{
				break;
			}

			HWND hOleWnd = NULL;
			pOleWnd->GetWindow(&hOleWnd);//For windowless objects, this method should always fail and return E_FAIL：eg: IE [Shell Embedding] window
			if (!hOleWnd)
			{
				break;
			}

			HWND hWnd = NULL;
			if (!bVisible)
			{
				HWND hFocusWnd = ::GetFocus();
				hWnd = hFocusWnd;
				while (hWnd && hWnd != hOleWnd)
				{
					hWnd = ::GetParent(hWnd);
				}
			}

			ShowWindow(hOleWnd, bVisible?SW_SHOW:SW_HIDE);
			if (hWnd == hOleWnd) // 焦点在ActiveX的窗口上
			{
				if (false == bFocus)
				{
					::SetFocus(GetContainer()->OnGetHWnd());// 让主DUI窗口得到焦点
				}	
			}
			else
			{
				if (bFocus)
				{
					::SetFocus(hOleWnd);// 设置焦点到ActiveX的窗口上,如IE的[Shell Embedding]窗口
				}	
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUIActiveX::SetActiveXRect(CRect rcPos)
	{
		if (m_pAxContainer && m_pAxContainer->ActiveXControl())
		{
			return SUCCEEDED(m_pAxContainer->OnPosRectChange(rcPos));      
		}
		return false;
	}

	void DUIActiveX::OnSize(UINT nType, CSize size)
	{
		__super::OnSize(nType,size);
		SetActiveXRect(m_rcWindow);
	}

	void DUIActiveX::DM_OnPaint(IDMCanvas* pCanvas)
	{
		if (m_pAxContainer)
		{
			HDC hdc = pCanvas->GetDC();
			m_pAxContainer->ProcessPaint(hdc, m_rcWindow);
			pCanvas->ReleaseDC(hdc);
		}
	}

	void DUIActiveX::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		__super::OnShowWindow(bShow, nStatus);
		if (m_bDelayInit)
		{
			if (DM_IsVisible(true)&&false == m_bInit)// 窗口显示时才初始化
			{
				m_bInit = InitActiveX();
			}
		}
		else
		{
			if (false == m_bInit)
			{
				m_bInit = InitActiveX();
			}
		}

		SetActiveXVisible(DM_IsVisible(true));
	}

	LRESULT DUIActiveX::OnMouseEvent(UINT uMsg,WPARAM wp,LPARAM lp)
	{
		LRESULT lr = 0;
		do 
		{
			if (NULL == m_pAxContainer)
			{
				break;
			}
			
			if (!m_pAxContainer->ActiveXControl())
			{
				break;
			}

			if (WM_LBUTTONDOWN == uMsg)
			{
				DV_SetFocusWnd();
			}
			lr = m_pAxContainer->OnWindowMessage(uMsg, wp, lp);
		} while (false);
		return lr;
	}

	LRESULT DUIActiveX::OnKeyEvent(UINT uMsg,WPARAM wp,LPARAM lp)
	{
		LRESULT lr = 0;
		do 
		{
			if (NULL == m_pAxContainer)
			{
				break;
			}

			if (!m_pAxContainer->ActiveXControl())
			{
				break;
			}
			lr = m_pAxContainer->OnWindowMessage(uMsg, wp, lp);
		} while (false);
		return lr;
	}

	DMCode DUIActiveX::OnAttrClsid(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			if (NULL == pszValue||0 == wcslen(pszValue))
			{
				break;
			}
			OLECHAR szCLSID[200] = {0};
			wcscpy_s(szCLSID,pszValue);

			if (szCLSID[0] == L'{') 
			{
				::CLSIDFromString(szCLSID, &m_ClsId);
			}
			else
			{
				::CLSIDFromProgID(szCLSID, &m_ClsId);
			}
		} while (false);
		return DM_ECODE_OK;
	}

	// 辅助
	bool DUIActiveX::InitActiveX()
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pAxContainer)
			{
				break;
			}
			if (!m_pAxContainer->CreateControl(m_rcWindow,m_ClsId,m_ClsCtx,m_bSupportMultThread))
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);
			m_pAxContainer->ActivateAx(NULL);
			SetActiveXVisible(DM_IsVisible(true));
			m_pAxContainer->OnPosRectChange(m_rcWindow);
			bRet = true;
		} while (false);
		return bRet;
	}
}// namespace DM

