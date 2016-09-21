#include "DmMainAfx.h"
#include "DMHWndHelper.h"

namespace DM
{

	//---------------------------------------DMHWnd_Data-----------------------
	DMHWnd_Data::DMHWnd_Data()
	{
		m_lpszXmlId		= NULL;
		m_bTrackFlag    = false;
	}

	DMHWnd_Data::~DMHWnd_Data()
	{
	
	}

	//---------------------------------------DMHWnd_XmlInfo-----------------------
	DMHWnd_XmlInfo::DMHWnd_XmlInfo(DMHWnd* pOwner)
	{
		m_pOwner = pOwner;
		ResetXmlInfo();
	}

	DMHWnd_XmlInfo::~DMHWnd_XmlInfo()
	{
		ResetXmlInfo();
	}

	void DMHWnd_XmlInfo::ResetXmlInfo()
	{
		m_szMin.SetSize(0, 0);
		m_szMax.SetSize(-1, -1);// -1表示不限制
		m_szInit.SetSize(200, 200);

		m_bResizable    = false;
		m_bTranslucent	= false;

		m_byAlpha       = 0xFF;
		m_H             = 0;
		m_S             = 100;
		m_L             = 100;
	}

	DMCode DMHWnd_XmlInfo::OnAttributeMinSize(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			dm_parsesize(lpszValue,m_szMin);
			if (-1!=m_szMax.cx&&m_szMin.cx>m_szMax.cx)
			{
				DMASSERT_EXPR(0,L"最小宽度大于最大宽度了！");
				break;
			}
			if (-1!=m_szMax.cy&&m_szMin.cy>m_szMax.cy)
			{
				DMASSERT_EXPR(0,L"最小高度大于最大高度了！");
				break;
			}

			if (!bLoadXml&&m_pOwner&&m_pOwner->IsWindow())
			{
				CRect rcWnd;
				m_pOwner->GetWindowRect(rcWnd);
				bool bUpdateSize = false;
				if (rcWnd.Width()<m_szMin.cx)
				{
					rcWnd.right = rcWnd.left+m_szMin.cx;
					bUpdateSize = true;
				}
				if (rcWnd.Height()<m_szMin.cy)
				{
					rcWnd.bottom = rcWnd.top+m_szMin.cy;
					bUpdateSize = true;
				}
				if (bUpdateSize)
				{
					m_pOwner->MoveWindow(rcWnd);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd_XmlInfo::OnAttributeMaxSize(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			dm_parsesize(lpszValue,m_szMax);
			if (-1!=m_szMax.cx&&m_szMin.cx>m_szMax.cx)
			{
				DMASSERT_EXPR(0,L"最小宽度大于最大宽度了！");
				break;
			}
			if (-1!=m_szMax.cy&&m_szMin.cy>m_szMax.cy)
			{
				DMASSERT_EXPR(0,L"最小高度大于最大高度了！");
				break;
			}

			if (!bLoadXml&&m_pOwner&&m_pOwner->IsWindow())
			{
				CRect rcWnd;
				m_pOwner->GetWindowRect(rcWnd);
				bool bUpdateSize = false;
				if (-1!=m_szMax.cx&&rcWnd.Width()>m_szMax.cx)
				{
					rcWnd.right = rcWnd.left+m_szMax.cx;
					bUpdateSize = true;
				}
				if (-1!=m_szMax.cy&&rcWnd.Height()>m_szMin.cy)
				{
					rcWnd.bottom = rcWnd.top+m_szMin.cy;
					bUpdateSize = true;
				}
				if (bUpdateSize)
				{
					m_pOwner->MoveWindow(rcWnd);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd_XmlInfo::OnAttributeResize(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			dm_parsebool(lpszValue,m_bResizable);
			if (!bLoadXml&&m_pOwner&&m_pOwner->IsWindow())
			{
				DWORD dwStyle = WS_MAXIMIZEBOX | WS_THICKFRAME;
				if (m_bResizable)
				{
					m_pOwner->ModifyStyle(0,dwStyle);
				}
				else
				{
					m_pOwner->ModifyStyle(dwStyle,0);
				}
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd_XmlInfo::OnAttributeTranslucent(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			dm_parsebool(lpszValue,m_bTranslucent);
			if (!bLoadXml&&m_pOwner&&m_pOwner->IsWindow())
			{
				if (m_bTranslucent)
				{
					if (m_pOwner->m_dummyWnd.IsWindow())
					{
						iErr = DM_ECODE_OK;
						break;
					}
					DWORD dwExStyle = m_pOwner->GetExStyle();
					dwExStyle &= ~WS_EX_LAYERED;
					m_pOwner->SetWindowLongPtr(GWL_EXSTYLE,dwExStyle);// 需先去掉再设置
					m_pOwner->SetWindowLongPtr(GWL_EXSTYLE, dwExStyle|WS_EX_LAYERED);
					m_pOwner->m_dummyWnd.CreateWindowEx(LPCWSTR(g_pDMAppData->m_Atom),L"DM_DUMMY_WND",WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE,0,0,10,10,m_pOwner->m_hWnd,NULL);
					m_pOwner->m_dummyWnd.SetWindowLongPtr(GWL_EXSTYLE,m_pOwner->m_dummyWnd.GetWindowLongPtr(GWL_EXSTYLE) | WS_EX_LAYERED);
					::SetLayeredWindowAttributes(m_pOwner->m_dummyWnd.m_hWnd,0,0,LWA_ALPHA);
					m_pOwner->m_dummyWnd.ShowWindow(SW_SHOWNOACTIVATE);
					m_pOwner->UpdateHWnd(NULL,m_pOwner->m_rcWindow);
				}
				else
				{	
					DWORD dwExStyle = m_pOwner->GetExStyle();
					if (0xff==m_byAlpha)
					{
						dwExStyle &= ~WS_EX_LAYERED;
						m_pOwner->SetWindowLongPtr(GWL_EXSTYLE,dwExStyle);
					}
					else
					{
						dwExStyle |= WS_EX_LAYERED;
						m_pOwner->SetWindowLongPtr(GWL_EXSTYLE,dwExStyle);
						::SetLayeredWindowAttributes(m_pOwner->m_hWnd,0,m_byAlpha,LWA_ALPHA);
					}
					if (m_pOwner->m_dummyWnd.IsWindow())
					{
						m_pOwner->m_dummyWnd.DestroyWindow();
					}
					m_pOwner->RedrawAll();
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd_XmlInfo::OnAttributeAlpha(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int temp = 0xff;
			dm_parseint(lpszValue,temp);
			m_byAlpha = temp;
			if (!bLoadXml&&m_pOwner&&m_pOwner->IsWindow())
			{
				if (m_bTranslucent)// 如假窗口存在，则不需设置透明度
				{
					m_pOwner->DM_Invalidate();
					iErr = DM_ECODE_OK;
					break;
				}

				DWORD dwExStyle = m_pOwner->GetExStyle();
				if (0xff!=m_byAlpha)
				{
					dwExStyle |= WS_EX_LAYERED;
					m_pOwner->SetWindowLongPtr(GWL_EXSTYLE,dwExStyle);
					::SetLayeredWindowAttributes(m_pOwner->m_hWnd,0,m_byAlpha,LWA_ALPHA);
				}
				else
				{
					dwExStyle &= ~WS_EX_LAYERED;
					m_pOwner->SetWindowLongPtr(GWL_EXSTYLE,dwExStyle);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd_XmlInfo::OnAttributeH(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int H = m_H;
			dm_parseint(lpszValue, H);
			if (H<-180||H>180)
			{
				break;
			}
			if (!bLoadXml&&m_pOwner&&m_pOwner->IsWindow()&&H!=m_H)
			{
				m_H = H;
				m_pOwner->DM_Invalidate();
			}
			m_H = H;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd_XmlInfo::OnAttributeS(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int S = m_S;
			dm_parseint(lpszValue, S);
			if (S<0||S>200)
			{
				break;
			}
			if (!bLoadXml&&m_pOwner&&m_pOwner->IsWindow()&&S!=m_S)
			{
				m_S = S;
				m_pOwner->DM_Invalidate();
			}
			m_S = S;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd_XmlInfo::OnAttributeL(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int L = m_L;
			dm_parseint(lpszValue, L);
			if (L<0||L>200)
			{
				break;
			}
			if (!bLoadXml&&m_pOwner&&m_pOwner->IsWindow()&&L!=m_L)
			{
				m_L = L;
				m_pOwner->DM_Invalidate();
			}
			m_L = L;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd_XmlInfo::OnAttributeRegDraw(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMSmartPtrT<IDMDraw> pDraw;
			if (!DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pDraw,lpszValue,DMREG_Draw)))
			{
				CStringW szInfo = lpszValue;
				szInfo += L"注册draw失败";
				DMASSERT_EXPR(0,szInfo);
				break;
			}

			m_pOwner->m_pDraw.reset(pDraw.get());
			m_strRegDraw = lpszValue;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	// --------------------透明窗口蒙板---------------------------------------
	BEGIN_MSG_MAP(DMDummyWnd)
		MSG_WM_PAINT(OnPaint)
	END_MSG_MAP()
	DMDummyWnd::DMDummyWnd(DMHWnd* pOwner)
		:m_pOwner(pOwner)
	{
	}

	void DMDummyWnd::OnPaint(HDC hdc)
	{	
		m_pOwner->OnPrint(NULL,DUMMY_PAINT);
	}
}