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

	DMCode DUIScrollFL::DV_CreateChildWnds(DMXmlNode &XmlNode)
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
				if (0 != _wcsicmp(pChild->V_GetClassName(),DUINAME_RectTracker))
				{// ������Ƥ���ؼ�
					CRect rcChild;
					pChild->DV_GetWindowRect(rcChild);
					rcMeasure.UnionRect(rcMeasure, rcChild);
				}
				pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}

			//1.ת����FL����Ϊԭ�������ϵ
			rcMeasure.OffsetRect(-m_rcWindow.TopLeft());	
			m_rcMeasure = rcMeasure;// m_rcMeasure��¼����ScrollFL�����Ͻ�Ϊ����ԭ��,���������ӿؼ�����С����(������ScrollFL�ǿͻ���)

			//2.֪ͨ�����ڸ���FL������
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
	// Function Des: ����ӿ� methods
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
				//1.��rcMeasure�����ϽǺ�FL���Ͻ��غ�,���½�����NC����
				rcFL.SetRect(0, 0, rcFL.right+rcNcMargin.right, rcFL.bottom+rcNcMargin.bottom);

				//2.FL���ڵ����ϽǺ�ScrollWnd���Ͻ��غ�
				rcFL.OffsetRect(rcClient.TopLeft());

				rcFL.right = DMMAX(rcClient.right-m_isbWid, rcFL.right);
				rcFL.bottom = DMMAX(rcClient.bottom-m_isbWid, rcFL.bottom);

				m_pFLChild->m_EventMgr.SetMuted(true);// ������DMEventChildLayoutFinishedArgs
				m_pFLChild->DM_FloatLayout(rcFL);	  // ���FLλ�øı�,�����Ӵ��ڶ�Ҫ����Ӧ�ĵ���
				m_pFLChild->m_EventMgr.SetMuted(false);
				SetRangeSize(CSize(m_pFLChild->m_rcWindow.Width(), m_pFLChild->m_rcWindow.Height()));
				OnRangeCurPosChanged(m_ptCurPos, m_ptCurPos);// ��Rangeǰ��һ��ʱ,SetRangeSize��Ч,����Ҫ�ֶ�������FL�ĵ���
			}
			else// �״γ�ʼ��FL�������ӿؼ�
			{
				m_pFLChild->DM_FloatLayout(rcClient); //����FL�Ӵ��ڵĲ���,�����ո���FL�Ĵ�С
			}
		}
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: DV methods
	//---------------------------------------------------
#pragma region DV
	DMCode DUIScrollWnd::DV_CreateChildWnds(DMXmlNode &XmlNode)
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
				DMASSERT_EXPR(0,L"DUIScrollWnd������DUIScrollFL�Ӵ������ڹ�������");
				break;
			}
			m_pFLChild->m_bFloatLayout = true;// ����Ϊ���Բ���

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	void DUIScrollWnd::OnRangeCurPosChanged(CPoint ptOld, CPoint ptNew)
	{
		if (m_pFLChild && m_pFLChild->DM_IsLayoutFinished())
		{// �ڹ�������ʧǰ��ptOld��ptNew�����Ǵ����
			CRect rcFL = m_pFLChild->m_rcWindow;
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CPoint ptOffset = rcClient.TopLeft()-rcFL.TopLeft()-m_ptCurPos;
			if (ptOffset.x != 0 || ptOffset.y !=0)
			{// FL���ڵ����ϽǺ�ScrollWnd���Ͻ��غ�
				rcFL.OffsetRect(ptOffset);
				m_pFLChild->m_EventMgr.SetMuted(true);// ������DMEventChildLayoutFinishedArgs
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