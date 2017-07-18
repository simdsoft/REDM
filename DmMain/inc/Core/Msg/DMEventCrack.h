//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
//
// File Name: DMEventCrack.h
// File Des:
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-3	1.0
//    k000      2017-07-18            为DECLARE_EVENT_MAP/BEGIN_EVENT_MAP/
//                                    BEGIN_EVENT_MAPT/END_EVENT_MAP宏中DM下的类型添加
//                                    命名空间
//--------------------------------------------------------
#pragma once

namespace DM
{
//-------------------------------------------------------
// 在头文件中声明,在源文件中定义,通用的作法，调试时可使用MacroTool转换成换行的源代码调试
#define DECLARE_EVENT_MAP()\
public:\
	virtual DM::DMCode DMHandleEvent(DM::DMEventArgs *pEvt);

#define BEGIN_EVENT_MAP(classname)\
	DM::DMCode classname::DMHandleEvent(DM::DMEventArgs *pEvt)\
	{ \
		DM::DMCode iErr = DM::DM_ECODE_FAIL;\
		do\
		{\
			UINT  uCode = pEvt->GetEventID();

//-------------------------------------------------------
// 在头文件中声明并定义,用于模板类
#define BEGIN_EVENT_MAPT(classname)\
	DM::DMCode DMHandleEvent(DM::DMEventArgs *pEvt)\
	{ \
		DM::DMCode iErr = DM_ECODE_FAIL;\
		do\
		{\
		UINT  uCode = pEvt->GetEventID();


//-------------------------------------------------------
// 结束
#define END_EVENT_MAP()\
		} while (false);\
		if (DM::DM_ECODE_FAIL==iErr)\
		{\
			return __super::DMHandleEvent(pEvt);\
		}\
		return iErr;\
	}

#define END_EVENT_INBASE()\
		} while (false);\
		return iErr;\
	}

//-------------------------------------------------------
// 切换到父类处理,切换到其他类静态函数处理
#define CHAIN_EVENT_MAP(ChainClass)\
	if (DM_ECODE_FAIL!=(iErr=ChainClass::DMHandleEvent(pEvt)))\
	{\
		break;\
	}

#define CHAIN_EVENT_STATIC_MAP(ChainClass,OtherFun)\
	if (DM_ECODE_FAIL!=(iErr=ChainClass::OtherFun(pEvt)))\
	{\
		break;\
	}

//-------------------------------------------------------
// DMCode OnEvent(DMEventArgs *pEvt)
#define EVENT_HANDLER(cd, func)\
	if (cd == uCode)\
	{\
		iErr = func(pEvt);\
		break;\
	}

#define EVENT_ID_HANDLER(id, cd, func)\
	if (cd == uCode && id == pEvt->m_IdFrom)\
	{\
		iErr = func(pEvt);\
		break;\
	}

#define EVENT_NAME_HANDLER(name, cd, func)\
	if (cd == uCode && IsValidString(pEvt->m_szNameFrom) && 0==_wcsicmp(pEvt->m_szNameFrom,name))\
	{\
		iErr = func(pEvt);\
		break;\
	}

#define EVENT_COMMAND(func)\
	if (DM::DMEVT_CMD == uCode)\
	{\
		iErr = func(pEvt);\
		break;\
	}

//-------------------------------------------------------
// DMCode OnCommand()
#define EVENT_ID_COMMAND(id, func)\
	if (DM::DMEVT_CMD == uCode && id == pEvt->m_IdFrom)\
	{\
		iErr = func();\
		break;\
	}

#define EVENT_NAME_COMMAND(name, func)\
	if (DM::DMEVT_CMD == uCode && IsValidString(pEvt->m_szNameFrom) && 0==_wcsicmp(pEvt->m_szNameFrom,name))\
	{\
		iErr = func();\
		break;\
	}

//-------------------------------------------------------
// DMCode OnCommand(int idFrom)
#define EVENT_ID_COMMAND_RANGE(idMin, idMax, func)\
	if (DM::DMEVT_CMD == uCode && idMin <= pEvt->m_IdFrom && idMax >= pEvt->m_IdFrom )\
	{\
		iErr = func(pEvt->m_IdFrom);\
		break;\
	}
}
