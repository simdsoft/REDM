//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMMsgCrack.h 
// File Des: 仿GDI+、仿CString.CArray.CMap.CList
// File Summary: CString使用注意:http://www.cppblog.com/weiym/archive/2012/07/13/183262.html
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once
#include "DMStringT.h"

namespace DM
{
	// forward declarations
	class CSize;
	class CPoint;
	class CRect;

	//---------------------------------------------------
	// CSize - Wrapper for Windows SIZE structure.
	class CSize : public SIZE
	{
	public:
		// Constructors
		CSize()
		{
			cx = 0;
			cy = 0;
		}

		CSize(int initCX, int initCY)
		{
			cx = initCX;
			cy = initCY;
		}

		CSize(SIZE initSize)
		{
			*(SIZE*)this = initSize;
		}

		CSize(POINT initPt)
		{
			*(POINT*)this = initPt;
		}

		CSize(DWORD dwSize)
		{
			cx = (short)LOWORD(dwSize);
			cy = (short)HIWORD(dwSize);
		}

		// Operations
		BOOL operator ==(SIZE size) const
		{
			return (cx == size.cx && cy == size.cy);
		}

		BOOL operator !=(SIZE size) const
		{
			return (cx != size.cx || cy != size.cy);
		}

		void operator +=(SIZE size)
		{
			cx += size.cx;
			cy += size.cy;
		}

		void operator -=(SIZE size)
		{
			cx -= size.cx;
			cy -= size.cy;
		}

		void SetSize(int CX, int CY)
		{
			cx = CX;
			cy = CY;
		}

		// Operators returning CSize values
		CSize operator +(SIZE size) const
		{
			return CSize(cx + size.cx, cy + size.cy);
		}

		CSize operator -(SIZE size) const
		{
			return CSize(cx - size.cx, cy - size.cy);
		}

		CSize operator -() const
		{
			return CSize(-cx, -cy);
		}

		// Operators returning CPoint values
		CPoint operator +(POINT point) const;
		CPoint operator -(POINT point) const;

		// Operators returning CRect values
		CRect operator +(const RECT* lpRect) const;
		CRect operator -(const RECT* lpRect) const;
	};


	//---------------------------------------------------
	// CPoint - Wrapper for Windows POINT structure.
	class CPoint : public POINT
	{
	public:
		// Constructors
		CPoint()
		{
			x = 0;
			y = 0;
		}

		CPoint(int initX, int initY)
		{
			x = initX;
			y = initY;
		}

		CPoint(POINT initPt)
		{
			*(POINT*)this = initPt;
		}

		CPoint(SIZE initSize)
		{
			*(SIZE*)this = initSize;
		}

		CPoint(DWORD dwPoint)
		{
			x = (short)LOWORD(dwPoint);
			y = (short)HIWORD(dwPoint);
		}

		// Operations
		void Offset(int xOffset, int yOffset)
		{
			x += xOffset;
			y += yOffset;
		}

		void Offset(POINT point)
		{
			x += point.x;
			y += point.y;
		}

		void Offset(SIZE size)
		{
			x += size.cx;
			y += size.cy;
		}

		BOOL operator ==(POINT point) const
		{
			return (x == point.x && y == point.y);
		}

		BOOL operator !=(POINT point) const
		{
			return (x != point.x || y != point.y);
		}

		void operator +=(SIZE size)
		{
			x += size.cx;
			y += size.cy;
		}

		void operator -=(SIZE size)
		{
			x -= size.cx;
			y -= size.cy;
		}

		void operator +=(POINT point)
		{
			x += point.x;
			y += point.y;
		}

		void operator -=(POINT point)
		{
			x -= point.x;
			y -= point.y;
		}

		void SetPoint(int X, int Y)
		{
			x = X;
			y = Y;
		}

		// Operators returning CPoint values
		CPoint operator +(SIZE size) const
		{
			return CPoint(x + size.cx, y + size.cy);
		}

		CPoint operator -(SIZE size) const
		{
			return CPoint(x - size.cx, y - size.cy);
		}

		CPoint operator -() const
		{
			return CPoint(-x, -y);
		}

		CPoint operator +(POINT point) const
		{
			return CPoint(x + point.x, y + point.y);
		}

		// Operators returning CSize values
		CSize operator -(POINT point) const
		{
			return CSize(x - point.x, y - point.y);
		}

		// Operators returning CRect values
		CRect operator +(const RECT* lpRect) const;
		CRect operator -(const RECT* lpRect) const;
	};


	//---------------------------------------------------
	// CRect - Wrapper for Windows RECT structure.
	class CRect : public RECT
	{
	public:
		// Constructors
		CRect()
		{
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
		}

		CRect(int l, int t, int r, int b)
		{
			left = l;
			top = t;
			right = r;
			bottom = b;
		}

		CRect(const RECT& srcRect)
		{
			::CopyRect(this, &srcRect);
		}

		CRect(LPCRECT lpSrcRect)
		{
			::CopyRect(this, lpSrcRect);
		}

		CRect(POINT point, SIZE size)
		{
			right = (left = point.x) + size.cx;
			bottom = (top = point.y) + size.cy;
		}

		CRect(POINT topLeft, POINT bottomRight)
		{
			left = topLeft.x;
			top = topLeft.y;
			right = bottomRight.x;
			bottom = bottomRight.y;
		}

		// Attributes (in addition to RECT members)
		int Width() const
		{
			return right - left;
		}

		int Height() const
		{
			return bottom - top;
		}

		CSize Size() const
		{
			return CSize(right - left, bottom - top);
		}

		CPoint& TopLeft()
		{
			return *((CPoint*)this);
		}

		CPoint& BottomRight()
		{
			return *((CPoint*)this + 1);
		}

		const CPoint& TopLeft() const
		{
			return *((CPoint*)this);
		}

		const CPoint& BottomRight() const
		{
			return *((CPoint*)this + 1);
		}

		CPoint CenterPoint() const
		{
			return CPoint((left + right) / 2, (top + bottom) / 2);
		}

		// convert between CRect and LPRECT/LPCRECT (no need for &)
		operator LPRECT()
		{
			return this;
		}

		operator LPCRECT() const
		{
			return this;
		}

		BOOL IsRectEmpty() const
		{
			return ::IsRectEmpty(this);
		}

		BOOL IsRectNull() const
		{
			return (left == 0 && right == 0 && top == 0 && bottom == 0);
		}

		BOOL PtInRect(POINT point) const
		{
			return ::PtInRect(this, point);
		}

		// Operations
		void SetRect(int x1, int y1, int x2, int y2)
		{
			::SetRect(this, x1, y1, x2, y2);
		}

		void SetRect(POINT topLeft, POINT bottomRight)
		{
			::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
		}

		void SetRectEmpty()
		{
			::SetRectEmpty(this);
		}

		void CopyRect(LPCRECT lpSrcRect)
		{
			::CopyRect(this, lpSrcRect);
		}

		BOOL EqualRect(LPCRECT lpRect) const
		{
			return ::EqualRect(this, lpRect);
		}

		void InflateRect(int x, int y)
		{
			::InflateRect(this, x, y);
		}

		void InflateRect(SIZE size)
		{
			::InflateRect(this, size.cx, size.cy);
		}

		void InflateRect(LPCRECT lpRect)
		{
			left -= lpRect->left;
			top -= lpRect->top;
			right += lpRect->right;
			bottom += lpRect->bottom;
		}

		void InflateRect(int l, int t, int r, int b)
		{
			left -= l;
			top -= t;
			right += r;
			bottom += b;
		}

		void DeflateRect(int x, int y)
		{
			::InflateRect(this, -x, -y);
		}

		void DeflateRect(SIZE size)
		{
			::InflateRect(this, -size.cx, -size.cy);
		}

		void DeflateRect(LPCRECT lpRect)
		{
			left += lpRect->left;
			top += lpRect->top;
			right -= lpRect->right;
			bottom -= lpRect->bottom;
		}

		void DeflateRect(int l, int t, int r, int b)
		{
			left += l;
			top += t;
			right -= r;
			bottom -= b;
		}

		void OffsetRect(int x, int y)
		{
			::OffsetRect(this, x, y);
		}
		void OffsetRect(SIZE size)
		{
			::OffsetRect(this, size.cx, size.cy);
		}

		void OffsetRect(POINT point)
		{
			::OffsetRect(this, point.x, point.y);
		}

		void NormalizeRect()
		{
			int nTemp;
			if (left > right)
			{
				nTemp = left;
				left = right;
				right = nTemp;
			}
			if (top > bottom)
			{
				nTemp = top;
				top = bottom;
				bottom = nTemp;
			}
		}

		// absolute position of rectangle
		void MoveToY(int y)
		{
			bottom = Height() + y;
			top = y;
		}

		void MoveToX(int x)
		{
			right = Width() + x;
			left = x;
		}

		void MoveToXY(int x, int y)
		{
			MoveToX(x);
			MoveToY(y);
		}

		void MoveToXY(POINT pt)
		{
			MoveToX(pt.x);
			MoveToY(pt.y);
		}

		// operations that fill '*this' with result
		BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2)
		{
			return ::IntersectRect(this, lpRect1, lpRect2);
		}

		BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2)
		{
			return ::UnionRect(this, lpRect1, lpRect2);
		}

		BOOL SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2)
		{
			return ::SubtractRect(this, lpRectSrc1, lpRectSrc2);
		}

		// Additional Operations
		void operator =(const RECT& srcRect)
		{
			::CopyRect(this, &srcRect);
		}

		BOOL operator ==(const RECT& rect) const
		{
			return ::EqualRect(this, &rect);
		}

		BOOL operator !=(const RECT& rect) const
		{
			return !::EqualRect(this, &rect);
		}

		void operator +=(POINT point)
		{
			::OffsetRect(this, point.x, point.y);
		}

		void operator +=(SIZE size)
		{
			::OffsetRect(this, size.cx, size.cy);
		}

		void operator +=(LPCRECT lpRect)
		{
			InflateRect(lpRect);
		}

		void operator -=(POINT point)
		{
			::OffsetRect(this, -point.x, -point.y);
		}

		void operator -=(SIZE size)
		{
			::OffsetRect(this, -size.cx, -size.cy);
		}

		void operator -=(LPCRECT lpRect)
		{
			DeflateRect(lpRect);
		}

		void operator &=(const RECT& rect)
		{
			::IntersectRect(this, this, &rect);
		}

		void operator |=(const RECT& rect)
		{
			::UnionRect(this, this, &rect);
		}

		// Operators returning CRect values
		CRect operator +(POINT pt) const
		{
			CRect rect(*this);
			::OffsetRect(&rect, pt.x, pt.y);
			return rect;
		}

		CRect operator -(POINT pt) const
		{
			CRect rect(*this);
			::OffsetRect(&rect, -pt.x, -pt.y);
			return rect;
		}

		CRect operator +(LPCRECT lpRect) const
		{
			CRect rect(this);
			rect.InflateRect(lpRect);
			return rect;
		}

		CRect operator +(SIZE size) const
		{
			CRect rect(*this);
			::OffsetRect(&rect, size.cx, size.cy);
			return rect;
		}

		CRect operator -(SIZE size) const
		{
			CRect rect(*this);
			::OffsetRect(&rect, -size.cx, -size.cy);
			return rect;
		}

		CRect operator -(LPCRECT lpRect) const
		{
			CRect rect(this);
			rect.DeflateRect(lpRect);
			return rect;
		}

		CRect operator &(const RECT& rect2) const
		{
			CRect rect;
			::IntersectRect(&rect, this, &rect2);
			return rect;
		}

		CRect operator |(const RECT& rect2) const
		{
			CRect rect;
			::UnionRect(&rect, this, &rect2);
			return rect;
		}

		CRect MulDiv(int nMultiplier, int nDivisor) const
		{
			return CRect(
				::MulDiv(left, nMultiplier, nDivisor),
				::MulDiv(top, nMultiplier, nDivisor),
				::MulDiv(right, nMultiplier, nDivisor),
				::MulDiv(bottom, nMultiplier, nDivisor));
		}
	};

	//---------------------------------------------------
	// CSize implementation
	inline CPoint CSize::operator +(POINT point) const
	{ return CPoint(cx + point.x, cy + point.y); }

	inline CPoint CSize::operator -(POINT point) const
	{ return CPoint(cx - point.x, cy - point.y); }

	inline CRect CSize::operator +(const RECT* lpRect) const
	{ return CRect(lpRect) + *this; }

	inline CRect CSize::operator -(const RECT* lpRect) const
	{ return CRect(lpRect) - *this; }


	// CPoint implementation
	inline CRect CPoint::operator +(const RECT* lpRect) const
	{ return CRect(lpRect) + *this; }

	inline CRect CPoint::operator -(const RECT* lpRect) const
	{ return CRect(lpRect) - *this; }


	//---------------------------------------------------
	// DM::CSize or DM::CSize scalar operators 
	template <class Num>
	inline CSize operator *(SIZE s, Num n) 
	{
		return CSize((int)(s.cx * n), (int)(s.cy * n));
	};

	template <class Num>
	inline void operator *=(SIZE & s, Num n)
	{
		s = s * n;
	};	

	template <class Num>
	inline CSize operator /(SIZE s, Num n) 
	{
		return CSize((int)(s.cx / n), (int)(s.cy / n));
	};

	template <class Num>
	inline void operator /=(SIZE & s, Num n)
	{
		s = s / n;
	};	
} // namespace DM

//ATL实现CArray.CList.CMap----------------------------------------------
#include <limits.h>
#include <malloc.h>

// new.h中定义
#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
inline void *__CRTDECL operator new(size_t, void *_Where)
{return (_Where); }
#if     _MSC_VER >= 1200
inline void __CRTDECL operator delete(void *, void *)
{return; }
#endif
#endif

#ifndef _AFX_PACKING
#define _AFX_PACKING 4
#endif

#ifndef _DM_PACKING
#define _DM_PACKING 8
#endif

#define DMThrow(expr) __noop;
#define DMASSERT_VALID(x) __noop;

#if !defined(_DM_W64)
#if !defined(__midl) &&	(defined(_X86_)	|| defined(_M_IX86))
#define    _DM_W64 __w64
#else
#define    _DM_W64
#endif
#endif

namespace DM
{

#pragma pack(push,_DM_PACKING)
#pragma warning(push)
#pragma warning(disable: 4702)  // Unreachable code.  This file will have lots of it, especially without EH enabled.
#pragma warning(disable: 4512)  // assignment operator could not be generated
#pragma warning(disable: 4290)  // C++ Exception Specification ignored
#pragma warning(disable: 4127)  // conditional expression constant
#pragma warning(disable: 4571)  //catch(...) blocks compiled with /EHs do NOT catch or re-throw Structured Exceptions

	struct __POSITION
	{
	};
	typedef __POSITION* POSITION;

	template<typename T>
	class DMLimits;

	template<>
	class DMLimits<int _DM_W64>
	{
	public:
		static const int _Min=INT_MIN;
		static const int _Max=INT_MAX;
	};

	template<>
	class DMLimits<unsigned int _DM_W64>
	{
	public:
		static const unsigned int _Min=0;
		static const unsigned int _Max=UINT_MAX;
	};

	template<>
	class DMLimits<long _DM_W64>
	{
	public:
		static const long _Min=LONG_MIN;
		static const long _Max=LONG_MAX;
	};

	template<>
	class DMLimits<unsigned long _DM_W64>
	{
	public:
		static const unsigned long _Min=0;
		static const unsigned long _Max=ULONG_MAX;
	};

	template<>
	class DMLimits<long long>
	{
	public:
		static const long long _Min=LLONG_MIN;
		static const long long _Max=LLONG_MAX;
	};

	template<>
	class DMLimits<unsigned long long>
	{
	public:
		static const unsigned long long _Min=0;
		static const unsigned long long _Max=ULLONG_MAX;
	};

	//------------------------------------------------------
	// 以下代码来自atl7.atlalloc.h
	/* generic version */
	template<typename T>
	inline HRESULT Add(T* ptResult, T tLeft, T tRight)
	{
		if(DMLimits<T>::_Max-tLeft < tRight)
		{
			return E_INVALIDARG;
		}
		*ptResult= tLeft + tRight;
		return S_OK;
	}

	/* generic but compariatively slow version */
	template<typename T>
	inline HRESULT Multiply(T* ptResult,    T tLeft, T tRight)
	{
		/* avoid divide 0 */
		if(tLeft==0)
		{
			*ptResult=0;
			return S_OK;
		}
		if(DMLimits<T>::_Max/tLeft < tRight)
		{
			return E_INVALIDARG;
		}
		*ptResult= tLeft * tRight;
		return S_OK;
	}

	/* fast    version    for    32 bit integers    */
	template<>
	inline HRESULT Multiply(int _DM_W64    *piResult, int _DM_W64    iLeft, int _DM_W64 iRight)
	{
		__int64 i64Result=static_cast<__int64>(iLeft) * static_cast<__int64>(iRight);
		if(i64Result>INT_MAX || i64Result < INT_MIN)
		{
			return E_INVALIDARG;
		}
		*piResult=static_cast<int _DM_W64>(i64Result);
		return S_OK;
	}

	template<>
	inline HRESULT Multiply(unsigned int    _DM_W64 *piResult, unsigned int _DM_W64 iLeft, unsigned int _DM_W64 iRight)
	{
		unsigned __int64 i64Result=static_cast<unsigned __int64>(iLeft) * static_cast<unsigned __int64>(iRight);
		if(i64Result>UINT_MAX)
		{
			return E_INVALIDARG;
		}
		*piResult=static_cast<unsigned int _DM_W64>(i64Result);
		return S_OK;
	}

	template<>
	inline HRESULT Multiply(long    _DM_W64 *piResult, long _DM_W64 iLeft, long _DM_W64 iRight)
	{
		__int64 i64Result=static_cast<__int64>(iLeft) * static_cast<__int64>(iRight);
		if(i64Result>LONG_MAX || i64Result < LONG_MIN)
		{
			return E_INVALIDARG;
		}
		*piResult=static_cast<long _DM_W64>(i64Result);
		return S_OK;
	}

	template<>
	inline HRESULT Multiply(unsigned long _DM_W64 *piResult, unsigned long _DM_W64 iLeft, unsigned long _DM_W64 iRight)
	{
		unsigned __int64 i64Result=static_cast<unsigned __int64>(iLeft) * static_cast<unsigned __int64>(iRight);
		if(i64Result>ULONG_MAX)
		{
			return E_INVALIDARG;
		}
		*piResult=static_cast<unsigned long _DM_W64>(i64Result);
		return S_OK;
	}

	struct DMPlex     // warning variable length structure
	{
		DMPlex* pNext;
#if (_AFX_PACKING >= 8)
		DWORD dwReserved[1];    // align on 8 byte boundary
#endif
		// BYTE data[maxNum*elementSize];

		void* data() { return this+1; }

		static DMPlex* Create(DMPlex*& head, size_t nMax, size_t cbElement);
		// like 'calloc' but no zero fill
		// may throw memory exceptions

		void FreeDataChain();       // free this one and links
	};

	inline DMPlex* DMPlex::Create( DMPlex*& pHead, size_t nMax, size_t nElementSize )
	{
		DMPlex* pPlex;

		DMASSERT( nMax > 0 );
		DMASSERT( nElementSize > 0 );

		size_t nBytes=0;
		if(     FAILED(Multiply(&nBytes, nMax, nElementSize)) ||
			FAILED(Add(&nBytes, nBytes, sizeof(DMPlex))) )
		{
			return NULL;
		}
		pPlex = static_cast< DMPlex* >( DMMemDispatch::DM_malloc( nBytes ) );
		if( pPlex == NULL )
		{
			return( NULL );
		}

		pPlex->pNext = pHead;
		pHead = pPlex;

		return( pPlex );
	}

	inline void DMPlex::FreeDataChain()
	{
		DMPlex* pPlex;

		pPlex = this;
		while( pPlex != NULL )
		{
			DMPlex* pNext;

			pNext = pPlex->pNext;
			DMMemDispatch::DM_free( pPlex );
			pPlex = pNext;
		}
	}

	template< typename T >
	class CElementTraitsBase
	{
	public:
		typedef const T& INARGTYPE;
		typedef T& OUTARGTYPE;

		static void CopyElements( T* pDest, const T* pSrc, size_t nElements )
		{
			for( size_t iElement = 0; iElement < nElements; iElement++ )
			{
				pDest[iElement] = pSrc[iElement];
			}
		}

		static void RelocateElements( T* pDest, T* pSrc, size_t nElements )
		{
			// A simple memmove works for nearly all types.
			// You'll have to override this for types that have pointers to their
			// own members.
			memmove_s( pDest, nElements*sizeof( T ), pSrc, nElements*sizeof( T ));
		}
	};

	template< typename T >
	class CDefaultHashTraits
	{
	public:
		static ULONG Hash( const T& element )
		{
			return( ULONG( ULONG_PTR( element ) ) );
		}
	};

	template< typename T >
	class CDefaultCompareTraits
	{
	public:
		static bool CompareElements( const T& element1, const T& element2 )
		{
			return( (element1 == element2) != 0 );  // != 0 to handle overloads of operator== that return BOOL instead of bool
		}

		static int CompareElementsOrdered( const T& element1, const T& element2 )
		{
			if( element1 < element2 )
			{
				return( -1 );
			}
			else if( element1 == element2 )
			{
				return( 0 );
			}
			else
			{
				DMASSERT( element1 > element2 );
				return( 1 );
			}
		}
	};

	template< typename T >
	class CDefaultElementTraits :
		public CElementTraitsBase< T >,
		public CDefaultHashTraits< T >,
		public CDefaultCompareTraits< T >
	{
	};

	template< typename T >
	class CElementTraits :
		public CDefaultElementTraits< T >
	{
	};

	template< typename E, class ETraits = CElementTraits< E > >
	class  CArray
	{
	public:
		typedef typename ETraits::INARGTYPE INARGTYPE;
		typedef typename ETraits::OUTARGTYPE OUTARGTYPE;

	public:
		CArray();

		size_t GetCount() const;
		bool IsEmpty() const;
		bool SetCount( size_t nNewSize, int nGrowBy = -1 );

		void FreeExtra();
		void RemoveAll();

		const E& GetAt( size_t iElement ) const;
		void SetAt( size_t iElement, INARGTYPE element );
		E& GetAt( size_t iElement );

		const E* GetData() const;
		E* GetData();

		void SetAtGrow( size_t iElement, INARGTYPE element );
		// Add an empty element to the end of the array
		size_t Add();
		// Add an element to the end of the array
		size_t Add( INARGTYPE element );
		size_t Append( const CArray< E, ETraits >& aSrc );
		void Copy( const CArray< E, ETraits >& aSrc );

		const E& operator[]( size_t iElement ) const;
		E& operator[]( size_t iElement );

		void InsertAt( size_t iElement, INARGTYPE element, size_t nCount = 1 );
		void InsertArrayAt( size_t iStart, const CArray< E, ETraits >* paNew );
		void RemoveAt( size_t iElement, size_t nCount = 1 );

#ifdef _DEBUG
		void AssertValid() const;
#endif  // _DEBUG

	private:
		bool GrowBuffer( size_t nNewSize );

		// Implementation
	private:
		E* m_pData;
		size_t m_nSize;
		size_t m_nMaxSize;
		int m_nGrowBy;

	private:
		static void CallConstructors( E* pElements, size_t nElements );
		static void CallDestructors( E* pElements, size_t nElements );

	public:
		~CArray();

	};

	template< typename E, class ETraits >
	inline size_t CArray< E, ETraits >::GetCount() const
	{
		return( m_nSize );
	}

	template< typename E, class ETraits >
	inline bool CArray< E, ETraits >::IsEmpty() const
	{
		return( m_nSize == 0 );
	}

	template< typename E, class ETraits >
	inline void CArray< E, ETraits >::RemoveAll()
	{
		SetCount( 0, -1 );
	}

	template< typename E, class ETraits >
	inline const E& CArray< E, ETraits >::GetAt( size_t iElement ) const
	{
		DMASSERT( iElement < m_nSize );
		if(iElement >= m_nSize)
			DMThrow(E_INVALIDARG);

		return( m_pData[iElement] );
	}

	template< typename E, class ETraits >
	inline void CArray< E, ETraits >::SetAt( size_t iElement, INARGTYPE element )
	{
		DMASSERT( iElement < m_nSize );
		if(iElement >= m_nSize)
			DMThrow(E_INVALIDARG);

		m_pData[iElement] = element;
	}

	template< typename E, class ETraits >
	inline E& CArray< E, ETraits >::GetAt( size_t iElement )
	{
		DMASSERT( iElement < m_nSize );
		if(iElement >= m_nSize)
			DMThrow(E_INVALIDARG);

		return( m_pData[iElement] );
	}

	template< typename E, class ETraits >
	inline const E* CArray< E, ETraits >::GetData() const
	{
		return( m_pData );
	}

	template< typename E, class ETraits >
	inline E* CArray< E, ETraits >::GetData()
	{
		return( m_pData );
	}

	template< typename E, class ETraits >
	inline size_t CArray< E, ETraits >::Add()
	{
		size_t iElement;

		iElement = m_nSize;
		bool bSuccess=SetCount( m_nSize+1 );
		if( !bSuccess )
		{
			DMThrow( E_OUTOFMEMORY );
		}

		return( iElement );
	}

#pragma push_macro("new")
#undef new

	template< typename E, class ETraits >
	inline size_t CArray< E, ETraits >::Add( INARGTYPE element )
	{
		size_t iElement;

		iElement = m_nSize;
		if( iElement >= m_nMaxSize )
		{
			bool bSuccess = GrowBuffer( iElement+1 );
			if( !bSuccess )
			{
				DMThrow( E_OUTOFMEMORY );
			}
		}
		::new( m_pData+iElement ) E( element );
		m_nSize++;

		return( iElement );
	}

#pragma pop_macro("new")

	template< typename E, class ETraits >
	inline const E& CArray< E, ETraits >::operator[]( size_t iElement ) const
	{
		DMASSERT( iElement < m_nSize );
		if(iElement >= m_nSize)
			DMThrow(E_INVALIDARG);

		return( m_pData[iElement] );
	}

	template< typename E, class ETraits >
	inline E& CArray< E, ETraits >::operator[]( size_t iElement )
	{
		DMASSERT( iElement < m_nSize );
		if(iElement >= m_nSize)
			DMThrow(E_INVALIDARG);

		return( m_pData[iElement] );
	}

	template< typename E, class ETraits >
	CArray< E, ETraits >::CArray() :
	m_pData( NULL ),
		m_nSize( 0 ),
		m_nMaxSize( 0 ),
		m_nGrowBy( 0 )
	{
	}

	template< typename E, class ETraits >
	CArray< E, ETraits >::~CArray()
	{
		if( m_pData != NULL )
		{
			CallDestructors( m_pData, m_nSize );
			DMMemDispatch::DM_free( m_pData );
		}
	}

	template< typename E, class ETraits >
	bool CArray< E, ETraits >::GrowBuffer( size_t nNewSize )
	{
		if( nNewSize > m_nMaxSize )
		{
			if( m_pData == NULL )
			{
				size_t nAllocSize =  size_t( m_nGrowBy ) > nNewSize ? size_t( m_nGrowBy ) : nNewSize ;
				m_pData = static_cast< E* >( DMMemDispatch::DM_calloc( nAllocSize,sizeof( E ) ) );
				if( m_pData == NULL )
				{
					return( false );
				}
				m_nMaxSize = nAllocSize;
			}
			else
			{
				// otherwise, grow array
				size_t nGrowBy = m_nGrowBy;
				if( nGrowBy == 0 )
				{
					// heuristically determine growth when nGrowBy == 0
					//  (this avoids heap fragmentation in many situations)
					nGrowBy = m_nSize/8;
					nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
				}
				size_t nNewMax;
				if( nNewSize < (m_nMaxSize+nGrowBy) )
					nNewMax = m_nMaxSize+nGrowBy;  // granularity
				else
					nNewMax = nNewSize;  // no slush

				DMASSERT( nNewMax >= m_nMaxSize );  // no wrap around
#ifdef SIZE_T_MAX
				DMASSERT( nNewMax <= SIZE_T_MAX/sizeof( E ) ); // no overflow
#endif
				E* pNewData = static_cast< E* >( DMMemDispatch::DM_calloc( nNewMax,sizeof( E ) ) );
				if( pNewData == NULL )
				{
					return false;
				}

				// copy new data from old
				ETraits::RelocateElements( pNewData, m_pData, m_nSize );

				// get rid of old stuff (note: no destructors called)
				DMMemDispatch::DM_free( m_pData );
				m_pData = pNewData;
				m_nMaxSize = nNewMax;
			}
		}

		return true;
	}

	template< typename E, class ETraits >
	bool CArray< E, ETraits >::SetCount( size_t nNewSize, int nGrowBy )
	{
		DMASSERT_VALID(this);

		if( nGrowBy != -1 )
		{
			m_nGrowBy = nGrowBy;  // set new size
		}

		if( nNewSize == 0 )
		{
			// shrink to nothing
			if( m_pData != NULL )
			{
				CallDestructors( m_pData, m_nSize );
				DMMemDispatch::DM_free( m_pData );
				m_pData = NULL;
			}
			m_nSize = 0;
			m_nMaxSize = 0;
		}
		else if( nNewSize <= m_nMaxSize )
		{
			// it fits
			if( nNewSize > m_nSize )
			{
				// initialize the new elements
				CallConstructors( m_pData+m_nSize, nNewSize-m_nSize );
			}
			else if( m_nSize > nNewSize )
			{
				// destroy the old elements
				CallDestructors( m_pData+nNewSize, m_nSize-nNewSize );
			}
			m_nSize = nNewSize;
		}
		else
		{
			bool bSuccess;

			bSuccess = GrowBuffer( nNewSize );
			if( !bSuccess )
			{
				return( false );
			}

			// construct new elements
			DMASSERT( nNewSize > m_nSize );
			CallConstructors( m_pData+m_nSize, nNewSize-m_nSize );

			m_nSize = nNewSize;
		}

		return true;
	}

	template< typename E, class ETraits >
	size_t CArray< E, ETraits >::Append( const CArray< E, ETraits >& aSrc )
	{
		DMASSERT_VALID(this);
		DMASSERT( this != &aSrc );   // cannot append to itself

		size_t nOldSize = m_nSize;
		bool bSuccess=SetCount( m_nSize+aSrc.m_nSize );
		if( !bSuccess )
		{
			DMThrow( E_OUTOFMEMORY );
		}

		ETraits::CopyElements( m_pData+nOldSize, aSrc.m_pData, aSrc.m_nSize );

		return( nOldSize );
	}

	template< typename E, class ETraits >
	void CArray< E, ETraits >::Copy( const CArray< E, ETraits >& aSrc )
	{
		DMASSERT_VALID(this);
		DMASSERT( this != &aSrc );   // cannot append to itself

		bool bSuccess=SetCount( aSrc.m_nSize );
		if( !bSuccess )
		{
			DMThrow( E_OUTOFMEMORY );
		}

		ETraits::CopyElements( m_pData, aSrc.m_pData, aSrc.m_nSize );
	}

	template< typename E, class ETraits >
	void CArray< E, ETraits >::FreeExtra()
	{
		DMASSERT_VALID(this);

		if( m_nSize != m_nMaxSize )
		{
			// shrink to desired size
#ifdef SIZE_T_MAX
			DMASSERT( m_nSize <= (SIZE_T_MAX/sizeof( E )) ); // no overflow
#endif
			E* pNewData = NULL;
			if( m_nSize != 0 )
			{
				pNewData = (E*)DMMemDispatch::DM_calloc( m_nSize,sizeof( E ) );
				if( pNewData == NULL )
				{
					return;
				}

				// copy new data from old
				ETraits::RelocateElements( pNewData, m_pData, m_nSize );
			}

			// get rid of old stuff (note: no destructors called)
			DMMemDispatch::DM_free( m_pData );
			m_pData = pNewData;
			m_nMaxSize = m_nSize;
		}
	}

	template< typename E, class ETraits >
	void CArray< E, ETraits >::SetAtGrow( size_t iElement, INARGTYPE element )
	{
		DMASSERT_VALID(this);
		size_t nOldSize;

		nOldSize = m_nSize;
		if( iElement >= m_nSize )
		{
			bool bSuccess=SetCount( iElement+1, -1 );
			if( !bSuccess )
			{
				DMThrow( E_OUTOFMEMORY );
			}
		}

		{
			m_pData[iElement] = element;
		}
	}

	template< typename E, class ETraits >
	void CArray< E, ETraits >::InsertAt( size_t iElement, INARGTYPE element, size_t nElements /*=1*/)
	{
		DMASSERT_VALID(this);
		DMASSERT( nElements > 0 );     // zero size not allowed

		if( iElement >= m_nSize )
		{
			// adding after the end of the array
			bool bSuccess=SetCount( iElement+nElements, -1 );   // grow so nIndex is valid
			if( !bSuccess )
			{
				DMThrow( E_OUTOFMEMORY );
			}
		}
		else
		{
			// inserting in the middle of the array
			size_t nOldSize = m_nSize;
			bool bSuccess=SetCount( m_nSize+nElements, -1 );  // grow it to new size
			if( !bSuccess )
			{
				DMThrow( E_OUTOFMEMORY );
			}
			// destroy intial data before copying over it
			CallDestructors( m_pData+nOldSize, nElements );
			// shift old data up to fill gap
			ETraits::RelocateElements( m_pData+(iElement+nElements), m_pData+iElement,
				nOldSize-iElement );

			{
				// re-init slots we copied from
				CallConstructors( m_pData+iElement, nElements );
			}
		}

		// insert new value in the gap
		DMASSERT( (iElement+nElements) <= m_nSize );
		for( size_t iNewElement = iElement; iNewElement < (iElement+nElements); iNewElement++ )
		{
			m_pData[iNewElement] = element;
		}
	}

	template< typename E, class ETraits >
	void CArray< E, ETraits >::RemoveAt( size_t iElement, size_t nElements )
	{
		DMASSERT_VALID(this);
		DMASSERT( (iElement+nElements) <= m_nSize );

		size_t newCount = iElement+nElements;
		if ((newCount < iElement) || (newCount < nElements) || (newCount > m_nSize))
			DMThrow(E_INVALIDARG);

		// just remove a range
		size_t nMoveCount = m_nSize-(newCount);
		CallDestructors( m_pData+iElement, nElements );
		if( nMoveCount > 0 )
		{
			ETraits::RelocateElements( m_pData+iElement, m_pData+(newCount),
				nMoveCount );
		}
		m_nSize -= nElements;
	}

	template< typename E, class ETraits >
	void CArray< E, ETraits >::InsertArrayAt( size_t iStartElement,
		const CArray< E, ETraits >* paNew )
	{
		DMASSERT_VALID( this );
		DMASSERT( paNew != NULL );
		DMASSERT_VALID( paNew );

		if( paNew->GetCount() > 0 )
		{
			InsertAt( iStartElement, paNew->GetAt( 0 ), paNew->GetCount() );
			for( size_t iElement = 0; iElement < paNew->GetCount(); iElement++ )
			{
				SetAt( iStartElement+iElement, paNew->GetAt( iElement ) );
			}
		}
	}

#ifdef _DEBUG
	template< typename E, class ETraits >
	void CArray< E, ETraits >::AssertValid() const
	{
		if( m_pData == NULL )
		{
			DMASSERT( m_nSize == 0 );
			DMASSERT( m_nMaxSize == 0 );
		}
		else
		{
			DMASSERT( m_nSize <= m_nMaxSize );
		}
	}
#endif

#pragma push_macro("new")
#undef new

	template< typename E, class ETraits >
	void CArray< E, ETraits >::CallConstructors( E* pElements, size_t nElements )
	{
		size_t iElement = 0;

		{
			for( iElement = 0; iElement < nElements; iElement++ )
			{
				::new( pElements+iElement ) E;
			}
		}
	}

#pragma pop_macro("new")

	template< typename E, class ETraits >
	void CArray< E, ETraits >::CallDestructors( E* pElements, size_t nElements )
	{
		(void)pElements;

		for( size_t iElement = 0; iElement < nElements; iElement++ )
		{
			pElements[iElement].~E();
		}
	}


	template< typename E, class ETraits = CElementTraits< E > >
	class CList
	{
	public:
		typedef typename ETraits::INARGTYPE INARGTYPE;

	private:
		class CNode :
			public __POSITION
		{
		public:
			CNode()
			{
			}
			CNode( INARGTYPE element ) :
			m_element( element )
			{
			}
			~CNode()
			{
			}

		public:
			CNode* m_pNext;
			CNode* m_pPrev;
			E m_element;

		private:
			CNode( const CNode& );
		};

	public:
		CList( UINT nBlockSize = 10 );

		size_t GetCount() const;
		bool IsEmpty() const;

		E& GetHead();
		const E& GetHead() const;
		E& GetTail();
		const E& GetTail() const;

		E RemoveHead();
		E RemoveTail();
		void RemoveHeadNoReturn();
		void RemoveTailNoReturn();

		POSITION AddHead();
		POSITION AddHead( INARGTYPE element );
		void AddHeadList( const CList< E, ETraits >* plNew );
		POSITION AddTail();
		POSITION AddTail( INARGTYPE element );
		void AddTailList( const CList< E, ETraits >* plNew );

		void RemoveAll();

		POSITION GetHeadPosition() const;
		POSITION GetTailPosition() const;
		E& GetNext( POSITION& pos );
		const E& GetNext( POSITION& pos ) const;
		E& GetPrev( POSITION& pos );
		const E& GetPrev( POSITION& pos ) const;

		E& GetAt( POSITION pos );
		const E& GetAt( POSITION pos ) const;
		void SetAt( POSITION pos, INARGTYPE element );
		void RemoveAt( POSITION pos );

		POSITION InsertBefore( POSITION pos, INARGTYPE element );
		POSITION InsertAfter( POSITION pos, INARGTYPE element );

		POSITION Find( INARGTYPE element, POSITION posStartAfter = NULL ) const;
		POSITION FindIndex( size_t iElement ) const;

		void MoveToHead( POSITION pos );
		void MoveToTail( POSITION pos );
		void SwapElements( POSITION pos1, POSITION pos2 );

#ifdef _DEBUG
		void AssertValid() const;
#endif  // _DEBUG

		// Implementation
	private:
		CNode* m_pHead;
		CNode* m_pTail;
		size_t m_nElements;
		DMPlex* m_pBlocks;
		CNode* m_pFree;
		UINT m_nBlockSize;

	private:
		void GetFreeNode();
		CNode* NewNode( CNode* pPrev, CNode* pNext );
		CNode* NewNode( INARGTYPE element, CNode* pPrev, CNode* pNext );
		void FreeNode( CNode* pNode );

	public:
		~CList();

	private:
		// Private to prevent use
		CList( const CList& );
		CList& operator=( const CList& );
	};

	template< typename E, class ETraits >
	inline size_t CList< E, ETraits >::GetCount() const
	{
		return( m_nElements );
	}

	template< typename E, class ETraits >
	inline bool CList< E, ETraits >::IsEmpty() const
	{
		return( m_nElements == 0 );
	}

	template< typename E, class ETraits >
	inline E& CList< E, ETraits >::GetHead()
	{
		DMASSERT( m_pHead != NULL );
		return( m_pHead->m_element );
	}

	template< typename E, class ETraits >
	inline const E& CList< E, ETraits >::GetHead() const
	{
		DMASSERT( m_pHead != NULL );
		return( m_pHead->m_element );
	}

	template< typename E, class ETraits >
	inline E& CList< E, ETraits >::GetTail()
	{
		DMASSERT( m_pTail != NULL );
		return( m_pTail->m_element );
	}

	template< typename E, class ETraits >
	inline const E& CList< E, ETraits >::GetTail() const
	{
		DMASSERT( m_pTail != NULL );
		return( m_pTail->m_element );
	}

	template< typename E, class ETraits >
	inline POSITION CList< E, ETraits >::GetHeadPosition() const
	{
		return( POSITION( m_pHead ) );
	}

	template< typename E, class ETraits >
	inline POSITION CList< E, ETraits >::GetTailPosition() const
	{
		return( POSITION( m_pTail ) );
	}

	template< typename E, class ETraits >
	inline E& CList< E, ETraits >::GetNext( POSITION& pos )
	{
		CNode* pNode;

		DMASSERT( pos != NULL );
		pNode = (CNode*)pos;
		pos = POSITION( pNode->m_pNext );

		return( pNode->m_element );
	}

	template< typename E, class ETraits >
	inline const E& CList< E, ETraits >::GetNext( POSITION& pos ) const
	{
		CNode* pNode;

		DMASSERT( pos != NULL );
		pNode = (CNode*)pos;
		pos = POSITION( pNode->m_pNext );

		return( pNode->m_element );
	}

	template< typename E, class ETraits >
	inline E& CList< E, ETraits >::GetPrev( POSITION& pos )
	{
		CNode* pNode;

		DMASSERT( pos != NULL );
		pNode = (CNode*)pos;
		pos = POSITION( pNode->m_pPrev );

		return( pNode->m_element );
	}

	template< typename E, class ETraits >
	inline const E& CList< E, ETraits >::GetPrev( POSITION& pos ) const
	{
		CNode* pNode;

		DMASSERT( pos != NULL );
		pNode = (CNode*)pos;
		pos = POSITION( pNode->m_pPrev );

		return( pNode->m_element );
	}

	template< typename E, class ETraits >
	inline E& CList< E, ETraits >::GetAt( POSITION pos )
	{
		DMASSERT( pos != NULL );
		CNode* pNode = (CNode*)pos;
		return( pNode->m_element );
	}

	template< typename E, class ETraits >
	inline const E& CList< E, ETraits >::GetAt( POSITION pos ) const
	{
		DMASSERT( pos != NULL );
		CNode* pNode = (CNode*)pos;
		return( pNode->m_element );
	}

	template< typename E, class ETraits >
	inline void CList< E, ETraits >::SetAt( POSITION pos, INARGTYPE element )
	{
		DMASSERT( pos != NULL );
		CNode* pNode = (CNode*)pos;
		pNode->m_element = element;
	}

	template< typename E, class ETraits >
	CList< E, ETraits >::CList( UINT nBlockSize ) :
	m_nElements( 0 ),
		m_pHead( NULL ),
		m_pTail( NULL ),
		m_nBlockSize( nBlockSize ),
		m_pBlocks( NULL ),
		m_pFree( NULL )
	{
		DMASSERT( nBlockSize > 0 );
	}

	template< typename E, class ETraits >
	void CList< E, ETraits >::RemoveAll()
	{
		while( m_nElements > 0 )
		{
			CNode* pKill = m_pHead;
			DMASSERT( pKill != NULL );

			m_pHead = m_pHead->m_pNext;
			FreeNode( pKill );
		}

		DMASSERT( m_nElements == 0 );
		m_pHead = NULL;
		m_pTail = NULL;
		m_pFree = NULL;

		if( m_pBlocks != NULL )
		{
			m_pBlocks->FreeDataChain();
			m_pBlocks = NULL;
		}
	}

	template< typename E, class ETraits >
	CList< E, ETraits >::~CList()
	{
		RemoveAll();
		DMASSERT( m_nElements == 0 );
	}

#pragma push_macro("new")
#undef new

	template< typename E, class ETraits >
	void CList< E, ETraits >::GetFreeNode()
	{
		if( m_pFree == NULL )
		{
			DMPlex* pPlex;
			CNode* pNode;

			pPlex = DMPlex::Create( m_pBlocks, m_nBlockSize, sizeof( CNode ) );
			if( pPlex == NULL )
			{
				DMThrow( E_OUTOFMEMORY );
			}
			pNode = (CNode*)pPlex->data();
			pNode += m_nBlockSize-1;
			for( int iBlock = m_nBlockSize-1; iBlock >= 0; iBlock-- )
			{
				pNode->m_pNext = m_pFree;
				m_pFree = pNode;
				pNode--;
			}
		}
		DMASSERT( m_pFree != NULL );
	}

	template< typename E, class ETraits >
	typename CList< E, ETraits >::CNode* CList< E, ETraits >::NewNode( CNode* pPrev, CNode* pNext )
	{
		GetFreeNode();

		CNode* pNewNode = m_pFree;
		CNode* pNextFree = m_pFree->m_pNext;

		::new( pNewNode ) CNode;

		m_pFree = pNextFree;
		pNewNode->m_pPrev = pPrev;
		pNewNode->m_pNext = pNext;
		m_nElements++;
		DMASSERT( m_nElements > 0 );

		return( pNewNode );
	}

	template< typename E, class ETraits >
	typename CList< E, ETraits >::CNode* CList< E, ETraits >::NewNode( INARGTYPE element, CNode* pPrev,
		CNode* pNext )
	{
		GetFreeNode();

		CNode* pNewNode = m_pFree;
		CNode* pNextFree = m_pFree->m_pNext;

		::new( pNewNode ) CNode( element );

		m_pFree = pNextFree;
		pNewNode->m_pPrev = pPrev;
		pNewNode->m_pNext = pNext;
		m_nElements++;
		DMASSERT( m_nElements > 0 );

		return( pNewNode );
	}

#pragma pop_macro("new")

	template< typename E, class ETraits >
	void CList< E, ETraits >::FreeNode( CNode* pNode )
	{
		pNode->~CNode();
		pNode->m_pNext = m_pFree;
		m_pFree = pNode;
		DMASSERT( m_nElements > 0 );
		m_nElements--;
		if( m_nElements == 0 )
		{
			RemoveAll();
		}
	}

	template< typename E, class ETraits >
	POSITION CList< E, ETraits >::AddHead()
	{
		CNode* pNode = NewNode( NULL, m_pHead );
		if( m_pHead != NULL )
		{
			m_pHead->m_pPrev = pNode;
		}
		else
		{
			m_pTail = pNode;
		}
		m_pHead = pNode;

		return( POSITION( pNode ) );
	}

	template< typename E, class ETraits >
	POSITION CList< E, ETraits >::AddHead( INARGTYPE element )
	{
		CNode* pNode;

		pNode = NewNode( element, NULL, m_pHead );

		if( m_pHead != NULL )
		{
			m_pHead->m_pPrev = pNode;
		}
		else
		{
			m_pTail = pNode;
		}
		m_pHead = pNode;

		return( POSITION( pNode ) );
	}

	template< typename E, class ETraits >
	POSITION CList< E, ETraits >::AddTail()
	{
		CNode* pNode = NewNode( m_pTail, NULL );
		if( m_pTail != NULL )
		{
			m_pTail->m_pNext = pNode;
		}
		else
		{
			m_pHead = pNode;
		}
		m_pTail = pNode;

		return( POSITION( pNode ) );
	}

	template< typename E, class ETraits >
	POSITION CList< E, ETraits >::AddTail( INARGTYPE element )
	{
		CNode* pNode;

		pNode = NewNode( element, m_pTail, NULL );

		if( m_pTail != NULL )
		{
			m_pTail->m_pNext = pNode;
		}
		else
		{
			m_pHead = pNode;
		}
		m_pTail = pNode;

		return( POSITION( pNode ) );
	}

	template< typename E, class ETraits >
	void CList< E, ETraits >::AddHeadList( const CList< E, ETraits >* plNew )
	{
		DMASSERT( plNew != NULL );

		POSITION pos = plNew->GetTailPosition();
		while( pos != NULL )
		{
			INARGTYPE element = plNew->GetPrev( pos );
			AddHead( element );
		}
	}

	template< typename E, class ETraits >
	void CList< E, ETraits >::AddTailList( const CList< E, ETraits >* plNew )
	{
		DMASSERT( plNew != NULL );

		POSITION pos = plNew->GetHeadPosition();
		while( pos != NULL )
		{
			INARGTYPE element = plNew->GetNext( pos );
			AddTail( element );
		}
	}

	template< typename E, class ETraits >
	E CList< E, ETraits >::RemoveHead()
	{
		DMASSERT( m_pHead != NULL );

		CNode* pNode = m_pHead;
		E element( pNode->m_element );

		m_pHead = pNode->m_pNext;
		if( m_pHead != NULL )
		{
			m_pHead->m_pPrev = NULL;
		}
		else
		{
			m_pTail = NULL;
		}
		FreeNode( pNode );

		return( element );
	}

	template< typename E, class ETraits >
	void CList< E, ETraits >::RemoveHeadNoReturn()
	{
		DMASSERT( m_pHead != NULL );

		CNode* pNode = m_pHead;

		m_pHead = pNode->m_pNext;
		if( m_pHead != NULL )
		{
			m_pHead->m_pPrev = NULL;
		}
		else
		{
			m_pTail = NULL;
		}
		FreeNode( pNode );
	}

	template< typename E, class ETraits >
	E CList< E, ETraits >::RemoveTail()
	{
		DMASSERT( m_pTail != NULL );

		CNode* pNode = m_pTail;

		E element( pNode->m_element );

		m_pTail = pNode->m_pPrev;
		if( m_pTail != NULL )
		{
			m_pTail->m_pNext = NULL;
		}
		else
		{
			m_pHead = NULL;
		}
		FreeNode( pNode );

		return( element );
	}

	template< typename E, class ETraits >
	void CList< E, ETraits >::RemoveTailNoReturn()
	{
		DMASSERT( m_pTail != NULL );

		CNode* pNode = m_pTail;

		m_pTail = pNode->m_pPrev;
		if( m_pTail != NULL )
		{
			m_pTail->m_pNext = NULL;
		}
		else
		{
			m_pHead = NULL;
		}
		FreeNode( pNode );
	}

	template< typename E, class ETraits >
	POSITION CList< E, ETraits >::InsertBefore( POSITION pos, INARGTYPE element )
	{
		DMASSERT_VALID(this);

		if( pos == NULL )
			return AddHead( element ); // insert before nothing -> head of the list

		// Insert it before position
		CNode* pOldNode = (CNode*)pos;
		CNode* pNewNode = NewNode( element, pOldNode->m_pPrev, pOldNode );

		if( pOldNode->m_pPrev != NULL )
		{
			pOldNode->m_pPrev->m_pNext = pNewNode;
		}
		else
		{
			DMASSERT( pOldNode == m_pHead );
			m_pHead = pNewNode;
		}
		pOldNode->m_pPrev = pNewNode;

		return( POSITION( pNewNode ) );
	}

	template< typename E, class ETraits >
	POSITION CList< E, ETraits >::InsertAfter( POSITION pos, INARGTYPE element )
	{
		DMASSERT_VALID(this);

		if( pos == NULL )
			return AddTail( element ); // insert after nothing -> tail of the list

		// Insert it after position
		CNode* pOldNode = (CNode*)pos;
		CNode* pNewNode = NewNode( element, pOldNode, pOldNode->m_pNext );

		if( pOldNode->m_pNext != NULL )
		{
			pOldNode->m_pNext->m_pPrev = pNewNode;
		}
		else
		{
			DMASSERT( pOldNode == m_pTail );
			m_pTail = pNewNode;
		}
		pOldNode->m_pNext = pNewNode;

		return( POSITION( pNewNode ) );
	}

	template< typename E, class ETraits >
	void CList< E, ETraits >::RemoveAt( POSITION pos )
	{
		DMASSERT_VALID(this);
		DMASSERT( pos != NULL );

		CNode* pOldNode = (CNode*)pos;

		// remove pOldNode from list
		if( pOldNode == m_pHead )
		{
			m_pHead = pOldNode->m_pNext;
		}
		else
		{
			pOldNode->m_pPrev->m_pNext = pOldNode->m_pNext;
		}
		if( pOldNode == m_pTail )
		{
			m_pTail = pOldNode->m_pPrev;
		}
		else
		{
			pOldNode->m_pNext->m_pPrev = pOldNode->m_pPrev;
		}
		FreeNode( pOldNode );
	}

	template< typename E, class ETraits >
	POSITION CList< E, ETraits >::FindIndex( size_t iElement ) const
	{
		DMASSERT_VALID(this);

		if( iElement >= m_nElements )
			return NULL;  // went too far

		if(m_pHead == NULL)
			return NULL;

		CNode* pNode = m_pHead;
		for( size_t iSearch = 0; iSearch < iElement; iSearch++ )
		{
			pNode = pNode->m_pNext;
		}

		return( POSITION( pNode ) );
	}

	template< typename E, class ETraits >
	void CList< E, ETraits >::MoveToHead( POSITION pos )
	{
		DMASSERT( pos != NULL );

		CNode* pNode = static_cast< CNode* >( pos );

		if( pNode == m_pHead )
		{
			// Already at the head
			return;
		}

		if( pNode->m_pNext == NULL )
		{
			DMASSERT( pNode == m_pTail );
			m_pTail = pNode->m_pPrev;
		}
		else
		{
			pNode->m_pNext->m_pPrev = pNode->m_pPrev;
		}

		DMASSERT( pNode->m_pPrev != NULL );  // This node can't be the head, since we already checked that case
		pNode->m_pPrev->m_pNext = pNode->m_pNext;

		m_pHead->m_pPrev = pNode;
		pNode->m_pNext = m_pHead;
		pNode->m_pPrev = NULL;
		m_pHead = pNode;
	}

	template< typename E, class ETraits >
	void CList< E, ETraits >::MoveToTail( POSITION pos )
	{
		DMASSERT( pos != NULL );
		CNode* pNode = static_cast< CNode* >( pos );

		if( pNode == m_pTail )
		{
			// Already at the tail
			return;
		}

		if( pNode->m_pPrev == NULL )
		{
			DMASSERT( pNode == m_pHead );
			m_pHead = pNode->m_pNext;
		}
		else
		{
			pNode->m_pPrev->m_pNext = pNode->m_pNext;
		}

		pNode->m_pNext->m_pPrev = pNode->m_pPrev;

		m_pTail->m_pNext = pNode;
		pNode->m_pPrev = m_pTail;
		pNode->m_pNext = NULL;
		m_pTail = pNode;
	}

	template< typename E, class ETraits >
	void CList< E, ETraits >::SwapElements( POSITION pos1, POSITION pos2 )
	{
		DMASSERT( pos1 != NULL );
		DMASSERT( pos2 != NULL );

		if( pos1 == pos2 )
		{
			// Nothing to do
			return;
		}

		CNode* pNode1 = static_cast< CNode* >( pos1 );
		CNode* pNode2 = static_cast< CNode* >( pos2 );
		if( pNode2->m_pNext == pNode1 )
		{
			// Swap pNode2 and pNode1 so that the next case works
			CNode* pNodeTemp = pNode1;
			pNode1 = pNode2;
			pNode2 = pNodeTemp;
		}
		if( pNode1->m_pNext == pNode2 )
		{
			// Node1 and Node2 are adjacent
			pNode2->m_pPrev = pNode1->m_pPrev;
			if( pNode1->m_pPrev != NULL )
			{
				pNode1->m_pPrev->m_pNext = pNode2;
			}
			else
			{
				DMASSERT( m_pHead == pNode1 );
				m_pHead = pNode2;
			}
			pNode1->m_pNext = pNode2->m_pNext;
			if( pNode2->m_pNext != NULL )
			{
				pNode2->m_pNext->m_pPrev = pNode1;
			}
			else
			{
				DMASSERT( m_pTail == pNode2 );
				m_pTail = pNode1;
			}
			pNode2->m_pNext = pNode1;
			pNode1->m_pPrev = pNode2;
		}
		else
		{
			// The two nodes are not adjacent
			CNode* pNodeTemp;

			pNodeTemp = pNode1->m_pPrev;
			pNode1->m_pPrev = pNode2->m_pPrev;
			pNode2->m_pPrev = pNodeTemp;

			pNodeTemp = pNode1->m_pNext;
			pNode1->m_pNext = pNode2->m_pNext;
			pNode2->m_pNext = pNodeTemp;

			if( pNode1->m_pNext != NULL )
			{
				pNode1->m_pNext->m_pPrev = pNode1;
			}
			else
			{
				DMASSERT( m_pTail == pNode2 );
				m_pTail = pNode1;
			}
			if( pNode1->m_pPrev != NULL )
			{
				pNode1->m_pPrev->m_pNext = pNode1;
			}
			else
			{
				DMASSERT( m_pHead == pNode2 );
				m_pHead = pNode1;
			}
			if( pNode2->m_pNext != NULL )
			{
				pNode2->m_pNext->m_pPrev = pNode2;
			}
			else
			{
				DMASSERT( m_pTail == pNode1 );
				m_pTail = pNode2;
			}
			if( pNode2->m_pPrev != NULL )
			{
				pNode2->m_pPrev->m_pNext = pNode2;
			}
			else
			{
				DMASSERT( m_pHead == pNode1 );
				m_pHead = pNode2;
			}
		}
	}

	template< typename E, class ETraits >
	POSITION CList< E, ETraits >::Find( INARGTYPE element, POSITION posStartAfter ) const
	{
		DMASSERT_VALID(this);

		CNode* pNode = (CNode*)posStartAfter;
		if( pNode == NULL )
		{
			pNode = m_pHead;  // start at head
		}
		else
		{
			pNode = pNode->m_pNext;  // start after the one specified
		}

		for( ; pNode != NULL; pNode = pNode->m_pNext )
		{
			if( ETraits::CompareElements( pNode->m_element, element ) )
				return( POSITION( pNode ) );
		}

		return( NULL );
	}

#ifdef _DEBUG
	template< typename E, class ETraits >
	void CList< E, ETraits >::AssertValid() const
	{
		if( IsEmpty() )
		{
			// empty list
			DMASSERT(m_pHead == NULL);
			DMASSERT(m_pTail == NULL);
		}
		else
		{
			// non-empty list
		}
	}
#endif

	template< typename K, typename V, class KTraits = CElementTraits< K >, class VTraits = CElementTraits< V > >
	class  CMap
	{
	public:
		typedef typename KTraits::INARGTYPE KINARGTYPE;
		typedef typename KTraits::OUTARGTYPE KOUTARGTYPE;
		typedef typename VTraits::INARGTYPE VINARGTYPE;
		typedef typename VTraits::OUTARGTYPE VOUTARGTYPE;

		class CPair :
			public __POSITION
		{
		protected:
			CPair( KINARGTYPE key ) :
				 m_key( key )
				 {
				 }

		public:
			const K m_key;
			V m_value;
		};

	private:
		class CNode :
			public CPair
		{
		public:
			CNode( KINARGTYPE key, UINT nHash ) :
			  CPair( key ),
				  m_nHash( nHash )
			  {
			  }

		public:
			UINT GetHash() const
			{
				return( m_nHash );
			}

		public:
			CNode* m_pNext;
			UINT m_nHash;
		};

	public:
		CMap( UINT nBins = 17, float fOptimalLoad = 0.75f,
			float fLoThreshold = 0.25f, float fHiThreshold = 2.25f, UINT nBlockSize = 10 );

		size_t GetCount() const;
		bool IsEmpty() const;

		bool Lookup( KINARGTYPE key, VOUTARGTYPE value ) const;
		const CPair* Lookup( KINARGTYPE key ) const;
		CPair* Lookup( KINARGTYPE key );
		V& operator[]( KINARGTYPE key ) throw(...);

		POSITION SetAt( KINARGTYPE key, VINARGTYPE value );
		void SetValueAt( POSITION pos, VINARGTYPE value );

		bool RemoveKey( KINARGTYPE key );
		void RemoveAll();
		void RemoveAtPos( POSITION pos );

		POSITION GetStartPosition() const;
		void GetNextAssoc( POSITION& pos, KOUTARGTYPE key, VOUTARGTYPE value ) const;
		const CPair* GetNext( POSITION& pos ) const;
		CPair* GetNext( POSITION& pos );
		const K& GetNextKey( POSITION& pos ) const;
		const V& GetNextValue( POSITION& pos ) const;
		V& GetNextValue( POSITION& pos );
		void GetAt( POSITION pos, KOUTARGTYPE key, VOUTARGTYPE value ) const;
		CPair* GetAt( POSITION pos );
		const CPair* GetAt( POSITION pos ) const;
		const K& GetKeyAt( POSITION pos ) const;
		const V& GetValueAt( POSITION pos ) const;
		V& GetValueAt( POSITION pos );

		UINT GetHashTableSize() const;
		bool InitHashTable( UINT nBins, bool bAllocNow = true );
		void EnableAutoRehash();
		void DisableAutoRehash();
		void Rehash( UINT nBins = 0 );
		void SetOptimalLoad( float fOptimalLoad, float fLoThreshold, float fHiThreshold,
			bool bRehashNow = false );

#ifdef _DEBUG
		void AssertValid() const;
#endif  // _DEBUG

		// Implementation
	private:
		CNode** m_ppBins;
		size_t m_nElements;
		UINT m_nBins;
		float m_fOptimalLoad;
		float m_fLoThreshold;
		float m_fHiThreshold;
		size_t m_nHiRehashThreshold;
		size_t m_nLoRehashThreshold;
		ULONG m_nLockCount;
		UINT m_nBlockSize;
		DMPlex* m_pBlocks;
		CNode* m_pFree;

	private:
		bool IsLocked() const;
		UINT PickSize( size_t nElements ) const;
		CNode* NewNode( KINARGTYPE key, UINT iBin, UINT nHash );
		void FreeNode( CNode* pNode );
		void FreePlexes();
		CNode* GetNode( KINARGTYPE key, UINT& iBin, UINT& nHash, CNode*& pPrev ) const;
		CNode* CreateNode( KINARGTYPE key, UINT iBin, UINT nHash ) throw(...);
		void RemoveNode( CNode* pNode, CNode* pPrev );
		CNode* FindNextNode( CNode* pNode ) const;
		void UpdateRehashThresholds();

	public:
		~CMap();

	private:
		// Private to prevent use
		CMap( const CMap& );
		CMap& operator=( const CMap& );
	};


	template< typename K, typename V, class KTraits, class VTraits >
	inline size_t CMap< K, V, KTraits, VTraits >::GetCount() const
	{
		return( m_nElements );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline bool CMap< K, V, KTraits, VTraits >::IsEmpty() const
	{
		return( m_nElements == 0 );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline V& CMap< K, V, KTraits, VTraits >::operator[]( KINARGTYPE key ) throw(...)
	{
		CNode* pNode;
		UINT iBin;
		UINT nHash;
		CNode* pPrev;

		pNode = GetNode( key, iBin, nHash, pPrev );
		if( pNode == NULL )
		{
			pNode = CreateNode( key, iBin, nHash );
		}

		return( pNode->m_value );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline UINT CMap< K, V, KTraits, VTraits >::GetHashTableSize() const
	{
		return( m_nBins );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline void CMap< K, V, KTraits, VTraits >::GetAt( POSITION pos, KOUTARGTYPE key, VOUTARGTYPE value ) const
	{
		DMASSERT( pos != NULL );

		CNode* pNode = static_cast< CNode* >( pos );

		key = pNode->m_key;
		value = pNode->m_value;
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline typename CMap< K, V, KTraits, VTraits >::CPair* CMap< K, V, KTraits, VTraits >::GetAt( POSITION pos )
	{
		DMASSERT( pos != NULL );

		return( static_cast< CPair* >( pos ) );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline const typename CMap< K, V, KTraits, VTraits >::CPair* CMap< K, V, KTraits, VTraits >::GetAt( POSITION pos ) const
	{
		DMASSERT( pos != NULL );

		return( static_cast< const CPair* >( pos ) );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline const K& CMap< K, V, KTraits, VTraits >::GetKeyAt( POSITION pos ) const
	{
		DMASSERT( pos != NULL );

		CNode* pNode = (CNode*)pos;

		return( pNode->m_key );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline const V& CMap< K, V, KTraits, VTraits >::GetValueAt( POSITION pos ) const
	{
		DMASSERT( pos != NULL );

		CNode* pNode = (CNode*)pos;

		return( pNode->m_value );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline V& CMap< K, V, KTraits, VTraits >::GetValueAt( POSITION pos )
	{
		DMASSERT( pos != NULL );

		CNode* pNode = (CNode*)pos;

		return( pNode->m_value );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline void CMap< K, V, KTraits, VTraits >::DisableAutoRehash()
	{
		m_nLockCount++;
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline void CMap< K, V, KTraits, VTraits >::EnableAutoRehash()
	{
		DMASSERT( m_nLockCount > 0 );
		m_nLockCount--;
	}

	template< typename K, typename V, class KTraits, class VTraits >
	inline bool CMap< K, V, KTraits, VTraits >::IsLocked() const
	{
		return( m_nLockCount != 0 );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	UINT CMap< K, V, KTraits, VTraits >::PickSize( size_t nElements ) const
	{
		// List of primes such that s_anPrimes[i] is the smallest prime greater than 2^(5+i/3)
		static const UINT s_anPrimes[] =
		{
			17, 23, 29, 37, 41, 53, 67, 83, 103, 131, 163, 211, 257, 331, 409, 521, 647, 821,
			1031, 1291, 1627, 2053, 2591, 3251, 4099, 5167, 6521, 8209, 10331,
			13007, 16411, 20663, 26017, 32771, 41299, 52021, 65537, 82571, 104033,
			131101, 165161, 208067, 262147, 330287, 416147, 524309, 660563,
			832291, 1048583, 1321139, 1664543, 2097169, 2642257, 3329023, 4194319,
			5284493, 6658049, 8388617, 10568993, 13316089, UINT_MAX
		};

		size_t nBins = (size_t)(nElements/m_fOptimalLoad);
		UINT nBinsEstimate = UINT(  UINT_MAX < nBins ? UINT_MAX : nBins );

		// Find the smallest prime greater than our estimate
		int iPrime = 0;
		while( nBinsEstimate > s_anPrimes[iPrime] )
		{
			iPrime++;
		}

		if( s_anPrimes[iPrime] == UINT_MAX )
		{
			return( nBinsEstimate );
		}
		else
		{
			return( s_anPrimes[iPrime] );
		}
	}

	template< typename K, typename V, class KTraits, class VTraits >
	typename CMap< K, V, KTraits, VTraits >::CNode* CMap< K, V, KTraits, VTraits >::CreateNode(
		KINARGTYPE key, UINT iBin, UINT nHash ) throw(...)
	{
		CNode* pNode;

		if( m_ppBins == NULL )
		{
			bool bSuccess;

			bSuccess = InitHashTable( m_nBins );
			if( !bSuccess )
			{
				DMThrow( E_OUTOFMEMORY );
			}
		}

		pNode = NewNode( key, iBin, nHash );

		return( pNode );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	POSITION CMap< K, V, KTraits, VTraits >::GetStartPosition() const
	{
		if( IsEmpty() )
		{
			return( NULL );
		}

		for( UINT iBin = 0; iBin < m_nBins; iBin++ )
		{
			if( m_ppBins[iBin] != NULL )
			{
				return( POSITION( m_ppBins[iBin] ) );
			}
		}
		DMASSERT( false );

		return( NULL );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	POSITION CMap< K, V, KTraits, VTraits >::SetAt( KINARGTYPE key, VINARGTYPE value )
	{
		CNode* pNode;
		UINT iBin;
		UINT nHash;
		CNode* pPrev;

		pNode = GetNode( key, iBin, nHash, pPrev );
		if( pNode == NULL )
		{
			pNode = CreateNode( key, iBin, nHash );
			//         _STRY
			{
				pNode->m_value = value;
			}
			//         _SCATCHALL()
			//         {
			//             RemoveAtPos( POSITION( pNode ) );
			//         }
		}
		else
		{
			pNode->m_value = value;
		}

		return( POSITION( pNode ) );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::SetValueAt( POSITION pos, VINARGTYPE value )
	{
		DMASSERT( pos != NULL );

		CNode* pNode = static_cast< CNode* >( pos );

		pNode->m_value = value;
	}

	template< typename K, typename V, class KTraits, class VTraits >
	CMap< K, V, KTraits, VTraits >::CMap( UINT nBins, float fOptimalLoad,
		float fLoThreshold, float fHiThreshold, UINT nBlockSize ) :
	m_ppBins( NULL ),
		m_nBins( nBins ),
		m_nElements( 0 ),
		m_nLockCount( 0 ),  // Start unlocked
		m_fOptimalLoad( fOptimalLoad ),
		m_fLoThreshold( fLoThreshold ),
		m_fHiThreshold( fHiThreshold ),
		m_nHiRehashThreshold( UINT_MAX ),
		m_nLoRehashThreshold( 0 ),
		m_pBlocks( NULL ),
		m_pFree( NULL ),
		m_nBlockSize( nBlockSize )
	{
		DMASSERT( nBins > 0 );
		DMASSERT( nBlockSize > 0 );

		SetOptimalLoad( fOptimalLoad, fLoThreshold, fHiThreshold, false );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::SetOptimalLoad( float fOptimalLoad, float fLoThreshold,
		float fHiThreshold, bool bRehashNow )
	{
		DMASSERT( fOptimalLoad > 0 );
		DMASSERT( (fLoThreshold >= 0) && (fLoThreshold < fOptimalLoad) );
		DMASSERT( fHiThreshold > fOptimalLoad );

		m_fOptimalLoad = fOptimalLoad;
		m_fLoThreshold = fLoThreshold;
		m_fHiThreshold = fHiThreshold;

		UpdateRehashThresholds();

		if( bRehashNow && ((m_nElements > m_nHiRehashThreshold) ||
			(m_nElements < m_nLoRehashThreshold)) )
		{
			Rehash( PickSize( m_nElements ) );
		}
	}

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::UpdateRehashThresholds()
	{
		m_nHiRehashThreshold = size_t( m_fHiThreshold*m_nBins );
		m_nLoRehashThreshold = size_t( m_fLoThreshold*m_nBins );
		if( m_nLoRehashThreshold < 17 )
		{
			m_nLoRehashThreshold = 0;
		}
	}

	template< typename K, typename V, class KTraits, class VTraits >
	bool CMap< K, V, KTraits, VTraits >::InitHashTable( UINT nBins, bool bAllocNow )
	{
		DMASSERT( m_nElements == 0 );
		DMASSERT( nBins > 0 );

		if( m_ppBins != NULL )
		{
			delete[] m_ppBins;
			m_ppBins = NULL;
		}

		if( bAllocNow )
		{
			m_ppBins = new CNode*[nBins];
			if( m_ppBins == NULL )
			{
				return false;
			}

			DMASSERT( UINT_MAX / sizeof( CNode* ) >= nBins );
			memset( m_ppBins, 0, sizeof( CNode* )*nBins );
		}
		m_nBins = nBins;

		UpdateRehashThresholds();

		return true;
	}

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::RemoveAll()
	{
		DisableAutoRehash();
		if( m_ppBins != NULL )
		{
			for( UINT iBin = 0; iBin < m_nBins; iBin++ )
			{
				CNode* pNext;

				pNext = m_ppBins[iBin];
				while( pNext != NULL )
				{
					CNode* pKill;

					pKill = pNext;
					pNext = pNext->m_pNext;
					FreeNode( pKill );
				}
			}
		}

		delete[] m_ppBins;
		m_ppBins = NULL;
		m_nElements = 0;

		if( !IsLocked() )
		{
			InitHashTable( PickSize( m_nElements ), false );
		}

		FreePlexes();
		EnableAutoRehash();
	}

	template< typename K, typename V, class KTraits, class VTraits >
	CMap< K, V, KTraits, VTraits >::~CMap()
	{
		RemoveAll();
	}

#pragma push_macro("new")
#undef new

	template< typename K, typename V, class KTraits, class VTraits >
	typename CMap< K, V, KTraits, VTraits >::CNode* CMap< K, V, KTraits, VTraits >::NewNode(
		KINARGTYPE key, UINT iBin, UINT nHash )
	{
		CNode* pNewNode;

		if( m_pFree == NULL )
		{
			DMPlex* pPlex;
			CNode* pNode;

			pPlex = DMPlex::Create( m_pBlocks, m_nBlockSize, sizeof( CNode ) );
			if( pPlex == NULL )
			{
				DMThrow( E_OUTOFMEMORY );
			}
			pNode = (CNode*)pPlex->data();
			pNode += m_nBlockSize-1;
			for( int iBlock = m_nBlockSize-1; iBlock >= 0; iBlock-- )
			{
				pNode->m_pNext = m_pFree;
				m_pFree = pNode;
				pNode--;
			}
		}
		DMASSERT(m_pFree != NULL );
		pNewNode = m_pFree;
		m_pFree = pNewNode->m_pNext;

		{
			::new( pNewNode ) CNode( key, nHash );
		}

		m_nElements++;

		pNewNode->m_pNext = m_ppBins[iBin];
		m_ppBins[iBin] = pNewNode;

		if( (m_nElements > m_nHiRehashThreshold) && !IsLocked() )
		{
			Rehash( PickSize( m_nElements ) );
		}

		return( pNewNode );
	}

#pragma pop_macro("new")

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::FreeNode( CNode* pNode )
	{
		DMASSERT( pNode != NULL );

		pNode->~CNode();
		pNode->m_pNext = m_pFree;
		m_pFree = pNode;

		DMASSERT( m_nElements > 0 );
		m_nElements--;

		if( (m_nElements < m_nLoRehashThreshold) && !IsLocked() )
		{
			Rehash( PickSize( m_nElements ) );
		}

		if( m_nElements == 0 )
		{
			FreePlexes();
		}
	}

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::FreePlexes()
	{
		m_pFree = NULL;
		if( m_pBlocks != NULL )
		{
			m_pBlocks->FreeDataChain();
			m_pBlocks = NULL;
		}
	}

	template< typename K, typename V, class KTraits, class VTraits >
	typename CMap< K, V, KTraits, VTraits >::CNode* CMap< K, V, KTraits, VTraits >::GetNode(
		KINARGTYPE key, UINT& iBin, UINT& nHash, CNode*& pPrev ) const
	{
		CNode* pFollow;

		nHash = KTraits::Hash( key );
		iBin = nHash%m_nBins;

		if( m_ppBins == NULL )
		{
			return( NULL );
		}

		pFollow = NULL;
		pPrev = NULL;
		for( CNode* pNode = m_ppBins[iBin]; pNode != NULL; pNode = pNode->m_pNext )
		{
			if( (pNode->GetHash() == nHash) && KTraits::CompareElements( pNode->m_key, key ) )
			{
				pPrev = pFollow;
				return( pNode );
			}
			pFollow = pNode;
		}

		return( NULL );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	bool CMap< K, V, KTraits, VTraits >::Lookup( KINARGTYPE key, VOUTARGTYPE value ) const
	{
		UINT iBin;
		UINT nHash;
		CNode* pNode;
		CNode* pPrev;

		pNode = GetNode( key, iBin, nHash, pPrev );
		if( pNode == NULL )
		{
			return( false );
		}

		value = pNode->m_value;

		return( true );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	const typename CMap< K, V, KTraits, VTraits >::CPair* CMap< K, V, KTraits, VTraits >::Lookup( KINARGTYPE key ) const
	{
		UINT iBin;
		UINT nHash;
		CNode* pNode;
		CNode* pPrev;

		pNode = GetNode( key, iBin, nHash, pPrev );

		return( pNode );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	typename CMap< K, V, KTraits, VTraits >::CPair* CMap< K, V, KTraits, VTraits >::Lookup( KINARGTYPE key )
	{
		UINT iBin;
		UINT nHash;
		CNode* pNode;
		CNode* pPrev;

		pNode = GetNode( key, iBin, nHash, pPrev );

		return( pNode );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	bool CMap< K, V, KTraits, VTraits >::RemoveKey( KINARGTYPE key )
	{
		CNode* pNode;
		UINT iBin;
		UINT nHash;
		CNode* pPrev;

		pPrev = NULL;
		pNode = GetNode( key, iBin, nHash, pPrev );
		if( pNode == NULL )
		{
			return( false );
		}

		RemoveNode( pNode, pPrev );

		return( true );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::RemoveNode( CNode* pNode, CNode* pPrev )
	{
		DMASSERT( pNode != NULL );

		UINT iBin = pNode->GetHash() % m_nBins;

		if( pPrev == NULL )
		{
			DMASSERT( m_ppBins[iBin] == pNode );
			m_ppBins[iBin] = pNode->m_pNext;
		}
		else
		{
			DMASSERT( pPrev->m_pNext == pNode );
			pPrev->m_pNext = pNode->m_pNext;
		}
		FreeNode( pNode );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::RemoveAtPos( POSITION pos )
	{
		DMASSERT( pos != NULL );

		CNode* pNode = static_cast< CNode* >( pos );
		CNode* pPrev = NULL;
		UINT iBin = pNode->GetHash() % m_nBins;

		DMASSERT( m_ppBins[iBin] != NULL );
		if( pNode == m_ppBins[iBin] )
		{
			pPrev = NULL;
		}
		else
		{
			pPrev = m_ppBins[iBin];
			while( pPrev->m_pNext != pNode )
			{
				pPrev = pPrev->m_pNext;
				DMASSERT( pPrev != NULL );
			}
		}
		RemoveNode( pNode, pPrev );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::Rehash( UINT nBins )
	{
		CNode** ppBins = NULL;

		if( nBins == 0 )
		{
			nBins = PickSize( m_nElements );
		}

		if( nBins == m_nBins )
		{
			return;
		}

		if( m_ppBins == NULL )
		{
			// Just set the new number of bins
			InitHashTable( nBins, false );
			return;
		}

		ppBins = new CNode*[nBins];
		if (ppBins == NULL)
		{
			DMThrow( E_OUTOFMEMORY );
		}

		DMASSERT( UINT_MAX / sizeof( CNode* ) >= nBins );
		memset( ppBins, 0, nBins*sizeof( CNode* ) );

		// Nothing gets copied.  We just rewire the old nodes
		// into the new bins.
		for( UINT iSrcBin = 0; iSrcBin < m_nBins; iSrcBin++ )
		{
			CNode* pNode;

			pNode = m_ppBins[iSrcBin];
			while( pNode != NULL )
			{
				CNode* pNext;
				UINT iDestBin;

				pNext = pNode->m_pNext;  // Save so we don't trash it
				iDestBin = pNode->GetHash()%nBins;
				pNode->m_pNext = ppBins[iDestBin];
				ppBins[iDestBin] = pNode;

				pNode = pNext;
			}
		}

		delete[] m_ppBins;
		m_ppBins = ppBins;
		m_nBins = nBins;

		UpdateRehashThresholds();
	}

	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::GetNextAssoc( POSITION& pos, KOUTARGTYPE key,
		VOUTARGTYPE value ) const
	{
		CNode* pNode;
		CNode* pNext;

		DMASSERT( m_ppBins != NULL );
		DMASSERT( pos != NULL );

		pNode = (CNode*)pos;
		pNext = FindNextNode( pNode );

		pos = POSITION( pNext );
		key = pNode->m_key;
		value = pNode->m_value;
	}

	template< typename K, typename V, class KTraits, class VTraits >
	const typename CMap< K, V, KTraits, VTraits >::CPair* CMap< K, V, KTraits, VTraits >::GetNext( POSITION& pos ) const
	{
		CNode* pNode;
		CNode* pNext;

		DMASSERT( m_ppBins != NULL );
		DMASSERT( pos != NULL );

		pNode = (CNode*)pos;
		pNext = FindNextNode( pNode );

		pos = POSITION( pNext );

		return( pNode );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	typename CMap< K, V, KTraits, VTraits >::CPair* CMap< K, V, KTraits, VTraits >::GetNext(
		POSITION& pos )
	{
		DMASSERT( m_ppBins != NULL );
		DMASSERT( pos != NULL );

		CNode* pNode = static_cast< CNode* >( pos );
		CNode* pNext = FindNextNode( pNode );

		pos = POSITION( pNext );

		return( pNode );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	const K& CMap< K, V, KTraits, VTraits >::GetNextKey( POSITION& pos ) const
	{
		CNode* pNode;
		CNode* pNext;

		DMASSERT( m_ppBins != NULL );
		DMASSERT( pos != NULL );

		pNode = (CNode*)pos;
		pNext = FindNextNode( pNode );

		pos = POSITION( pNext );

		return( pNode->m_key );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	const V& CMap< K, V, KTraits, VTraits >::GetNextValue( POSITION& pos ) const
	{
		CNode* pNode;
		CNode* pNext;

		DMASSERT( m_ppBins != NULL );
		DMASSERT( pos != NULL );

		pNode = (CNode*)pos;
		pNext = FindNextNode( pNode );

		pos = POSITION( pNext );

		return( pNode->m_value );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	V& CMap< K, V, KTraits, VTraits >::GetNextValue( POSITION& pos )
	{
		CNode* pNode;
		CNode* pNext;

		DMASSERT( m_ppBins != NULL );
		DMASSERT( pos != NULL );

		pNode = (CNode*)pos;
		pNext = FindNextNode( pNode );

		pos = POSITION( pNext );

		return( pNode->m_value );
	}

	template< typename K, typename V, class KTraits, class VTraits >
	typename CMap< K, V, KTraits, VTraits >::CNode* CMap< K, V, KTraits, VTraits >::FindNextNode( CNode* pNode ) const
	{
		CNode* pNext;

		if(pNode == NULL)
		{
			DMASSERT(FALSE);
			return NULL;
		}

		if( pNode->m_pNext != NULL )
		{
			pNext = pNode->m_pNext;
		}
		else
		{
			UINT iBin;

			pNext = NULL;
			iBin = (pNode->GetHash()%m_nBins)+1;
			while( (pNext == NULL) && (iBin < m_nBins) )
			{
				if( m_ppBins[iBin] != NULL )
				{
					pNext = m_ppBins[iBin];
				}

				iBin++;
			}
		}

		return( pNext );
	}

#ifdef _DEBUG
	template< typename K, typename V, class KTraits, class VTraits >
	void CMap< K, V, KTraits, VTraits >::AssertValid() const
	{
		DMASSERT( m_nBins > 0 );
		// non-empty map should have hash table
		DMASSERT( IsEmpty() || (m_ppBins != NULL) );
	}
#endif

	// ----------------------------
	template<class T,class TC>
	size_t SplitStringT(const T&str, TC cSep ,DM::CArray<T>&strLst)
	{
		int iStart = 0;
		int iEnd   = 0;
		int iLen   = str.GetLength();
		while (iEnd != iLen)
		{
			if (cSep == str[iEnd])
			{
				if(iEnd>iStart)
				{
					strLst.Add(str.Mid(iStart,iEnd-iStart));
				}
				iStart = iEnd+1;
			}
			iEnd++;
		}
		if (iEnd>iStart)
		{
			strLst.Add(str.Mid(iStart,iEnd-iStart));
		}
		return strLst.GetCount();
	}
	typedef CArray<CStringA> CStringAList;
	typedef CArray<CStringW> CStringWList;

	template size_t SplitStringT<CStringA,char>  (const CStringA&str, char cSep,    CStringAList&strLst);
	template size_t SplitStringT<CStringW,wchar_t>(const CStringW&str,wchar_t cSep, CStringWList&strLst);

	template<typename T>
	void QuickSortArrayT(CArray<T,CElementTraits< T >>& array,int low,int high,int (*CmpT)(T,T)) 
	{ 
		if (low>=high)
		{
			return; 
		}
		int hi = high+1; 
		int lo = low;
		T elem= array[low]; 
		for( ; ; ) 
		{ 
			while(lo<high && 0>CmpT(array[++lo],elem)); //array[++lo]<elem
			while(hi>low && 0<CmpT(array[--hi],elem)); //array[--hi]>elem
			if(lo<hi) 
			{
				T elemTemp = array[lo];
				array[lo] = array[hi];
				array[hi] = elemTemp;
			}
			else 
			{
				break; 
			}
		}

		array[low] = array[hi];
		array[hi] = elem;

		QuickSortArrayT(array,low,hi-1,CmpT); 
		QuickSortArrayT(array,hi+1,high,CmpT); 
	}

#pragma pack(pop,_DM_PACKING)
#pragma warning(pop)

}// namespace DM