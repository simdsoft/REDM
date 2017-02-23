//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMStringT.h 
// File Des: CString抽离成模板
// File Summary: 也可考虑ATL.CSimpleStringT 抽离
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------

#pragma once

#ifndef TSTRING_PADDING
#define TSTRING_PADDING 0
#endif

namespace DM
{
	__pragma(warning(push))
	__pragma(warning(disable: 4995))
	__pragma(warning(disable: 4996))

	/// <summary>
	///		基于atlsimpstr.h->CStringData
	/// </summary>
    struct TStringData
    {
        long nRefs;				  // reference count
        int nDataLength;		  // Length of currently used data in XCHARs (not including terminating null)
        int nAllocLength;		  // Length of allocated data in XCHARs (not including terminating null)
     
		void* data() throw()
        {
            return (this+1);
        }

        void AddRef() throw()
        {
            DMASSERT(nRefs > 0);
            InterlockedIncrement(&nRefs);
        }

		bool IsLocked() const throw()
		{
			return nRefs < 0;
		}

		bool IsShared() const throw()
		{
			return nRefs > 1;
		}

		void Lock() throw()
		{
			DMASSERT(nRefs <= 1);
			nRefs--;  // Locked buffers can't be shared, so no interlocked operation necessary
			if (0 == nRefs)
			{
				nRefs = -1;
			}
		}

        void Release() throw()
        {
            DMASSERT(nRefs != 0);
            if (InterlockedDecrement(&nRefs) <= 0)
			{
                DMMemDispatch::DM_free(this);
			}
        }
       
		void Unlock() throw()
        {
            DMASSERT(IsLocked());
            if (IsLocked())
            {
                nRefs++;  // Locked buffers can't be shared, so no interlocked operation necessary
                if (nRefs == 0)
				{
                    nRefs = 1;
				}
            }
        }
    };

    // Globals
	//wtl9->atlmisc.h->755行
	_declspec(selectany) TStringData* g_DMTempDataNil = (TStringData*)DMMemDispatch::Get_CString_InitData();
	_declspec(selectany) const void*  g_DMtmpPchNil = (const void*)(((unsigned char*)DMMemDispatch::Get_CString_InitData()) + sizeof(TStringData));

    struct wchar_traits;
    struct char_traits
    {
        static size_t StrLen(const char* psz)
        {
            return psz ? strlen(psz) : 0;
        }
        static int StrCmp(const char* psz1, const char* psz2)
        {
            return strcmp(psz1, psz2);
        }
        static int StrICmp(const char* psz1, const char* psz2)
        {
            return stricmp(psz1, psz2);
        }
        static char* StrChr(const char* psz, char ch)
        {
            return (char*)strchr(psz, ch);
        }
        static char* StrRChr(const char* psz, char ch)
        {
            return (char*)strrchr(psz, ch);
        }
        static char* StrStr(const char* psz, const char* psz2)
        {
            return (char*)strstr(psz, psz2);
        }
        static char* StrUpper(char* psz)
        {
            return (char*)strupr(psz);
        }
        static char* StrLower(char* psz)
        {
            return (char*)strlwr(psz);
        }
        static int IsSpace(char ch)
        {
            return isspace(ch);
        }
        static char CharLower(char ch)
        {
            return (char)tolower(ch);
        }
        static char CharUpper(char ch)
        {
            return (char)toupper(ch);
        }
        static char* CharNext(char* psz)
        {
            return psz + 1;
        }
		static int Format(char** ppszDst, const char* pszFormat, va_list args)
		{
			int len = _vscprintf(pszFormat, args); 
			*ppszDst = (char*)DMMemDispatch::DM_malloc(len+1);
			vsprintf(*ppszDst, pszFormat, args);
			return len;
		}
		static int LoadString(HINSTANCE hInst,
			UINT uID,
			char* lpBuffer,
			int nBufferMax)
		{
			return ::LoadStringA(hInst,uID,lpBuffer,nBufferMax);
		}
		static char* _cstrrev(char* pStr)
		{
			// optimize NULL, zero-length, and single-char case
			if ((pStr == NULL) || (pStr[0] == '\0') || (pStr[1] == '\0'))
				return pStr;

			char* p = pStr;

			while (*p != 0) 
			{
				char* pNext = ::CharNextA(p);
				if(pNext > p + 1)
				{
					char p1 = *(char*)p;
					*(char*)p = *(char*)(p + 1);
					*(char*)(p + 1) = p1;
				}
				p = pNext;
			}

			p--;
			char* q = pStr;

			while (q < p)
			{
				char t = *q;
				*q = *p;
				*p = t;
				q++;
				p--;
			}
			return pStr;
		}
		static const char* _cstrchr_db(const char* p, char ch1, char ch2)
		{
			const char* lpsz = NULL;
			while (*p != 0)
			{
				if (*p == ch1 && *(p + 1) == ch2)
				{
					lpsz = p;
					break;
				}
				p = ::CharNextA(p);
			}
			return lpsz;
		}
		static const char* _cstrchr(const char* p, char ch)
		{
			// strchr for '\0' should succeed
			while (*p != 0)
			{
				if (*p == ch)
					break;
				p = ::CharNextA(p);
			}
			return (*p == ch) ? p : NULL;
		}
		static int _cstrspn(const char* pStr, const char* pCharSet)
		{
			int nRet = 0;
			const char* p = pStr;
			while (*p != 0)
			{
				const char* pNext = ::CharNextA(p);
				if(pNext > p + 1)
				{
					if(_cstrchr_db(pCharSet, *p, *(p + 1)) == NULL)
						break;
					nRet += 2;
				}
				else
				{
					if(_cstrchr(pCharSet, *p) == NULL)
						break;
					nRet++;
				}
				p = pNext;
			}
			return nRet;
		}

		static int _cstrcspn(const char* pStr, const char* pCharSet)
		{
			int nRet = 0;
			char* p = (char*)pStr;
			while (*p != 0)
			{
				char* pNext = ::CharNextA(p);
				if(pNext > p + 1)
				{
					if(_cstrchr_db(pCharSet, *p, *(p + 1)) != NULL)
						break;
					nRet += 2;
				}
				else
				{
					if(_cstrchr(pCharSet, *p) != NULL)
						break;
					nRet++;
				}
				p = pNext;
			}
			return nRet;
		}
    };

    struct wchar_traits
    {
        typedef wchar_t            this_type;
        typedef wchar_traits    this_traits;

        static size_t StrLen(const wchar_t* psz)
        {
            return psz ? wcslen(psz) : 0;
        }
        static int StrCmp(const wchar_t* psz1, const wchar_t* psz2)
        {
            return wcscmp(psz1, psz2);
        }
        static int StrICmp(const wchar_t* psz1, const wchar_t* psz2)
        {
            return _wcsicmp(psz1, psz2);
        }
        static wchar_t* StrChr(const wchar_t* psz, wchar_t ch)
        {
            return const_cast<wchar_t*>(wcschr(psz, ch));
        }
        static wchar_t* StrRChr(const wchar_t* psz, wchar_t ch)
        {
            return const_cast<wchar_t*>(wcsrchr(psz, ch));
        }
        static wchar_t* StrStr(const wchar_t* psz, const wchar_t* psz2)
        {
            return const_cast<wchar_t*>(wcsstr(psz, psz2));
        }
        static wchar_t* StrUpper(wchar_t* psz)
        {
            return _wcsupr(psz);
        }
        static wchar_t* StrLower(wchar_t* psz)
        {
            return _wcslwr(psz);
        }
        static int IsSpace(wchar_t ch)
        {
            return iswspace(ch);
        }
        static wchar_t CharLower(wchar_t ch)
        {
            return (wchar_t)towlower(ch);
        }
        static wchar_t CharUpper(wchar_t ch)
        {
            return (wchar_t)towupper(ch);
        }
        static wchar_t* CharNext(wchar_t* psz)
        {
            return psz + 1;
        }
		static int Format(wchar_t** ppszDst, const wchar_t* pszFormat, va_list args)
		{
			int len = _vscwprintf(pszFormat, args);
			*ppszDst = (wchar_t*)DMMemDispatch::DM_malloc((len+1)*sizeof(wchar_t));
			vswprintf(*ppszDst, pszFormat, args);
			return len;
		}
		static int LoadString(HINSTANCE hInst,
			UINT uID,
			wchar_t* lpBuffer,
			int nBufferMax)
		{
			return ::LoadStringW(hInst,uID,lpBuffer,nBufferMax);
		}
		static wchar_t* _cstrrev(wchar_t* pStr)
		{
			// optimize NULL, zero-length, and single-char case
			if ((pStr == NULL) || (pStr[0] == L'\0') || (pStr[1] == L'\0'))
				return pStr;

			wchar_t* p = pStr;

			while (*p != 0) 
			{
				wchar_t* pNext = ::CharNextW(p);
				if(pNext > p + 1)
				{
					char p1 = *(char*)p;
					*(char*)p = *(char*)(p + 1);
					*(char*)(p + 1) = p1;
				}
				p = pNext;
			}

			p--;
			wchar_t* q = pStr;

			while (q < p)
			{
				wchar_t t = *q;
				*q = *p;
				*p = t;
				q++;
				p--;
			}
			return pStr;
		}
		static const wchar_t* _cstrchr_db(const wchar_t* p, wchar_t ch1, wchar_t ch2)
		{
			const wchar_t* lpsz = NULL;
			while (*p != 0)
			{
				if (*p == ch1 && *(p + 1) == ch2)
				{
					lpsz = p;
					break;
				}
				p = ::CharNextW(p);
			}
			return lpsz;
		}
		static const wchar_t* _cstrchr(const wchar_t* p, wchar_t ch)
		{
			// strchr for '\0' should succeed
			while (*p != 0)
			{
				if (*p == ch)
					break;
				p = ::CharNextW(p);
			}
			return (*p == ch) ? p : NULL;
		}
		static int _cstrspn(const wchar_t* pStr, const wchar_t* pCharSet)
		{
			int nRet = 0;
			const wchar_t* p = pStr;
			while (*p != 0)
			{
				const wchar_t* pNext = ::CharNextW(p);
				if(pNext > p + 1)
				{
					if(_cstrchr_db(pCharSet, *p, *(p + 1)) == NULL)
						break;
					nRet += 2;
				}
				else
				{
					if(_cstrchr(pCharSet, *p) == NULL)
						break;
					nRet++;
				}
				p = pNext;
			}
			return nRet;
		}

		static int _cstrcspn(const wchar_t* pStr, const wchar_t* pCharSet)
		{
			int nRet = 0;
			wchar_t* p = (wchar_t*)pStr;
			while (*p != 0)
			{
				wchar_t* pNext = ::CharNextW(p);
				if(pNext > p + 1)
				{
					if(_cstrchr_db(pCharSet, *p, *(p + 1)) != NULL)
						break;
					nRet += 2;
				}
				else
				{
					if(_cstrchr(pCharSet, *p) != NULL)
						break;
					nRet++;
				}
				p = pNext;
			}
			return nRet;
		}
    };

	//---------------------------------------------------------------------------------------------------------------------------------cstring!
	// DM - helper functions
	namespace DMSecureHelper
	{
		__pragma(warning(push))
		__pragma(warning(disable: 4995))
		__pragma(warning(disable: 4996))
#ifndef _SECURECRT_ERRCODE_VALUES_DEFINED
#define _SECURECRT_ERRCODE_VALUES_DEFINED
#define EINVAL          22
#define ERANGE          34
#define EILSEQ          42
#define STRUNCATE       80
#endif
	inline void memcpy_x(void* pDest, size_t cbDest, const void* pSrc, size_t cbSrc)
	{
		if(cbDest >= cbSrc)
			memcpy(pDest, pSrc, cbSrc);
		else
			DMASSERT(FALSE);
	}

	inline void memmove_x(void* pDest, size_t cbDest, const void* pSrc, size_t cbSrc)
	{
		if(cbDest >= cbSrc)
			memmove(pDest, pSrc, cbSrc);
		else
			DMASSERT(FALSE);
	}

	inline int vsprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, va_list args)
	{
		cchBuff;   // Avoid unused argument warning
		return _vstprintf(lpstrBuff, lpstrFormat, args);
	}

	inline int wvsprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, va_list args)
	{
		cchBuff;   // Avoid unused argument warning
		return ::wvsprintf(lpstrBuff, lpstrFormat, args);
	}

	inline int sprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, ...)
	{
		va_list args;
		va_start(args, lpstrFormat);
		int nRes = vsprintf_x(lpstrBuff, cchBuff, lpstrFormat, args);
		va_end(args);
		return nRes;
	}

	inline int wsprintf_x(LPTSTR lpstrBuff, size_t cchBuff, LPCTSTR lpstrFormat, ...)
	{
		va_list args;
		va_start(args, lpstrFormat);
		int nRes = wvsprintf_x(lpstrBuff, cchBuff, lpstrFormat, args);
		va_end(args);
		return nRes;
	}
	__pragma(warning(pop))
	}//namespace DMSecureHelper 

	/// <summary>
	///		DMStringT模板类\n 基于atlmisch.h->CString使用traits迭代器改变
	///     内部基于它使用了DM::CStringW,DM::CStringA,DM::CStringW\n
	///     用法可参考系统的CString
	/// </summary>
    template <class tchar, class tchar_traits>
    class DMStringT
    {
    public:
        typedef tchar    _tchar;
        typedef const _tchar * pctstr;
        typedef tchar_traits    _tchar_traits;

        // Constructors
        DMStringT()
        {
            Init();
        }
        DMStringT(const DMStringT& stringSrc)
        {
            DMASSERT(stringSrc.GetData()->nRefs != 0);
            if (stringSrc.GetData()->nRefs >= 0)
            {
                DMASSERT(stringSrc.GetData() != g_DMTempDataNil);
                m_pszData = stringSrc.m_pszData;
                GetData()->AddRef();
            }
            else
            {
                Init();
                *this = stringSrc.m_pszData;
            }
        }
        DMStringT(tchar ch, int nRepeat = 1)
        {
			DMASSERT(!_istlead(ch));   // can't create a lead byte string
            Init();
            if (nRepeat >= 1)
            {
                if (AllocBuffer(nRepeat))
                {
                    for (int i = 0; i < nRepeat; i++)
                        m_pszData[i] = ch;
                }
            }
        }
        DMStringT(const tchar* lpsz, int nLen)
        {
            Init();
            if(nLen<0)
			{
				nLen = SafeStrlen(lpsz);
			}
            if (nLen != 0)
            {
                if (AllocBuffer(nLen))
                    DMSecureHelper::memcpy_x(m_pszData, (nLen + 1) * sizeof(tchar), lpsz, nLen* sizeof(tchar));
            }
        }
        DMStringT(const tchar* lpsz)
        {
            Init();
            int nLen = SafeStrlen(lpsz);
            if (nLen != 0)
            {
                if (AllocBuffer(nLen))
                    DMSecureHelper::memcpy_x(m_pszData,  (nLen + 1) * sizeof(tchar),lpsz, nLen * sizeof(tchar));
            }
        }

        ~DMStringT()
        {
            //  free any attached data
            TStringData* pData = GetData();
            if (pData != g_DMTempDataNil)
                pData->Release();
        }

        // Attributes & Operations
        // as an array of characters
        int GetLength() const
        {
            return GetData()->nDataLength;
        }
        bool IsEmpty() const
        {
            return GetData()->nDataLength == 0;
        }
        void Empty()    // free up the data
        {
            TStringData* pData = GetData();
            if (pData->nDataLength == 0)
                return;

            if (pData->nRefs >= 0)
			{
                Release();
			}
            else
            {
                tchar sz[1] = {0};
                *this = sz;
            }

            DMASSERT(GetData()->nDataLength == 0);
            DMASSERT(GetData()->IsLocked() || GetData()->nAllocLength == 0);
        }

        tchar GetAt(int nIndex) const
        {
            DMASSERT(nIndex >= 0);
            DMASSERT(nIndex < GetData()->nDataLength);
            return m_pszData[nIndex];
        }
        tchar operator[](int nIndex) const
        {
            // same as GetAt
            DMASSERT(nIndex >= 0);
            DMASSERT(nIndex < GetData()->nDataLength);
            return m_pszData[nIndex];
        }
        void SetAt(int nIndex, tchar ch)
        {
            DMASSERT(nIndex >= 0);
            DMASSERT(nIndex < GetData()->nDataLength);

            CopyBeforeWrite();
            m_pszData[nIndex] = ch;
        }
        operator const tchar*() const    // as a C string
        {
            return m_pszData;
        }

        // overloaded assignment
        DMStringT& operator=(const DMStringT& stringSrc)
        {
            if (m_pszData != stringSrc.m_pszData)
            {
                TStringData* pData = GetData();
                if ((pData->IsLocked() && pData != g_DMTempDataNil) || stringSrc.GetData()->IsLocked())
                {
                    // actual copy necessary since one of the strings is locked
                    AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pszData);
                }
                else
                {
                    // can just copy references around
                    Release();
                    DMASSERT(stringSrc.GetData() != g_DMTempDataNil);
                    m_pszData = stringSrc.m_pszData;
                    GetData()->AddRef();
                }
            }
            return *this;
        }
        const DMStringT& operator=(const tchar* psz)
        {
            DMStringT strCopy(psz);
            AssignCopy(strCopy.GetData()->nDataLength, strCopy.m_pszData);
            return *this;
        }
        const DMStringT& operator=(tchar ch)
        {
            AssignCopy(1, &ch);
            return *this;
        }

        // string concatenation
        const DMStringT& operator+=(const tchar* psz)
        {
            return Append(psz);
        }
        
        const DMStringT& operator+=(tchar ch)
        {
            return AppendChar(ch);
        }
        
        const DMStringT& operator+=(const DMStringT& src)
        {
            return AppendStr(src);
        }
        
        const DMStringT& AppendChar(tchar ch)
        {
            ConcatInPlace(1, &ch);
            return *this;
        }

        const DMStringT& Append(const tchar * psz)
        {
            DMStringT strCopy(psz);
            ConcatInPlace(strCopy.GetData()->nDataLength, strCopy.m_pszData);
            return *this;
        }
        
        const DMStringT& AppendStr(const DMStringT& src)
        {
            DMStringT strCopy(src);
            ConcatInPlace(strCopy.GetData()->nDataLength, strCopy.m_pszData);
            return *this;
        }

        // string comparison
        int Compare(const tchar* psz) const
        {
            return tchar_traits::StrCmp(m_pszData, psz);
        }
        int CompareNoCase(const tchar* psz) const
        {
            return tchar_traits::StrICmp(m_pszData, psz);
        }

        // simple sub-string extraction
        DMStringT MidLimit(int nFirst) const
        {
            return Mid(nFirst, GetData()->nDataLength - nFirst);
        }
        DMStringT Mid(int nFirst, int nCount) const
        {
            // out-of-bounds requests return sensible things
            if (nFirst < 0)
                nFirst = 0;
            if (nCount < 0)
                nCount = 0;

            TStringData* pData = GetData();
            if (nFirst + nCount > pData->nDataLength)
                nCount = pData->nDataLength - nFirst;
            if (nFirst > pData->nDataLength)
                nCount = 0;

            DMStringT dest;
            AllocCopy(dest, nCount, nFirst, 0);
            return dest;
        }
        DMStringT Right(int nCount) const
        {
            TStringData* pData = GetData();
            if (nCount < 0)
                nCount = 0;
            else if (nCount > pData->nDataLength)
                nCount = pData->nDataLength;

            DMStringT dest;
            AllocCopy(dest, nCount, pData->nDataLength - nCount, 0);
            return dest;
        }
        DMStringT Left(int nCount) const
        {
            TStringData* pData = GetData();
            if (nCount < 0)
                nCount = 0;
            else if (nCount > pData->nDataLength)
                nCount = pData->nDataLength;

            DMStringT dest;
            AllocCopy(dest, nCount, 0, 0);
            return dest;
        }
		DMStringT SpanIncluding(const tchar* lpszCharSet) const   // strspn equivalent
		{
			DMASSERT(NULL != lpszCharSet);
			return Left(tchar_traits::_cstrspn(m_pszData, lpszCharSet));
		}
		DMStringT SpanExcluding(const tchar* lpszCharSet) const   // strcspn equivalent
		{
			DMASSERT(NULL != lpszCharSet);
			return Left(tchar_traits::_cstrcspn(m_pszData, lpszCharSet));
		}

        //    string utilities
        DMStringT& MakeUpper()
        {
            CopyBeforeWrite();

            if (m_pszData != NULL)
                tchar_traits::StrUpper(m_pszData);
            return *this;
        }
        DMStringT& MakeLower()
        {
            CopyBeforeWrite();

            if (m_pszData != NULL)
                tchar_traits::StrLower(m_pszData);
            return *this;
        }
		void MakeReverse()
		{
			CopyBeforeWrite();
			tchar_traits::_cstrrev(m_pszData);
		}

        // remove continuous occcurrences of characters in passed string, starting from right
        DMStringT & TrimRight(tchar chTarget=VK_SPACE)
        {
            CopyBeforeWrite();

            // find beginning of trailing matches
            // by starting at beginning (DBCS aware)
            tchar* psz = m_pszData;
            tchar* pszLast = NULL;

            while (*psz != '\0')
            {
                if (*psz == chTarget)
                {
                    if (pszLast == NULL)
                        pszLast = psz;
                }
                else
                    pszLast = NULL;
                psz = tchar_traits::CharNext(psz);
            }

            if (pszLast != NULL)
            {
                // truncate at left-most matching character
                *pszLast = '\0';
                GetData()->nDataLength = (int)(pszLast - m_pszData);
            }
            return *this;
        }

		DMStringT & TrimRightStr(const tchar* lpszTargetList)
		{
			// find beginning of trailing matches by starting at beginning (DBCS aware)

			CopyBeforeWrite();
			const tchar* lpsz = m_pszData;
			const tchar* lpszLast = NULL;

			while (*lpsz)
			{
				const tchar* pNext = tchar_traits::CharNext((tchar*)lpsz);
				if(pNext > lpsz + 1)
				{
					if (tchar_traits::_cstrchr_db(lpszTargetList, *lpsz, *(lpsz + 1)) != NULL)
					{
						if (lpszLast == NULL)
							lpszLast = lpsz;
					}
					else
					{
						lpszLast = NULL;
					}
				}
				else
				{
					if (tchar_traits::_cstrchr(lpszTargetList, *lpsz) != NULL)
					{
						if (lpszLast == NULL)
							lpszLast = lpsz;
					}
					else
					{
						lpszLast = NULL;
					}
				}

				lpsz = pNext;
			}

			if (lpszLast != NULL)
			{
				// truncate at left-most matching character
				tchar sz[1] = {0};
				DMSecureHelper::memcpy_x((void*)lpszLast,sizeof(tchar),sz, sizeof(tchar));
				GetData()->nDataLength = (int)(DWORD_PTR)(lpszLast - m_pszData);
			}
			return *this;
		}

        // remove continuous occurrences of chTarget starting from left
        DMStringT & TrimLeft(tchar chTarget=VK_SPACE)
        {
            CopyBeforeWrite();

            // find first non-matching character
            tchar* psz = m_pszData;

            while (chTarget == *psz)
                psz = tchar_traits::CharNext(psz);

            if (psz != m_pszData)
            {
                // fix up data and length
                TStringData* pData = GetData();
                int nDataLength = pData->nDataLength - (int)(psz - m_pszData);
                DMSecureHelper::memmove_x(m_pszData, (GetData()->nAllocLength + 1) * sizeof(tchar),psz, (nDataLength + 1) * sizeof(tchar));
                pData->nDataLength = nDataLength;
            }
            return *this;
        }

		DMStringT & TrimLeftStr(const tchar* lpszTargets)
		{
			// if we're not trimming anything, we're not doing any work
			if (SafeStrlen(lpszTargets) == 0)
				return *this;

			CopyBeforeWrite();
			const tchar* lpsz = m_pszData;

			while (*lpsz)
			{
				tchar* pNext = tchar_traits::CharNext((tchar*)lpsz);
				if(pNext > lpsz + 1)
				{
					if (tchar_traits::_cstrchr_db(lpszTargets, *lpsz, *(lpsz + 1)) == NULL)
						break;
				}
				else
				{
					if (tchar_traits::_cstrchr(lpszTargets, *lpsz) == NULL)
						break;
				}
				lpsz = pNext;
			}

			if (lpsz != m_pszData)
			{
				// fix up data and length
				int nDataLength = GetData()->nDataLength - (int)(DWORD_PTR)(lpsz - m_pszData);
				DMSecureHelper::memmove_x(m_pszData, (GetData()->nAllocLength + 1) * sizeof(tchar), lpsz, (nDataLength + 1) * sizeof(tchar));
				GetData()->nDataLength = nDataLength;
			}
			 return *this;
		}

        DMStringT & Trim(tchar ch=VK_SPACE)
        {
            TrimRight(ch);
            TrimLeft(ch);
            return *this;
        }
        static bool IsBlankChar(const tchar &c)
        {
            const tchar szBlank[]={0x0a,0x0d,0x20,0x09};
            for(int i = 0; i<ARRAYSIZE(szBlank);i++)
            {
                if(c == szBlank[i]) return true;
            }
            return false;
        }

        void TrimBlank()
        {
            if(IsEmpty()) return;

            const tchar * pbuf=m_pszData;
            const tchar * p = pbuf;
            //look for start
            while(*p)
            {
                if(!IsBlankChar(*p)) break;
                p++;
            }
            const tchar * p1=p;   //get start
            //look for end
            const tchar * p2=pbuf + GetLength()-1;
            while(p2>=p1)
            {
                if(!IsBlankChar(*p2)) break;
                p2--;
            }
            if(p2 < p1)
                Empty();
            else
                (*this) = DMStringT<tchar, tchar_traits>(p1,(int)(p2-p1+1));
        }

        // insert character at zero-based index; concatenates if index is past end of string
        int InsertChar(int nIndex, tchar ch)
        {
            CopyBeforeWrite();

            if (nIndex < 0)
                nIndex = 0;

            TStringData* pData = GetData();
            int nNewLength = pData->nDataLength;
            if (nIndex > nNewLength)
                nIndex = nNewLength;
            nNewLength++;

            if (pData->nAllocLength < nNewLength)
                if (! ReallocBuffer(nNewLength))
                    return -1;

            // move existing bytes down
            DMSecureHelper::memmove_x(m_pszData + nIndex + 1, (GetData()->nAllocLength - nIndex) * sizeof(tchar), m_pszData + nIndex, (nNewLength - nIndex) * sizeof(tchar));
            m_pszData[nIndex] = ch;
            GetData()->nDataLength = nNewLength;

            return nNewLength;
        }
        // insert substring at zero-based index; concatenates if index is past end of string
        inline int Insert(int nIndex, const tchar* psz)
        {
            if (nIndex < 0)
                nIndex = 0;

            int nInsertLength = SafeStrlen(psz);
            int nNewLength = GetData()->nDataLength;
            if (nInsertLength > 0)
            {
                CopyBeforeWrite();

                if (nIndex > nNewLength)
                    nIndex = nNewLength;
                nNewLength += nInsertLength;

                TStringData* pData = GetData();
                if (pData->nAllocLength < nNewLength)
                    if (! ReallocBuffer(nNewLength))
                        return -1;

                // move existing bytes down
                DMSecureHelper::memmove_x(m_pszData + nIndex + nInsertLength,(GetData()->nAllocLength + 1 - nIndex - nInsertLength) * sizeof(tchar), m_pszData + nIndex, (nNewLength - nIndex - nInsertLength + 1) * sizeof(tchar));
                DMSecureHelper::memcpy_x(m_pszData + nIndex, (GetData()->nAllocLength + 1 - nIndex) * sizeof(tchar), psz, nInsertLength * sizeof(tchar));
                GetData()->nDataLength = nNewLength;
            }
            return nNewLength;
        }
        inline int Delete(int nIndex, int nCount = 1)
        {
            if (nIndex < 0)
                nIndex = 0;
            int nLength = GetData()->nDataLength;
            if (nCount > 0 && nIndex < nLength)
            {
                if((nIndex + nCount) > nLength)
                    nCount = nLength - nIndex;

                CopyBeforeWrite();

                int nBytesToCopy = nLength - (nIndex + nCount) + 1;
                DMSecureHelper::memmove_x(m_pszData + nIndex, (GetData()->nAllocLength + 1 - nIndex) * sizeof(tchar), m_pszData + nIndex + nCount, nBytesToCopy * sizeof(tchar));
                nLength -= nCount;
                GetData()->nDataLength = nLength;
            }

            return nLength;
        }
        int ReplaceChar(tchar chOld, tchar chNew)
        {
            int nCount = 0;

            // short-circuit the nop case
            if (chOld != chNew)
            {
                CopyBeforeWrite();

                // otherwise modify each character that matches in the string
                tchar* psz = m_pszData;
                tchar* pszEnd = psz + GetData()->nDataLength;
                while (psz < pszEnd)
                {
                    // replace instances of the specified character only
                    if (*psz == chOld)
                    {
                        *psz = chNew;
                        nCount++;
                    }
                    psz = tchar_traits::CharNext(psz);
                }
            }
            return nCount;
        }
        inline int Replace(const tchar* pszOld, const tchar* pszNew)
        {
            // can't have empty or NULL pszOld
            int nSourceLen = SafeStrlen(pszOld);
            if (nSourceLen == 0)
                return 0;
            int nReplacementLen = SafeStrlen(pszNew);

            // loop once to figure out the size of the result string
            int nCount = 0;
            tchar* pszStart = m_pszData;
            tchar* pszEnd = m_pszData + GetData()->nDataLength;
            tchar* pszTarget;
            while (pszStart < pszEnd)
            {
                while ((pszTarget = tchar_traits::StrStr(pszStart, pszOld)) != NULL)
                {
                    nCount++;
                    pszStart = pszTarget + nSourceLen;
                }
                pszStart += tchar_traits::StrLen(pszStart) + 1;
            }

            // if any changes were made, make them
            if (nCount > 0)
            {
                CopyBeforeWrite();

                // if the buffer is too small, just
                //   allocate a new buffer (slow but sure)
                TStringData* pOldData = GetData();
                int nOldLength = pOldData->nDataLength;
                int nNewLength =  nOldLength + (nReplacementLen - nSourceLen) * nCount;
                if (pOldData->nAllocLength < nNewLength || pOldData->IsShared())
                    if (! ReallocBuffer(nNewLength))
                        return -1;

                // else, we just do it in-place
                pszStart = m_pszData;
                pszEnd = m_pszData + GetData()->nDataLength;

                // loop again to actually do the work
                while (pszStart < pszEnd)
                {
                    while ((pszTarget = tchar_traits::StrStr(pszStart, pszOld)) != NULL)
                    {
                        int nBalance = nOldLength - ((int)(pszTarget - m_pszData) + nSourceLen);
						int cchBuffLen = GetData()->nAllocLength - (int)(DWORD_PTR)(pszTarget - m_pszData);
                        DMSecureHelper::memmove_x(pszTarget + nReplacementLen, (cchBuffLen - nReplacementLen + 1) * sizeof(tchar), pszTarget + nSourceLen, nBalance * sizeof(tchar));
                        DMSecureHelper::memcpy_x(pszTarget, (cchBuffLen + 1) * sizeof(tchar), pszNew, nReplacementLen * sizeof(tchar));
                        pszStart = pszTarget + nReplacementLen;
                        pszStart[nBalance] = '\0';
                        nOldLength += (nReplacementLen - nSourceLen);
                    }
                    pszStart += tchar_traits::StrLen(pszStart) + 1;
                }
                DMASSERT(m_pszData[nNewLength] == '\0');
                GetData()->nDataLength = nNewLength;
            }
            return nCount;
        }
        inline int Remove(tchar chRemove)
        {
            CopyBeforeWrite();

            tchar* pstrSource = m_pszData;
            tchar* pstrDest = m_pszData;
            tchar* pstrEnd = m_pszData + GetData()->nDataLength;

            while (pstrSource < pstrEnd)
            {
                if (*pstrSource != chRemove)
                {
                    *pstrDest = *pstrSource;
                    pstrDest = tchar_traits::CharNext(pstrDest);
                }
                pstrSource = tchar_traits::CharNext(pstrSource);
            }
            *pstrDest = '\0';
            int nCount = (int)(pstrSource - pstrDest);
            GetData()->nDataLength -= nCount;

            return nCount;
        }

        // searching (return starting index, or -1 if not found)
        // look for a single character match
        int FindChar(tchar ch, int nStart = 0) const
        {
            int nLength = GetData()->nDataLength;
            if (nStart >= nLength)
                return -1;

            // find first single character
            tchar* psz = tchar_traits::StrChr(m_pszData + nStart, ch);

            // return -1 if not found and index otherwise
            return (psz == NULL) ? -1 : (int)(psz - m_pszData);
        }
        int ReverseFind(tchar ch) const
        {
            // find last single character
            tchar* psz = tchar_traits::StrRChr(m_pszData, ch);

            // return -1 if not found, distance from beginning otherwise
            return (psz == NULL) ? -1 : (int)(psz - m_pszData);
        }

        // find a sub-string (like strstr)
        int Find(const tchar* pszSub, int nStart = 0) const
        {
            int nLength = GetData()->nDataLength;
            if (nStart > nLength)
                return -1;

            // find first matching substring
            tchar* psz = tchar_traits::StrStr(m_pszData + nStart, pszSub);

            // return -1 for not found, distance from beginning otherwise
            return (psz == NULL) ? -1 : (int)(psz - m_pszData);
        }
        // formatting (using sprintf style formatting)
        DMStringT __cdecl Format(const tchar* pszFormat, ...)
        {
			va_list argList;
			va_start(argList, pszFormat);
			_Format(pszFormat, argList);
			va_end(argList);
			return *this;
        }
        // Append formatted data using format string 'pszFormat'
        DMStringT  __cdecl AppendFormat(const tchar* pszFormat, ...)
        {
			va_list argList;
			va_start(argList, pszFormat);
			_AppendFormat(pszFormat, argList);
			va_end(argList);
			return *this;
        }
		bool LoadString(HINSTANCE hInst, UINT nID)
		{
			tchar szTemp[256] = {0};
			int nChar = tchar_traits::LoadString(hInst, nID,szTemp,256);
			if(nChar==0) return false;
			AssignCopy(nChar,buf);
			return true;
		}


        // Access to string implementation buffer as "C" character array
        tchar* GetBuffer(int nMinBufLength=-1)
        {
			if (-1==nMinBufLength)
			{
				nMinBufLength = GetLength();
			}
            DMASSERT(nMinBufLength >= 0);

            TStringData* pData = GetData();
            if (pData->IsShared() || nMinBufLength > pData->nAllocLength)
            {
                // we have to grow the buffer
                int nOldLen = pData->nDataLength;
                if (nMinBufLength < nOldLen)
                    nMinBufLength = nOldLen;
                if (! ReallocBuffer(nMinBufLength))
                    return NULL;
            }
            DMASSERT(GetData()->nRefs <= 1);

            // return a pointer to the character storage for this string
            DMASSERT(m_pszData != NULL);
            return m_pszData;
        }
        void ReleaseBuffer(int nNewLength = -1)
        {
            CopyBeforeWrite();  // just in case GetBuffer was not called

            if (nNewLength == -1)
                nNewLength = SafeStrlen(m_pszData); // zero terminated

            TStringData* pData = GetData();
            DMASSERT(nNewLength <= pData->nAllocLength);
            pData->nDataLength = nNewLength;
            m_pszData[nNewLength] = '\0';
        }
        tchar* GetBufferSetLength(int nNewLength)
        {
            DMASSERT(nNewLength >= 0);

            if (GetBuffer(nNewLength) == NULL)
                return NULL;

            GetData()->nDataLength = nNewLength;
            m_pszData[nNewLength] = '\0';
            return m_pszData;
        }
        void SetLength(int nLength)
        {
            DMASSERT(nLength >= 0);
            DMASSERT(nLength <= GetData()->nAllocLength);

            if (nLength >= 0 && nLength < GetData()->nAllocLength)
            {
                GetData()->nDataLength = nLength;
                m_pszData[nLength] = 0;
            }
        }
        void Preallocate(int nLength)
        {
            int nOldLength = GetLength();
            GetBuffer(nLength);
            ReleaseBuffer(nOldLength);
        }
        void FreeExtra()
        {
            TStringData* pData = GetData();
            DMASSERT(pData->nDataLength <= pData->nAllocLength);
            if (pData->nDataLength < pData->nAllocLength)
            {
                if (ReallocBuffer(pData->nDataLength))
                    DMASSERT(m_pszData[GetData()->nDataLength] == '\0');
            }
            DMASSERT(GetData() != NULL);
        }

        // Use LockBuffer/UnlockBuffer to turn refcounting off
        tchar* LockBuffer()
        {
            tchar* psz = GetBuffer(0);
            if (psz != NULL)
                GetData()->Lock();
            return psz;
        }
        void UnlockBuffer()
        {
            if (GetData() != g_DMTempDataNil)
                GetData()->Unlock();
        }

        friend inline bool __stdcall operator==(const DMStringT& s1, const DMStringT& s2)
        { return s1.Compare(s2) == 0; }
        friend inline bool __stdcall operator==(const DMStringT& s1, const tchar* s2)
        { return s1.Compare(s2) == 0; }
        friend inline bool __stdcall operator==(const tchar* s1, const DMStringT& s2)
        { return s2.Compare(s1) == 0; }

        friend inline bool __stdcall operator!=(const DMStringT& s1, const DMStringT& s2)
        { return s1.Compare(s2) != 0; }
        friend inline bool __stdcall operator!=(const DMStringT& s1, const tchar* s2)
        { return s1.Compare(s2) != 0; }
        friend inline bool __stdcall operator!=(const tchar* s1, const DMStringT& s2)
        { return s2.Compare(s1) != 0; }

        friend inline bool __stdcall operator<(const DMStringT& s1, const DMStringT& s2)
        { return s1.Compare(s2) < 0; }
        friend inline bool __stdcall operator<(const DMStringT& s1, const tchar* s2)
        { return s1.Compare(s2) < 0; }
        friend inline bool __stdcall operator<(const tchar* s1, const DMStringT& s2)
        { return s2.Compare(s1) > 0; }

        friend inline bool __stdcall operator>(const DMStringT& s1, const DMStringT& s2)
        { return s1.Compare(s2) > 0; }
        friend inline bool __stdcall operator>(const DMStringT& s1, const tchar* s2)
        { return s1.Compare(s2) > 0; }
        friend inline bool __stdcall operator>(const tchar* s1, const DMStringT& s2)
        { return s2.Compare(s1) < 0; }

        friend inline bool __stdcall operator<=(const DMStringT& s1, const DMStringT& s2)
        { return s1.Compare(s2) <= 0; }
        friend inline bool __stdcall operator<=(const DMStringT& s1, const tchar* s2)
        { return s1.Compare(s2) <= 0; }
        friend inline bool __stdcall operator<=(const tchar* s1, const DMStringT& s2)
        { return s2.Compare(s1) >= 0; }

        friend inline bool __stdcall operator>=(const DMStringT& s1, const DMStringT& s2)
        { return s1.Compare(s2) >= 0; }
        friend inline bool __stdcall operator>=(const DMStringT& s1, const tchar* s2)
        { return s1.Compare(s2) >= 0; }
        friend inline bool __stdcall operator>=(const tchar* s1, const DMStringT& s2)
        { return s2.Compare(s1) <= 0; }

        friend inline DMStringT __stdcall operator+(const DMStringT& string1, const DMStringT& string2)
        {
            DMStringT s;
            s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pszData, string2.GetData()->nDataLength, string2.m_pszData);
            return s;
        }
        friend inline DMStringT __stdcall operator+(const DMStringT& string, const tchar* psz)
        {
            DMASSERT(psz != NULL);
            DMStringT s;
            s.ConcatCopy(string.GetData()->nDataLength, string.m_pszData, DMStringT::SafeStrlen(psz), psz);
            return s;
        }
        friend inline DMStringT __stdcall operator+(const tchar* psz, const DMStringT& string)
        {
            DMASSERT(psz != NULL);
            DMStringT s;
            s.ConcatCopy(DMStringT::SafeStrlen(psz), psz, string.GetData()->nDataLength, string.m_pszData);
            return s;
        }
        friend inline DMStringT __stdcall operator+(const DMStringT& string1, tchar ch)
        {
            DMStringT s;
            s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pszData, 1, &ch);
            return s;
        }
        friend inline DMStringT __stdcall operator+(tchar ch, const DMStringT& string)
        {
            DMStringT s;
            s.ConcatCopy(1, &ch, string.GetData()->nDataLength, string.m_pszData);
            return s;
        }

        // Implementation
    public:
        inline int GetAllocLength() const
        {
            return GetData()->nAllocLength;
        }

        static int SafeStrlen(const tchar* psz)
        {
            return (psz == NULL) ? 0 : (int)tchar_traits::StrLen(psz);
        }

    protected:
        // implementation helpers
        inline TStringData* GetData() const
        {
            DMASSERT(m_pszData != NULL);
            return ((TStringData*)m_pszData) - 1;
        }
        inline void Init()
        {
            m_pszData = (tchar*)g_DMtmpPchNil;
        }

        // Assignment operators
        //  All assign a new value to the string
        //      (a) first see if the buffer is big enough
        //      (b) if enough room, copy on top of old buffer, set size and type
        //      (c) otherwise free old string data, and create a new one
        //
        //  All routines return the new string (but as a 'const DMStringT&' so that
        //      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
        //
        void AllocCopy(DMStringT& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
        {
            // will clone the data attached to this string
            // allocating 'nExtraLen' characters
            // Places results in uninitialized string 'dest'
            // Will copy the part or all of original data to start of new string

            int nNewLen = nCopyLen + nExtraLen;
            if (nNewLen == 0)
            {
                dest.Init();
            }
            else
            {
                if (dest.ReallocBuffer(nNewLen))
                    memcpy(dest.m_pszData, m_pszData + nCopyIndex, nCopyLen * sizeof(tchar));
            }
        }

        void AssignCopy(int nSrcLen, const tchar* pszSrcData)
        {
            if (AllocBeforeWrite(nSrcLen))
            {
                memcpy(m_pszData, pszSrcData, nSrcLen * sizeof(tchar));
                GetData()->nDataLength = nSrcLen;
                m_pszData[nSrcLen] = '\0';
            }
        }

        // Concatenation
        // NOTE: "operator+" is done as friend functions for simplicity
        //      There are three variants:
        //          DMStringT + DMStringT
        // and for ? = tchar, const tchar*
        //          DMStringT + ?
        //          ? + DMStringT

        bool ConcatCopy(int nSrc1Len, const tchar* pszSrc1Data, int nSrc2Len, const tchar* pszSrc2Data)
        {
            // -- master concatenation routine
            // Concatenate two sources
            // -- assume that 'this' is a new DMStringT object

            bool bRet = true;
            int nNewLength = nSrc1Len + nSrc2Len;
            if (nNewLength != 0)
            {
                bRet = ReallocBuffer(nNewLength);
                if (bRet)
                {
                    memcpy(m_pszData, pszSrc1Data, nSrc1Len * sizeof(tchar));
                    memcpy(m_pszData + nSrc1Len, pszSrc2Data, nSrc2Len * sizeof(tchar));
                }
            }
            return bRet;
        }
        void ConcatInPlace(int nSrcLen, const tchar* pszSrcData)
        {
            //  -- the main routine for += operators

            // concatenating an empty string is a no-op!
            if (nSrcLen == 0)
                return;

            // if the buffer is too small, or we have a width mis-match, just
            //   allocate a new buffer (slow but sure)
            TStringData* pData = GetData();
            if (pData->IsShared() || pData->nDataLength + nSrcLen > pData->nAllocLength)
            {
                // we have to grow the buffer
                int nOldDataLength = pData->nDataLength;
                int nNewLength = nOldDataLength + nSrcLen;
                if (ReallocBuffer(nNewLength))
                    memcpy(m_pszData + nOldDataLength, pszSrcData, nSrcLen * sizeof(tchar));
            }
            else
            {
                // fast concatenation when buffer big enough
                memcpy(m_pszData + pData->nDataLength, pszSrcData, nSrcLen * sizeof(tchar));
                pData->nDataLength += nSrcLen;
                DMASSERT(pData->nDataLength <= pData->nAllocLength);
                m_pszData[pData->nDataLength] = '\0';
            }
        }
        void CopyBeforeWrite()
        {
            TStringData* pData = GetData();
            if (pData->IsShared())
            {
                Release();
                if (AllocBuffer(pData->nDataLength))
                    memcpy(m_pszData, pData->data(), (pData->nDataLength + 1) * sizeof(tchar));
            }
            DMASSERT(GetData()->nRefs <= 1);
        }
        bool AllocBeforeWrite(int nLen)
        {
            bool bRet = true;
            TStringData* pData = GetData();
            if (pData->IsShared() || nLen > pData->nAllocLength)
            {
                Release();
                bRet = AllocBuffer(nLen);
            }
            DMASSERT(GetData()->nRefs <= 1);
            return bRet;
        }

        bool AllocBuffer(int nLength)
        {
            TStringData* pData = AllocData(nLength);
            if (pData != NULL)
            {
                m_pszData = (tchar*)pData->data();
                return true;
            }
            return false;
        }

        bool ReallocBuffer(int nNewLength)
        {
#define TSTRING_REALLOC
#ifdef TSTRING_REALLOC
            TStringData* pData = GetData();
            if (! pData->IsShared() && pData != g_DMTempDataNil)
            {
                pData = AllocData(nNewLength, pData);
                if (pData != NULL)
                {
                    m_pszData = (tchar*)pData->data();
                    return true;
                }

                Init();
                return false;
            }
#endif
            TStringData* pOldData = GetData();
            tchar* psz = m_pszData;
            if (AllocBuffer(nNewLength))
            {
                int nLength = min(pOldData->nDataLength, nNewLength) + 1;
                memcpy(m_pszData, psz, nLength * sizeof(tchar));
                ReleaseData(pOldData);
                return true;
            }
            return false;
        }
        void Release()
        {
            TStringData* pData = GetData();
            if (pData != g_DMTempDataNil)
            {
                DMASSERT(pData->nRefs != 0);
                pData->Release();
                Init();
            }
        }

		bool _Format(const tchar* pszFormat, va_list args)
		{
			if (pszFormat == NULL || *pszFormat == '\0')
			{
				Empty();
				return false;
			}

			tchar* pszBuffer = NULL;
			int nLength = tchar_traits::Format(&pszBuffer, pszFormat, args);
			if (nLength > 0 && pszBuffer != NULL)
			{
				*this = DMStringT(pszBuffer, nLength);
				DMMemDispatch::DM_free(pszBuffer);
				return true;
			}
			return false;
		}

		void _AppendFormat(const tchar* pszFormat, va_list args)
		{
			if (pszFormat == NULL || *pszFormat == '\0')
				return;

			tchar* pszBuffer = NULL;
			int nLength = tchar_traits::Format(&pszBuffer, pszFormat, args);
			if (nLength > 0 && pszBuffer != NULL)
			{
				*this += DMStringT(pszBuffer, nLength);
				DMMemDispatch::DM_free(pszBuffer);
			}
		}

        // always allocate one extra character for '\0' termination
        // assumes [optimistically] that data length will equal allocation length
        static TStringData* AllocData(int nLength, TStringData* pOldData = NULL)
        {
            DMASSERT(nLength >= 0);
            DMASSERT(nLength <= 0x7fffffff);    // max size (enough room for 1 extra)

            if (nLength == 0)
                return g_DMTempDataNil;

            int nSize = sizeof(TStringData) + (nLength + 1 + TSTRING_PADDING) * sizeof(tchar);
            TStringData* pData;
            if (pOldData == NULL)
                pData = (TStringData*)DMMemDispatch::DM_malloc(nSize);
            else
                pData = (TStringData*)DMMemDispatch::DM_realloc(pOldData, nSize);
            if (pData == NULL)
                return NULL;

            pData->nRefs = 1;
            pData->nDataLength = nLength;
            pData->nAllocLength = nLength;

            tchar* pchData = (tchar*)pData->data();
            pchData[nLength] = '\0';

#if TSTRING_PADDING > 0
            for (int i = 1; i <= TSTRING_PADDING; i++)
                pchData[nLength + i] = '\0';
#endif

            return pData;
        }
        static void ReleaseData(TStringData* pData)
        {
            if (pData != g_DMTempDataNil)
            {
                DMASSERT(pData->nRefs != 0);
                pData->Release();
            }
        }

    protected:
        tchar* m_pszData;   // pointer to ref counted string data
    };

#ifdef DM_EXPORT
#    define EXPIMP_TEMPLATE
#else
#    define EXPIMP_TEMPLATE extern
#endif

    #pragma warning (disable : 4231)
	typedef DMStringT<char, char_traits>        CStringA;
	typedef DMStringT<wchar_t, wchar_traits>     CStringW;// 做为函数参数时，最好使用引用，然后内部建立临时变量处理，不然跨dll可能崩溃

#ifdef _UNICODE
    typedef CStringW                        CStringT;
#else
    typedef CStringA                        CStringT;
#endif

	static CStringW DMA2W(const CStringA &str, UINT CodePage=CP_ACP)
	{
		int nSize = ::MultiByteToWideChar(CodePage, 0, str, str.GetLength(), NULL, 0);		
		if (nSize>0)
		{
			 wchar_t *pBuf=new wchar_t[nSize];
			 ::MultiByteToWideChar(CodePage, 0, str, str.GetLength(), pBuf, nSize);
			 CStringW strw(pBuf, nSize);
			 delete []pBuf;
			 pBuf = NULL;
			 return strw;
		}
		return L"";
	}

	/// <summary>
	///	用于脚本中char*直接转CStringW
	/// </summary>
	static CStringW DMCA2W(LPCSTR lpsz, UINT CodePage=CP_ACP)
	{
		CStringA str = lpsz;
		CStringW strw = DMA2W(str, CodePage);
		return strw;
	}

	static CStringA DMW2A(const CStringW &str, UINT CodePage=CP_ACP)
	{
		int nSize = ::WideCharToMultiByte(CodePage,0,str, str.GetLength(), NULL, 0, NULL, NULL);
		if (nSize>0)
		{
			char *pBuf = new char[nSize];
			::WideCharToMultiByte(CodePage,0,str, str.GetLength(), pBuf, nSize, NULL, NULL);
			CStringA stra(pBuf,nSize);
			delete []pBuf;
			pBuf = NULL;
			return stra;
		}
		return "";
	}

	static CStringW DMW2W(const CStringW &str)
	{
		return str;
	}

	static CStringA DMA2A(const CStringA &str, UINT CodePageFrom=CP_UTF8, UINT CodePageTo=CP_ACP)
	{
		if (CodePageFrom == CodePageTo)
		{
			return str;
		}
		CStringW strw = DMA2W(str,CodePageFrom);
		return DMW2A(strw, CodePageTo);
	}


#ifdef UNICODE
#define  DMA2T		DMA2W
#define  DMW2T      DMW2W
#define  DMT2A      DMW2A
#define  DMT2W      DMW2W
#else
#define  DMA2T		DMA2A
#define  DMW2T      DMW2A
#define  DMT2A      DMA2A
#define  DMT2W      DMA2W
#endif

    template< typename T >
    class SStringElementTraits
    {
    public:
        typedef typename T::pctstr INARGTYPE;
        typedef T& OUTARGTYPE;

        static void __cdecl CopyElements( T* pDest, const T* pSrc, size_t nElements )
        {
            for( size_t iElement = 0; iElement < nElements; iElement++ )
            {
                pDest[iElement] = pSrc[iElement];
            }
        }

        static void __cdecl RelocateElements(  T* pDest, T* pSrc,size_t nElements )
        {
            memmove_s( pDest, nElements*sizeof( T ), pSrc, nElements*sizeof( T ) );
        }

        static ULONG __cdecl Hash(  INARGTYPE  str )
        {
            DMASSERT( str != NULL );
            ULONG nHash = 0;
            const T::_tchar * pch = str;
            while( *pch != 0 )
            {
                nHash = (nHash<<5)+nHash+(*pch);
                pch++;
            }

            return( nHash );
        }

        static bool __cdecl CompareElements(  INARGTYPE str1,  INARGTYPE str2 )
        {
            return( T::_tchar_traits::StrCmp( str1, str2 ) == 0 );
        }

        static int __cdecl CompareElementsOrdered(  INARGTYPE str1,  INARGTYPE str2 )
        {
            return( T::_tchar_traits::StrCmp( str1, str2 ) );
        }
    };

    template< typename T >
    class CElementTraits;

    template<>
    class CElementTraits< CStringA > :
        public SStringElementTraits< CStringA >
    {
    };

    template<>
    class CElementTraits< CStringW > :
        public SStringElementTraits< CStringW >
    {
    };
	#pragma warning(pop)

}//end of namespace

