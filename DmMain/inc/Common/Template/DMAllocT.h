//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMAllocT.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-01-13	1.0			
//-------------------------------------------------------
#pragma once
#include <new>

namespace DM
{
	/// <summary>
	///		将指针转换成指定类型
	/// </summary>
	template <class Dst> Dst DMCastT(const void* ptr) 
	{
		union 
		{
			const void* src;
			Dst dst;
		} data;
		data.src = ptr;
		return data.dst;
	}

	// -----------------------------------------------------以下为DMLazyT--------------------------------------------------------------------
	/// <summary>
	///		作为一个临时缓存，该类的实现先考虑在栈上分配内存，栈上空间不够时，就从堆中分配
	/// </summary>
	/// <remarks>
	///		基于atlapp.CTempBuffer,多次分配,会先覆盖上一次的
	/// </remarks>
	template<class T, int t_nFixedBytes = 128>
	class DMBufT
	{
	public:
		DMBufT() : m_p(NULL)
		{
		}

		DMBufT(size_t nElements) : m_p(NULL)
		{
			Allocate(nElements);
		}

		~DMBufT()
		{
			Free();
		}

		operator T*() const
		{
			return m_p;
		}

		T* operator ->() const
		{
			DMASSERT(NULL != m_p);
			return m_p;
		}

		T * get(void) const {return m_p;}
		
		/// -------------------------------------------------
		/// @brief			释放内存
		void Free()
		{
			if (m_p&&m_p != DMCastT<T*>(m_abFixedBuffer))
			{
				DM_DELETE_ARRAY(m_p);
			}
		}

		/// -------------------------------------------------
		/// @brief			分配内存（按对象个数）
		T* Allocate(size_t nElements)
		{
			DMASSERT(nElements <= (SIZE_MAX / sizeof(T)));
			return AllocateBytes(nElements * sizeof(T));
		}

		/// -------------------------------------------------
		/// @brief			分配内存（按字节数）
		T* AllocateBytes(size_t nBytes)
		{
			Free();
			//DMASSERT(m_p == NULL);
			if (nBytes > t_nFixedBytes)
			{
				m_p = reinterpret_cast<T*>(new BYTE[nBytes]);/// 不使用static_cast,static_cast是显式类型转换，有可能不能转换，reinterpret_cast是低层解释
			}
			else
			{
				m_p = DMCastT<T*>(m_abFixedBuffer);
			}

			// 置为0
			memset(m_p, 0, (nBytes>t_nFixedBytes)?nBytes:t_nFixedBytes);
			return m_p;
		}

	private:
		T*			m_p;
		BYTE		m_abFixedBuffer[t_nFixedBytes];
	};

	// -----------------------------------------------------以下为DMLazyT--------------------------------------------------------------------
	/// <summary>
	///		默认正常析构对象
	/// </summary>
	template <class T>
	struct DMDefLazyTraits 
	{
		static T* New(void* Storage) 
		{
			/// 注意，这样使用new，会以Storage做为起始地址
			return new (DMCastT<T*>(Storage)) T;
		}
		static T* NewWithCopy(void* Storage, T* src)
		{
			return new (DMCastT<T*>(Storage)) (*src);
		}
		static void	Delete(T* Storage)
		{
			Storage->~T();
		}
	};

	/// <summary>
	///		不析构对象，注意Delete的处理
	/// </summary>
	template <typename T>
	struct DMLeakLazyTraits 
	{
		static T* New(void* Storage) 
		{
			return new (DMCastT<T*>(Storage)) T;
		}

		static T* NewWithCopy(void* Storage, T* src)
		{
			return new (DMCastT<T*>(Storage)) (*src);
		}

		static void	Delete(T* Storage)
		{
			// nothing
		}
	};

	template <typename T, bool bAutoInit,typename LazyTraits, int STORAGESIZE> class DMLazyBaseT;
	/// <summary>
	///		LayzT的代码抽离于SkTLazy.h+SkTypes.h,感谢zc的原始代码
	/// </summary>
	/// <remarks>
	///		延迟初始化就是将对象的初始化延迟到第一次使用该对象时，从而提高程序的效率，使程序占用更少的内存
	/// </remarks>
	template<typename T, bool bAutoInit=true, typename LazyTraits = DMDefLazyTraits<T> > 
	class DMLazyT
		: public DMLazyBaseT<T, bAutoInit,LazyTraits, sizeof(T)>
	{
		typedef	DMLazyBaseT<T, bAutoInit,LazyTraits, sizeof(T)>	__baseClass;
		typedef T*										TPtr;
	public:
		DMLazyT():__baseClass()
		{
		}

		explicit DMLazyT(const TPtr src):__baseClass()
		{
			this->InitWithCopy(src);
		}

		DMLazyT(const DMLazyT<T>& src): __baseClass()
		{
			if (src.IsValid())
			{
				this->InitWithCopy(src.Get());
			}
		}
	public:
		/// -------------------------------------------------
		/// @brief			初始化
		/// @return			如果原始的已存在,则先销毁原始的，再创建新的
		TPtr InitWithNew() 
		{
			this->destroy();
			return this->Init();
		}

		/// -------------------------------------------------
		/// @brief			设置
		/// @return			复制一个源，并返回它的指针，会覆盖原始的（如果原来已初始化）
		TPtr Set(const T& src)
		{
			if (!IsValid())
			{
				return __baseClass::InitWithCopy(&src);
			}
			*Get() = src;
			return Get();
		}
	};

	/// <summary>
	///		DMLazyT的基类,预先分配内存并延时构造对象
	/// </summary>
	template <typename T, bool bAutoInit=true, typename LazyTraits = DMDefLazyTraits<T>, int STORAGESIZE = sizeof(T)> 
	class DMLazyBaseT 
	{
	public:
		typedef T					TClass;
		typedef T*					TPtr;
		typedef T&					TRef;
	public:
		DMLazyBaseT():m_ptr(NULL) {}

		~DMLazyBaseT() 
		{
			if (this->IsValid())
			{
				// LeakyDMLazyTraits不会调用析构
				LazyTraits::Delete(m_ptr);
			}
		}

	public:
		/// -------------------------------------------------
		/// @brief			初始化
		/// @return			创建对象或返回已存在的对象.
		TPtr Init() 
		{
			if (NULL == m_ptr)
			{
				m_ptr = LazyTraits::New(m_storage);
			}
			return m_ptr;
		}

		/// -------------------------------------------------
		/// @brief			创建对象并复制，返回已存在的对象
		/// @return			返回存在的对象.
		template<size_t N>
		TPtr InitWithCopy(TPtr src) 
		{
			DMASSERT_EXPR(src, L"DMLazyBaseT::InitWithCopy");
			if (NULL == m_ptr) 
			{
				m_ptr = LazyTraits::NewWithCopy(m_storage, src);
			}
			return m_ptr;
		}

		/// -------------------------------------------------
		/// @brief			销毁DMLazy对象.
		/// @return			销毁DMLazy对象.
		void destroy() 
		{
			if (this->IsValid()) 
			{
				LazyTraits::Delete(m_ptr);
			}
			m_ptr = NULL;
		}

		/// -------------------------------------------------
		/// @brief			对象是否有效
		/// @return			true:有效,false:无效
		bool IsValid() const
		{ 
			return NULL != m_ptr; 
		}

		/// -------------------------------------------------
		/// @brief			获取对象
		/// @return			对象，可能为NULL
		TPtr Get() 
		{ 
			if (!m_ptr)
			{
				if (bAutoInit)
				{
					Init();
				}
			}

			DMASSERT_EXPR(this->IsValid(), L"DMLazyBaseT::Get");
			return m_ptr; 
		}

		/// -------------------------------------------------
		/// @brief			操作符重载
		/// @return			->操作符重载
		const TPtr operator->() const 
		{
			if (!m_ptr)
			{
				if (bAutoInit)
				{
					Init();
				}
			}
			DMASSERT_EXPR(m_ptr, L"const DMLazyBaseT::->");
			return m_ptr;
		}

		/// -------------------------------------------------
		/// @brief			操作符重载
		/// @return			->操作符重载
		TPtr operator->() 
		{
			if (!m_ptr)
			{
				if (bAutoInit)
				{
					Init();
				}
			}
			DMASSERT_EXPR(m_ptr, L"DMLazyBaseT::->");
			return m_ptr;
		}

		/// -------------------------------------------------
		/// @brief			unspecified_bool_type
		/// @remark         http://hgy413.com/1958.html
		/// @return			增加类对象bool值判定
		typedef TPtr DMLazyBaseT::*unspecified_bool_type;
		operator unspecified_bool_type() const 
		{
			return m_ptr ? &DMLazyBaseT::m_ptr : NULL;
		}
	protected:
		/// -------------------------------------------------
		/// @brief			返回存贮区给对象进行初始化.
		/// @return			无
		void* InitWithStorage() 
		{
			if (NULL == m_ptr)
			{
				m_ptr = reinterpret_cast<T*>(this->m_storage);
			}
			return m_ptr;
		}

	protected:
		TPtr		m_ptr;					///<要么为NULL，要么指向m_storage所在位
		char		m_storage[STORAGESIZE]; ///<小数据默认使用栈存储
	};

}//namespace DM

