// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMBundleImpl.h
// File mark:   
// File summary:DMBundle的实现体
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-2-24
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	class DMBundleData
	{
	public:
		enum{DB_null, DB_bool=1, DB_byte=2, DB_char=3, DB_dword=4, DB_double=5, DB_float=6, DB_int=7, DB_int64=8, DB_short=9, DB_string=10, DB_stringw=11, DB_size=12, DB_rect=13, DB_voidptr=14, DB_iunknown=15,DB_xmlnode=16};
		DMBundleData();
		DMBundleData(const DMBundleData &src);
		DMBundleData& operator=(const DMBundleData& src);
		~DMBundleData();

		bool PutBool(bool value);
		bool PutByte(byte value);
		bool PutChar(char value);
		bool PutDword(DWORD value);
		bool PutDouble(double value);
		bool PutFloat(float value);
		bool PutInt(int value);
		bool PutInt64(INT64 value);
		bool PutShort(short value);
		bool PutString(LPCSTR value);
		bool PutStringW(LPCWSTR value);
		bool PutSize(CSize value);
		bool PutRect(CRect value);
		bool PutVoidPtr(void* value);
		bool PutIUnknownPtr(IUnknown* value);
		bool PutXmlNode(DMXmlNode value);

		bool   GetBool();
		byte   GetByte();
		char   GetChar();
		DWORD  GetDword();
		double GetDouble();
		float  GetFloat();
		int    GetInt();
		INT64  GetInt64();
		short  GetShort();
		LPCSTR GetString();
		LPCWSTR GetStringW();
		CSize  GetSize();
		CRect  GetRect();
		void*  GetVoidPtr();
		IUnknown* GetIUnknownPtr();			///<  注意,get后引用计数+1
		DMXmlNode GetXmlNode();

	public:
		int					m_dbType;
		union
		{
			bool			ubool;
			byte			ubyte;
			char			uchar;
			DWORD			udword;
			double			udouble;
			float           ufloat;
			int				uint;
			INT64			uint64;
			short			ushort;
			LPCSTR			ustring;
			LPCWSTR			ustringw;
			SIZE			usize;
			RECT			urect;
			void*			uvoidptr;
			IUnknown*       uiunknown;
			DMXmlDocument*  uxml;
		}m_Value;
	};



	class DMBundleImpl : public DMRefNum, public DMMapT<CStringW, DMBundleData>
	{
	public:
		DMBundleImpl& operator=(const DMBundleImpl& src);
		virtual ~DMBundleImpl();


		void Clear();											
		int Size();											    
		bool Remove(LPCWSTR key);							
		bool HasKey(LPCWSTR key);								
	
		bool PutBool(LPCWSTR key, bool value);
		bool PutByte(LPCWSTR key, byte value);
		bool PutChar(LPCWSTR key, char value);
		bool PutDword(LPCWSTR key, DWORD value);
		bool PutDouble(LPCWSTR key, double value);
		bool PutFloat(LPCWSTR key, float value);
		bool PutInt(LPCWSTR key, int value);
		bool PutInt64(LPCWSTR key, INT64 value);
		bool PutShort(LPCWSTR key, short value);
		bool PutString(LPCWSTR key, LPCSTR value);
		bool PutStringW(LPCWSTR key, LPCWSTR value);
		bool PutSize(LPCWSTR key, CSize value);
		bool PutRect(LPCWSTR key, CRect value);
		bool PutVoidPtr(LPCWSTR key, void* value);
		bool PutIUnknownPtr(LPCWSTR key, IUnknown* value);
		bool PutXmlNode(LPCWSTR key, DMXmlNode value);

		bool   GetBool(LPCWSTR key, bool defaultValue=false);
		byte   GetByte(LPCWSTR key, byte defaultValue=0);
		char   GetChar(LPCWSTR key, char defaultValue=0);
		DWORD  GetDword(LPCWSTR key, DWORD defaultValue=0);
		double GetDouble(LPCWSTR key, double defaultValue=0);
		float  GetFloat(LPCWSTR key, float defaultValue=0);
		int    GetInt(LPCWSTR key, int defaultValue=0);
		INT64  GetInt64(LPCWSTR key, INT64 defaultValue=0);
		short  GetShort(LPCWSTR key, short defaultValue=0);
		LPCSTR GetString(LPCWSTR key, LPCSTR defaultValue="");
		LPCWSTR GetStringW(LPCWSTR key, LPCWSTR defaultValue=L"");
		CSize  GetSize(LPCWSTR key, CSize defaultValue=CSize());
		CRect  GetRect(LPCWSTR key, CRect defaultValue=CRect());
		void*  GetVoidPtr(LPCWSTR key, void* defaultValue=NULL);
		IUnknown* GetIUnknownPtr(LPCWSTR key, IUnknown* defaultValue=NULL);
		DMXmlNode GetXmlNode(LPCWSTR key);
	};

}// namespace DM