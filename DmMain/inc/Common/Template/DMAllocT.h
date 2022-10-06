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
	///		��ָ��ת����ָ������
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

	// -----------------------------------------------------����ΪDMLazyT--------------------------------------------------------------------
	/// <summary>
	///		��Ϊһ����ʱ���棬�����ʵ���ȿ�����ջ�Ϸ����ڴ棬ջ�Ͽռ䲻��ʱ���ʹӶ��з���
	/// </summary>
	/// <remarks>
	///		����atlapp.CTempBuffer,��η���,���ȸ�����һ�ε�
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
		/// @brief			�ͷ��ڴ�
		void Free()
		{
			if (m_p&&m_p != DMCastT<T*>(m_abFixedBuffer))
			{
				free(m_p);
			}
		}

		/// -------------------------------------------------
		/// @brief			�����ڴ棨�����������
		T* Allocate(size_t nElements)
		{
			DMASSERT(nElements <= (SIZE_MAX / sizeof(T)));
			return AllocateBytes(nElements * sizeof(T));
		}

		/// -------------------------------------------------
		/// @brief			�����ڴ棨���ֽ�����
		T* AllocateBytes(size_t nBytes)
		{
			Free();
			//DMASSERT(m_p == NULL);
			if (nBytes > t_nFixedBytes)
			{
				m_p = reinterpret_cast<T*>(malloc(nBytes));/// ��ʹ��static_cast,static_cast����ʽ����ת�����п��ܲ���ת����reinterpret_cast�ǵͲ����
			}
			else
			{
				m_p = DMCastT<T*>(m_abFixedBuffer);
			}

			// ��Ϊ0
			memset(m_p, 0, (nBytes>t_nFixedBytes)?nBytes:t_nFixedBytes);
			return m_p;
		}

	private:
		T*			m_p;
		BYTE		m_abFixedBuffer[t_nFixedBytes];
	};

	// -----------------------------------------------------����ΪDMLazyT--------------------------------------------------------------------
	/// <summary>
	///		Ĭ��������������
	/// </summary>
	template <class T>
	struct DMDefLazyTraits 
	{
		static T* New(void* Storage) 
		{
			/// ע�⣬����ʹ��new������Storage��Ϊ��ʼ��ַ
			return new (Storage) T;
		}
		static T* NewWithCopy(void* Storage, T* src)
		{
			return new (Storage) T(*src);
		}
		static void	Delete(T* Storage)
		{
			Storage->~T();
		}
	};

	/// <summary>
	///		����������ע��Delete�Ĵ���
	/// </summary>
	template <typename T>
	struct DMLeakLazyTraits 
	{
		static T* New(void* Storage) 
		{
			return new (Storage) T;
		}

		static T* NewWithCopy(void* Storage, T* src)
		{
			return new (Storage) T(*src);
		}

		static void	Delete(T* Storage)
		{
			// nothing
		}
	};


	/// <summary>
	///		DMLazyT�Ļ���,Ԥ�ȷ����ڴ沢��ʱ�������
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
				// LeakyDMLazyTraits�����������
				LazyTraits::Delete(m_ptr);
			}
		}

	public:
		/// -------------------------------------------------
		/// @brief			��ʼ��
		/// @return			��������򷵻��Ѵ��ڵĶ���.
		TPtr Init() 
		{
			if (NULL == m_ptr)
			{
				m_ptr = LazyTraits::New(m_storage);
			}
			return m_ptr;
		}

		/// -------------------------------------------------
		/// @brief			�������󲢸��ƣ������Ѵ��ڵĶ���
		/// @return			���ش��ڵĶ���.
		template<size_t N>
		TPtr InitWithCopy(TPtr src) 
		{
			DMASSERT_MSG(src, "DMLazyBaseT::InitWithCopy");
			if (NULL == m_ptr) 
			{
				m_ptr = LazyTraits::NewWithCopy(m_storage, src);
			}
			return m_ptr;
		}

		/// -------------------------------------------------
		/// @brief			����DMLazy����.
		/// @return			����DMLazy����.
		void destroy() 
		{
			if (this->IsValid()) 
			{
				LazyTraits::Delete(m_ptr);
			}
			m_ptr = NULL;
		}

		/// -------------------------------------------------
		/// @brief			�����Ƿ���Ч
		/// @return			true:��Ч,false:��Ч
		bool IsValid() const
		{ 
			return NULL != m_ptr; 
		}

		/// -------------------------------------------------
		/// @brief			��ȡ����
		/// @return			���󣬿���ΪNULL
		TPtr Get() 
		{ 
			if (!m_ptr)
			{
				if (bAutoInit)
				{
					Init();
				}
			}

			DMASSERT_MSG(this->IsValid(), "DMLazyBaseT::Get");
			return m_ptr; 
		}

		/// -------------------------------------------------
		/// @brief			����������
		/// @return			->����������
		const TPtr operator->() const 
		{
			if (!m_ptr)
			{
				if (bAutoInit)
				{
					Init();
				}
			}
			DMASSERT_MSG(m_ptr, "const DMLazyBaseT::->");
			return m_ptr;
		}

		/// -------------------------------------------------
		/// @brief			����������
		/// @return			->����������
		TPtr operator->() 
		{
			if (!m_ptr)
			{
				if (bAutoInit)
				{
					Init();
				}
			}
			DMASSERT_MSG(m_ptr, "DMLazyBaseT::->");
			return m_ptr;
		}

		/// -------------------------------------------------
		/// @brief			unspecified_bool_type
		/// @remark         http://hgy413.com/1958.html
		/// @return			���������boolֵ�ж�
		typedef TPtr DMLazyBaseT::*unspecified_bool_type;
		operator unspecified_bool_type() const 
		{
			return m_ptr ? &DMLazyBaseT::m_ptr : NULL;
		}
	protected:
		/// -------------------------------------------------
		/// @brief			���ش�������������г�ʼ��.
		/// @return			��
		void* InitWithStorage() 
		{
			if (NULL == m_ptr)
			{
				m_ptr = reinterpret_cast<T*>(this->m_storage);
			}
			return m_ptr;
		}

	protected:
		TPtr		m_ptr;					///<ҪôΪNULL��Ҫôָ��m_storage����λ
		char		m_storage[STORAGESIZE]; ///<С����Ĭ��ʹ��ջ�洢
	};

	template <typename T, bool bAutoInit, typename LazyTraits, int STORAGESIZE> class DMLazyBaseT;
	/// <summary>
	///		LayzT�Ĵ��������SkTLazy.h+SkTypes.h,��лzc��ԭʼ����
	/// </summary>
	/// <remarks>
	///		�ӳٳ�ʼ�����ǽ�����ĳ�ʼ���ӳٵ���һ��ʹ�øö���ʱ���Ӷ���߳����Ч�ʣ�ʹ����ռ�ø��ٵ��ڴ�
	/// </remarks>
	template<typename T, bool bAutoInit = true, typename LazyTraits = DMDefLazyTraits<T> >
	class DMLazyT
		: public DMLazyBaseT<T, bAutoInit, LazyTraits, sizeof(T)>
	{
		typedef	DMLazyBaseT<T, bAutoInit, LazyTraits, sizeof(T)>	__baseClass;
		typedef T* TPtr;
	public:
		DMLazyT() :__baseClass()
		{
		}

		explicit DMLazyT(const TPtr src) :__baseClass()
		{
			this->InitWithCopy(src);
		}

		DMLazyT(const DMLazyT<T>& src) : __baseClass()
		{
			if (src.IsValid())
			{
				this->InitWithCopy(src.Get());
			}
		}
	public:
		/// -------------------------------------------------
		/// @brief			��ʼ��
		/// @return			���ԭʼ���Ѵ���,��������ԭʼ�ģ��ٴ����µ�
		TPtr InitWithNew()
		{
			this->destroy();
			return this->Init();
		}

		/// -------------------------------------------------
		/// @brief			����
		/// @return			����һ��Դ������������ָ�룬�Ḳ��ԭʼ�ģ����ԭ���ѳ�ʼ����
		TPtr Set(const T& src)
		{
			if (!this->IsValid())
			{
				return __baseClass::InitWithCopy(&src);
			}
			*this->Get() = src;
			return this->Get();
		}
	};

}//namespace DM

