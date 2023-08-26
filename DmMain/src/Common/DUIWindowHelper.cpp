﻿#include "DmMainAfx.h"
#include "DUIWindowHelper.h"

namespace DM
{

	//---------------------------------------DUIWindow_Data-----------------------
	DUIWindow_Data::DUIWindow_Data()
	{
		m_bDirty            = true;             // 默认为脏区
		m_bUpdateLocked     = false;	
		m_pCurMsg			= NULL;

		m_dcFlags           = -1;
		m_bClipCanvas       = false;
	}

	//---------------------------------------DUIWindow_XmlInfo-----------------------
	DUIWindow_XmlInfo::DUIWindow_XmlInfo(DUIWindow*pOwner)
	{
		m_pOwner            = pOwner;
		m_iID               = 0;
		m_bOnlyDrawClient   = false;
		m_bCacheDraw		= false;
		m_bVisible			= true;
		m_bDisable          = false;
		m_bPlaceHolder	    = true;
		m_bFocusable        = false;
		m_bShowDrawFocus    = false;
		m_bMsgNoHandle      = false;
		m_bDrag				= false;
		m_bNoPreSize        = false;

		m_iTooltipDelayTime = 500;
		m_iTooltipSpanTime  = 5000;
		m_rcTooltipPosFlags.SetRect(-1,0,0,0);

		// FlowLayout -----------------------
		m_nMaxWidth         = -1;
		m_nMinWidth         = -1;
		m_nMaxHeight        = -1;
		m_nMinHeight        = -1;
		m_iHeight           = -1;
		m_iWidth            = -1;

		g_pDMApp->CreateRegObj((void**)&m_pStyle,NULL,DMREG_Style);
		DMASSERT(NULL!=m_pStyle);
	}

	DMCode DUIWindow_XmlInfo::OnAttributeVisible(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			bool bVisible = false;
			dm_parsebool(lpszValue,bVisible);
			if (!bLoadXml)
			{
				m_pOwner->DM_SetVisible(bVisible,true);// 在show时会适当更新m_bVisible	
			}
			else
			{	
				m_bVisible = bVisible;
			}	
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow_XmlInfo::OnAttributeDisable(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			bool bDisable = false;
			dm_parsebool(lpszValue,bDisable);
			if (!bLoadXml)
			{
				m_pOwner->DM_EnableWindow(false==bDisable,true);
			}
			else
			{	
				if (bDisable)
				{
					m_pOwner->DM_ModifyState(DUIWNDSTATE_Disable,0,false);
				}
				else
				{
					m_pOwner->DM_ModifyState(0,DUIWNDSTATE_Disable,false);
				}
			}	
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow_XmlInfo::OnAttributePlaceHolder(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			dm_parsebool(lpszValue,m_bPlaceHolder);
			if (false == bLoadXml&&!m_pOwner->DM_IsVisible(true))
			{
				DUIWindow* pParent = m_pOwner->DM_GetWindow(GDW_PARENT);
				if (pParent)
				{
					pParent->DV_UpdateChildLayout();
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow_XmlInfo::OnAttributeCache(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{	
			dm_parsebool(lpszValue,m_bCacheDraw);
			if (false == bLoadXml)
			{
				if (m_bCacheDraw)
				{
					if(!m_pOwner->m_pCacheCanvas)
					{
						g_pDMRender->CreateCanvas(m_pOwner->m_rcWindow.Width(),m_pOwner->m_rcWindow.Height(),&m_pOwner->m_pCacheCanvas);
						m_pOwner->m_pCacheCanvas->SelectObject(g_pDMFontPool->GetFont(""));
						m_pOwner->DM_MarkCacheDirty(true);
						m_pOwner->DM_Invalidate();
					}
				}
			}
	
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow_XmlInfo::OnAttributeFinished(LPCSTR pszAttribute, LPCSTR pszValue,bool bLoadXml,DMCode iErr)
	{	
		if (m_pOwner)
		{
			return m_pOwner->OnAttributeFinished(pszAttribute,pszValue,bLoadXml, iErr);
		}
		return iErr;
	}

	DMCode DUIWindow_XmlInfo::OnAttributeStyle(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMXmlNode XmlStyle = g_pDMApp->GetStyle(lpszValue);
			if (!XmlStyle.IsValid())
			{
				break;
			}
			DMXmlAttribute XmlAttribute = XmlStyle.FirstAttribute();
			while (XmlAttribute.IsValid())
			{
				if (DM_ECODE_FAIL == m_pStyle->SetAttribute(XmlAttribute.GetName(), XmlAttribute.GetValue(), true)
					&& m_pOwner)
				{
					m_pOwner->SetAttribute(XmlAttribute.GetName(), XmlAttribute.GetValue(), true);
				}
				XmlAttribute = XmlAttribute.NextAttribute();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	//---------------------------------------DUIWindowNode-----------------------
	DUIWindowNode::DUIWindowNode()
	{
		m_pOwner = m_pParent = m_pPanelParent = m_pFirstChild = m_pLastChild = m_pNextSibling = m_pPrevSibling = NULL;
		m_nChildrenCount = 0;
	}
	
}//namespace DM