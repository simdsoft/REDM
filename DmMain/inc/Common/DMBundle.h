// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMBundle.h
// File mark:   
// File summary:DMBundle内部使用了new DMBundleImpl来实现,直接声明即可,不需要new，在赋值或做为参数传递时,内部的DMBundleImpl引用计数会自动+1-1，所以不用担心跨模块传递时,内存被突然释放
// 使用方式： DMBundle bundle;
//            DMBundle bundle1 = bundle; // 指向同一块DMBundleImpl内存，引用计数+1
//            DMBundle bundle2(bundle);  // 指向同一块DMBundleImpl内存，引用计数+1
//            DMBundle bundle3 = bundle.Clone();// 复制一块新的DMBundleImpl内存,bundle3初始引用计数为1
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-2-24
// ----------------------------------------------------------------
#pragma once
#include "Template/DMSmartPtrT.h"
#include "Common/DMDelegate.h"

namespace DM
{
	/// <summary>
	///		内置绑定数据
	/// <summary>
	class DMBundleImpl;
	class DM_EXPORT DMBundle
	{
	public:
		DMBundle();
		DMBundle(const DMBundle& src);
		DMBundle& operator=(const DMBundle& src);
		~DMBundle();

	public:
		DMBundle Clone();										///< 复制当前Bundle
		void Clear();											///< 移除当前Bundle映射中的所有元素
		int Size() const;									    ///< 得到当前Bundle的映射大小
		bool Remove(LPCWSTR key);								///< 移除当前Bundle的映射Key
		bool HasKey(LPCWSTR key);								///< 当前Bundle的映射Key是否存在,DM_ECODE_OK表示存在

		bool PutBool(LPCWSTR key, bool value);
		bool GetBool(LPCWSTR key, bool defaultValue=false) const;

		bool PutByte(LPCWSTR key, byte value);
		byte GetByte(LPCWSTR key, byte defaultValue=0) const;

		bool PutChar(LPCWSTR key, char value);
		char GetChar(LPCWSTR key, char defaultValue=0) const;

		bool PutDword(LPCWSTR key, DWORD value);
		DWORD GetDword(LPCWSTR key, DWORD defaultValue=0) const;

		bool PutDouble(LPCWSTR key, double value);
		double GetDouble(LPCWSTR key, double defaultValue=0.0f) const;

		bool PutFloat(LPCWSTR key, float value);
		float GetFloat(LPCWSTR key, float defaultValue=0.0f) const;

		bool PutInt(LPCWSTR key, int value);
		int  GetInt(LPCWSTR key, int defaultValue=0) const;

		bool PutInt64(LPCWSTR key, INT64 value);
		INT64 GetInt64(LPCWSTR key, INT64 defaultValue=0) const;

		bool PutShort(LPCWSTR key, short value);
		short GetShort(LPCWSTR key, short defaultValue=0) const;

		bool PutString(LPCWSTR key, LPCSTR value);
		LPCSTR GetString(LPCWSTR key, LPCSTR defaultValue="") const;

		bool PutStringW(LPCWSTR key, LPCWSTR value);
		LPCWSTR GetStringW(LPCWSTR key, LPCWSTR defaultValue=L"") const;

		bool PutSize(LPCWSTR key, CSize value);
		CSize GetSize(LPCWSTR key, CSize defaultValue=CSize()) const;

		bool PutRect(LPCWSTR key, CRect value);
		CRect GetRect(LPCWSTR key, CRect defaultValue=CRect()) const;

		bool PutVoidPtr(LPCWSTR key, void* value);
		void* GetVoidPtr(LPCWSTR key, void* defaultValue=NULL) const;

		bool PutIUnknownPtr(LPCWSTR key, IUnknown* value);
		IUnknown* GetIUnknownPtr(LPCWSTR key, IUnknown* defaultValue=NULL) const;

		bool PutXmlNode(LPCWSTR key, DMXmlNode value);
		DMXmlNode GetXmlNode(LPCWSTR key) const;

	public:
		DM::DMSmartPtrT<DMBundleImpl>					   m_pBundleImpl;		///< Bundle在传递时，m_pBundleImpl自动+1，从而保证BundleImpl的生命周期		
	};

	/// <summary>
	///		发送方
	/// </summary>
	enum DMEventSenderID{DMESenderID_Unknown,DMESenderID_Base};
	class DM_EXPORT DMEventSender
	{
	public:
		DMEventSender():m_dwSize(sizeof(DMEventSender)),m_dwId(DMESenderID_Unknown){}
	public:
		DWORD                                         m_dwSize;
		DWORD                                         m_dwId;
	};


	/// <summary>
	///		事件槽bool function(const DMEventSender& , const DMBundle&)函数进行抽象
	/// </summary>
	typedef DM::DMDelegate< bool(const DMEventSender& sender, const DMBundle& evt) > DMSlot;

}//namespace DM