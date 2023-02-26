#include "DMDesignerAfx.h"
#include "DUIPropCtrl.h"
#include "DUIPropCtrlHelper.h"

namespace DM
{
	// DUIPropFrame----------------------------------------------------------
	DUIPropFrame::DUIPropFrame()
	{
		m_iHeadHei    = 24;
		m_iDescHei    = 60;
		m_bShowHead   = true;
		m_bShowDesc   = true;
		m_bFirst      = true;
		m_pHeadCtrl   = NULL;
		m_pSplit      = NULL;
		m_pPropList   = NULL;
		m_pPropDesc   = NULL;
	}

	IPropPtr DUIPropFrame::AddInitAttrProperty(DMXmlInitAttrPtr pInitAttr,IPropPtr pParent, CStringA strValue)
	{
		IPropPtr pNew = NULL;
		do 
		{
			CStringA strType,strName,strTempValue,strDesc;
			g_pAttr->Parse(pInitAttr->m_pAttr,strType,strName,strTempValue,strDesc);
			if (strValue.IsEmpty())// ����ⲿû��ָ������ʹ��Ԥ�õ�ֵ
			{
				strValue = strTempValue;
			}
			DMXmlDocument doc;
			DMXmlNode XmlBase = doc.Base();
			DMXmlNode XmlRoot = XmlBase.InsertChildNode("root");
			DMXmlNode XmlNode;
			bool bMatch = false;
			int nCount = countof(g_stAttrDesc);  
			for (int i=0; i<nCount; i++)
			{
				if (0 == strType.CompareNoCase(g_stAttrDesc[i].strOne))
				{
					XmlNode = XmlRoot.InsertChildNode(g_stAttrDesc[i].strThree);
					strTempValue = g_stAttrDesc[i].strFour;
					bMatch = true;

					if (0 == strType.CompareNoCase("bool"))
					{
						XmlNode.SetAttribute(XML_OPTIONS,"0|1");
					}
					else if (0 == strType.CompareNoCase("OPTION"))// ���⴦��
					{
						if (0 == strName.CompareNoCase("align")||0 == strName.CompareNoCase("surfacealign")) 
						{
							XmlNode.SetAttribute(XML_OPTIONS,"left|center|right");
							strTempValue = "center";
						}
						else if (0 == strName.CompareNoCase("valign"))
						{
							XmlNode.SetAttribute(XML_OPTIONS,"top|middle|bottom");
							strTempValue = "middle";
						}
						else if (0 == strName.CompareNoCase("tabalign"))
						{
							XmlNode.SetAttribute(XML_OPTIONS,"top|left|right|bottom");
							strTempValue = "top";
						}
						else if (0 == strName.CompareNoCase("sbenable")
							||0 == strName.CompareNoCase("animatetype"))
						{
							XmlNode.SetAttribute(XML_OPTIONS,"0|1|2|3");
							strTempValue = "3";
						}
					}
					break;
				}
			}
			if (!bMatch)
			{
				XmlNode = XmlRoot.InsertChildNode("propstring");
			}
 
			if (strValue.IsEmpty())// ����ⲿû��ָ��value����ʹ��Ԥ�õ�ֵ
			{
				strValue = strTempValue;
			}
			XmlNode.SetAttribute(XML_NAME,strName);
			XmlNode.SetAttribute(XML_VALUE,strValue);
			XmlNode.SetAttribute(XML_DESC,strDesc);

			m_pPropList->CreateRegObj((void**)&pNew, XmlNode.GetName());
			if (pNew)
			{
				pNew->SetOwner(m_pPropList);
				pNew->InitDMData(XmlNode);
				pNew->OnCreateInPlaceCtrl();
				pNew->SetData((LPARAM)pInitAttr);
				if (pParent)
				{
					pParent->AddSubItem(pNew);
				}
				else
				{
					m_pPropList->AddProperty(pNew);	
				}
				m_pPropList->AdjustLayout();
			}
		} while (false);

		return pNew;
	}

	IPropPtr DUIPropFrame::FindItemByData(LPARAM lpData)
	{
		return m_pPropList->FindItemByData(lpData);
	}

	void DUIPropFrame::OnSize(UINT nType,CSize size)
	{
		AdjustLayout();
		SetMsgHandled(FALSE);
	}

	void DUIPropFrame::OnDestroy()
	{
		if (m_pHeadCtrl)
		{
			m_pHeadCtrl->m_EventMgr.UnSubscribeEvent(DM::DMEventHeaderItemChangingArgs::EventID, Subscriber(&DUIPropFrame::OnHeaderSizeChanging, this));
		}

		SetMsgHandled(FALSE);
	}

	DMCode DUIPropFrame::DV_CreateChildWnds(const DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			//1.����header
			CStringW strWXml = L"<headerctrl align=\"center\" itemdelta=\"4\"  itemskin=\"%s\" bswap=\"0\">"\
				L"<item text=\"Name\"/>"\
				L"<item text=\"Value\"/>"\
				L"</headerctrl>";
			strWXml.Format(strWXml,m_strHeadItemSkin);
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument HeadDoc;
			HeadDoc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode HeadNode = HeadDoc.Root();
			g_pDMApp->CreateRegObj((void**)&m_pHeadCtrl, HeadNode.GetName(),DMREG_Window);
			DM_InsertChild(m_pHeadCtrl);
			m_pHeadCtrl->InitDMData(HeadNode);
			// ���ý���header��Ϣ
			m_pHeadCtrl->m_EventMgr.SubscribeEvent(DM::DMEventHeaderItemChangingArgs::EventID, Subscriber(&DUIPropFrame::OnHeaderSizeChanging, this));

			//2.����spliter
			strWXml = L"<splitlayout sliderwidth=\"4\" bvert=\"1\" clrslider=\"pbgra(22,22,22,ff)\" cursor=\"ds_splitv\" firstchildwidth=\"40\">"\
				L"<proplist clrtextdisable=\"pbgra(c0,c0,c0,ff)\" clrtext=\"pbgra(ff,ff,ff,ff)\" id=\"100\"/>"\
				L"<propdesc clrtextdisable=\"pbgra(c0,c0,c0,ff)\" id=\"200\"/>"\
				L"</splitlayout>";
			strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument SplitDoc;
			SplitDoc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode SplitNode = SplitDoc.Root();
			g_pDMApp->CreateRegObj((void**)&m_pSplit, SplitNode.GetName(),DMREG_FlowLayout);
			DM_InsertChild(m_pSplit); 
			m_pSplit->InitDMData(SplitNode);

			//3.�Ѳ�������m_pPropList+m_pPropDesc
			m_pPropList = m_pSplit->FindChildByIdT<DUIPropList>(100);
			m_pPropDesc = m_pSplit->FindChildByIdT<DUIPropDesc>(200);
			m_pPropList->InitDMData(XmlNode);
			m_pPropList->m_pFrame = this;
			m_pPropList->m_pDesc = m_pPropDesc;
			m_pPropDesc->InitDMData(XmlNode); 
			m_pPropDesc->m_pPropList = m_pPropList;
			
			iErr = DM_ECODE_OK;
		} while (false); 
		return iErr;
	}

	DMCode DUIPropFrame::AdjustLayout()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcClient;
			DV_GetClientRect(rcClient);
			if (rcClient.IsRectEmpty())
			{
				break;
			}

			//1.����Head����
			CRect rcHead;
			if (m_pHeadCtrl)
			{
				if (m_bShowHead)
				{
					rcHead = CRect(rcClient.left,rcClient.top,rcClient.right,rcClient.top+m_iHeadHei);
					m_pHeadCtrl->DM_FloatLayout(rcHead);
					int iLeftWid = m_pPropList->m_iLeftColWid;
					iLeftWid = iLeftWid<rcClient.Width()?iLeftWid:rcClient.Width();
					m_pHeadCtrl->SetItemWidth(0,iLeftWid);
					m_pHeadCtrl->SetItemWidth(1,rcClient.Width()-iLeftWid);	
				}
				else
				{
					m_pHeadCtrl->DM_SetVisible(false,false);
				}
			}

			//2.����Split������
			CRect rcSplit = rcClient;
			rcSplit.top += rcHead.Height();
			m_pSplit->DM_FloatLayout(rcSplit);
			if (m_bFirst)
			{
				int iFirstChildWidth = rcSplit.Height()-m_iDescHei;
				iFirstChildWidth = iFirstChildWidth?iFirstChildWidth:10;
				CStringA str;str.Format("%d",iFirstChildWidth);
				m_pSplit->SetAttribute("firstchildwidth",str);
					
				m_bFirst = false;
			}
			m_pPropList->AdjustLayout();

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIPropFrame::OnHeaderSizeChanging(DMEventArgs *pEvt)
	{
		m_pPropList->m_iLeftColWid = m_pHeadCtrl->GetItemWidth(0);
		CRect rcClient;
		DV_GetClientRect(rcClient);
		if (m_pPropList->m_iLeftColWid>rcClient.Width())
		{
			m_pPropList->m_iLeftColWid = rcClient.Width();
		}
		int iRightColWid = rcClient.Width() - m_pPropList->m_iLeftColWid;
		m_pHeadCtrl->SetItemWidth(0,m_pPropList->m_iLeftColWid,false);
		m_pHeadCtrl->SetItemWidth(1,iRightColWid,false);
		AdjustLayout();
		m_pPropList->DM_Invalidate();
		return DM_ECODE_OK;
	}

	//DUIPropList-----------------------------------------------------
	DUIPropList::DUIPropList()
	{
		m_iRowHei	  = 20;
		m_iLeftColWid = 60;
		m_pExpandSkin = NULL;
		m_pItemBgSkin = NULL;
		m_pSel        = NULL;
		m_bDragging   = false;
		m_pFrame      = NULL;
		m_pDesc       = NULL;
		RegisterProp(DMRegHelperT<PropGrid>(),true);
		RegisterProp(DMRegHelperT<PropString>(),true);
		RegisterProp(DMRegHelperT<PropInt>(),true);
		RegisterProp(DMRegHelperT<PropByte>(),true);
		RegisterProp(DMRegHelperT<PropOption>(),true);
		RegisterProp(DMRegHelperT<PropClr>(),true);
		RegisterProp(DMRegHelperT<PropRect>(),true);
		RegisterProp(DMRegHelperT<PropSize>(),true);
		RegisterProp(DMRegHelperT<PropFont>(),true);
		RegisterProp(DMRegHelperT<PropAccel>(),true);

		DMADDEVENT(PropDelingArgs::EventID);
		DMADDEVENT(PropDelArgs::EventID);
		DMADDEVENT(PropValueChangedArgs::EventID);

		m_accel = 0;
	}

	DUIPropList::~DUIPropList()
	{
		m_pSel = NULL;
		RemoveAll();
		for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pListProp = m_lstProps.GetNext(pos);
			pListProp->Release();
		}
		m_lstProps.RemoveAll();
	}

	DMCode DUIPropList::AdjustLayout()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcClient;
			DV_GetClientRect(rcClient);
			if (rcClient.IsRectEmpty())
			{ 
				break;
			}
			int y = rcClient.top - m_ptCurPos.y - 1;

			for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
			{
				IPropPtr pProp = m_lstProps.GetNext(pos);
				pProp->Reposition(y);
			}
			UpdateScrollRangeSize();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIPropList::RegisterProp(const IDMReg &RegObj,bool bReplace)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int iElement = FindObj(&const_cast<IDMReg&>(RegObj));
			if (DM_INVALID_VALUE != iElement)// �Ѵ���
			{
				if (bReplace)
				{
					RemoveObj(iElement);
				}
				else
				{
					iErr = DMREGMGR_REGISTERBYTYPE_REG_EXIST;
					break;
				}
			}
			if (true == AddObj(RegObj.Clone()))
			{
				iErr = DM_ECODE_OK;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIPropList::CreateRegObj(void** ppObj, LPCSTR lpszClassName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppObj)
			{
				DMASSERT_EXPR(0,L"��,�ǲ���û��˫ָ�������");
				iErr = DMREGMGR_CREATEREGOBJBYTYPE_PARAM_INVALID;
				break;
			}

			if (NULL == lpszClassName)
			{
				DMASSERT_EXPR(0,L"��,û������������");
				iErr = DMREGMGR_CREATEREGOBJBYTYPE_PARAM_INVALID;
				break;
			}

			int count = (int)GetCount();
			if (0 == count)
			{
				iErr = DMREGMGR_CREATEREGOBJBYTYPE_COUNT_ISZERO;
				break;
			}

			for (int i=0; i<count; i++)
			{
				if (0 == dm_xmlstrcmp(lpszClassName, GetObj(i)->GetClassName()))
				{
					*ppObj = (void**)GetObj(i)->NewObj();
					iErr = DM_ECODE_OK;
					break;
				}
			}
		} while (false);
		return iErr;
	}

	int DUIPropList::AddProperty(IPropPtr pProp, bool bRedraw)
	{
		for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pListProp = m_lstProps.GetNext(pos);
			if (pListProp == pProp || pListProp->IsSubItem(pProp))
			{
				// Can't ad the same property twice
				return -1;
			}
		}
		pProp->SetOwner(this);
		m_lstProps.AddTail(pProp);
		int nIndex = (int) m_lstProps.GetCount() - 1;
		if (bRedraw)
		{
			AdjustLayout();
		}
		return nIndex;
	}

	bool DUIPropList::DelProperty(IPropPtr pProp, bool bRedraw)
	{
		bool bRet = false;
		do 
		{
			PropDelingArgs Evt(this);
			Evt.m_pSel = pProp;
			DV_FireEvent(Evt);
			if (Evt.m_bCancel)
			{
				break;
			}

			bool bFound = false;
			for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
			{
				POSITION posSaved = pos;
				IPropPtr pListProp = m_lstProps.GetNext(pos);

				if (pListProp == pProp) // Top level property
				{
					bFound = true;
					m_lstProps.RemoveAt(posSaved);
					break;
				}
				if (DMSUCCEEDED(pListProp->RemoveSubItem(pProp, false)))
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				break;
			}

			if (m_pSel == pProp)
			{
				m_pSel = NULL;
			}
			PropDelArgs EndEvt(this);
			EndEvt.m_pSel = pProp;
			DV_FireEvent(EndEvt);// ��ʱ��û������ɾ����ֻ���Ƴ���

			// ��ʼ��ʵ��ɾ��
			pProp->PreDelete();
			pProp->Release();
			if (bRedraw)
			{
				AdjustLayout();
				m_pDesc->DM_Invalidate();
			}
		} while (false);
		return bRet;
	}

	bool DUIPropList::RemoveAllProp(bool bDelete,bool bRedraw)
	{
		m_pSel = NULL;
		for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pListProp = m_lstProps.GetNext(pos);
			if (bDelete)
			{
				pListProp->PreDelete();
				pListProp->Release();
			}
		}
		m_lstProps.RemoveAll();
		if (bRedraw)
		{
			AdjustLayout();
			m_pDesc->DM_Invalidate();
		}
		return true;
	}

	void DUIPropList::UpdateXml(DMXmlNode &XmlNode)
	{
		for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pListProp = m_lstProps.GetNext(pos);
			XmlNode.SetAttribute(pListProp->GetName(),pListProp->GetValue());
			pListProp->UpdateSubXml(XmlNode);
		}	
	}

	IPropPtr DUIPropList::FindItemByData(LPARAM lpData)
	{
		IPropPtr pProp = NULL;
		for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
		{
			POSITION posSaved = pos;
			IPropPtr pListProp = m_lstProps.GetNext(pos);

			if (pListProp->GetData() == lpData) // Top level property
			{
				pProp = pListProp;
				break;
			}

			pProp = pListProp->FindSubItemByData(lpData);
			if (NULL != pProp)
			{
				break;
			}
		}
		return pProp;
	}

	IPropPtr DUIPropList::SetCurSel(IPropPtr pProp, bool bRedraw)
	{
		IPropPtr pOldSel = NULL;
		do 
		{
			if (pProp == m_pSel)
			{
				break;
			}

			pOldSel = m_pSel;
			if (m_pSel && m_pSel->IsInPlaceEdit())
			{
				m_pSel->OnInPlaceCtrlShow(false);
			}
			m_pSel = pProp;	
			if (pOldSel)
			{
				pOldSel->Invalidate();
			}
			if (m_pSel)
			{
				m_pSel->Invalidate();
			}
			if (m_pDesc)
			{
				m_pDesc->DM_Invalidate();
			}
		} while (false);
		return pOldSel;
	}

	IPropPtr DUIPropList::GetCurSel()
	{
		return m_pSel;
	}

	IPropPtr DUIPropList::HitTest(CPoint pt,IProp::ClickArea* pnArea,bool bPropsOnly)
	{
		IPropPtr pHit = NULL;
		do 
		{
			for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
			{
				IPropPtr pProp = m_lstProps.GetNext(pos);
				pHit = pProp->HitTest(pt, pnArea);
				if (pHit != NULL)
				{
					break;
				}	
			}
		} while (false);
		return pHit;
	}

	void DUIPropList::OnSize(UINT nType,CSize size)
	{
		AdjustLayout();
		SetMsgHandled(FALSE);
	}

	void DUIPropList::OnDestroy()
	{
		if (m_accel)
		{
			DUIAccel acc(m_accel);
			GetContainer()->GetAccelMgr()->UnregisterAccel(acc,this);
		}
		SetMsgHandled(FALSE);
	}

	void DUIPropList::DM_OnPaint(IDMCanvas* pCanvas)
	{  
		do 
		{
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas,DrawEnviron);

			for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
			{
				IPropPtr pProp = m_lstProps.GetNext(pos);
				pProp->OnRender(pCanvas);
			}
	
			DV_PopDrawEnviron(pCanvas,DrawEnviron);

		} while (false);
	}

	void DUIPropList::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		do 
		{
			IProp::ClickArea clickArea;
			IPropPtr pHit = HitTest(pt,&clickArea);
			SetCurSel(pHit);
			if (NULL == pHit)
			{
				break;
			}
			switch (clickArea)
			{
			case IProp::ClickExpandBox:
				{
					pHit->Expand(!pHit->IsExpanded());
				}
				break;

			case IProp::ClickName:
				{
					pHit->OnClickName(WM_LBUTTONDOWN,pt);
				}
				break;

			case IProp::ClickValue:
				{
					pHit->OnClickValue(WM_LBUTTONDOWN,pt);
				}
				break;

			case IProp::ClickDrag:
				{
					pHit->OnClickDrag(WM_LBUTTONDOWN,pt);
					m_ptDrag = pt;
					m_bDragging = true;
				}
				break;

			default:
				break;
			}
		} while (false);
		// Ϊ����DUIList����OnSetCapture����������FALSE����Ȼ���ܳ���������״̬:
		// dragʱ����WM_LBUTTONDOWN�������Ϣû�е���OnSetCapture����m_pCapturePanel��Ȼ��drag����DUIList���ͷţ�
		//��ʱWM_LBUTTONUP��Ϣû�б�����(ֻ��capture���ܲ���),�Ӷ�m_bDragging�޷�����false,�Ӷ���MOVE��ͬ����dragЧ��
		SetMsgHandled(FALSE);
	}

	void DUIPropList::OnLButtonDbClick(UINT nFlags,CPoint pt)
	{
		do 
		{
			IProp::ClickArea clickArea;
			IPropPtr pHit = HitTest(pt,&clickArea);
			SetCurSel(pHit);
			if (NULL == pHit)
			{
				break;
			}

			switch (clickArea)
			{
			case IProp::ClickExpandBox:
			case IProp::ClickName:
				{
					pHit->Expand(!pHit->IsExpanded());
				}
				break;

			default:
				break;
			}

		} while (false);
	}

	void DUIPropList::OnMouseMove(UINT nFlags,CPoint pt)
	{
		if (PUSH_LBUTTONDOWN&&m_bDragging)// �Ӹ�PUSH_LBUTTONDOWN��˫�ر���
		{
			m_iLeftColWid += pt.x - m_ptDrag.x;
			m_ptDrag = pt;
			if (m_pFrame&&m_pFrame->m_pHeadCtrl)
			{
				CRect rcClient;
				m_pFrame->DV_GetClientRect(rcClient);
				m_iLeftColWid = m_iLeftColWid<40?40:m_iLeftColWid;
				m_iLeftColWid = m_iLeftColWid<rcClient.Width()-30?m_iLeftColWid:rcClient.Width()-30;

				m_pFrame->m_pHeadCtrl->SetItemWidth(0,m_iLeftColWid);
				m_pFrame->m_pHeadCtrl->SetItemWidth(1,rcClient.Width()-m_iLeftColWid);	
			}
			AdjustLayout();
			DM_Invalidate();
			GetContainer()->OnUpdateWindow();// �������´���
		}
		SetMsgHandled(FALSE);
	}

	void DUIPropList::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		m_bDragging = false;
		SetMsgHandled(FALSE);
	}

	void DUIPropList::DM_OnEnable(BOOL bEnable,UINT nStatus)
	{
		if (m_pSel && m_pSel->IsInPlaceEdit())
		{
			m_pSel->OnInPlaceCtrlShow(false);
		}
		SetMsgHandled(FALSE);
	}

	DMCode DUIPropList::DV_CreateChildWnds(const DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMXmlNode XmlItem = XmlNode.FirstChild();
			while (XmlItem.IsValid())
			{
				IPropPtr pNew = NULL;
				CreateRegObj((void**)&pNew, XmlItem.GetName());
				if (pNew)
				{
					pNew->SetOwner(this);
					pNew->InitDMData(XmlItem);
					pNew->OnCreateInPlaceCtrl();
					AddProperty(pNew);	
				}
				XmlItem = XmlItem.NextSibling();
			}

			if (0 == m_accel)
			{
				m_accel = DUIAccel::TranslateAccelKey("delete");
				DUIAccel acc(m_accel);
				GetContainer()->GetAccelMgr()->RegisterAccel(acc,this);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIPropList::DV_OnSetCursor(const CPoint &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			IProp::ClickArea clickArea;
			IPropPtr pHit = HitTest(pt,&clickArea);
			if (IProp::ClickDrag != clickArea)
			{
				break;
			}

			HCURSOR hCursor = ::LoadCursorW(NULL, IDC_SIZEWE);
			::SetCursor(hCursor);
			iErr = DM_ECODE_OK;
		} while (false);
		if (!DMSUCCEEDED(iErr))
		{
			iErr = __super::DV_OnSetCursor(pt);
		}
		return iErr;
	}

	bool DUIPropList::EqualArrayObj(const IDMRegPtr &objsrc, const IDMRegPtr &objdest)
	{
		return (0==dm_xmlstrcmp(objsrc->GetClassName(), objdest->GetClassName()));
	}

	void DUIPropList::PreArrayObjRemove(const IDMRegPtr &obj)
	{
		delete obj;
	}

	void DUIPropList::UpdateScrollRangeSize()
	{
		CRect rcClient;
		DV_GetClientRect(&rcClient);
		int nTotalHeight = 0;
		for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pProp = m_lstProps.GetNext(pos);
			nTotalHeight += pProp->GetHeiWithSub();
		}
		CSize szView(0,nTotalHeight);
		
		SetRangeSize(szView);
	}

	void DUIPropList::OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew)
	{
		AdjustLayout();
	}

	void DUIPropList::OnRangeSizeChanged(CSize szOld,CSize szNew)
	{
		AdjustLayout();
	}

	bool DUIPropList::OnAccelPressed(const DUIAccel& Accel)
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pSel)
			{
				break;
			}

			if (IDOK == DM_MessageBox(L"ȷ��ɾ�������Լ���������",MB_OKCANCEL,L"MSG",GetContainer()->OnGetHWnd()))
			{
				DelProperty(m_pSel);
			}
			
			bRet = true;
		} while (false);
		return bRet;
	}

	DUIPropDesc::DUIPropDesc()
	{
		m_pPropList = NULL;
	}

	void DUIPropDesc::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			if (NULL == m_pPropList)
			{
				break;
			}
			IPropPtr pPropSel = m_pPropList->GetCurSel();
			if (NULL == pPropSel)
			{
				break;
			}
			
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas,DrawEnviron);
			CRect rcClient;
			DV_GetClientRect(rcClient);
			pPropSel->OnDrawDesc(pCanvas,rcClient);
			DV_PopDrawEnviron(pCanvas,DrawEnviron);
		} while (false);
	}

}//namespace DM
