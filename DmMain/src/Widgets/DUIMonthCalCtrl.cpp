#include "DmMainAfx.h"
#include "DUIMonthCalCtrl.h"

namespace DM
{
	static CStringW WEEKS[] =
	{// 默认文本
		L"日", L"一", L"二", L"三", L"四", L"五", L"六",
	};

	DUIMonthCalCtrl::DUIMonthCalCtrl()
	{
		// header
		m_iHeaderHei = 24;
		m_iBtnOffset = 4;
		m_crHeaderBg.SetTextInvalid();
		m_pHeaderBgSkin = m_pBtnSkin[0] = m_pBtnSkin[1] = NULL;
		m_BtnSize.SetSize(-1,-1);
		m_dwBtnState[0] = m_dwBtnState[1] = DUIWNDSTATE_Normal;
		m_crHeaderText = PBGRA(00,00,00,0XFF);
		m_strHeaderTextFormat = L"%04d年%02d月";

		// title
		m_iTitleHei = 24;
		m_crTitleBg.SetTextInvalid();
		m_crTitleText = PBGRA(00,00,00,0XFF);
		m_pTitleBgSkin = NULL;
		for (int i=0; i<CALENDAR_COLUMNS; i++)
		{
			m_strWeekTitle[i] = WEEKS[i];
		}

		// cell
		m_bHideGrayCell = false;
		m_pCellSkin = NULL;
		for (int i=0; i<3; i++)
		{
			m_crGrayCellText[i] = PBGRA(0xD2,0xD2,0xD2,0xff);
			m_crCell[i].SetTextInvalid();
		}
		m_crCell[2] = PBGRA(00,0XDA,0XA8,0XFF);
		m_bShowJieri = true;
		m_crJieriText = PBGRA(0xF2,0XB1,0x50,0XFF);

		// today
		m_iTodayHei = 24;
		for (int i=0; i<3; i++)
		{
			m_crTodayText[i] = PBGRA(00,00,00,0XFF);
			m_crToday[i].SetTextInvalid();
		}
		m_pTodaySkin = NULL;
		m_strTodayTextFormat = L"今天:%04d/%02d/%02d";
		m_dwTodayState = DUIWNDSTATE_Normal;
		
		// other
		m_ihoverCell = -1;
		m_displayDate = DMDateTime::Today();
		UpdateDisplayCells();// 第一次初始化.
		m_iselCell = m_displayDate.GetDayCount()-m_dayCells[0][0].date.GetDayCount();

		DMADDEVENT(DMEventCalenderSetDateArgs::EventID);
		DMADDEVENT(DMEventCalenderClickDateChangedArgs::EventID);
	}


	//---------------------------------------------------
	// Function Des: Draw methods
	//---------------------------------------------------
#pragma region Interface
	DMDateTime DUIMonthCalCtrl::GetDisplayDate() const
	{
		return m_displayDate;
	}

	void DUIMonthCalCtrl::SetDisplayDate(DMDateTime date, bool bNotify)
	{
		DMDateTime oldDate = m_displayDate;
		m_displayDate = date;

		if (oldDate.GetYear() != m_displayDate.GetYear()
			|| oldDate.GetMonth() != m_displayDate.GetMonth())
		{// 切换一个月时需重置刷新.
			m_ihoverCell = -1;
			UpdateDisplayCells();
			DM_Invalidate();
		}

		int iOld = m_iselCell;
		m_iselCell = m_displayDate.GetDayCount()-m_dayCells[0][0].date.GetDayCount();
		if (m_iselCell != iOld)
		{
			if (iOld != -1)
			{
				DM_InvalidateRect(m_dayCells[iOld/CALENDAR_COLUMNS][iOld%CALENDAR_COLUMNS].rect);
			}
			if (m_iselCell != -1)
			{
				DM_InvalidateRect(m_dayCells[m_iselCell/CALENDAR_COLUMNS][m_iselCell%CALENDAR_COLUMNS].rect);
			}
		}
		
		if (bNotify)
		{
			DMEventCalenderSetDateArgs Evt(this);
			Evt.m_iYear = date.GetYear();
			Evt.m_iMonth = date.GetMonth();
			Evt.m_iDay = date.GetDay();
			DV_FireEvent(Evt);
		}
	}

	void DUIMonthCalCtrl::UpdateDisplayCells()
	{
		DMDateTime date = GetDisplayDate();
		date.SetMonthBegin();
		int dayofweek = date.GetWeekDay();// 这个月的第一天,0表示周日
		dayofweek = (0 == dayofweek) ? 7: dayofweek; 
		int indexofmoth = dayofweek + GetDisplayDate().GetMonthEndDay();// 这个月一共有多少天
		int i = 0;
		int iDay = 1;
		if (dayofweek > 0)
		{
			DMDateTime prevdate = date;
			prevdate.AddMonths(-1);
			int prevdaycount = prevdate.GetMonthEndDay();
			int prevcount = dayofweek;

			for (i = 0; i < dayofweek; ++i)
			{
				iDay = prevdaycount - prevcount + i  + 1;
				(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).date.SetDate(prevdate.GetYear(), prevdate.GetMonth(), iDay);
				(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).title.Format(L"%d", iDay);
				(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).jieri = CalcJieri(prevdate.GetMonth(), iDay);
				(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).bMark = false;
				(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).bGray = true; 
			}
		}


		for (i = dayofweek; i < indexofmoth; ++i)
		{
			iDay = i - dayofweek + 1;
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).date.SetDate(date.GetYear(), date.GetMonth(), iDay);
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).title.Format(L"%d",iDay);
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).jieri = CalcJieri(date.GetMonth(),iDay);
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).bMark = false;
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).bGray = false; 
		}

		DMDateTime nextdate = date;
		nextdate.AddMonths(1);
		for (; i<CALENDAR_ROWS*CALENDAR_COLUMNS; ++i)
		{
			iDay = i - indexofmoth + 1;
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).date.SetDate(nextdate.GetYear(), nextdate.GetMonth(), iDay);
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).title.Format(L"%d",iDay);
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).jieri = CalcJieri(nextdate.GetMonth(), iDay);
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).bMark = false;
			(m_dayCells[i/CALENDAR_COLUMNS][i%CALENDAR_COLUMNS]).bGray = true; 
		}
	}

	void DUIMonthCalCtrl::UpdateDisplayCellsRect()
	{
		DV_GetClientRect(m_rcCells);
		m_rcCells.top += m_iHeaderHei + m_iTitleHei;
		m_rcCells.bottom -= m_iTodayHei;

		int iCellWid = m_rcCells.Width()/CALENDAR_COLUMNS;
		int iCellHei = m_rcCells.Height()/6;

		for (int r=0; r<CALENDAR_ROWS ; r++)
		{
			for (int c=0; c<CALENDAR_COLUMNS ; c++)
			{
				m_dayCells[r][c].rect.SetRect(iCellWid*c, iCellHei*r, 
					iCellWid*c+iCellWid > m_rcCells.right ? m_rcCells.right : iCellWid*c+iCellWid,
					iCellHei*r+iCellHei > m_rcCells.bottom ? m_rcCells.bottom : iCellHei*r+iCellHei);
				m_dayCells[r][c].rect.OffsetRect(m_rcCells.TopLeft());
			}
		}
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: Draw methods
	//---------------------------------------------------
#pragma region Draw
	void DUIMonthCalCtrl::DrawHeader(IDMCanvas* pCanvas)
	{
		if (m_iHeaderHei<=0)
		{
			return;
		}

		byte alpha = 0xff;
		m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);

		// 1.绘制背景
		if (m_pHeaderBgSkin || !m_crHeaderBg.IsTextInvalid())
		{
			CRect rcHeader = CalcHeaderRect(HEADER);
			if (m_pHeaderBgSkin)
			{
				m_pHeaderBgSkin->Draw(pCanvas,rcHeader,0,alpha);
			}
			else
			{
				pCanvas->FillSolidRect(&rcHeader,m_crHeaderBg);
			}
		}

		// 2.绘制左右三角
		if (m_pBtnSkin[0])
		{
			CRect rcLeftBtn = CalcHeaderRect(LEFTBTN);
			m_pBtnSkin[0]->Draw(pCanvas,rcLeftBtn,IIF_STATE4(m_dwBtnState[0],0,1,2,3));
		}
		if (m_pBtnSkin[1])
		{
			CRect rcRightBtn = CalcHeaderRect(RIGHTBTN);
			m_pBtnSkin[1]->Draw(pCanvas,rcRightBtn,IIF_STATE4(m_dwBtnState[1],0,1,2,3));
		}

		//3.绘制中间
		CRect rcMid = CalcHeaderRect(MID);
		if (!rcMid.IsRectEmpty() && !m_strHeaderTextFormat.IsEmpty())
		{
			CStringW strHeaderText;
			strHeaderText.Format(m_strHeaderTextFormat, GetDisplayDate().GetYear(), GetDisplayDate().GetMonth());

			DMSmartPtrT<IDMFont> pOldFont;
			if (m_ftHeader)
			{
				pCanvas->SelectObject(m_ftHeader, (IDMMetaFile**)&pOldFont);
			}
			DMColor oldTextClr;
			if (!m_crHeaderText.IsTextInvalid())
			{
				oldTextClr = pCanvas->SetTextColor(m_crHeaderText);
			}
			pCanvas->DrawTextW(strHeaderText, -1, rcMid, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS, alpha);
			if (!m_crHeaderText.IsTextInvalid())
			{
				pCanvas->SetTextColor(oldTextClr);
			}
			if (m_ftHeader)
			{
				pCanvas->SelectObject(pOldFont);
			}
		}
	}

	void DUIMonthCalCtrl::DrawTitle(IDMCanvas* pCanvas)
	{
		if (m_iTitleHei<=0)
		{
			return;
		}

		byte alpha = 0xff;
		m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);

		CRect rcTitle = CalcTitleRect();

		// 1.绘制背景
		if (m_pTitleBgSkin || !m_crTitleBg.IsTextInvalid())
		{
			if (m_pTitleBgSkin)
			{
				m_pTitleBgSkin->Draw(pCanvas,rcTitle,0,alpha);
			}
			else
			{
				pCanvas->FillSolidRect(&rcTitle,m_crTitleBg);
			}
		}

		// 2.绘制文本
		DMSmartPtrT<IDMFont> pOldFont;
		if (m_ftTitle)
		{
			pCanvas->SelectObject(m_ftTitle, (IDMMetaFile**)&pOldFont);
		}
		DMColor oldTextClr;
		if (!m_crTitleText.IsTextInvalid())
		{
			oldTextClr = pCanvas->SetTextColor(m_crTitleText);
		}

		int iItemWid = rcTitle.Width()/CALENDAR_COLUMNS;
		CRect rcItem(rcTitle.left, rcTitle.top,rcTitle.left+iItemWid, rcTitle.bottom);
		for(int i=0; i <CALENDAR_COLUMNS; i++)
		{
			pCanvas->DrawTextW(m_strWeekTitle[i], m_strWeekTitle[i].GetLength(), rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER, alpha);
			rcItem.OffsetRect(iItemWid,0);
		}

		if (!m_crTitleText.IsTextInvalid())
		{
			pCanvas->SetTextColor(oldTextClr);
		}
		if (m_ftTitle)
		{
			pCanvas->SelectObject(pOldFont);
		}
	}

	void DUIMonthCalCtrl::DrawCell(IDMCanvas* pCanvas, int iRow, int iCol)
	{
		do 
		{
			if (iRow>=CALENDAR_ROWS || iCol>=CALENDAR_COLUMNS)
			{
				break;
			}
			DMCalendarCell &cell = m_dayCells[iRow][iCol];
			if (true == m_bHideGrayCell && cell.bGray)
			{
				break;
			}

			byte alpha = 0xff;
			m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);

			int iCell = iRow*CALENDAR_COLUMNS + iCol;
			DWORD dwState = (iCell==m_iselCell) ? 2 : (iCell==m_ihoverCell) ? 1 : 0;

			// 1. 绘制背景
			if (m_pCellSkin || !m_crCell[dwState].IsTextInvalid())
			{
				if (m_pCellSkin)
				{
					m_pCellSkin->Draw(pCanvas, m_dayCells[iRow][iCol].rect, dwState, alpha);
				}
				else
				{
					pCanvas->FillSolidRect(m_dayCells[iRow][iCol].rect, m_crCell[dwState]);
				}
			}

			// 2.绘制文本
			DMColor oldTextClr;
			if (cell.bGray)
			{
				oldTextClr = pCanvas->SetTextColor(m_crGrayCellText[dwState]);
			}
			CRect rcText = cell.rect;
			if (m_bShowJieri)
			{
				rcText.bottom = rcText.top + (cell.rect.Height()/3)*2;
			}
			pCanvas->DrawTextW(cell.title, -1, rcText, DT_SINGLELINE|DT_VCENTER|DT_CENTER, alpha);
			if (cell.bGray)
			{
				pCanvas->SetTextColor(oldTextClr);
			}

			// 3.绘制节日
			if (m_bShowJieri && !cell.jieri.IsEmpty())
			{
				CRect rcJieri(rcText.left, rcText.bottom, rcText.right, cell.rect.bottom);
				DMSmartPtrT<IDMFont> pOldFont;
				if (m_ftJieri)
				{
					pCanvas->SelectObject(m_ftJieri, (IDMMetaFile**)&pOldFont);
				}
				DMColor oldJieriClr;
				if (!m_crJieriText.IsTextInvalid())
				{
					oldJieriClr = pCanvas->SetTextColor(m_crJieriText);
				}

				pCanvas->DrawTextW(cell.jieri, -1, rcJieri, DT_SINGLELINE|DT_VCENTER|DT_CENTER, alpha);

				if (!m_crJieriText.IsTextInvalid())
				{
					pCanvas->SetTextColor(oldJieriClr);
				}
				if (m_ftJieri)
				{
					pCanvas->SelectObject(pOldFont);
				}
			}
		} while (false);
	}

	void DUIMonthCalCtrl::DrawToday(IDMCanvas* pCanvas)
	{
		if (m_iTodayHei<=0)
		{
			return;
		}

		byte alpha = 0xff;
		m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);

		CRect rcToday = CalcTodayRect();
		DWORD dwState = IIF_STATE3(m_dwTodayState,0,1,2);

		// 1.绘制背景
		if (m_pTodaySkin || !m_crToday[dwState].IsTextInvalid())
		{
			if (m_pTodaySkin)
			{
				m_pTodaySkin->Draw(pCanvas,rcToday,dwState,alpha);
			}
			else
			{
				pCanvas->FillSolidRect(&rcToday,m_crToday[dwState]);
			}
		}

		// 2.绘制文本
		if (!m_strTodayTextFormat.IsEmpty())
		{
			CStringW strTodayText;
			DMDateTime today = DMDateTime::Today();
			strTodayText.Format(m_strTodayTextFormat, today.GetYear(), today.GetMonth(), today.GetDay());

			DMSmartPtrT<IDMFont> pOldFont;
			if (m_ftToday)
			{
				pCanvas->SelectObject(m_ftToday, (IDMMetaFile**)&pOldFont);
			}
			DMColor oldTextClr;
			if (!m_crTodayText[dwState].IsTextInvalid())
			{
				oldTextClr = pCanvas->SetTextColor(m_crTodayText[dwState]);
			}
			pCanvas->DrawTextW(strTodayText, -1, rcToday, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS, alpha);
			if (!m_crTodayText[dwState].IsTextInvalid())
			{
				pCanvas->SetTextColor(oldTextClr);
			}
			if (m_ftToday)
			{
				pCanvas->SelectObject(pOldFont);
			}
		}
	}

#pragma endregion

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
#pragma region MsgDispatch
	void DUIMonthCalCtrl::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			CRect rcInvalid;
			pCanvas->GetClipBox(rcInvalid);
			if (rcInvalid.IsRectEmpty())
			{
				break;
			}
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas, DrawEnviron);
			CRect rcDraw,rcIntersect, rcUnicon;

			rcUnicon = rcIntersect = rcDraw = CalcHeaderRect(HEADER);
			rcIntersect.IntersectRect(rcDraw,rcInvalid);
			rcUnicon.UnionRect(rcDraw,rcInvalid);
			if (!rcIntersect.IsRectEmpty())// 和无效区存在交集
			{
				DrawHeader(pCanvas);
				if (rcUnicon.EqualRect(rcDraw))// rcDraw包含了无效区
				{
					break;
				}
			}

			rcUnicon = rcIntersect = rcDraw = CalcTitleRect();
			rcIntersect.IntersectRect(rcDraw,rcInvalid);
			rcUnicon.UnionRect(rcDraw,rcInvalid);
			if (!rcIntersect.IsRectEmpty())// 和无效区存在交集
			{
				DrawTitle(pCanvas);
				if (rcUnicon.EqualRect(rcDraw))// rcDraw包含了无效区
				{
					break;
				}
			}

			rcUnicon = rcIntersect = rcDraw = CalcTodayRect();
			rcIntersect.IntersectRect(rcDraw,rcInvalid);
			rcUnicon.UnionRect(rcDraw,rcInvalid);
			if (!rcIntersect.IsRectEmpty())// 和无效区存在交集
			{
				DrawToday(pCanvas);
				if (rcUnicon.EqualRect(rcDraw))// rcDraw包含了无效区
				{
					break;
				}
			}

			if (-1 != m_ihoverCell)
			{
				CRect rcHover = m_dayCells[m_ihoverCell/CALENDAR_COLUMNS][m_ihoverCell%CALENDAR_COLUMNS].rect;
				rcUnicon.UnionRect(rcHover,rcInvalid);
				if (rcUnicon.EqualRect(rcHover))
				{
					DrawCell(pCanvas, m_ihoverCell/CALENDAR_COLUMNS, m_ihoverCell%CALENDAR_COLUMNS);
					DV_PopDrawEnviron(pCanvas, DrawEnviron);
					break;
				}
			}
			if (-1 != m_iselCell)
			{
				CRect rcSel = m_dayCells[m_iselCell/CALENDAR_COLUMNS][m_iselCell%CALENDAR_COLUMNS].rect;
				rcUnicon.UnionRect(rcSel,rcInvalid);
				if (rcUnicon.EqualRect(rcSel))
				{
					DrawCell(pCanvas, m_iselCell/CALENDAR_COLUMNS, m_iselCell%CALENDAR_COLUMNS);
					DV_PopDrawEnviron(pCanvas, DrawEnviron);
					break;
				}
			}
			
			for (int r=0; r<CALENDAR_ROWS ; r++)
			{
				for (int c=0; c<CALENDAR_COLUMNS ; c++)
				{
					rcIntersect = m_dayCells[r][c].rect;
					rcIntersect.IntersectRect(rcIntersect,rcInvalid);
					if (!rcIntersect.IsRectEmpty())
					{
						DrawCell(pCanvas, r, c);
					}
				}
			}

			DV_PopDrawEnviron(pCanvas, DrawEnviron);
		} while (false);
	}

	void DUIMonthCalCtrl::OnSize(UINT nType, CSize size)
	{
		__super::OnSize(nType,size);
		UpdateDisplayCellsRect();
	}

	void DUIMonthCalCtrl::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		CRect rcBtn[2] = {CalcHeaderRect(LEFTBTN), CalcHeaderRect(RIGHTBTN)};
		DWORD dwBtnState[2] = {rcBtn[0].PtInRect(pt) ? DUIWNDSTATE_PushDown : m_dwBtnState[0],
			rcBtn[1].PtInRect(pt) ? DUIWNDSTATE_PushDown : m_dwBtnState[1]};

		if (dwBtnState[0] != m_dwBtnState[0])
		{// 不等于肯定是DUIWNDSTATE_PushDown
			m_dwBtnState[0] = dwBtnState[0];
			DM_InvalidateRect(rcBtn[0]);
			OnPreviousClick();
		}

		if (dwBtnState[1] != m_dwBtnState[1])
		{
			m_dwBtnState[1] = dwBtnState[1];
			DM_InvalidateRect(rcBtn[1]);
			OnNextClick();
		}
		
		int iSel = CalcHitCell(pt);
		if (-1 != iSel && m_iselCell != iSel)
		{
			int iOld = m_iselCell;
			m_iselCell = iSel;
			if (-1 != iOld)
			{
				DM_InvalidateRect(m_dayCells[iOld/CALENDAR_COLUMNS][iOld%CALENDAR_COLUMNS].rect);
			}
			DM_InvalidateRect(m_dayCells[m_iselCell/CALENDAR_COLUMNS][m_iselCell%CALENDAR_COLUMNS].rect);
			OnCellClick();
		}

		CRect rcToday = CalcTodayRect();
		DWORD dwTodayState = rcToday.PtInRect(pt) ? DUIWNDSTATE_PushDown : m_dwTodayState;
		if (dwTodayState != m_dwTodayState)
		{
			m_dwTodayState = dwTodayState;
			DM_InvalidateRect(rcToday);
			OnTodayClick();
		}
	}

	void DUIMonthCalCtrl::OnMouseMove(UINT nFlags,CPoint pt)
	{
		do 
		{
			CRect rcLeftBtn = CalcHeaderRect(LEFTBTN);
			CRect rcRightBtn = CalcHeaderRect(RIGHTBTN);

			DWORD dwBtnState[2] = {m_dwBtnState[0],m_dwBtnState[1]};
			dwBtnState[0] = rcLeftBtn.PtInRect(pt) ?  (PUSH_LBUTTONDOWN ? DUIWNDSTATE_PushDown : DUIWNDSTATE_Hover) : DUIWNDSTATE_Normal;
			dwBtnState[1] = rcRightBtn.PtInRect(pt) ? (PUSH_LBUTTONDOWN ? DUIWNDSTATE_PushDown : DUIWNDSTATE_Hover) : DUIWNDSTATE_Normal;

			if (dwBtnState[0] != m_dwBtnState[0])
			{
				m_dwBtnState[0] = dwBtnState[0];
				DM_InvalidateRect(rcLeftBtn);
			}
			if (dwBtnState[1] != m_dwBtnState[1])
			{
				m_dwBtnState[1] = dwBtnState[1];
				DM_InvalidateRect(rcRightBtn);
			}

			int iOld = m_ihoverCell;
			m_ihoverCell = CalcHitCell(pt);
			if (iOld != m_ihoverCell)
			{
				if (-1 != iOld)
				{
					DM_InvalidateRect(m_dayCells[iOld/CALENDAR_COLUMNS][iOld%CALENDAR_COLUMNS].rect);
				}
				if (-1 != m_ihoverCell)
				{
					DM_InvalidateRect(m_dayCells[m_ihoverCell/CALENDAR_COLUMNS][m_ihoverCell%CALENDAR_COLUMNS].rect);
				}
			}

			CRect rcToday = CalcTodayRect();
			DWORD dwTodayState = rcToday.PtInRect(pt) ?  (PUSH_LBUTTONDOWN ? DUIWNDSTATE_PushDown : DUIWNDSTATE_Hover) : DUIWNDSTATE_Normal;
			if (dwTodayState != m_dwTodayState)
			{
				m_dwTodayState = dwTodayState;
				DM_InvalidateRect(rcToday);
			}

			__super::OnMouseHover(nFlags,pt);
		} while (false);
	}

	void DUIMonthCalCtrl::OnMouseLeave()
	{
		do 
		{
			if (DUIWNDSTATE_Normal != m_dwBtnState[0])
			{
				m_dwBtnState[0] = DUIWNDSTATE_Normal;
				DM_InvalidateRect(CalcHeaderRect(LEFTBTN));
			}
			if (DUIWNDSTATE_Normal != m_dwBtnState[1])
			{
				m_dwBtnState[1] = DUIWNDSTATE_Normal;
				DM_InvalidateRect(CalcHeaderRect(RIGHTBTN));
			}

			int iOldHover = m_ihoverCell;
			m_ihoverCell = -1;
			if (-1 != iOldHover)
			{
				DM_InvalidateRect(m_dayCells[iOldHover/CALENDAR_COLUMNS][iOldHover%CALENDAR_COLUMNS].rect);
			}

			if (DUIWNDSTATE_Normal != m_dwTodayState)
			{
				m_dwTodayState = DUIWNDSTATE_Normal;
				DM_InvalidateRect(CalcTodayRect());
			}

			__super::OnMouseLeave();

		} while (false);
	}

	BOOL DUIMonthCalCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		zDelta>0 ?  OnPreviousClick() : OnNextClick();
		return TRUE;
	}

#pragma endregion


#pragma region MsgDispatch
	CRect DUIMonthCalCtrl::CalcHeaderRect(HeadType type)
	{
		if (m_iHeaderHei<=0)
		{
			return CRect();
		}

		CRect rcRect;
		DV_GetClientRect(rcRect);
		rcRect.bottom = rcRect.top + m_iHeaderHei;
		CSize sz = m_BtnSize;
		switch (type)
		{
		case HEADER:
			break;

		case LEFTBTN:
			{
				if (CSize(-1,-1)==sz && m_pBtnSkin[0])
				{
					m_pBtnSkin[0]->GetStateSize(sz);
				}

				rcRect = (CSize(-1,-1)==sz) ? CRect() : 
					CRect(rcRect.left+m_iBtnOffset, rcRect.top+(rcRect.Height()-sz.cy)/2, 
					rcRect.left+m_iBtnOffset+sz.cx, rcRect.top+(rcRect.Height()-sz.cy)/2+sz.cy);
			}
			break;

		case RIGHTBTN:
			{
				if (CSize(-1,-1)==sz && m_pBtnSkin[1])
				{
					m_pBtnSkin[1]->GetStateSize(sz);
				}

				rcRect = (CSize(-1,-1)==sz) ? CRect() : 
					CRect(rcRect.right-m_iBtnOffset-sz.cx, rcRect.top+(rcRect.Height()-sz.cy)/2, 
					rcRect.right-m_iBtnOffset, rcRect.top+(rcRect.Height()-sz.cy)/2+sz.cy);
			}
			break;

		case MID:
			{
				int iLeft = rcRect.left + m_iBtnOffset;
				if (CSize(-1,-1)==sz && m_pBtnSkin[0])
				{
					m_pBtnSkin[0]->GetStateSize(sz);
				}
				iLeft += (-1 == sz.cx) ? 0 : sz.cx;

				int iRight = rcRect.right - m_iBtnOffset;
				sz = m_BtnSize;
				if (CSize(-1,-1)==sz && m_pBtnSkin[1])
				{
					m_pBtnSkin[1]->GetStateSize(sz);
				}
				iRight -= (-1 == sz.cx) ? 0 : sz.cx;
				rcRect = (iRight<iLeft) ? CRect() : CRect(iLeft, rcRect.top, iRight, rcRect.bottom);
			}
			break;

		default:
			rcRect.SetRectEmpty();
		}

		return rcRect;
	}

	CRect DUIMonthCalCtrl::CalcTitleRect()
	{
		if (m_iTitleHei<=0)
		{
			return CRect();
		}

		CRect rcTitle;
		DV_GetClientRect(rcTitle);
		rcTitle.top += m_iHeaderHei;
		rcTitle.bottom = rcTitle.top + m_iTitleHei;
		return rcTitle;
	}

	CRect DUIMonthCalCtrl::CalcTodayRect()
	{
		if (m_iTodayHei<=0)
		{
			return CRect();
		}

		CRect rcRect;
		DV_GetClientRect(rcRect);
		rcRect.top = rcRect.bottom - m_iTodayHei;
		return rcRect;
	}

	CStringW DUIMonthCalCtrl::CalcJieri(int iMon, int iDay)
	{
		CStringW strJieri;
		if (iMon == 10 && iDay == 1)
		{
			strJieri = L"国庆节";
		}
		else if (iMon == 10 && iDay == 16)
		{
			strJieri = L"世界粮食日";
		}
		else if (iMon == 1 && iDay == 1)
		{
			strJieri = L"元旦节";
		}
		else if (iMon == 2 && iDay == 14)
		{
			strJieri = L"情人节";
		}
		else if (iMon == 2 && iDay == 10)
		{
			strJieri = L"国际气象节";
		}
		else if (iMon == 5 && iDay == 1)
		{
			strJieri = L"国庆劳动节";
		}
		else if (iMon == 5 && iDay == 22)
		{
			strJieri = L"国际护士节";
		}
		else if (iMon == 5 && iDay == 31)
		{
			strJieri = L"世界无烟日";
		}
		else if (iMon == 8 && iDay == 1)
		{
			strJieri = L"建军节";
		}
		else if (iMon == 12 && iDay == 25)
		{
			strJieri = L"圣诞节";
		}
		else if (iMon == 12 && iDay == 10)
		{
			strJieri = L"世界人权日";
		}
		else if (iMon == 4 && iDay == 1)
		{
			strJieri = L"愚人节";
		}
		else if (iMon == 6 && iDay == 1)
		{
			strJieri = L"儿童节";
		}
		else if (iMon == 3 && iDay == 8)
		{
			strJieri = L"妇女节";
		}
		else if (iMon == 3 && iDay == 12)
		{
			strJieri = L"植树节";
		}
		else if (iMon == 3 && iDay == 14)
		{
			strJieri = L"国际警察节";
		}
		else if (iMon == 4 && iDay == 5)
		{
			strJieri = L"清明节";
		}
		else if (iMon == 4 && iDay == 22)
		{
			strJieri = L"世界地球日";
		}
		else if (iMon == 7 && iDay == 1)
		{
			strJieri = L"建党节";
		}
		else if (iMon == 9 && iDay == 10)
		{
			strJieri = L"教师节";
		}

		return strJieri;
	}

	int DUIMonthCalCtrl::CalcHitCell(CPoint &pt)
	{
		if (pt.y>m_rcCells.top && pt.y<m_rcCells.bottom)
		{
			int iWid = pt.x - m_rcCells.left;
			int iHei = pt.y - m_rcCells.top;
			int iCellWid = m_rcCells.Width()/CALENDAR_COLUMNS;
			int iCellHei = m_rcCells.Height()/CALENDAR_ROWS;

			int iRow = iWid/iCellWid;
			if (iRow > CALENDAR_ROWS)
			{
				iRow = CALENDAR_ROWS;
			}
			int iRet = iRow + (iHei/iCellHei )*CALENDAR_COLUMNS;
			if (iRet>=CALENDAR_ROWS*CALENDAR_COLUMNS
				|| (m_bHideGrayCell && m_dayCells[iRet/CALENDAR_COLUMNS][iRet%CALENDAR_COLUMNS].bGray))
			{
				iRet = -1;
			}
			return iRet;
		}

		return -1;
	}

	void DUIMonthCalCtrl::OnPreviousClick()
	{
		DMDateTime date = GetDisplayDate();
		date.AddMonths(-1);
		SetDisplayDate(date);
	}

	void DUIMonthCalCtrl::OnNextClick()
	{
		DMDateTime date = GetDisplayDate();
		date.AddMonths(1);
		SetDisplayDate(date);
	}

	void DUIMonthCalCtrl::OnCellClick()
	{
		 if (-1 != m_iselCell)
		 {
			 DMDateTime oldDate = GetDisplayDate();
			 DMCalendarCell &cell = m_dayCells[m_iselCell/7][m_iselCell%7];
			 SetDisplayDate(cell.date);

			 DMEventCalenderClickDateChangedArgs Evt(this);
			 Evt.m_iOldYear = oldDate.GetYear();
			 Evt.m_iOldMonth = oldDate.GetMonth();
			 Evt.m_iOldDay = oldDate.GetDay();
			 Evt.m_iNewYear = GetDisplayDate().GetYear();
			 Evt.m_iNewMonth = GetDisplayDate().GetMonth();
			 Evt.m_iNewDay = GetDisplayDate().GetDay();
			 DV_FireEvent(Evt);
		 }
	}

	void DUIMonthCalCtrl::OnTodayClick()
	{
		SetDisplayDate(DMDateTime::Today());
	}

#pragma endregion

	DMCode DUIMonthCalCtrl::OnAttributeHeaderHeight(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int iOld = m_iHeaderHei;
			dm_parseint(lpszValue,m_iHeaderHei);

			if (false == bLoadXml && iOld != m_iHeaderHei)
			{
				UpdateDisplayCellsRect();
				DM_Invalidate();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIMonthCalCtrl::OnAttributeTitleHeight(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int iOld = m_iTitleHei;
			dm_parseint(lpszValue,m_iTitleHei);

			if (false == bLoadXml && iOld != m_iTitleHei)
			{
				UpdateDisplayCellsRect();
				DM_Invalidate();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIMonthCalCtrl::OnAttributeTodayHeight(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int iOld = m_iTodayHei;
			dm_parseint(lpszValue,m_iTodayHei);

			if (false == bLoadXml && iOld != m_iTodayHei)
			{
				UpdateDisplayCellsRect();
				DM_Invalidate();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIMonthCalCtrl::OnAttributeWeekTitle(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CStringW strValue = DMCA2W(lpszValue, -1, CP_UTF8);
			if (strValue.IsEmpty())
			{
				break;
			}

			strValue.Trim();
			CStringWList strList;
			SplitStringT(strValue,',',strList);
			for (int i=0; i<(int)strList.GetCount()&&i<CALENDAR_COLUMNS; i++)
			{
				m_strWeekTitle[i] = strList[i];
				m_strWeekTitle[i].Trim();
			}

			iErr = bLoadXml ? DM_ECODE_OK : DM_ECODE_NOXMLLOADREFRESH;
		} while (false);
		return iErr;
	}

	DMCode DUIMonthCalCtrl::OnAttributebHideGrayCell(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			bool bOld = m_bHideGrayCell;
			dm_parsebool(lpszValue,m_bHideGrayCell);

			if (false == bLoadXml && bOld != m_bHideGrayCell)
			{
				if (-1 != m_iselCell && m_dayCells[m_iselCell/CALENDAR_COLUMNS][m_iselCell%CALENDAR_COLUMNS].bGray)
				{
					SetDisplayDate(DMDateTime::Today());// 跳转到今天
				}
				DM_Invalidate();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

}// namespace DM