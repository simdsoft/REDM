#include "DmMainAfx.h"
#include "DUIRectTracker.h"

namespace DM
{
	DMDragMeta::DMDragMeta(int idx/*=-1*/,HCURSOR hCursor /*= ::LoadCursor(NULL,IDC_ARROW)*/)
	{
		m_Index		 = idx;
		m_hCursor	 = hCursor;
		m_crDot[0]	 = PBGRA(0,0,0,0xff);
		m_crDot[1]	 = PBGRA(198,198,198,0xff);
		m_crBox[0]   = PBGRA(0xff,0xff,0xff,0xff);
		m_crBox[1]   = PBGRA(0xff, 0, 0, 0xff);
		m_bDragDot	 = true;
		m_bEnable	 = true;     
	}

	void DMDragMeta::OnRender(IDMCanvas* pCanvas)
	{
		if (0 == m_Index)
		{
			CRect rcFill = m_Rect;
			AutoDrawRoundRect(pCanvas, m_crBox[0], PS_SOLID,1, rcFill, CPoint(0, 0));
			AutoDrawRoundRect(pCanvas, m_crBox[1], PS_DASH,1,rcFill,CPoint(0,0));
		}
		else if (m_Index<9)
		{
			AutoDrawRoundRect(pCanvas,m_bEnable?m_crDot[0]:m_crDot[1],PS_SOLID,2,m_Rect,CPoint(0,0));
		}
		else 
		{
			if (m_pMoveSkin)
			{
				m_pMoveSkin->Draw(pCanvas,m_Rect,m_bEnable?0:1);
			}
		}
	}

	void DMDragMeta::OnRenderCursor()
	{
		if (m_bEnable)
		{
			::SetCursor(m_hCursor);
		}
	}

	DMCode DMDragMeta::AutoDrawRoundRect(IDMCanvas*pCanvas,DMColor TextClr,int iStyle,int iWidth,LPCRECT lpRect,POINT &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->CreateRegObj((void**)&pRender,NULL, DMREG_Render);
			DMSmartPtrT<IDMPen> pPen;
			pRender->CreatePen(TextClr,iStyle,iWidth, &pPen);

			DMSmartPtrT<IDMPen> pOldPen;
			pCanvas->SelectObject(pPen,(IDMMetaFile**)&pOldPen);;

			pCanvas->DrawRoundRect(lpRect,pt);

			pCanvas->SelectObject(pOldPen);	
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}


	//-----------------------------------------------------------------------------------------
	DUIRectTracker::DUIRectTracker()
	{
		m_crDot[0] = PBGRA(0, 0, 0, 0xff);
		m_crDot[1] = PBGRA(198, 198, 198, 0xff);
		m_crBox[0] = PBGRA(0xff, 0xff, 0xff, 0xff);
		m_crBox[1] = PBGRA(0xff, 0, 0, 0xff);
		m_dotWidth   = 6;
		m_bDown = false;
		m_bMain = false;
		m_bAllGray = false;
		m_bFirstInit = true;
		m_pCurdragMeta = NULL;
		m_dragMetaCount = 10;
		m_pDUIXmlInfo->m_bFocusable = true;
		LinkDragMetaEvent();

		// recttracker
		DMADDEVENT(DMEventRTBOXChangingArgs::EventID);
		DMADDEVENT(DMEventRTBOXChangedArgs::EventID);
	}

	//---------------------------------------------------
	// Function Des: 对外接口 methods
	//---------------------------------------------------
#pragma region Public methods
	bool DUIRectTracker::SetBoxRect(CRect& rect, bool bFire)
	{
		bool bRet = true;
		CRect rcBoxNew = rect;
		do
		{
			CRect rcBoxOld = m_StartDragRc;
			if (bFire)
			{
				DMEventRTBOXChangingArgs Evt0(this);
				Evt0.m_rcBoxOld = rcBoxOld;
				Evt0.m_rcBoxNew = rcBoxNew;
				DV_FireEvent(Evt0);
				if (TRUE == Evt0.m_bCancel)
				{
					break;
				}
				rcBoxNew = Evt0.m_rcBoxNew; // 外部可改变
			}
			
			InitDragMeta(rcBoxNew, m_bMain, m_bAllGray);
			CRect rcInvalid = m_rcWindow;
			m_rcWindow = CalcTrackerRect(rcBoxNew);
			rcInvalid.UnionRect(rcInvalid, m_rcWindow);
			CRect rcParent;
			DM_GetWindow(GDW_PARENT)->DV_GetClientRect(rcParent);
			rcInvalid.IntersectRect(rcInvalid,rcParent);
			DM_GetWindow(GDW_PARENT)->DM_InvalidateRect(rcInvalid);

			if (bFire)
			{
				DMEventRTBOXChangedArgs Evt1(this);
				Evt1.m_rcBoxOld = rcBoxOld;
				Evt1.m_rcBoxNew = rcBoxNew;
				Evt1.m_message = WM_MOUSEMOVE;
				DV_FireEvent(Evt1);
			}
		} while (false);
		if (rcBoxNew != rect)
		{
			rect = rcBoxNew;
			bRet = false;
		}
		return bRet;
	}

	void DUIRectTracker::InitDragMeta(CRect rcBox,bool bMain/*=false*/,bool bAllGray/*=false*/)
	{
		m_bMain = bMain;
		m_bAllGray = bAllGray;
		m_dragMetas[0].m_Rect = rcBox;
		rcBox.InflateRect(m_dotWidth, m_dotWidth);

		m_dragMetas[1].m_Rect = rcBox;
		m_dragMetas[1].m_Rect.top += (rcBox.Height() - m_dotWidth) / 2;
		m_dragMetas[1].m_Rect.bottom = m_dragMetas[1].m_Rect.top + m_dotWidth;
		m_dragMetas[1].m_Rect.right = m_dragMetas[1].m_Rect.left + m_dotWidth;

		CRect rrr = m_dragMetas[1].m_Rect;

		m_dragMetas[2].m_Rect = rcBox;
		m_dragMetas[2].m_Rect.bottom = rcBox.top + m_dotWidth;
		m_dragMetas[2].m_Rect.right = rcBox.left + m_dotWidth;

		rrr = m_dragMetas[2].m_Rect;

		m_dragMetas[3].m_Rect = rcBox;
		m_dragMetas[3].m_Rect.left += (rcBox.Width() - m_dotWidth) / 2;
		m_dragMetas[3].m_Rect.right = m_dragMetas[3].m_Rect.left + m_dotWidth;
		m_dragMetas[3].m_Rect.bottom = rcBox.top + m_dotWidth;

		m_dragMetas[4].m_Rect = rcBox;
		m_dragMetas[4].m_Rect.left = rcBox.right - m_dotWidth;
		m_dragMetas[4].m_Rect.bottom = m_dragMetas[4].m_Rect.top + m_dotWidth;

		// Right
		m_dragMetas[5].m_Rect = rcBox;
		m_dragMetas[5].m_Rect.top += (rcBox.Height() - m_dotWidth) / 2;
		m_dragMetas[5].m_Rect.bottom = m_dragMetas[5].m_Rect.top + m_dotWidth;
		m_dragMetas[5].m_Rect.left = rcBox.right - m_dotWidth;

		// RightBottom
		m_dragMetas[6].m_Rect = rcBox;
		m_dragMetas[6].m_Rect.top = rcBox.bottom - m_dotWidth;
		m_dragMetas[6].m_Rect.left = rcBox.right - m_dotWidth;

		// Bottom
		m_dragMetas[7].m_Rect = rcBox;
		m_dragMetas[7].m_Rect.left += (rcBox.Width() - m_dotWidth) / 2;
		m_dragMetas[7].m_Rect.right = m_dragMetas[7].m_Rect.left + m_dotWidth;
		m_dragMetas[7].m_Rect.top = rcBox.bottom - m_dotWidth;

		// LeftBottom
		m_dragMetas[8].m_Rect = rcBox;
		m_dragMetas[8].m_Rect.right = rcBox.left + m_dotWidth;
		m_dragMetas[8].m_Rect.top = rcBox.bottom - m_dotWidth;

		for (int i=0; i<9; i++)
		{
			m_dragMetas[i].m_crDot[0] = m_crDot[0];
			m_dragMetas[i].m_crDot[1] = m_crDot[1];
			m_dragMetas[i].m_crBox[0] = m_crBox[0];
			m_dragMetas[i].m_crBox[1] = m_crBox[1];
		}

		// DragMode
		m_dragMetas[9].m_pMoveSkin = m_pMoveSkin;
		if (m_dragMetas[9].m_pMoveSkin)
		{
			CSize size;
			m_dragMetas[9].m_pMoveSkin->GetStateSize(size);
			m_dragMetas[9].m_Rect.right = m_dragMetas[2].m_Rect.left;
			m_dragMetas[9].m_Rect.bottom = m_dragMetas[2].m_Rect.top;
			m_dragMetas[9].m_Rect.left = m_dragMetas[2].m_Rect.left - size.cx;
			m_dragMetas[9].m_Rect.top = m_dragMetas[2].m_Rect.top - size.cy;
			m_dragMetas[9].m_crDot[0] = m_crDot[0];
			m_dragMetas[9].m_crDot[1] = m_crDot[1];
		}
		
		for (int i=0;i<m_dragMetaCount;i++)
		{
			m_dragMetas[i].m_bEnable = true;
		}

		m_dragMetas[1].m_bEnable = !bMain;
		m_dragMetas[2].m_bEnable = !bMain;
		m_dragMetas[3].m_bEnable = !bMain;
		m_dragMetas[4].m_bEnable = !bMain;
		m_dragMetas[8].m_bEnable = !bMain;
		m_dragMetas[9].m_bEnable = !bMain;

		if (bAllGray)
		{
			for (int i=0;i<m_dragMetaCount;i++)
			{
				m_dragMetas[i].m_bEnable = false;
			}
		}
	}

	DMDragMeta* DUIRectTracker::HitTestDragMeta(CPoint pt)
	{
		for (int i=1; i<m_dragMetaCount; ++i)
		{
			if (m_dragMetas[i].m_Rect.PtInRect(pt))
			{
				return &m_dragMetas[i];
			}
		}

		if (m_dragMetas[0].m_Rect.PtInRect(pt))
		{
			return &m_dragMetas[0];
		}

		return NULL;
	}


	CRect DUIRectTracker::CalcTrackerRect(CRect rcBox)
	{
		CRect rcTracker = rcBox;
		rcTracker.InflateRect(m_dotWidth,m_dotWidth);
		if (m_pMoveSkin)
		{//  左上角预留mov的大小
			CSize sz;
			m_pMoveSkin->GetStateSize(sz);
			rcTracker.left -= sz.cx; 
			rcTracker.top -= sz.cy;
		}
		return rcTracker;
	}

	CRect DUIRectTracker::CalcBoxRect(CRect rcTracker)
	{
		CRect rcBox = rcTracker;
		if (m_pMoveSkin)
		{//  左上角预留mov的大小
			CSize sz;
			m_pMoveSkin->GetStateSize(sz);
			rcBox.left += sz.cx; 
			rcBox.top += sz.cy;
		}
		rcBox.DeflateRect(m_dotWidth,m_dotWidth);
		return rcBox;
	}
#pragma endregion

	//---------------------------------------------------
	// Function Des: 辅助 methods
	//---------------------------------------------------
#pragma region Helpers
	void DUIRectTracker::LinkDragMetaEvent()
	{
		m_dragMetas[0] = DMDragMeta(0, ::LoadCursor(NULL,IDC_HAND));
		m_dragMetas[0].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragBox,this);

		m_dragMetas[1] = DMDragMeta(1, ::LoadCursor(NULL,IDC_SIZEWE));
		m_dragMetas[1].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragLeft,this);

		m_dragMetas[2] = DMDragMeta(2, ::LoadCursor(NULL,IDC_SIZENWSE));
		m_dragMetas[2].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragLeftTop,this);

		m_dragMetas[3] = DMDragMeta(3, ::LoadCursor(NULL,IDC_SIZENS));
		m_dragMetas[3].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragTop,this);

		m_dragMetas[4] = DMDragMeta(4, ::LoadCursor(NULL,IDC_SIZENESW));
		m_dragMetas[4].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragRightTop,this);

		m_dragMetas[5] = DMDragMeta(5, ::LoadCursor(NULL,IDC_SIZEWE));
		m_dragMetas[5].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragRight,this);

		m_dragMetas[6] = DMDragMeta(6, ::LoadCursor(NULL,IDC_SIZENWSE));
		m_dragMetas[6].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragRightBottom,this);

		m_dragMetas[7] = DMDragMeta(7, ::LoadCursor(NULL,IDC_SIZENS));
		m_dragMetas[7].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragBottom,this);

		m_dragMetas[8] = DMDragMeta(8, ::LoadCursor(NULL,IDC_SIZENESW));
		m_dragMetas[8].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragLeftBottom,this);

		m_dragMetas[9] = DMDragMeta(9, ::LoadCursor(NULL,IDC_HAND));
		m_dragMetas[9].m_DragMetaAction = DMBind(&DUIRectTracker::OnDragMove,this);
	}

	void DUIRectTracker::OnDragBox(DMDragMeta& meta, int iAction)
	{
		OnDragMove(meta,iAction);
	}
	
	void DUIRectTracker::OnDragLeft(DMDragMeta& meta, int iAction)
	{
		if (meta.m_bEnable)
		{
			CRect rect;
			InternalHandleDrag(rect, (int*)&rect.left, NULL);
		}
	}

	void DUIRectTracker::OnDragLeftTop(DMDragMeta& meta, int iAction)
	{
		if (meta.m_bEnable)
		{
			CRect rect;
			InternalHandleDrag(rect, (int*)&rect.left, (int*)&rect.top);
		}
	}

	void DUIRectTracker::OnDragTop(DMDragMeta& meta, int iAction)
	{
		if (meta.m_bEnable)
		{
			CRect rect;
			InternalHandleDrag(rect, NULL, (int*)&rect.top);
		}
	}

	void DUIRectTracker::OnDragRightTop(DMDragMeta& meta, int iAction)
	{
		if (meta.m_bEnable)
		{
			CRect rect;
			InternalHandleDrag(rect, (int*)&rect.right, (int*)&rect.top);
		}
	}

	void DUIRectTracker::OnDragRight(DMDragMeta& meta, int iAction)
	{
		if (meta.m_bEnable)
		{
			CRect rect;
			InternalHandleDrag(rect, (int*)&rect.right, NULL);
		}
	}

	void DUIRectTracker::OnDragRightBottom(DMDragMeta& meta, int iAction)
	{
		if (meta.m_bEnable)
		{
			CRect rect;
			InternalHandleDrag(rect, (int*)&rect.right, (int*)&rect.bottom);
		}
	}

	void DUIRectTracker::OnDragBottom(DMDragMeta& meta, int iAction)
	{
		if (meta.m_bEnable)
		{
			CRect rect;
			InternalHandleDrag(rect, NULL, (int*)&rect.bottom);
		}
	}

	void DUIRectTracker::OnDragLeftBottom(DMDragMeta& meta, int iAction)
	{
		if (meta.m_bEnable)
		{
			CRect rect;
			InternalHandleDrag(rect, (int*)&rect.left, (int*)&rect.bottom);
		}
	}

	void DUIRectTracker::OnDragMove(DMDragMeta& meta, int iAction)
	{
		if (meta.m_bEnable)
		{
			CRect rect = m_StartDragRc; // rect总是表示当前大小
			int x = m_TrackDragPt.x - m_StartDragPt.x;
			int y = m_TrackDragPt.y - m_StartDragPt.y;
			rect.OffsetRect(x,y);

			SetBoxRect(rect);
		}
	}

	void DUIRectTracker::InternalHandleDrag(CRect& rect, int* pHori, int* pVert)
	{
		do 
		{
			int x = (pHori ? (m_TrackDragPt.x - m_StartDragPt.x) : 0);
			int y = (pVert ? (m_TrackDragPt.y - m_StartDragPt.y) : 0);	

			rect = m_StartDragRc; // rect总是表示当前大小

			if (pHori)
			{
				*pHori += x;
			}

			if (pVert)
			{
				*pVert += y;
			}

			rect.NormalizeRect();
			SetBoxRect(rect);
		} while (false);
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
#pragma region MsgDispatch
	void DUIRectTracker::DM_OnPaint(IDMCanvas* pCanvas)
	{
		CRect rcParent;
		DM_GetWindow(GDW_PARENT)->DV_GetClientRect(rcParent);
		pCanvas->PushClip(rcParent);
		for (int i=0; i<=9; i++)
		{
			m_dragMetas[i].OnRender(pCanvas);
		}
		pCanvas->PopClip();
	}

	void DUIRectTracker::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		DMDragMeta* pMeta = HitTestDragMeta(pt);
		if (pMeta)
		{
			DV_SetFocusWnd();
			DM_SetCapture();
			m_bDown = true;
			pMeta->OnRenderCursor();
			m_StartDragPt = pt;
			m_TrackDragPt = m_StartDragPt;
			if (0 == pMeta->m_Index)
			{// 显示框选区
				DM_Invalidate();
			}
			m_StartDragRc = m_dragMetas[0].m_Rect;
		}
	}

	void DUIRectTracker::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		m_bDown = false;
		CRect EndDragRc = m_dragMetas[0].m_Rect;
		if (EndDragRc != m_StartDragRc)
		{
			DMEventRTBOXChangedArgs Evt1(this);
			Evt1.m_rcBoxOld = m_StartDragRc;
			Evt1.m_rcBoxNew = EndDragRc;
			Evt1.m_message = WM_LBUTTONUP;
			DV_FireEvent(Evt1);
		}
		DM_ReleaseCapture();
		DM_Invalidate();
	}

	void DUIRectTracker::OnMouseMove(UINT nFlags,CPoint pt)
	{
		if (!m_bDown)
		{
			m_pCurdragMeta = HitTestDragMeta(pt);
			if (m_pCurdragMeta)
			{
				m_pCurdragMeta->OnRenderCursor();
			}	
		}
		else if (m_bDown&&m_pCurdragMeta)
		{
			m_pCurdragMeta->OnRenderCursor();
			m_TrackDragPt = pt;
			if (m_TrackDragPt != m_StartDragPt)
			{
				m_pCurdragMeta->m_DragMetaAction(*m_pCurdragMeta,0);
			}
		}
	}

	void DUIRectTracker::OnSize(UINT nType, CSize size)
	{
		__super::OnSize(nType, size);
		if (DM_IsLayoutFinished())
		{
			m_bFloatLayout = true;					///< 首次布局完成后,它的布局就变成了绝对布局
			CRect rcBox = CalcBoxRect(m_rcWindow);
			rcBox.NormalizeRect();
			CRect rcTracker = CalcTrackerRect(rcBox);
			if (!rcTracker.EqualRect(m_rcWindow))
			{// 说明m_rcWindow过小.
#if defined(_DEBUG)
				DUIWindow *pMain = g_pDMApp->FindDUIWnd(1);
				if (pMain && (0 != pMain->GetData("1C3A5807-CEE1-438C-BC46-624F74BDC8D1").CompareNoCase("440A2781-8BC2-4AC4-8225-9AC451FE42B4"))) // 设计器debug下不弹框
				{
					DMASSERT_EXPR(0,L"不要把DUIRectTracker的区域设置得过小");
				}
#endif	
				m_rcWindow = rcTracker;
			}
			InitDragMeta(rcBox, m_bMain, m_bAllGray);
		}
	}
#pragma endregion

	DMCode DUIRectTracker::OnAttributeMoveSkin(LPCSTR lpszValue, bool bLoadXml)
	{
		m_pMoveSkin = g_pDMApp->GetSkin(lpszValue);
		if (!bLoadXml)
		{
			CRect rcBox = CalcBoxRect(m_rcWindow);
			InitDragMeta(rcBox, m_bMain, m_bAllGray);
		}
		return DM_ECODE_NOXMLLOADPARENTREFRESH;
	}

	DMCode DUIRectTracker::OnAttributeDotClr(LPCSTR lpszValue, bool bLoadXml)
	{
		DMColor clrFill = m_crDot[0];
		dm_parsecolor(lpszValue,m_crDot[0]);
		if (!bLoadXml && m_crDot[0] != clrFill)
		{
			for (int i=0; i<9; i++)
			{
				m_dragMetas[i].m_crDot[0] = m_crDot[0];
			}		
			return DM_ECODE_NOXMLLOADREFRESH;
		}
		return DM_ECODE_OK;
	}

	DMCode DUIRectTracker::OnAttributeDotGrayClr(LPCSTR lpszValue, bool bLoadXml)
	{
		DMColor clrFill = m_crDot[1];
		dm_parsecolor(lpszValue,m_crDot[1]);
		if (!bLoadXml && m_crDot[1] != clrFill)
		{
			for (int i=0; i<9; i++)
			{
				m_dragMetas[i].m_crDot[1] = m_crDot[1];
			}
			return DM_ECODE_NOXMLLOADREFRESH;
		}
		return DM_ECODE_OK;
	}

	DMCode DUIRectTracker::OnAttributeBoxSolidClr(LPCSTR lpszValue, bool bLoadXml)
	{
		DMColor clrBox = m_crBox[0];
		dm_parsecolor(lpszValue,m_crBox[0]);
		if (!bLoadXml && m_crBox[0] != clrBox)
		{
			for (int i=0; i<9; i++)
			{
				m_dragMetas[i].m_crBox[0] = m_crBox[0];
			}		
			return DM_ECODE_NOXMLLOADREFRESH;
		}
		return DM_ECODE_OK;
	}

	DMCode DUIRectTracker::OnAttributeBoxDashClr(LPCSTR lpszValue, bool bLoadXml)
	{
		DMColor clrBox = m_crBox[1];
		dm_parsecolor(lpszValue, m_crBox[1]);
		if (!bLoadXml && m_crBox[1] != clrBox)
		{
			for (int i = 0; i < 9; i++)
			{
				m_dragMetas[i].m_crBox[1] = m_crBox[1];
			}
			return DM_ECODE_NOXMLLOADREFRESH;
		}
		return DM_ECODE_OK;
	}

	DMCode DUIRectTracker::OnAttributeDotWidth(LPCSTR lpszValue, bool bLoadXml)
	{
		int dotWidth = m_dotWidth;
		dm_parseint(lpszValue, m_dotWidth);
		if (!bLoadXml && dotWidth != m_dotWidth)
		{
			CRect rcBox = CalcBoxRect(m_rcWindow);
			InitDragMeta(rcBox, m_bMain, m_bAllGray);
			return DM_ECODE_NOXMLLOADPARENTREFRESH;
		}
		return DM_ECODE_OK;
	}

	DMCode DUIRectTracker::OnAttributeIsMain(LPCSTR lpszValue, bool bLoadXml)
	{
		bool bMain = m_bMain;
		dm_parsebool(lpszValue,m_bMain);
		if (!bLoadXml && bMain != m_bMain)
		{
			CRect rcBox = CalcBoxRect(m_rcWindow);
			InitDragMeta(rcBox, m_bMain, m_bAllGray);
			return DM_ECODE_NOXMLLOADREFRESH;
		}
		return DM_ECODE_OK;
	}

	DMCode DUIRectTracker::OnAttributeIsAllGray(LPCSTR lpszValue, bool bLoadXml)
	{
		bool bAllGray = m_bAllGray;
		dm_parsebool(lpszValue,m_bAllGray);
		if (!bLoadXml && bAllGray != m_bAllGray)
		{
			CRect rcBox = CalcBoxRect(m_rcWindow);
			InitDragMeta(rcBox, m_bMain, m_bAllGray);
			return DM_ECODE_NOXMLLOADREFRESH;
		}
		return DM_ECODE_OK;
	}
}