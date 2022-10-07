#include "DmMainAfx.h"
#include "DUIScrollWnd.h"

namespace DM
{
	CRect DUIScrollFL::GetMeasureRect(bool bUseFLCoordinate /*= false*/)
	{
		CRect rcMeasure = m_rcMeasure;
		if (false == bUseFLCoordinate)
		{
			rcMeasure.OffsetRect(m_rcWindow.TopLeft());
		}
		return rcMeasure;
	}

	DMCode DUIScrollFL::DV_GetChildMeasureLayout(LPRECT lpRect)
	{
		DV_GetClientRect(lpRect);
		return DM_ECODE_NOLOOP;
	}

	DMCode DUIScrollFL::DV_CreateChildWnds(const DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			if (!DMSUCCEEDED(__super::DV_CreateChildWnds(XmlNode)))
			{
				break;
			}

			m_EventMgr.SubscribeEvent(DM::DMEventChildLayoutFinishedArgs::EventID, Subscriber(&DUIScrollFL::OnChildLayoutFinishedEvent, this));
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIScrollFL::OnChildLayoutFinishedEvent(DMEventArgs *pEvt)
	{
		if (pEvt->m_pSender == this)
		{
			CRect rcMeasure;
			DUIWindow *pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				if (0 != dm_xmlstrcmp(pChild->V_GetClassName(),DUINAME_RectTracker))
				{// 过滤橡皮条控件
					CRect rcChild;
					pChild->DV_GetWindowRect(rcChild);
					rcMeasure.UnionRect(rcMeasure, rcChild);
				}
				pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}

			//1.转成以FL窗口为原点的坐标系
			rcMeasure.OffsetRect(-m_rcWindow.TopLeft());	
			m_rcMeasure = rcMeasure;// m_rcMeasure记录了以ScrollFL的左上角为坐标原点,包含所有子控件的最小区域(不包含ScrollFL非客户区)

			//2.通知父窗口更新FL的坐标
			DUIWindow* pParent = DM_GetWindow(GDW_PARENT);
			if (pParent)
			{
				CRect rcParent;
				pParent->DV_GetClientRect(rcParent);
				pParent->DM_SendMessage(WM_SIZE,0,MAKELPARAM(rcParent.Width(),rcParent.Height()));
			}
		}
		return DM_ECODE_OK;
	}

	void DUIScrollFL::OnDestroy()
	{
		m_EventMgr.UnSubscribeEvent(DM::DMEventChildLayoutFinishedArgs::EventID, Subscriber(&DUIScrollFL::OnChildLayoutFinishedEvent, this));
		__super::OnDestroy();
	}



	//------------------------------------------------------------------------------------------
	DUIScrollWnd::DUIScrollWnd()
	{
		m_pFLChild = NULL;
	}

	//---------------------------------------------------
	// Function Des: 对外接口 methods
	//---------------------------------------------------
#pragma region Public methods
	void DUIScrollWnd::UpdateScrollRange()
	{
		if (m_pFLChild )
		{
			CRect rcClient;
			DV_GetClientRect(rcClient);
			if (m_pFLChild->DM_IsLayoutFinished())
			{
				CRect rcFL = m_pFLChild->m_rcMeasure;
				CRect rcNcMargin;
				m_pFLChild->m_pDUIXmlInfo->m_pStyle->GetNcMargin(rcNcMargin);
				//1.让rcMeasure的左上角和FL左上角重合,右下角增加NC区域
				rcFL.SetRect(0, 0, rcFL.right+rcNcMargin.right, rcFL.bottom+rcNcMargin.bottom);

				//2.FL窗口的左上角和ScrollWnd左上角重合
				rcFL.OffsetRect(rcClient.TopLeft());

				rcFL.right = DMMAX(rcClient.right-m_isbWid, rcFL.right);
				rcFL.bottom = DMMAX(rcClient.bottom-m_isbWid, rcFL.bottom);

				m_pFLChild->m_EventMgr.SetMuted(true);// 不触发DMEventChildLayoutFinishedArgs
				m_pFLChild->DM_FloatLayout(rcFL);	  // 如果FL位置改变,所有子窗口都要做相应的调整
				m_pFLChild->m_EventMgr.SetMuted(false);
				SetRangeSize(CSize(m_pFLChild->m_rcWindow.Width(), m_pFLChild->m_rcWindow.Height()));
				OnRangeCurPosChanged(m_ptCurPos, m_ptCurPos);// 当Range前后一样时,SetRangeSize无效,所以要手动触发下FL的调整
			}
			else// 首次初始化FL及它的子控件
			{
				m_pFLChild->DM_FloatLayout(rcClient); //触发FL子窗口的布局,并最终更新FL的大小
			}
		}
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: DV methods
	//---------------------------------------------------
#pragma region DV
	DMCode DUIScrollWnd::DV_CreateChildWnds(const DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			if (!DMSUCCEEDED(__super::DV_CreateChildWnds(XmlNode)))
			{
				break;
			}

			m_pFLChild = dynamic_cast<DUIScrollFL*>(DM_GetWindow(GDW_FIRSTCHILD));
			if (NULL == m_pFLChild)
			{
				DMFAIL_MSG("DUIScrollWnd must have DUIScrollFL for scroll");
				break;
			}
			m_pFLChild->m_bFloatLayout = true;// 设置为绝对布局

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	void DUIScrollWnd::OnRangeCurPosChanged(CPoint ptOld, CPoint ptNew)
	{
		if (m_pFLChild && m_pFLChild->DM_IsLayoutFinished())
		{// 在滚动条消失前后ptOld和ptNew可能是错误的
			CRect rcFL = m_pFLChild->m_rcWindow;
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CPoint ptOffset = rcClient.TopLeft()-rcFL.TopLeft()-m_ptCurPos;
			if (ptOffset.x != 0 || ptOffset.y !=0)
			{// FL窗口的左上角和ScrollWnd左上角重合
				rcFL.OffsetRect(ptOffset);
				m_pFLChild->m_EventMgr.SetMuted(true);// 不触发DMEventChildLayoutFinishedArgs
				m_pFLChild->DM_FloatLayout(rcFL);
				m_pFLChild->m_EventMgr.SetMuted(false);
			}
		}
	}
#pragma endregion


	void DUIScrollWnd::OnSize(UINT nType, CSize size)
	{
		__super::OnSize(nType, size);
		UpdateScrollRange();
	}

}//namespace DM