//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des: 
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//           		2014-12-2	1.0			
//--------------------------------------------------------
#pragma once

#ifdef DMWebkit_Exports
#   define DMWEBKIT_EXPORT __declspec(dllexport)
#else
#   define DMWEBKIT_EXPORT __declspec(dllimport)
#endif

typedef char utf8;
typedef struct 
{
	int x;
	int y;
	int w;
	int h;
} DMRect;

enum DMMouseFlags
{
	DM_LBUTTON = 0x01,
	DM_RBUTTON = 0x02,
	DM_SHIFT   = 0x04,
	DM_CONTROL = 0x08,
	DM_MBUTTON = 0x10,
};

enum DMKeyFlags
{
	DM_EXTENDED = 0x0100,
	DM_REPEAT = 0x4000,
};

enum DMMouseMsg
{
	DM_MSG_MOUSEMOVE       =  0x0200,
	DM_MSG_LBUTTONDOWN     =  0x0201,
	DM_MSG_LBUTTONUP       =  0x0202,
	DM_MSG_LBUTTONDBLCLK   =  0x0203,
	DM_MSG_RBUTTONDOWN     =  0x0204,
	DM_MSG_RBUTTONUP       =  0x0205,
	DM_MSG_RBUTTONDBLCLK   =  0x0206,
	DM_MSG_MBUTTONDOWN     =  0x0207,
	DM_MSG_MBUTTONUP       =  0x0208,
	DM_MSG_MBUTTONDBLCLK   =  0x0209,
	DM_MSG_MOUSEWHEEL      =  0x020A,
};

typedef void* jsExecState;
typedef __int64 jsValue;

typedef void* DMString;
typedef void (*ON_TITLE_CHANGED) (const struct _DMClientHandler* clientHandler, const DMString title);
typedef void (*ON_URL_CHANGED) (const struct _DMClientHandler* clientHandler, const DMString url);

typedef struct _DMClientHandler
{
	ON_TITLE_CHANGED onTitleChanged;
	ON_URL_CHANGED onURLChanged;
	PVOID  pdata;
} DMClientHandler;

class DUIWndWebHost
{// 内部调用更新到DUIWnd
public:
	virtual void DUIWndUpdate(const HDC hdc,int x, int y, int cx, int cy) = 0;
};

//c++ interface
//-----------------------------------------------------------------------------------------------------------
namespace DM
{
	class IWebView
	{
	public:
		virtual void destroy() = 0;

		virtual const char* name() const = 0;
		virtual void setName(const char* name) = 0;

		virtual bool transparent() const = 0;
		virtual void setTransparent(bool transparent) = 0;

		/// @brief 传入url
		virtual void loadURL(const utf8* url) = 0;
		virtual void loadURL(const wchar_t* url) = 0;

		virtual void loadHTML(const utf8* html) = 0;
		virtual void loadHTML(const wchar_t* html) = 0;

		/// @brief 传入本地url路径,内部调用loadURL,在调试时需传全路径
		virtual void loadFile(const utf8* filename) = 0;
		virtual void loadFile(const wchar_t* filename) = 0;

		/// @brief 判断加载状态
		virtual bool isLoaded() const = 0;       
		virtual bool isLoadFailed() const = 0;   
		virtual bool isLoadComplete() const = 0; 
		virtual bool isDocumentReady() const = 0; 
		virtual void stopLoading() = 0;
		virtual void reload() = 0;// 刷新

		/// @brief 标题
		virtual const utf8* title() = 0;
		virtual const wchar_t* titleW() = 0;

		virtual void resize(int w, int h) = 0;
		virtual int width() const = 0;			// viewport width
		virtual int height() const = 0;			// viewport height

		virtual int contentsWidth() const = 0;  // contents width
		virtual int contentsHeight() const = 0; // contents height

		virtual void setDirty(bool dirty) = 0;
		virtual bool isDirty() const = 0;
		virtual void addDirtyArea(int x, int y, int w, int h) = 0;

		virtual void layoutIfNeeded() = 0;
		virtual void tick() = 0;
		virtual void paint(void* bits, int pitch)=0;
		virtual void paint(void* bits, int bufWid, int bufHei, int xDst, int yDst, int w, int h, int xSrc, int ySrc, bool bCopyAlpha)=0;
		virtual HDC getViewDC() =0;
		virtual bool canGoBack() const = 0;
		virtual bool goBack() = 0;
		virtual bool canGoForward() const = 0;
		virtual bool goForward() = 0;

		virtual void selectAll() = 0;
		virtual void copy() = 0;
		virtual void cut() = 0;
		virtual void paste() = 0;
		virtual void delete_() = 0;

		virtual void setCookieEnabled(bool enable) = 0;
		virtual bool cookieEnabled() const = 0;

		virtual void setMediaVolume(float volume) = 0;
		virtual float mediaVolume() const = 0;

		virtual bool mouseEvent(unsigned int message, int x, int y, unsigned int flags) = 0;
		virtual bool contextMenuEvent(int x, int y, unsigned int flags) = 0;
		virtual bool mouseWheel(int x, int y, int delta, unsigned int flags) = 0;
		virtual bool keyUp(unsigned int virtualKeyCode, unsigned int flags, bool systemKey) = 0;
		virtual bool keyDown(unsigned int virtualKeyCode, unsigned int flags, bool systemKey) = 0;
		virtual bool keyPress(unsigned int virtualKeyCode, unsigned int flags, bool systemKey) = 0;

		virtual void focus() = 0;
		virtual void unfocus() = 0;

		virtual DMRect getCaret() = 0;

		virtual jsValue runJS(const utf8* script) = 0;
		virtual jsValue runJS(const wchar_t* script) = 0;
		virtual jsExecState globalExec() = 0;

		virtual void sleep() = 0; //moveOffscreen
		virtual void awaken() = 0; //moveOnscreen
		virtual bool isAwake() const = 0;

		virtual void setZoomFactor(float factor) = 0;
		virtual float zoomFactor() const = 0;

		virtual void setEditable(bool editable) = 0;

		virtual void setClientHandler(const DMClientHandler* handler) = 0;
		virtual const DMClientHandler* getClientHandler() const = 0;

		virtual void setDUIWndHost(DUIWndWebHost* handler) = 0;
		virtual const DUIWndWebHost* getDUIWndHost() const  = 0;

		virtual const utf8* toString(const DMString string) = 0;
		virtual const wchar_t* toStringW(const DMString string) = 0;
	};
}

typedef DM::IWebView* IDMWebViewPtr;
//c interface
//----------------------------------------------------------------------------------------------------------

extern "C"
{

	DMWEBKIT_EXPORT void DMInit();
	DMWEBKIT_EXPORT void DMShutdown();
	DMWEBKIT_EXPORT void DMUpdate();
	DMWEBKIT_EXPORT unsigned int DMVersion();
	DMWEBKIT_EXPORT const utf8* DMVersionString();

	typedef void* (*FILE_OPEN) (const char* path);
	typedef void (*FILE_CLOSE) (void* handle);
	typedef size_t (*FILE_SIZE) (void* handle);
	typedef int (*FILE_READ) (void* handle, void* buffer, size_t size);
	typedef int (*FILE_SEEK) (void* handle, int offset, int origin);

	DMWEBKIT_EXPORT void DMSetFileSystem(FILE_OPEN pfn_open, FILE_CLOSE pfn_close, FILE_SIZE pfn_size, FILE_READ pfn_read, FILE_SEEK pfn_seek);


	DMWEBKIT_EXPORT IDMWebViewPtr DMCreateWebView();
	DMWEBKIT_EXPORT IDMWebViewPtr DMGetWebView(const char* name);
	DMWEBKIT_EXPORT void DMDestroyWebView(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT const char* DMWebViewName(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMSetWebViewName(IDMWebViewPtr pWebView, const char* name);

	DMWEBKIT_EXPORT bool DMIsTransparent(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMSetTransparent(IDMWebViewPtr pWebView, bool transparent);

	DMWEBKIT_EXPORT void DMLoadURL(IDMWebViewPtr pWebView, const utf8* url);
	DMWEBKIT_EXPORT void DMLoadURLW(IDMWebViewPtr pWebView, const wchar_t* url);

	DMWEBKIT_EXPORT void DMLoadHTML(IDMWebViewPtr pWebView, const utf8* html);
	DMWEBKIT_EXPORT void DMLoadHTMLW(IDMWebViewPtr pWebView, const wchar_t* html);

	DMWEBKIT_EXPORT void DMLoadFile(IDMWebViewPtr pWebView, const utf8* filename);
	DMWEBKIT_EXPORT void DMLoadFileW(IDMWebViewPtr pWebView, const wchar_t* filename);

	DMWEBKIT_EXPORT bool DMIsLoaded(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT bool DMIsLoadFailed(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT bool DMIsLoadComplete(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT bool DMIsDocumentReady(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT bool DMIsLoading(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMStopLoading(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMReload(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT const utf8* DMTitle(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT const wchar_t* DMTitleW(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT void DMResize(IDMWebViewPtr pWebView, int w, int h);
	DMWEBKIT_EXPORT int DMWidth(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT int DMHeight(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT int DMContentsWidth(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT int DMContentsHeight(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT void DMSetDirty(IDMWebViewPtr pWebView, bool dirty);
	DMWEBKIT_EXPORT bool DMIsDirty(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMAddDirtyArea(IDMWebViewPtr pWebView, int x, int y, int w, int h);
	DMWEBKIT_EXPORT void DMLayoutIfNeeded(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMPaintEx(IDMWebViewPtr pWebView, void* bits,int bufWid, int bufHei, int xDst, int yDst, int w, int h, int xSrc, int ySrc, bool bCopyAlpha);
	DMWEBKIT_EXPORT void DMPaint(IDMWebViewPtr pWebView, void* bits,int pitch);

	DMWEBKIT_EXPORT bool DMCanGoBack(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT bool DMGoBack(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT bool DMCanGoForward(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT bool DMGoForward(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT void DMSelectAll(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMCopy(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMCut(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMPaste(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMDelete(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT void DMSetCookieEnabled(IDMWebViewPtr pWebView, bool enable);
	DMWEBKIT_EXPORT bool DMCookieEnabled(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT void DMSetMediaVolume(IDMWebViewPtr pWebView, float volume);
	DMWEBKIT_EXPORT float DMMediaVolume(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT bool DMMouseEvent(IDMWebViewPtr pWebView, unsigned int message, int x, int y, unsigned int flags);
	DMWEBKIT_EXPORT bool DMContextMenuEvent(IDMWebViewPtr pWebView, int x, int y, unsigned int flags);
	DMWEBKIT_EXPORT bool DMMouseWheel(IDMWebViewPtr pWebView, int x, int y, int delta, unsigned int flags);
	DMWEBKIT_EXPORT bool DMKeyUp(IDMWebViewPtr pWebView, unsigned int virtualKeyCode, unsigned int flags, bool systemKey);
	DMWEBKIT_EXPORT bool DMKeyDown(IDMWebViewPtr pWebView, unsigned int virtualKeyCode, unsigned int flags, bool systemKey);
	DMWEBKIT_EXPORT bool DMKeyPress(IDMWebViewPtr pWebView, unsigned int charCode, unsigned int flags, bool systemKey);

	DMWEBKIT_EXPORT void DMFocus(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMUnfocus(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT DMRect DMGetCaret(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT jsValue DMRunJS(IDMWebViewPtr pWebView, const utf8* script);
	DMWEBKIT_EXPORT jsValue DMRunJSW(IDMWebViewPtr pWebView, const wchar_t* script);

	DMWEBKIT_EXPORT jsExecState DMGlobalExec(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT void DMSleep(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT void DMAwaken(IDMWebViewPtr pWebView);
	DMWEBKIT_EXPORT bool DMIsAwake(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT void DMSetZoomFactor(IDMWebViewPtr pWebView, float factor);
	DMWEBKIT_EXPORT float DMZoomFactor(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT void DMSetEditable(IDMWebViewPtr pWebView, bool editable);

	DMWEBKIT_EXPORT void DMSetClientHandler(IDMWebViewPtr pWebView, const DMClientHandler* handler);
	DMWEBKIT_EXPORT const DMClientHandler* DMGetClientHandler(IDMWebViewPtr pWebView);

	DMWEBKIT_EXPORT const utf8* DMToString(const DMString string);
	DMWEBKIT_EXPORT const wchar_t* DMToStringW(const DMString string);

	/***JavaScript Bind***/
#define JS_CALL __fastcall
	typedef jsValue (JS_CALL *jsNativeFunction) (jsExecState es);

	typedef enum
	{
		JSTYPE_NUMBER,
		JSTYPE_STRING,
		JSTYPE_BOOLEAN,
		JSTYPE_OBJECT,
		JSTYPE_FUNCTION,
		JSTYPE_UNDEFINED,
	} jsType;

	DMWEBKIT_EXPORT void jsBindFunction(const char* name, jsNativeFunction fn, unsigned int argCount);
	DMWEBKIT_EXPORT void jsBindGetter(const char* name, jsNativeFunction fn); /*get property*/
	DMWEBKIT_EXPORT void jsBindSetter(const char* name, jsNativeFunction fn); /*set property*/

	DMWEBKIT_EXPORT int jsArgCount(jsExecState es);
	DMWEBKIT_EXPORT jsType jsArgType(jsExecState es, int argIdx);
	DMWEBKIT_EXPORT jsValue jsArg(jsExecState es, int argIdx);

	DMWEBKIT_EXPORT jsType jsTypeOf(jsValue v);
	DMWEBKIT_EXPORT bool jsIsNumber(jsValue v);
	DMWEBKIT_EXPORT bool jsIsString(jsValue v);
	DMWEBKIT_EXPORT bool jsIsBoolean(jsValue v);
	DMWEBKIT_EXPORT bool jsIsObject(jsValue v);
	DMWEBKIT_EXPORT bool jsIsFunction(jsValue v);
	DMWEBKIT_EXPORT bool jsIsUndefined(jsValue v);
	DMWEBKIT_EXPORT bool jsIsNull(jsValue v);
	DMWEBKIT_EXPORT bool jsIsArray(jsValue v);
	DMWEBKIT_EXPORT bool jsIsTrue(jsValue v);
	DMWEBKIT_EXPORT bool jsIsFalse(jsValue v);

	DMWEBKIT_EXPORT int jsToInt(jsExecState es, jsValue v);
	DMWEBKIT_EXPORT float jsToFloat(jsExecState es, jsValue v);
	DMWEBKIT_EXPORT double jsToDouble(jsExecState es, jsValue v);
	DMWEBKIT_EXPORT bool jsToBoolean(jsExecState es, jsValue v);
	DMWEBKIT_EXPORT const utf8* jsToString(jsExecState es, jsValue v);
	DMWEBKIT_EXPORT const wchar_t* jsToStringW(jsExecState es, jsValue v);

	DMWEBKIT_EXPORT jsValue jsInt(int n);
	DMWEBKIT_EXPORT jsValue jsFloat(float f);
	DMWEBKIT_EXPORT jsValue jsDouble(double d);
	DMWEBKIT_EXPORT jsValue jsBoolean(bool b);

	DMWEBKIT_EXPORT jsValue jsUndefined();
	DMWEBKIT_EXPORT jsValue jsNull();
	DMWEBKIT_EXPORT jsValue jsTrue();
	DMWEBKIT_EXPORT jsValue jsFalse();

	DMWEBKIT_EXPORT jsValue jsString(jsExecState es, const utf8* str);
	DMWEBKIT_EXPORT jsValue jsStringW(jsExecState es, const wchar_t* str);
	DMWEBKIT_EXPORT jsValue jsObject(jsExecState es);
	DMWEBKIT_EXPORT jsValue jsArray(jsExecState es);

	DMWEBKIT_EXPORT jsValue jsFunction(jsExecState es, jsNativeFunction fn, unsigned int argCount);

	//return the window object
	DMWEBKIT_EXPORT jsValue jsGlobalObject(jsExecState es);

	DMWEBKIT_EXPORT jsValue jsEval(jsExecState es, const utf8* str);
	DMWEBKIT_EXPORT jsValue jsEvalW(jsExecState es, const wchar_t* str);

	DMWEBKIT_EXPORT jsValue jsCall(jsExecState es, jsValue func, jsValue thisObject, jsValue* args, int argCount);
	DMWEBKIT_EXPORT jsValue jsCallGlobal(jsExecState es, jsValue func, jsValue* args, int argCount);

	DMWEBKIT_EXPORT jsValue jsGet(jsExecState es, jsValue object, const char* prop);
	DMWEBKIT_EXPORT void jsSet(jsExecState es, jsValue object, const char* prop, jsValue v);

	DMWEBKIT_EXPORT jsValue jsGetGlobal(jsExecState es, const char* prop);
	DMWEBKIT_EXPORT void jsSetGlobal(jsExecState es, const char* prop, jsValue v);

	DMWEBKIT_EXPORT jsValue jsGetAt(jsExecState es, jsValue object, int index);
	DMWEBKIT_EXPORT void jsSetAt(jsExecState es, jsValue object, int index, jsValue v);

	DMWEBKIT_EXPORT int jsGetLength(jsExecState es, jsValue object);
	DMWEBKIT_EXPORT void jsSetLength(jsExecState es, jsValue object, int length);

	DMWEBKIT_EXPORT IDMWebViewPtr jsGetWebView(jsExecState es);

	DMWEBKIT_EXPORT void jsGC(); //garbage collect
}
