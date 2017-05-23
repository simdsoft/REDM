#include "DmMainAfx.h"
#include "DMRegMgr.h"

namespace DM
{
	// DMRegTypeItem---------------------------------------
	DMRegTypeItem::DMRegTypeItem()
	{
	}

	DMRegTypeItem::~DMRegTypeItem()
	{
		RemoveAll();
	}

	void DMRegTypeItem::PreArrayObjRemove(const IDMRegPtr &obj)
	{
		int RegType = obj->GetClassType();
		switch (RegType)
		{
		case DMREG_Skin:
		case DMREG_Style:
		case DMREG_Attribute:
		case DMREG_Window:
		case DMREG_Layout:
		case DMREG_ImgDecoder:
		case DMREG_Res:
		case DMREG_Render:
		case DMREG_Draw:
		case DMREG_Log:
		case DMREG_FlowLayout:
		case DMREG_Animate:
		case DMREG_ToolTip:
		case DMREG_Script:
		case DMREG_Trans:
			{
				delete obj;
			}
			break;
		default:
			{
				DMASSERT_EXPR((0), L"要移除的class类型不对");
			}
			break;
		}
	}

	bool DMRegTypeItem::EqualArrayObj(const IDMRegPtr &objsrc, const IDMRegPtr &objdest)
	{
		bool bRet = false;
		int RegType = objsrc->GetClassType();
		switch (RegType)
		{
		case DMREG_Skin:
		case DMREG_Style:
		case DMREG_Attribute:
		case DMREG_Window:
		case DMREG_Layout:
		case DMREG_ImgDecoder:
		case DMREG_Res:
		case DMREG_Render:
		case DMREG_Draw:
		case DMREG_Log:
		case DMREG_FlowLayout:
		case DMREG_Animate:
		case DMREG_ToolTip:
		case DMREG_Script:
		case DMREG_Trans:
			{
				LPCWSTR lpszBaseNameSrc  = objsrc->GetBaseClassName();
				LPCWSTR lpszBaseNameDest = objdest->GetBaseClassName();
				bool bMatchBase =
					(NULL == lpszBaseNameSrc&&NULL == lpszBaseNameDest)										 // 基类都为DMBase
					||(lpszBaseNameSrc&&lpszBaseNameDest&&(0 == _wcsicmp(lpszBaseNameSrc,lpszBaseNameDest)));// 都不为NULL且相等(_wcsicmp不能使用空字符串)
				bRet = (bMatchBase && 0==_wcsicmp(objsrc->GetClassName(), objdest->GetClassName()));		 // class作为唯一的标识，肯定不为空字符串
			}
			break;
		default:
			{
				DMASSERT_EXPR((0), L"注册的class类型不对");
			}
			break;
		}
		return bRet;
	}

	// DMRegMgr--------------------------------------------
	DMRegMgr::DMRegMgr()
	{
	}
	DMRegMgr::~DMRegMgr()
	{
	}

	DMCode DMRegMgr::Register(IDMReg &RegObj, bool bReplace/*=false*/)
	{
		DMCode iErr = DM_ECODE_OK;
		int RegType = RegObj.GetClassType();
		switch (RegType)
		{
		case DMREG_Skin:		iErr = RegisterByType(m_RtSkin,RegObj,bReplace);		break;
		case DMREG_Style:		iErr = RegisterByType(m_RtStyle,RegObj,bReplace);		break;
		case DMREG_Attribute:	iErr = RegisterByType(m_RtAttribute,RegObj,bReplace);   break;
		case DMREG_Window:		iErr = RegisterByType(m_RtWindow,RegObj,bReplace);      break;
		case DMREG_Layout:		iErr = RegisterByType(m_RtLayout,RegObj,bReplace);      break;
		case DMREG_ImgDecoder:	iErr = RegisterByType(m_RtImgDecoder,RegObj,bReplace);  break;
		case DMREG_Res:			iErr = RegisterByType(m_RtRes,RegObj,bReplace);			break;
		case DMREG_Render:		iErr = RegisterByType(m_RtRender,RegObj,bReplace);		break;
		case DMREG_Draw:		iErr = RegisterByType(m_RtDraw,RegObj,bReplace);		break;
		case DMREG_Log:			iErr = RegisterByType(m_RtLog,RegObj,bReplace);		    break;
		case DMREG_FlowLayout:  iErr = RegisterByType(m_RtFlowLayout,RegObj,bReplace);  break;
		case DMREG_Animate:		iErr = RegisterByType(m_RtAnimate,RegObj,bReplace);		break;
		case DMREG_ToolTip:     iErr = RegisterByType(m_RtToolTip,RegObj,bReplace);     break;
		case DMREG_Script:      iErr = RegisterByType(m_RtScript,RegObj,bReplace);		break;
		case DMREG_Trans:       iErr = RegisterByType(m_RtTrans,RegObj,bReplace);		break;
		default:
			{
				iErr = DM_ECODE_FAIL;
				DMASSERT_EXPR((0), L"注册类型错误");
			}
			break;
		}
			
		return iErr;
	}

	DMCode DMRegMgr::CreateRegObj(void** ppObj, LPCWSTR lpszClassName,int RegType)
	{
		LOG_DEBUG("[start]lpszClassName:%s,RegType:%d\n",lpszClassName, RegType);
		DMCode iErr = DM_ECODE_OK;

		switch (RegType)
		{
		case DMREG_Skin:		iErr = CreateRegObjByType(m_RtSkin,ppObj,lpszClassName);			break;
		case DMREG_Style:		iErr = CreateRegObjByType(m_RtStyle,ppObj,lpszClassName);			break;
		case DMREG_Attribute:	iErr = CreateRegObjByType(m_RtAttribute,ppObj,lpszClassName);		break;
		case DMREG_Window:		iErr = CreateRegObjByType(m_RtWindow,ppObj,lpszClassName);			break;
		case DMREG_Layout:		iErr = CreateRegObjByType(m_RtLayout,ppObj,lpszClassName);			break;
		case DMREG_ImgDecoder:	iErr = CreateRegObjByType(m_RtImgDecoder,ppObj,lpszClassName);		break;
		case DMREG_Res:			iErr = CreateRegObjByType(m_RtRes,ppObj,lpszClassName);				break;
		case DMREG_Render:		iErr = CreateRegObjByType(m_RtRender,ppObj,lpszClassName);			break;
		case DMREG_Draw:		iErr = CreateRegObjByType(m_RtDraw,ppObj,lpszClassName);			break;
		case DMREG_Log:			iErr = CreateRegObjByType(m_RtLog,ppObj,lpszClassName);				break;
		case DMREG_FlowLayout:  iErr = CreateRegObjByType(m_RtFlowLayout,ppObj,lpszClassName);		break;
		case DMREG_Animate:		iErr = CreateRegObjByType(m_RtAnimate,ppObj,lpszClassName);			break;
		case DMREG_ToolTip:     iErr = CreateRegObjByType(m_RtToolTip,ppObj,lpszClassName);		    break;
		case DMREG_Script:      iErr = CreateRegObjByType(m_RtScript,ppObj,lpszClassName);		    break;
		case DMREG_Trans:       iErr = CreateRegObjByType(m_RtTrans,ppObj,lpszClassName);		    break;
		default:
			{
				iErr = DM_ECODE_FAIL;
				DMASSERT_EXPR((0), L"注册的class类型错误");
			}
			break;
		}

		LOG_DEBUG("[end]iErr:%d\n",iErr);
		return iErr;
	}

	DMCode DMRegMgr::UnRegister(LPCWSTR lpszClassName,int RegType)
	{
		DMCode iErr = DM_ECODE_OK;
		switch (RegType)
		{
		case DMREG_Skin:		iErr = UnRegisterByType(m_RtSkin,lpszClassName);			break;
		case DMREG_Style:		iErr = UnRegisterByType(m_RtStyle,lpszClassName);			break;
		case DMREG_Attribute:	iErr = UnRegisterByType(m_RtAttribute,lpszClassName);		break;
		case DMREG_Window:		iErr = UnRegisterByType(m_RtWindow,lpszClassName);			break;
		case DMREG_Layout:		iErr = UnRegisterByType(m_RtLayout,lpszClassName);			break;
		case DMREG_ImgDecoder:	iErr = UnRegisterByType(m_RtImgDecoder,lpszClassName);		break;
		case DMREG_Res:			iErr = UnRegisterByType(m_RtRes,lpszClassName);				break;
		case DMREG_Render:		iErr = UnRegisterByType(m_RtRender,lpszClassName);			break;
		case DMREG_Draw:		iErr = UnRegisterByType(m_RtDraw,lpszClassName);			break;
		case DMREG_Log:		    iErr = UnRegisterByType(m_RtLog,lpszClassName);				break;
		case DMREG_FlowLayout:	iErr = UnRegisterByType(m_RtFlowLayout,lpszClassName);		break;
		case DMREG_Animate:		iErr = UnRegisterByType(m_RtAnimate,lpszClassName);			break;
		case DMREG_ToolTip:	    iErr = UnRegisterByType(m_RtToolTip,lpszClassName);		    break;
		case DMREG_Script:	    iErr = UnRegisterByType(m_RtScript,lpszClassName);		    break;
		case DMREG_Trans:	    iErr = UnRegisterByType(m_RtTrans,lpszClassName);		    break;
		default:
			{
				iErr = DM_ECODE_FAIL;
				DMASSERT_EXPR((0), L"反注册的class类型错误");
			}
			break;
		}
		return iErr;
	}

	DMCode DMRegMgr::SetDefRegObj(LPCWSTR lpszClassName,int RegType)
	{
		LOG_DEBUG("[start]lpszClassName:%s,RegType:%d\n",lpszClassName, RegType);
		DMCode iErr = DM_ECODE_OK;
		switch (RegType)
		{
		case DMREG_Skin:		iErr = SetDefRegObjByType(m_RtSkin,lpszClassName);				break;
		case DMREG_Style:		iErr = SetDefRegObjByType(m_RtStyle,lpszClassName);				break;
		case DMREG_Attribute:	iErr = SetDefRegObjByType(m_RtAttribute,lpszClassName);			break;
		case DMREG_Window:		iErr = SetDefRegObjByType(m_RtWindow,lpszClassName);			break;
		case DMREG_Layout:		iErr = SetDefRegObjByType(m_RtLayout,lpszClassName);			break;
		case DMREG_ImgDecoder:	iErr = SetDefRegObjByType(m_RtImgDecoder,lpszClassName);		break;
		case DMREG_Res:			iErr = SetDefRegObjByType(m_RtRes,lpszClassName);				break;
		case DMREG_Render:		iErr = SetDefRegObjByType(m_RtRender,lpszClassName);			break;
		case DMREG_Draw:		iErr = SetDefRegObjByType(m_RtDraw,lpszClassName);				break;
		case DMREG_Log:		    iErr = SetDefRegObjByType(m_RtLog,lpszClassName);				break;
		case DMREG_FlowLayout:  iErr = SetDefRegObjByType(m_RtFlowLayout,lpszClassName);		break;
		case DMREG_Animate:		iErr = SetDefRegObjByType(m_RtAnimate,lpszClassName);			break;
		case DMREG_ToolTip:     iErr = SetDefRegObjByType(m_RtToolTip,lpszClassName);		    break;
		case DMREG_Script:      iErr = SetDefRegObjByType(m_RtScript,lpszClassName);		    break;
		case DMREG_Trans:       iErr = SetDefRegObjByType(m_RtTrans,lpszClassName);			    break;
		default:
			{
				iErr = DM_ECODE_FAIL;
				DMASSERT_EXPR((0), L"设置当前注册项的class类型错误");
			}
			break;
		}
		LOG_DEBUG("[end]iErr:%d\n",iErr);
		return iErr;
	}

	DMCode DMRegMgr::GetDefRegObj(CStringW &szName,int RegType)
	{
		DMCode iErr = DM_ECODE_OK;
		switch (RegType)
		{
		case DMREG_Skin:		szName = m_RtSkin.m_DefRegName;				break;
		case DMREG_Style:		szName = m_RtStyle.m_DefRegName;			break;
		case DMREG_Attribute:	szName = m_RtAttribute.m_DefRegName;		break;
		case DMREG_Window:		szName = m_RtWindow.m_DefRegName;			break;
		case DMREG_Layout:		szName = m_RtLayout.m_DefRegName;			break;
		case DMREG_ImgDecoder:	szName = m_RtImgDecoder.m_DefRegName;		break;
		case DMREG_Res:			szName = m_RtRes.m_DefRegName;				break;
		case DMREG_Render:		szName = m_RtRender.m_DefRegName;			break;
		case DMREG_Draw:		szName = m_RtDraw.m_DefRegName;				break;
		case DMREG_Log:		    szName = m_RtLog.m_DefRegName; 				break;
		case DMREG_FlowLayout:  szName = m_RtFlowLayout.m_DefRegName; 		break;
		case DMREG_Animate:     szName = m_RtAnimate.m_DefRegName; 			break;
		case DMREG_ToolTip:     szName = m_RtToolTip.m_DefRegName; 		    break;
		case DMREG_Script:      szName = m_RtScript.m_DefRegName; 		    break;
		case DMREG_Trans:       szName = m_RtTrans.m_DefRegName; 		    break;
		default:
			{
				iErr = DM_ECODE_FAIL;
				DMASSERT_EXPR((0), L"获取当前注册项的class类型错误");
			}
			break;
		}
		return iErr;
	}

	// 辅助
	DMCode DMRegMgr::RegisterByType(DMRegTypeItem &RtItem, IDMReg &RegObj, bool bReplace)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int iElement = RtItem.FindObj(&RegObj);
			if (DM_INVALID_VALUE != iElement)// 已存在
			{
				if (bReplace)
				{
					RtItem.RemoveObj(iElement);
				}
				else
				{
					iErr = DMREGMGR_REGISTERBYTYPE_REG_EXIST;
					break;
				}
			}

			if (true == RtItem.AddObj(RegObj.Clone()))
			{
				iErr = DM_ECODE_OK;
			}

		} while (false);
		return iErr;
	}

	DMCode DMRegMgr::CreateRegObjByType(DMRegTypeItem &RtItem, void** ppObj, LPCWSTR lpszClassName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszClassName||0==wcslen(lpszClassName))
			{
				lpszClassName = RtItem.m_DefRegName;
			}

			if (NULL == ppObj)
			{
				DMASSERT_EXPR(0,L"亲,是不是没传双指针进来！");
				iErr = DMREGMGR_CREATEREGOBJBYTYPE_PARAM_INVALID;
				break;
			}

			if (NULL == lpszClassName)
			{
				DMASSERT_EXPR(0,L"亲,没设置类名啊！");
				iErr = DMREGMGR_CREATEREGOBJBYTYPE_PARAM_INVALID;
				break;
			}

			int count = (int)RtItem.GetCount();
			if (0 == count)
			{
				iErr = DMREGMGR_CREATEREGOBJBYTYPE_COUNT_ISZERO;
				break;
			}

			for (int i=0; i<count; i++)
			{
				if (0 == _wcsicmp(lpszClassName,  RtItem.GetObj(i)->GetClassName()))
				{
					*ppObj = (void**)RtItem.GetObj(i)->NewObj();
					iErr = DM_ECODE_OK;
					break;
				}
			}

		} while (false);
		return iErr;
	}

	DMCode DMRegMgr::UnRegisterByType(DMRegTypeItem &RtItem, LPCWSTR lpszClassName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszClassName)
			{
				break;
			}

			int count = (int)RtItem.GetCount();
			for (int i=0; i<count; i++)
			{
				if (0 == _wcsicmp(lpszClassName, RtItem.GetObj(i)->GetClassName()))
				{
					RtItem.RemoveObj(i);
					iErr = DM_ECODE_OK;
					break;
				}
			}
		} while (false);
		return iErr;
	}

	DMCode DMRegMgr::SetDefRegObjByType(DMRegTypeItem &RtItem, LPCWSTR lpszClassName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lpszClassName == RtItem.m_DefRegName)
			{// 和原来默认的相等
				iErr = DMREGMGR_SETDEFREGOBJBYTYPE_DEFREGNAME_SAME;
				break;
			}

			int count = (int)RtItem.GetCount();
			for (int i=0; i<count; i++)
			{
				if (0 == _wcsicmp(lpszClassName, RtItem.GetObj(i)->GetClassName()))
				{
					RtItem.m_DefRegName = lpszClassName;
					iErr = DM_ECODE_OK;
					break;
				}
			}
		} while (false);
		return iErr;
	}
}//namespace DM