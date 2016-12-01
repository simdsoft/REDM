#include "DMDesignerAfx.h"
#include "DUIDragFrame.h"

DragMeta::DragMeta(int idx/*=-1*/,HCURSOR hCursor /*= ::LoadCursor(NULL,IDC_ARROW)*/)
{
	m_Index		 = idx;
	m_hCursor	 = hCursor;
	m_Clr		 = PBGRA(0,0,0xff,0xff);
	m_GrayClr	 = PBGRA(198,198,198,0xff);
	m_bDragDot	 = true;
	m_bEnable	 = true;     
}

DMCode DragMeta::OnRender(IDMCanvas* pCanvas)
{
	if (0 == m_Index)
	{
		AutoDrawRoundRect(pCanvas,m_Clr,PS_SOLID,1,m_Rect,CPoint(0,0));
	}
	else if (m_Index<9)
	{
		AutoDrawRoundRect(pCanvas,m_bEnable?m_Clr:m_GrayClr,PS_SOLID,1,m_Rect,CPoint(0,0));
	}
	else 
	{
		if (m_pSkin)
		{
			m_pSkin->Draw(pCanvas,m_Rect,m_bEnable?0:1);
		}
	}

	return DM_ECODE_OK;
}

DMCode DragMeta::SetCurSor()
{
	if (m_bEnable)
	{
		::SetCursor(m_hCursor);
	}
	return DM_ECODE_OK;
}

// DUIDragFrame--------------------------------------
#define  DRAGSIZE    6
DUIDragFrame::DUIDragFrame()
{
	m_pCurdragMeta = NULL;
	m_dragMetaCount = 10;
	m_pData = NULL;
	m_bMain = false;
	m_bDown = false;
	m_pDUIXmlInfo->m_bFocusable = true;

	LinkDragMetaEvent();
}
 
DMCode DUIDragFrame::InitDragFrame(ObjTreeData* pData,CRect& rcLayout)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == pData)
		{
			break;
		}
		DUIWindow* &pDUIWnd = pData->m_pDUIWnd;
		DUIRoot* &pMainWnd = pData->m_pRootWnd;
		bool bMain = pDUIWnd == pMainWnd;// Root也算为Main
		bool bAllGray = false;
		if (!bMain)
		{
			if (false == IsSupportPos(pData))
			{
				bAllGray = true;///不支持的，以全灰显示
			}
		}
		CRect rcDrag = pDUIWnd->m_rcWindow;
		if (rcDrag.IsRectEmpty())
		{
			break;
		}
		// 支持面板变换
		pDUIWnd->GetContainer()->OnClientToScreen(rcDrag);
		g_pMainWnd->ScreenToClient(rcDrag);
		InitDragMeta(rcDrag,bMain,bAllGray);

		// 计算自身frame的区域
		rcLayout = CalcDragFrameRect(rcDrag);

		m_pData = pData;
		m_bMain = bMain;
		m_pRightXml = g_pMainWnd->m_pDesignerXml->m_pRighXml;
		m_pLayout = dynamic_cast<Layout*>(pData->m_pDUIWnd->m_pLayout.get());

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

bool DUIDragFrame::IsSupportPos(ObjTreeData* pData)
{
	bool bRet = false;
	do 
	{
		DMSmartPtrT<Layout>	pLayout;
		pLayout = dynamic_cast<Layout*>(pData->m_pDUIWnd->m_pLayout.get());
		DUIWindow* &pDUIWnd = pData->m_pDUIWnd;
		if (0 == _wcsicmp(pData->m_pDUIWnd->V_GetClassName(),DUIRoot::GetClassName())// 切换到Root层
			||pData->m_pDUIWnd->m_bFloatLayout// 绝对布局
			||pData->m_pDUIWnd->DM_IsParentFlowLayout()//流式布局的子项
			||4 != pLayout->m_nCount// 不是四点式锚点布局
			||0 == _wcsicmp(pData->m_pDUIWnd->V_GetClassName(),DUITabPage::GetClassName())// 切换到TabPage层
			)
		{
			break;
		}
		bRet = true;
	} while (false);
	return bRet;
}

// private
void DUIDragFrame::LinkDragMetaEvent()
{
	m_dragMetas[0] = DragMeta(0, ::LoadCursor(NULL,IDC_HAND));
	m_dragMetas[0].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragMove,this);

	m_dragMetas[1] = DragMeta(1, ::LoadCursor(NULL,IDC_SIZEWE));
	m_dragMetas[1].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragLeft,this);

	m_dragMetas[2] = DragMeta(2, ::LoadCursor(NULL,IDC_SIZENWSE));
	m_dragMetas[2].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragLeftTop,this);

	m_dragMetas[3] = DragMeta(3, ::LoadCursor(NULL,IDC_SIZENS));
	m_dragMetas[3].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragTop,this);

	m_dragMetas[4] = DragMeta(4, ::LoadCursor(NULL,IDC_SIZENESW));
	m_dragMetas[4].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragRightTop,this);

	m_dragMetas[5] = DragMeta(5, ::LoadCursor(NULL,IDC_SIZEWE));
	m_dragMetas[5].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragRight,this);

	m_dragMetas[6] = DragMeta(6, ::LoadCursor(NULL,IDC_SIZENWSE));
	m_dragMetas[6].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragRightBottom,this);

	m_dragMetas[7] = DragMeta(7, ::LoadCursor(NULL,IDC_SIZENS));
	m_dragMetas[7].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragBottom,this);

	m_dragMetas[8] = DragMeta(8, ::LoadCursor(NULL,IDC_SIZENESW));
	m_dragMetas[8].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragLeftBottom,this);

	m_dragMetas[9] = DragMeta(9, ::LoadCursor(NULL,IDC_HAND));
	m_dragMetas[9].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragBox,this);
}

void DUIDragFrame::InitDragMeta(CRect Rect,bool bMain,bool bAllGray)
{
	m_dragMetas[0].m_Rect = Rect;
	Rect.InflateRect(DRAGSIZE, DRAGSIZE);

	m_dragMetas[1].m_Rect = Rect;
	m_dragMetas[1].m_Rect.top += (Rect.Height() - DRAGSIZE) / 2;
	m_dragMetas[1].m_Rect.bottom = m_dragMetas[1].m_Rect.top + DRAGSIZE;
	m_dragMetas[1].m_Rect.right = m_dragMetas[1].m_Rect.left + DRAGSIZE;

	CRect rrr = m_dragMetas[1].m_Rect;

	m_dragMetas[2].m_Rect = Rect;
	m_dragMetas[2].m_Rect.bottom = Rect.top + DRAGSIZE;
	m_dragMetas[2].m_Rect.right = Rect.left + DRAGSIZE;

	rrr = m_dragMetas[2].m_Rect;

	m_dragMetas[3].m_Rect = Rect;
	m_dragMetas[3].m_Rect.left += (Rect.Width() - DRAGSIZE) / 2;
	m_dragMetas[3].m_Rect.right = m_dragMetas[3].m_Rect.left + DRAGSIZE;
	m_dragMetas[3].m_Rect.bottom = Rect.top + DRAGSIZE;

	m_dragMetas[4].m_Rect = Rect;
	m_dragMetas[4].m_Rect.left = Rect.right - DRAGSIZE;
	m_dragMetas[4].m_Rect.bottom = m_dragMetas[4].m_Rect.top + DRAGSIZE;

	// Right
	m_dragMetas[5].m_Rect = Rect;
	m_dragMetas[5].m_Rect.top += (Rect.Height() - DRAGSIZE) / 2;
	m_dragMetas[5].m_Rect.bottom = m_dragMetas[5].m_Rect.top + DRAGSIZE;
	m_dragMetas[5].m_Rect.left = Rect.right - DRAGSIZE;

	// RightBottom
	m_dragMetas[6].m_Rect = Rect;
	m_dragMetas[6].m_Rect.top = Rect.bottom - DRAGSIZE;
	m_dragMetas[6].m_Rect.left = Rect.right - DRAGSIZE;

	// Bottom
	m_dragMetas[7].m_Rect = Rect;
	m_dragMetas[7].m_Rect.left += (Rect.Width() - DRAGSIZE) / 2;
	m_dragMetas[7].m_Rect.right = m_dragMetas[7].m_Rect.left + DRAGSIZE;
	m_dragMetas[7].m_Rect.top = Rect.bottom - DRAGSIZE;

	// LeftBottom
	m_dragMetas[8].m_Rect = Rect;
	m_dragMetas[8].m_Rect.right = Rect.left + DRAGSIZE;
	m_dragMetas[8].m_Rect.top = Rect.bottom - DRAGSIZE;

	// DragMode
	m_dragMetas[9].m_Rect.right = m_dragMetas[2].m_Rect.left;
	m_dragMetas[9].m_Rect.bottom = m_dragMetas[2].m_Rect.top;
	m_dragMetas[9].m_Rect.left = m_dragMetas[2].m_Rect.left - 24;
	m_dragMetas[9].m_Rect.top = m_dragMetas[2].m_Rect.top - 24;
	m_dragMetas[9].m_Clr = PBGRA(0,0,0XFF,0XFF);
	m_dragMetas[9].m_pSkin = g_pDMApp->GetSkin(L"ds_drag_move");

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

DragMeta* DUIDragFrame::HittestDragMeta(CPoint pt,bool bMain)
{
	int iCount = m_dragMetaCount;

	if (bMain)
	{
		iCount -= 1;// 9不显示了
	}

	for (int i = 1; i < iCount; ++i)
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

void DUIDragFrame::OnDragLeft(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.left, NULL);
	}
}

void DUIDragFrame::OnDragLeftTop(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.left, (int*)&rect.top);
	}
}

void DUIDragFrame::OnDragTop(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, NULL, (int*)&rect.top);
	}
}

void DUIDragFrame::OnDragRightTop(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.right, (int*)&rect.top);
	}
}

void DUIDragFrame::OnDragRight(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.right, NULL);
	}
}

void DUIDragFrame::OnDragRightBottom(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.right, (int*)&rect.bottom);
	}
}

void DUIDragFrame::OnDragBottom(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, NULL, (int*)&rect.bottom);
	}
}

void DUIDragFrame::OnDragLeftBottom(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.left, (int*)&rect.bottom);
	}
}

void DUIDragFrame::OnDragBox(DragMeta& meta, int iAction)
{
	DM::LPTVITEMEX pData = g_pMainWnd->m_pDesignerXml->m_pObjTree->GetItem(g_pMainWnd->m_pDesignerXml->m_hObjSel);
	if (DMTVEXLock_UnLocked != pData->iLockValue||m_bMain)
	{	
		SetLogInfo(L"控件处于被锁定状态，无法拖动!");
		return;
	}
	if (meta.m_bEnable)
	{
		CRect rect = m_StartDragRc; // rect总是表示当前大小
		int x = m_TrackDragPt.x - m_StartDragPt.x;
		int y = m_TrackDragPt.y - m_StartDragPt.y;
		rect.OffsetRect(x,y);
		SetElementRect(rect);
	}
}

void DUIDragFrame::OnDragMove(DragMeta& meta, int iAction)
{
	//OnDragBox(meta,iAction);// 暂时屏蔽
}

CRect DUIDragFrame::CalcDragFrameRect(CRect rcDrag)
{
	CRect rcLayout;
	rcLayout = rcDrag;
	rcLayout.InflateRect(DRAGSIZE,DRAGSIZE);
	rcLayout.left -= 24;//24为box图标的大小
	rcLayout.top  -= 24;
	return rcLayout;
}

// public
void DUIDragFrame::DM_OnPaint(IDMCanvas* pCanvas)
{
	int i = 1;
	if (m_bDown&&NULL != m_pCurdragMeta&&0 == m_pCurdragMeta->m_Index)
	{
		i = 0;
	}
	for (;i<=9;i++)
	{
		m_dragMetas[i].OnRender(pCanvas);
	}
}

void DUIDragFrame::OnLButtonDown(UINT nFlags,CPoint pt)
{
	DragMeta* pMeta = HittestDragMeta(pt,false);
	if (pMeta)
	{
		DV_SetFocusWnd();
		DM_SetCapture();
		m_bDown = true;
		pMeta->SetCurSor();
		m_StartDragPt = pt;
		m_TrackDragPt = m_StartDragPt;
		m_StartDragRc = m_dragMetas[0].m_Rect;
		if (0 == pMeta->m_Index&&m_pData)
		{
			if (false == m_pData->m_pRootWnd->MLDownInSelMode(pt,m_pData->m_pDUIWnd))//说明点击在同一个dragframe上
			{
				DM_Invalidate();
			}
		}
	}
}

void DUIDragFrame::OnLButtonUp(UINT nFlags,CPoint pt)
{
	m_bDown = false;
	g_pMainWnd->m_pDesignerXml->m_pObjEditor->SelFrameInAllMode();// 故意在弹出后才更新选中框,这样可以在拖动时以选中框为参考
	DM_ReleaseCapture();
	DM_Invalidate();
}

void DUIDragFrame::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if (m_pData)
	{
		m_pData->m_pRootWnd->OnRButtonDown(nFlags,pt);
	}
}

void DUIDragFrame::OnMouseMove(UINT nFlags,CPoint pt)
{
	if (!m_bDown)
	{
		m_pCurdragMeta = HittestDragMeta(pt,false);
		if (m_pCurdragMeta)
		{
			m_pCurdragMeta->SetCurSor();
		}	
	}
	else if (m_bDown&&m_pCurdragMeta)
	{
		m_pCurdragMeta->SetCurSor();
		m_TrackDragPt = pt;
		if (m_TrackDragPt != m_StartDragPt)
		{
			m_pCurdragMeta->m_DragMetaAction(*m_pCurdragMeta,0);
		}
	}
}

DUIWND DUIDragFrame::HitTestPoint(CPoint pt,bool bFindNoMsg)
{
	if (!m_bDown)
	{// 拖动过程时,保持m_pCurdragMeta不变
		m_pCurdragMeta = HittestDragMeta(pt,false);
	}

	if (NULL != m_pCurdragMeta&&0 == m_pCurdragMeta->m_Index)
	{
		m_pData->m_pRootWnd->HitTestPoint(pt,true);
	}
	else if (NULL == m_pCurdragMeta)
	{
		return m_pData->m_pRootWnd->HitTestPoint(pt,true);// 注意，这里使用了return,因为它不在dragframe的范围内了
	}
	if (m_pData&&NULL != m_pCurdragMeta && 0 != m_pCurdragMeta->m_Index)
	{// 在DUIObjEditor保证了m_pDragFrame->DM_SetWndToTop(),从而HitTestPoint从dragframe->designeditor
		g_pMainWnd->m_pDesignerXml->m_pObjEditor->HoverInSelOrAddMode(m_pData->m_pDUIWnd);
	}
	
	return m_hDUIWnd;
}

void DUIDragFrame::InternalHandleDrag(CRect& rect, int* pHori, int* pVert)
{
	do 
	{
		DM::LPTVITEMEX pData = g_pMainWnd->m_pDesignerXml->m_pObjTree->GetItem(g_pMainWnd->m_pDesignerXml->m_hObjSel);
		if (DMTVEXLock_UnLocked != pData->iLockValue)
		{
			SetLogInfo(L"控件处于被锁定状态，无法拖动!");
			break;
		}

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

		if (!m_bMain)
		{
			rect.NormalizeRect();
			SetElementRect(rect);
			
			break;
		}

		if (pHori)
		{
			SetElementWidth(rect.Width());
		}

		if (pVert)
		{
			SetElementHeight(rect.Height());
		}
	} while (false);
}

void DUIDragFrame::SetElementWidth(int wid)
{
	if (m_bMain)
	{
		m_pRightXml->m_pHostAttr->OnSetWidth(wid);
	}
}

void DUIDragFrame::SetElementHeight(int hei)
{
	if (m_bMain)
	{
		m_pRightXml->m_pHostAttr->OnSetHei(hei);
	}
}

bool DUIDragFrame::SetElementRect(CRect rect)
{
	bool bRet = false;
	do 
	{
		CRect rcNew = rect;
		// 关键是重设置4个POS_ITEM把rcOld转换成rcNew
		POS_ITEM PosLeft,PosTop,PosRight,PosBtm;
		memcpy(&PosLeft,&m_pLayout->m_Left,sizeof(POS_ITEM));
		memcpy(&PosTop,&m_pLayout->m_Top,sizeof(POS_ITEM));
		memcpy(&PosRight,&m_pLayout->m_Right,sizeof(POS_ITEM));
		memcpy(&PosBtm,&m_pLayout->m_Bottom,sizeof(POS_ITEM));
		bRet = m_pLayout->ParseItemByRect(rcNew,PosLeft,PosTop,PosRight,PosBtm);

		m_pRightXml->m_pPosAttr->m_pPos->m_pItem[0]->OnItemChange(&PosLeft);
		m_pRightXml->m_pPosAttr->m_pPos->m_pItem[1]->OnItemChange(&PosTop);
		m_pRightXml->m_pPosAttr->m_pPos->m_pItem[2]->OnItemChange(&PosRight);
		m_pRightXml->m_pPosAttr->m_pPos->m_pItem[3]->OnItemChange(&PosBtm);
	} while (false);
	return bRet;
}
