#include "Plugin_ExpandAfx.h"
#include "DM3DAnimate.h"

namespace DM
{
	BEGIN_MSG_MAP(DM3DAnimate)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_PAINT(OnPaint)
		CHAIN_MSG_MAP(DMAnimateWndBase)
	END_MSG_MAP()
	DM3DAnimate::DM3DAnimate()
	{
		m_pOwner = NULL;
		memset(&m_3dparam,0,sizeof(m_3dparam));
		memset(&m_3doffset,0,sizeof(m_3doffset));
		m_count	   = 10;
		m_curFrame = 0;
		m_byAlpha  = 0xff;
	}


	DMCode DM3DAnimate::AnimateInit(IDMAnimateOwner*pOwner,IDMCanvas* pStartCanvas,IDMCanvas* pEndCanvas,WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pOwner||NULL == pStartCanvas)
			{
				break;
			}
			m_pOwner		= pOwner;


			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
			CSize szStart;
			pStartCanvas->GetSize(szStart);
			pRender->CreateCanvas(szStart.cx,szStart.cy,&m_pStartCanvas);
			m_pStartCanvas->BitBlt(pStartCanvas,0,0,CRect(0,0,szStart.cx,szStart.cy));
			if (NULL == pEndCanvas||pStartCanvas==pEndCanvas)
			{
				m_pEndCanvas = m_pStartCanvas;
			}
			else
			{
				CSize szEnd;
				pEndCanvas->GetSize(szEnd);
				pRender->CreateCanvas(szEnd.cx,szEnd.cy,&m_pEndCanvas);
				m_pEndCanvas->BitBlt(pEndCanvas,0,0,CRect(0,0,szEnd.cx,szEnd.cy));
			}
			if (wp!=0)
			{
				PARAM3DOFF* p = (PARAM3DOFF*)wp;
				memcpy(&m_3doffset, p, sizeof(IMAGE3D::PARAM3DTRANSFORM));
			}
			if (lp!=0)
			{
				m_count = (int)lp;
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DM3DAnimate::AnimateBegin(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner||NULL==m_pStartCanvas)
			{
				break;
			}
			CRect rcInit = m_pOwner->GetAnimateOwnerWnd()->m_rcWindow;
			HWND hWnd = m_pOwner->GetAnimateOwnerWnd()->GetContainer()->OnGetHWnd();
			DMANI_ClientToScreen(hWnd, rcInit);
			
			if(!DMSUCCEEDED(Create(rcInit)))
			{
				break;
			}

			// 创建转换画布
			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
			pRender->CreateCanvas(rcInit.Width(), rcInit.Height(),&m_pTransCanvas);

			SetWindowPos(HWND_TOPMOST,rcInit.left,rcInit.top,0,0,SWP_NOSIZE|SWP_NOSENDCHANGING|SWP_NOOWNERZORDER|SWP_SHOWWINDOW|SWP_NOACTIVATE);
			m_pOwner->AnimateBegin_Callback(this,0,0);

			BeginFade(false,LOWORD(wp),HIWORD(wp));// 开始渐显
			if (lp!=0)
			{
				SetTimer(TIMER_3D,(UINT_PTR)lp);
			}
			else
			{
				SetTimer(TIMER_3D,100);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DM3DAnimate::AnitmateGet(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (wp!=0)
			{
				IMAGE3D::PARAM3DTRANSFORM* p = (IMAGE3D::PARAM3DTRANSFORM*)wp;
				memcpy(&m_3dparam, p, sizeof(IMAGE3D::PARAM3DTRANSFORM));
			}
			DMSmartPtrT<IDMBitmap> pBmp;
			if (lp!=0)
			{
				pBmp = (IDMBitmap*)lp;
			}

			IMAGE3D::C3DTransform image3d;
			CSize szImg;
			szImg.cx = pBmp->GetWidth();
			szImg.cy = pBmp->GetHeight();

			LPVOID pBitSrc = pBmp->GetPixelBits();
			LPVOID pBitDst = pBmp->GetPixelBits();
			image3d.SetImage((LPBYTE)pBitSrc,(LPBYTE)pBitDst,szImg.cx,szImg.cy,32);
			image3d.Render(m_3dparam);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DM3DAnimate::AnimateEnd(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			BeginFade(true,LOWORD(wp),HIWORD(wp));// 开始渐显
			if (false==m_bFadeing&&IsWindow())
			{
				m_pOwner->AnimateEnd_Callback(this,0,0);
				DestroyWindow();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DM3DAnimate::EndFadeFinish()
	{
		if (false==m_bFadeing&&IsWindow())
		{
			m_pOwner->AnimateEnd_Callback(this,0,0);
			DestroyWindow();
		}
		return DM_ECODE_OK;
	}

	void DM3DAnimate::OnPaint(HDC hdc)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		ClientToScreen(rcClient);
		do 
		{
			BLENDFUNCTION bf = {AC_SRC_OVER,0,0xff,AC_SRC_ALPHA};
			if (m_bFadeing)// 渐显渐隐中
			{
				UpdateLayerWnd(m_pStartCanvas,(byte)m_byFadeAlpha);
				break;
			}
			UpdateLayerWnd(m_pTransCanvas,0xc0);
		} while (false);
	}

	void DM3DAnimate::OnTimer(UINT_PTR idEvent)
	{
		do 
		{
			__super::OnTimer(idEvent);
			if (m_bFadeing)
			{
				break;
			}

			if (TIMER_3D!=idEvent)
			{
				break;
			}

			if (m_curFrame>=m_count)
			{
				KillTimer(TIMER_3D);  
				m_pOwner->AnimateMid_Callback(this,0,0);
				m_curFrame = 0;
				break;
			}

			DMSmartPtrT<IDMCanvas> pCanvas;
			if (m_curFrame<m_count/2)
			{
				m_3dparam.nRotateX += m_3doffset.x;
				m_3dparam.nRotateY += m_3doffset.y;
				m_3dparam.nRotateZ += m_3doffset.z;
				m_3dparam.nOffsetZ += m_3doffset.f;
				pCanvas = m_pStartCanvas;
			}
			else
			{
				m_3dparam.nRotateX -= m_3doffset.x;
				m_3dparam.nRotateY -= m_3doffset.y;
				m_3dparam.nRotateZ -= m_3doffset.z;
				m_3dparam.nOffsetZ -= m_3doffset.f;
				pCanvas	= m_pEndCanvas;
			}
			

			IMAGE3D::C3DTransform image3d;
			CSize szImg;
			m_pStartCanvas->GetSize(szImg);

			LPVOID pBitSrc = m_pStartCanvas->GetPixelBits(NULL);
			LPVOID pBitDst = m_pTransCanvas->GetPixelBits(NULL);
			image3d.SetImage((LPBYTE)pBitSrc,(LPBYTE)pBitDst,szImg.cx,szImg.cy,32);
			image3d.Render(m_3dparam);
			if (IsWindow())
			{
				Invalidate(false);
			}
			m_curFrame++;
		} while (false);
	}

}//namespace DM