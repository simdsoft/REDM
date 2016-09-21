#include "QQDemoAfx.h"
#include "DMScriptImpl.h"

/// <summary>
///	 用于支持打印log
/// </summary>
class DMLuaLog
{
public:
	DMLuaLog()
	{
		m_fp = NULL;
		wchar_t szPath[MAX_PATH] = {0};
		GetRootFullPath(L".\\luaLog.dat",szPath,MAX_PATH);
		char szLuaLogPath[MAX_PATH] = {0};
		UnicodeToAscii(szPath,szLuaLogPath,MAX_PATH);
		freopen_s(&m_fp,szLuaLogPath, "w", stdout);
	}
	~DMLuaLog()
	{
		if (m_fp)
		{
			fclose(m_fp);
		}
	}
	FILE* m_fp;
};

//-------------------------------------------------------
DMScriptImpl::DMScriptImpl()
{
	m_bInit = false;
}

DMCode DMScriptImpl::InitScript()
{
	try
	{
		if (false == m_bInit)
		{
			luaL_openlibs(m_L);
			InitDMLua();
			m_bInit = true;
		}
		InitDMLua_Global();
		return DM_ECODE_OK;
	}
	catch(...)
	{
		return DM_ECODE_FAIL;
	}
}

DM::DMCode DMScriptImpl::RunString(LPCSTR lpszString)
{
	try
	{
		DMLuaLog log;
		DMLUA::dostring(m_L, lpszString);
		return DM_ECODE_OK;
	}
	catch(...)
	{
		return DM_ECODE_FAIL;
	}
}

DM::DMCode DMScriptImpl::RunBuf( LPCSTR pBuf, size_t len )
{
	try
	{
		DMLuaLog log;
		DMLUA::dobuffer(m_L,pBuf, len);
		return DM_ECODE_OK;
	}
	catch(...)
	{
		return DM_ECODE_FAIL;
	}
}

DMCode DMScriptImpl::RunFile(LPCSTR lpszFilePath)
{
	try
	{
		DMLuaLog log;
		DMLUA::dofile(m_L,lpszFilePath);
		return DM_ECODE_OK;
	}
	catch(...)
	{
		return DM_ECODE_FAIL;
	}
}

DMCode DMScriptImpl::RunEvent(DMEventArgs* pEvt)
{
	try
	{
		DMCode iErr = DM_ECODE_FAIL;
		DMLuaLog log;
		do 
		{
			if (NULL == pEvt)
			{
				break;
			}

			LPCSTR lpszEvtId = pEvt->GetEventName();
			if (NULL == lpszEvtId||0==strlen(lpszEvtId))
			{
				break;
			}
			// 未处理的返回0，所以这里还是用bool吧
			bool bRet = DMLUA::call<bool>(m_L,lpszEvtId,pEvt);
			if (bRet)
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}
	catch(...)
	{
		return DM_ECODE_FAIL;
	}
}


// private
DMCode DMScriptImpl::InitDMLua()
{
	try
	{
		DMLUA::init(m_L);

		InitDMLua_Struct();
		InitDMLua_DUIWnd();
		InitDMLua_Interface();
		InitDMLua_DMHWnd();


		InitDMLua_DMApp();
		return DM_ECODE_OK;
	}
	catch(...)
	{
		return DM_ECODE_FAIL;
	}
}


//def 
#define DMCLASS_ADD(X)				DMLUA::class_add<X>(m_L,#X);
#define DMCLASS_CON(X,...)			DMLUA::class_con<X>(m_L, DMLUA::constructor<X,__VA_ARGS__>)
#define DMCLASS_DEF(X,Y)			DMLUA::class_def<X>(m_L, #Y, &X::Y);
#define DMCLASS_MEM(X,Y)            DMLUA::class_mem<X>(m_L, #Y, &X::Y);
#define DMCLASS_INH(X,Y)            DMLUA::class_inh<X,Y>(m_L);

#define DM_DEF(X)                   DMLUA::def(m_L, #X, X);
#define DM_DEF_N(X,Y)               DMLUA::def(m_L, #X, Y);
#define DM_SET_N(X,Y)				DMLUA::set(m_L, #X, Y);
#define DM_SET_T(T,X)               DMLUA::set<T>(m_L, #X, X);

/// <summary>
///		1.重载函数不能加入
///     2.构造函数只能加一个
///     3.不支持不定参数
///     4.不支持函数定义的默认参数传入,默认参数就是0
///     5.只支持继承于一个基类，所以提供DMDyncT用于强制转换到其他基类（从而可以调用从其他基类的函数,和正常的C++转换一个效果）
/// </summary>

DMCode DMScriptImpl::InitDMLua_Struct()
{
	// DMColor 
	DMCLASS_ADD(DMColor);
	DMCLASS_CON(DMColor,BYTE,BYTE,BYTE,BYTE);
	DMCLASS_DEF(DMColor,SetRGBA);
	DMCLASS_DEF(DMColor,SetbyBGRA);

	// SIZE + CSize 
	DMCLASS_ADD(SIZE);
	DMCLASS_MEM(SIZE,cx);
	DMCLASS_MEM(SIZE,cy);

	DMCLASS_ADD(CSize);
	DMCLASS_INH(CSize,SIZE);
	DMCLASS_CON(CSize,int,int);
	DMCLASS_DEF(CSize,SetSize);

	// POINT + CPoint 
	DMCLASS_ADD(POINT);
	DMCLASS_MEM(POINT,x);
	DMCLASS_MEM(POINT,y);

	DMCLASS_ADD(CPoint);
	DMCLASS_INH(CPoint,POINT);
	DMCLASS_CON(CPoint,int,int);

	// RECT + CRect
	DMCLASS_ADD(RECT);
	DMCLASS_MEM(RECT,left);
	DMCLASS_MEM(RECT,top);
	DMCLASS_MEM(RECT,right);
	DMCLASS_MEM(RECT,bottom);
	DMCLASS_ADD(CRect);
	DMCLASS_INH(CRect,RECT);
	DMCLASS_CON(CRect,int,int,int,int);
	DMCLASS_DEF(CRect,Width);
	DMCLASS_DEF(CRect,Height);
	DMCLASS_DEF(CRect,Size);
	DMCLASS_DEF(CRect,CenterPoint);
	DMCLASS_DEF(CRect,IsRectEmpty);
	DMCLASS_DEF(CRect,IsRectNull);
	DMCLASS_DEF(CRect,PtInRect);
	DMCLASS_DEF(CRect,SetRectEmpty);
	DMCLASS_DEF(CRect,CopyRect);
	DMCLASS_DEF(CRect,EqualRect);
	DMCLASS_DEF(CRect,NormalizeRect);

	// CStringA + CStringW
	DMCLASS_ADD(CStringA);
	DMCLASS_CON(CStringA,const char*);// 如果要复制一份，使用b = CStringA(a:GetBuffer())
	DMCLASS_DEF(CStringA,GetLength);
	DMCLASS_DEF(CStringA,IsEmpty);
	DMCLASS_DEF(CStringA,Empty);
	DMCLASS_DEF(CStringA,GetAt);
	DMCLASS_DEF(CStringA,SetAt);
	DMCLASS_DEF(CStringA,AppendChar);
	DMCLASS_DEF(CStringA,Append);
	DMCLASS_DEF(CStringA,AppendStr);
	DMCLASS_DEF(CStringA,Compare);
	DMCLASS_DEF(CStringA,CompareNoCase);
	DMCLASS_DEF(CStringA,Mid);
	DMCLASS_DEF(CStringA,MidLimit);
	DMCLASS_DEF(CStringA,Right);
	DMCLASS_DEF(CStringA,Left);
	DMCLASS_DEF(CStringA,MakeUpper);
	DMCLASS_DEF(CStringA,MakeLower);
	DMCLASS_DEF(CStringA,TrimRight);// 注意传入参数0x20
	DMCLASS_DEF(CStringA,TrimLeft); // 注意传入参数0x20
	DMCLASS_DEF(CStringA,Trim);// 注意传入参数0x20
	DMCLASS_DEF(CStringA,InsertChar);
	DMCLASS_DEF(CStringA,Insert);
	DMCLASS_DEF(CStringA,Delete);
	DMCLASS_DEF(CStringA,ReplaceChar);
	DMCLASS_DEF(CStringA,Replace);
	DMCLASS_DEF(CStringA,Remove);
	DMCLASS_DEF(CStringA,FindChar);
	DMCLASS_DEF(CStringA,ReverseFind);
	DMCLASS_DEF(CStringA,Find);
	DMCLASS_DEF(CStringA,GetBuffer);// GetBuffer调用后，一定要调用ReleaseBuffer释放，不然内存就可能留在exe中！
	DMCLASS_DEF(CStringA,ReleaseBuffer);
	DMCLASS_DEF(CStringA,GetBufferSetLength);
	DMCLASS_DEF(CStringA,SetLength);
	DMCLASS_DEF(CStringA,LockBuffer);
	DMCLASS_DEF(CStringA,UnlockBuffer);

	DMCLASS_ADD(CStringW);
	DMCLASS_CON(CStringW,const wchar_t*);// 如果要复制一份，使用b = CStringW(a:GetBuffer())
	DM_DEF(DMCA2W);// 用于直接从脚本中初始化CStringW
	DMCLASS_DEF(CStringW,GetLength);
	DMCLASS_DEF(CStringW,IsEmpty);
	DMCLASS_DEF(CStringW,Empty);
	DMCLASS_DEF(CStringW,GetAt);
	DMCLASS_DEF(CStringW,SetAt);
	DMCLASS_DEF(CStringW,AppendChar);
	DMCLASS_DEF(CStringW,Append);
	DMCLASS_DEF(CStringW,AppendStr);
	DMCLASS_DEF(CStringW,Compare);
	DMCLASS_DEF(CStringW,CompareNoCase);
	DMCLASS_DEF(CStringW,Mid);
	DMCLASS_DEF(CStringW,MidLimit);
	DMCLASS_DEF(CStringW,Right);
	DMCLASS_DEF(CStringW,Left);
	DMCLASS_DEF(CStringW,MakeUpper);
	DMCLASS_DEF(CStringW,MakeLower);
	DMCLASS_DEF(CStringW,TrimRight);// 注意传入参数0x20
	DMCLASS_DEF(CStringW,TrimLeft); // 注意传入参数0x20
	DMCLASS_DEF(CStringW,Trim);// 注意传入参数0x20
	DMCLASS_DEF(CStringW,InsertChar);
	DMCLASS_DEF(CStringW,Insert);
	DMCLASS_DEF(CStringW,Delete);
	DMCLASS_DEF(CStringW,ReplaceChar);
	DMCLASS_DEF(CStringW,Replace);
	DMCLASS_DEF(CStringW,Remove);
	DMCLASS_DEF(CStringW,FindChar);
	DMCLASS_DEF(CStringW,ReverseFind);
	DMCLASS_DEF(CStringW,Find);
	DMCLASS_DEF(CStringW,GetBuffer);
	DMCLASS_DEF(CStringW,ReleaseBuffer);
	DMCLASS_DEF(CStringW,GetBufferSetLength);
	DMCLASS_DEF(CStringW,SetLength);
	DMCLASS_DEF(CStringW,LockBuffer);
	DMCLASS_DEF(CStringW,UnlockBuffer);

	// 注意,lua无法直接传wchar和print(wchar),所以要先转成多字节再print
	DM_DEF(DMA2W);
	DM_DEF(DMW2A);
	DM_DEF(DMA2A);

	// DMEventArgs
	DMCLASS_ADD(DMEventArgs);
	DMCLASS_DEF(DMEventArgs,IsValid);
	DMCLASS_DEF(DMEventArgs,GetEventID);
	DMCLASS_DEF(DMEventArgs,GetEventName);
	DMCLASS_MEM(DMEventArgs,m_iHandleCount);
	DMCLASS_MEM(DMEventArgs,m_IdFrom);
	DMCLASS_MEM(DMEventArgs,m_szNameFrom);
	DMCLASS_MEM(DMEventArgs,m_pSender);

	// DMCode
	DM_SET_T(int, DM_ECODE_OK);
	DM_SET_T(int, DM_ECODE_FAIL);

	// 内置定时器
	DM_SET_T(int, TIMER_NEXTFRAME);

	return DM_ECODE_OK;
}

DMCode DMScriptImpl::InitDMLua_DUIWnd()
{
	// DMRefNum,不允许直接创建
	DMCLASS_ADD(DMRefNum);
	DMCLASS_DEF(DMRefNum,AddRef);
	DMCLASS_DEF(DMRefNum,Release);
	DMCLASS_DEF(DMRefNum,GetRefCount);
	DMCLASS_DEF(DMRefNum,SetRefCount);
	DMCLASS_MEM(DMRefNum,m_RefCount);

	// DMDataBase
	DMCLASS_ADD(DMDataBase);
	DMCLASS_CON(DMDataBase);
	DMCLASS_INH(DMDataBase,DMRefNum);
	DMCLASS_DEF(DMDataBase,InitDMData);
	DMCLASS_DEF(DMDataBase,SetAttribute);
	DMCLASS_DEF(DMDataBase,SendExpandInfo);

	// DMBase
	DMCLASS_ADD(DMBase);
	DMCLASS_CON(DMBase);
	DMCLASS_INH(DMBase,DMDataBase);
	DMCLASS_DEF(DMBase,V_GetClassName);
	DMCLASS_DEF(DMBase,IsClass);
	DMCLASS_DEF(DMBase,V_GetClassType);

	// DUIMsgHandler
	DMCLASS_ADD(DUIMsgHandler);
	DMCLASS_CON(DUIMsgHandler);
	DMCLASS_DEF(DUIMsgHandler,IsMsgHandled);
	DMCLASS_DEF(DUIMsgHandler,SetMsgHandled);
	DMCLASS_MEM(DUIMsgHandler,m_bDuiMsgHandled);

	// DUIWindow
	DMCLASS_ADD(DUIWindow);
	DMCLASS_CON(DUIWindow);
	DMCLASS_INH(DUIWindow,DMBase);
	DM_DEF_N(DUIWindow2DUIMsgHandler,(DMDyncT<DUIWindowPtr,DUIMsgHandler*>));// DUIWindowPtr转成DUIMsgHandler*
	DMCLASS_DEF(DUIWindow,GetName);
	DMCLASS_DEF(DUIWindow,GetID);
	DMCLASS_DEF(DUIWindow,SetID);
	DMCLASS_DEF(DUIWindow,FindChildByName);
	DMCLASS_DEF(DUIWindow,FindChildById);
	DMCLASS_DEF(DUIWindow,HitTestPoint);
	DMCLASS_DEF(DUIWindow,GetDUIWnd);

	DMCLASS_DEF(DUIWindow,DM_SetTimer);// 255以内的定时器
	DMCLASS_DEF(DUIWindow,DM_KillTimer);

	return DM_ECODE_OK;
}

DMCode DMScriptImpl::InitDMLua_Interface()
{
	// IDMRes
	DMCLASS_ADD(IDMRes);
	DMCLASS_DEF(IDMRes,SetCurTheme);
	
	return DM_ECODE_OK;
}


DMCode DMScriptImpl::InitDMLua_DMHWnd()
{
	// DMHWnd
	DMCLASS_ADD(DMHWnd);
	DMCLASS_CON(DMHWnd);
	DM_DEF_N(DUIWindow2DMHWnd,(DMDyncT<DUIWindowPtr,DMHWnd*>));// DUIWindowPtr转成DMHWnd*
	

	return DM_ECODE_OK;
}

DMCode DMScriptImpl::InitDMLua_DMApp()
{
	DMCLASS_ADD(DMApp);
	DMCLASS_CON(DMApp,HINSTANCE);

	DMCLASS_DEF(DMApp,GetClassAtom);
	DMCLASS_DEF(DMApp,RedrawAll);

	return DM_ECODE_OK;
}

// -----------
DMCode DMScriptImpl::InitDMLua_Global()
{
	// 默认Res绑定
	do 
	{
		if (NULL == g_pDMApp)
		{
			break;
		}

		/* 两种方式,DM_SET_N关联变量(早绑定)和DM_DEF_N关联函数(晚绑定)，早绑定的坏处是有可能全局变量未初始化,就调用绑定了*/
		DM_SET_N(g_pDMApp,DMApp::getSingletonPtr());
		DM_SET_N(g_pDMRes,(DMDefRegT<IDMRes, DMREG_Res>()));


		DM_DEF_N(g_GetDMApp, &DMApp::getSingletonPtr);
		DM_DEF_N(g_GetDMRes, (DMDefRegT<IDMRes, DMREG_Res>));

	} while (false);

	return DM_ECODE_OK;
}


