#include "DmMainAfx.h"
#include "DMDrawImpl.h"

namespace DM
{
	DMDrawImpl::DMDrawImpl()
	{
		g_pDMApp->GetDefRegObj((void**)&m_pRender, DMREG_Render);
		m_pRender->CreateCanvas(0,0,&m_pMemCanvas);
		m_bNeedRepaint = m_bOnlyOneRectRepaint = false;
	}

	DMCode DM::DMDrawImpl::DrawBackground(IDMCanvas* pCanvas,IDMRegion* lpRgn,DUIWindow* pWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DUIWindow *pTopWnd = pWnd->DM_GetTopParentWnd();
			if (NULL == pTopWnd)
			{
				break;
			}
			pCanvas->PushClip(lpRgn);
			DMDRAW_STATE Render_State = DRAW_START;
			_OnRender(pCanvas,lpRgn,pTopWnd,pTopWnd,pWnd,Render_State);// 从顶层父窗口绘制到本窗口
			pCanvas->PopClip();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMDrawImpl::DrawForeground(IDMCanvas* pCanvas,IDMRegion* lpRgn,DUIWindow* pWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcDraw = pWnd->m_rcWindow;
			if (lpRgn)
			{
				lpRgn->CombineRect(rcDraw,RGN_AND);
			}
			CRect rcInvalid;
			lpRgn->GetRgnBox(&rcInvalid);
			rcDraw.IntersectRect(rcDraw,rcInvalid);
			DUIWindow* pStart = pWnd->DM_GetNextVisibleWnd(pWnd,rcInvalid);
			if (NULL == pStart)
			{
				break;
			}

			pCanvas->PushClip(lpRgn);
			DMDRAW_STATE Render_State = DRAW_START;
			_OnRender(pCanvas,lpRgn,pWnd,pStart,NULL,Render_State);
			pCanvas->PopClip();

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMDrawImpl::Draw(IDMCanvas* pCanvas, IDMRegion* lpRgn,DUIWindow* pWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMDRAW_STATE Render_State = DRAW_START;
			if (NULL == lpRgn)
			{	
				DMSmartPtrT<IDMRegion> pAllRgn;
				m_pRender->CreateRegion(&pAllRgn);
				CRect rcWnd;
				pWnd->DV_GetWindowRect(rcWnd);
				pAllRgn->CombineRect(rcWnd, RGN_OR);
				iErr = _OnRender(pCanvas,pAllRgn,pWnd,pWnd,NULL,Render_State);
			}	
			else
			{
				iErr = _OnRender(pCanvas,lpRgn,pWnd,pWnd,NULL,Render_State);
			}
		}while(false);
		return iErr;
	}

	DMCode DMDrawImpl::InvalidateRect(DUIWND hDUIWnd,LPCRECT lpRect,int fnCombineMode)
	{// 目标:m_pInvalidRegion保存所有无效区,m_bOnlyOneRectRepaint记录是否为无效矩形区

		CRect rcInvalid = m_rcCanvas;
		do 
		{
			//1.判断是否无效矩形
			if (NULL == lpRect)
			{// 为NULL时使用画布大小,此时只有无效矩形m_rcCanvas
				m_bOnlyOneRectRepaint = true;
				m_rcOnlyOne = m_rcCanvas;
				break;
			}

			//2.判断是否为空
			rcInvalid = lpRect;
			rcInvalid.left = rcInvalid.left>0?rcInvalid.left:0;
			rcInvalid.top = rcInvalid.top>0?rcInvalid.top:0;
			if (rcInvalid.IsRectEmpty())
			{
				rcInvalid.SetRectEmpty();
				break;
			}
			//4.仅有一个Rect时,前面排除lpRect为NULL的状态,此时只有无效矩形
			if (RGN_COPY == fnCombineMode
				||RGN_AND == fnCombineMode
				)
			{
				m_bOnlyOneRectRepaint = true;
				m_rcOnlyOne = rcInvalid;
				break;
			}

			//5. 前面条件限制了lpRect不为NULL,且不是全刷新,且不是RGN_COPY||RGN_AND
			if (m_bOnlyOneRectRepaint)
			{
				CRect rcNew = m_rcOnlyOne;
				rcNew.UnionRect(rcInvalid,m_rcOnlyOne);
				if (rcNew.EqualRect(m_rcOnlyOne)// 新的无效矩形在原无效矩形以内
					||rcNew.EqualRect(rcInvalid))// 旧的无效矩形在新无效矩形以内
				{
					m_rcOnlyOne = rcNew;
					break;
				}
			}
			m_bOnlyOneRectRepaint = false;// 不是一个无效矩形
		} while (false);

		// 最后进入无效Rgn设置
		if (NULL == m_pInvalidRegion)
		{
			m_pRender->CreateRegion(&m_pInvalidRegion);
		}
		if (!rcInvalid.IsRectEmpty())
		{
			m_pInvalidRegion->CombineRect(&rcInvalid,fnCombineMode);// 组合无效区
			m_bNeedRepaint = true;
		}
		return DM_ECODE_OK;
	}

	int DMDrawImpl::GetInvalidateRect(DM::CArray<CRect>& Array)
	{
		do 
		{// 和Render函数逻辑保持一致
			if (!m_bNeedRepaint)
			{
				break;
			}

			if (m_bOnlyOneRectRepaint)
			{
				if (!m_rcOnlyOne.IsRectEmpty())
				{
					Array.Add(m_rcOnlyOne);
				}
				break;
			}

			CRect rcInvalid;
			m_pInvalidRegion->GetRgnBox(&rcInvalid);
			if (rcInvalid.IsRectEmpty())
			{
				break;
			}
			if (rcInvalid.Width()+100<m_rcCanvas.Width()
				||rcInvalid.Height()+100<m_rcCanvas.Height())						// 大画布,无效区很小（gdi内部会优化）,绘制的效率还是要低于无效区大小的画布上直接绘制
			{
				int iNum = 0;
				DMBufT<RECT> pRect;
				if (DMSUCCEEDED(m_pInvalidRegion->GetRectNum(iNum)) // 成功获取
					&&iNum<10&&iNum>1                               // 无效矩形列表在[2,10)之间,1表示m_pInvalidRegion就是一个矩形
					&&NULL!=pRect.Allocate(iNum)                    // 内存分配成功
					&&DMSUCCEEDED(m_pInvalidRegion->GetRectArray(pRect,iNum))
					)
				{
					for (int i=0;i<iNum;i++)
					{
						CRect rcRect = pRect+i;
						Array.Add(rcRect);
					}
				}
				else
				{
					Array.Add(rcInvalid);
				}
			}
			else
			{
				Array.Add(rcInvalid);
			}

		} while (false);
		return (int)Array.GetCount();
	}

	DMCode DMDrawImpl::Render(DUIWindow* pWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!m_bNeedRepaint)
			{
				break;
			}
			m_bNeedRepaint = false;
			if (m_bOnlyOneRectRepaint)// 只有一个无效矩形
			{
				if (m_rcOnlyOne.IsRectEmpty())
				{
					break;
				}
				if (m_rcOnlyOne != m_rcCanvas)
				{
					m_pMemCanvas->PushClip(m_rcOnlyOne,RGN_COPY);	
				}

				m_pMemCanvas->ClearRect(m_rcOnlyOne,0);		
				DMSmartPtrT<IDMFont> pOldFont;
				m_pMemCanvas->SelectObject(g_pDMApp->GetFont(L""),(IDMMetaFile**)&pOldFont);
				m_pMemCanvas->SelectObject(pOldFont);

				Draw(m_pMemCanvas,m_pInvalidRegion,pWnd);

				if (m_rcOnlyOne != m_rcCanvas)
				{
					m_pMemCanvas->PopClip();	
				}
				m_pMemCanvas->SelectObject(pOldFont);
				m_rcOnlyOne.SetRectEmpty();
			}
			else// 可能有多个无效矩形的异形无效区
			{
				CRect rcInvalid;
				m_pInvalidRegion->GetRgnBox(&rcInvalid);
				m_pMemCanvas->PushClip(m_pInvalidRegion,RGN_COPY);						// 强制设置为拷贝区大小,注意不要传rcInvalid，因为rcInvalid可能包含了其他不需要刷新的子控件区域
				m_pMemCanvas->ClearRect(rcInvalid,0);									// 清除残留的alpha值
				DMSmartPtrT<IDMFont> pOldFont;
				m_pMemCanvas->SelectObject(g_pDMApp->GetFont(L""),(IDMMetaFile**)&pOldFont);
				if (rcInvalid.Width()+100<m_rcCanvas.Width()
					||rcInvalid.Height()+100<m_rcCanvas.Height())						// 大画布,无效区很小（gdi内部会优化）,绘制的效率还是要低于无效区大小的画布上直接绘制
				{
					int iNum = 0;
					DMBufT<RECT> pRect;
					if (DMSUCCEEDED(m_pInvalidRegion->GetRectNum(iNum)) // 成功获取
						&&iNum<10&&iNum>1                               // 无效矩形列表在[2,10)之间,1表示m_pInvalidRegion就是一个矩形
						&&NULL!=pRect.Allocate(iNum)                    // 内存分配成功
						&&DMSUCCEEDED(m_pInvalidRegion->GetRectArray(pRect,iNum))
						)
					{
						for (int i=0;i<iNum;i++)
						{
							CRect rcRect = pRect+i;
							DMSmartPtrT<IDMCanvas>   pTempCanvas;
							m_pRender->CreateCanvas(rcRect.Width(),rcRect.Height(),&pTempCanvas);
							DMSmartPtrT<IDMFont> pOldTempFont;
							pTempCanvas->SelectObject(g_pDMApp->GetFont(L""),(IDMMetaFile**)&pOldTempFont);
							pTempCanvas->OffsetViewportOrg(-rcRect.left,-rcRect.top);// 画布右移rcInvalid.left，下移rcInvalid.top
							DMSmartPtrT<IDMRegion> pRgn;
							m_pRender->CreateRegion(&pRgn);
							pRgn->CombineRect(rcRect);
							Draw(pTempCanvas,pRgn,pWnd);
							pTempCanvas->SelectObject(pOldTempFont);
							m_pMemCanvas->BitBlt(pTempCanvas,rcRect.left,rcRect.top,rcRect);
						}
					}
					else
					{
						Draw(m_pMemCanvas,m_pInvalidRegion,pWnd);
					}
				}
				else
				{
					Draw(m_pMemCanvas,m_pInvalidRegion,pWnd);
				}
				m_pMemCanvas->SelectObject(pOldFont);
				m_pMemCanvas->PopClip();		
			}
			m_pInvalidRegion->Clear();												// 清空刷新区
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMDrawImpl::GetCanvas(IDMCanvas** ppCanvas)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppCanvas
				||NULL == m_pMemCanvas)
			{
				break;
			}
			*ppCanvas = m_pMemCanvas;
			m_pMemCanvas->AddRef();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMDrawImpl::ResizeCanvas(SIZE size)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pMemCanvas)
			{
				iErr = m_pRender->CreateCanvas(size.cx,size.cy,&m_pMemCanvas);
			}
			else
			{
				iErr = m_pMemCanvas->Resize(size);
			}
			m_rcCanvas.SetRect(0,0,size.cx,size.cy);
		} while (false);
		return iErr;
	}

	// 辅助
	DMCode DMDrawImpl::_OnRender(IDMCanvas* pCanvas, IDMRegion* pRgn,DUIWindow* pWndCur,DUIWindow* pStart,DUIWindow *pEnd, DMDRAW_STATE& Draw_State)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pRgn										// 无效区为NULL
				||pRgn->IsEmpty()									// 无效区为空
				||DRAW_END == Draw_State							// 遍历到最后了
				||(DRAW_DRAWING == Draw_State && pWndCur == pEnd))  // 遍历到最后标志窗口了，最后标志窗口一般为NULL或起点窗口
			{
				Draw_State = DRAW_END;
				break;
			}

			CRect rcPlaceHolder;
			pWndCur->DV_GetWindowRect(&rcPlaceHolder);				// 获得占位大小，如不显示
			if (!pWndCur->DM_IsVisible(true)						// 窗口不可见
				||!pRgn->RectInRegion(&rcPlaceHolder)				// 占位和绘制区无交集
				||rcPlaceHolder.IsRectEmpty())                      // 占位为空
			{
				break;
			}

			if (DRAW_START == Draw_State&&pWndCur == pStart)
			{//0. 开始进行绘制,这是个轮询，pWndCur可以是pStart的父窗口（上层窗口），这里需遍历找到开始绘制点
				Draw_State = DRAW_DRAWING;
			}

			DMDRAW_STATE DrawBack = Draw_State;    // 保存当前的绘制状态,在递归结束后根据这个状态来判断是否需要绘制非客户区
			CRect rcClient;
			pWndCur->DV_GetClientRect(&rcClient);
			if (DRAW_DRAWING == DrawBack            // 绘制状态
				&&pRgn->RectInRegion(rcClient)		  // 子控件区域和绘制区有交集
				)
			{// 1.绘制自身客户区
				if (pWndCur->DV_IsOnlyDrawClient())
				{
					pCanvas->PushClip(&rcClient);
				}
				if (pWndCur->DM_IsDrawToCache())
				{
					CRect rcWnd = pWndCur->m_rcWindow;
					IDMCanvas* pCacheCanvas =  pWndCur->DM_GetCacheCanvas();
					if (pCacheCanvas&&pWndCur->DM_IsCacheDirty()) 
					{//1.1 缓存画布已脏，需要更新
						CPoint pt = -rcWnd.TopLeft();
						pCacheCanvas->SetViewportOrgEx(pt.x,pt.y);

						pCacheCanvas->BitBlt(pCanvas,rcWnd.left,rcWnd.top,&rcWnd,SRCCOPY);//把父窗口的内容复制过来。

						DMSmartPtrT<IDMFont> oldFont;
						DMSmartPtrT<IDMFont> pFont;
						DMColor crOld = pCanvas->GetTextColor();
						if (pWndCur->DV_IsNeedDrawEnviron())//是否需要准备绘制环境
						{
							pCanvas->GetObject((IDMMetaFile**)&pFont,DMF_FONT);// 取得传入的字体
							pCacheCanvas->SelectObject(pFont,(IDMMetaFile**)&oldFont);
							pCacheCanvas->SetTextColor(crOld);
						}

						pWndCur->DM_SendMessage(WM_ERASEBKGND, (WPARAM)pCacheCanvas);
						pWndCur->DM_SendMessage(WM_PAINT, (WPARAM)pCacheCanvas);

						if (pWndCur->DV_IsNeedDrawEnviron())
						{
							pCacheCanvas->SelectObject(oldFont);
							pCacheCanvas->SetTextColor(crOld);
						}

						pWndCur->DM_MarkCacheDirty(false);
					}
					pCanvas->BitBlt(pCacheCanvas,rcWnd.left,rcWnd.top,&rcWnd,SRCCOPY);
				}
				else
				{
					pWndCur->DM_SendMessage(WM_ERASEBKGND, (WPARAM)pCanvas);
					pWndCur->DM_SendMessage(WM_PAINT, (WPARAM)pCanvas);
				}
			}

			DUIDrawEnviron DrawEnviron;
			if (pWndCur->DV_IsNeedDrawEnviron())
			{			
				pWndCur->DV_PushDrawEnviron(pCanvas, DrawEnviron);    // 准备绘制环境
			}

			DUIWindow *pChild=pWndCur->DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild) 	           // 递归出口0
			{// 2. 递归绘制自身子窗口
				if (pChild==pEnd)
				{
					break;             // 递归出口1
				}
				_OnRender(pCanvas,pRgn,pChild,pStart,pEnd,Draw_State);
				if (Draw_State == DRAW_END)
				{
					break;             // 递归出口2
				}
				pChild=pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}

			if (pWndCur->DV_IsNeedDrawEnviron())
			{	
				pWndCur->DV_PopDrawEnviron(pCanvas, DrawEnviron);   // 结束绘制环境
			}
			if(Draw_State == DRAW_DRAWING&&pWndCur->DV_IsOnlyDrawClient())
			{
				pCanvas->PopClip();
			}

			if (DRAW_DRAWING == Draw_State)
			{// 3. 反向递归绘制非客户区，最后的子窗口最先绘非客户区
				pWndCur->DM_SendMessage(WM_NCPAINT, (WPARAM)pCanvas);
			}

			if (DRAW_DRAWING == Draw_State||Draw_State == DRAW_END)
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

}//namespace DM