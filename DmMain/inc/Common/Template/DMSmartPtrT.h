//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSmartPtrT.h 
// File Des: 智能指针模板类,管理所有基于DMRefNum的指针
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-9-19	1.0			
//      guoyou      2014-10-11  1.1           加入CComPtr功能，支持COM指针
//      guoyou		2015-01-09	1.2	           增加帮助chm注释
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		智能指针模板类，整个DM使用它来控制内存自动释放，针对<see cref="DMRefNum"/>的子类使用
	/// </summary>
	template<class T>
	class DMSmartPtrT
	{
	public:
		DMSmartPtrT(void) throw()
			:m_ptr(0)
		{
		}

		DMSmartPtrT(int nNull) throw()
		{
			(void)nNull;
			m_ptr = NULL;
		}

		DMSmartPtrT(T *ptr) throw()
			:m_ptr(ptr)
		{
			if (m_ptr)
			{
				m_ptr->AddRef();
			}
		}

		DMSmartPtrT(const DMSmartPtrT & sour) throw()
			:m_ptr(sour.m_ptr)
		{
			if (m_ptr)
			{
				m_ptr->AddRef();
			}
		}

		~DMSmartPtrT(void) throw()
		{
			if (0 != m_ptr)
			{
				m_ptr->Release();
			}
		}

		T * operator->() const throw()
		{
			DMASSERT_EXPR(NULL != m_ptr,L"m_ptr为空,请确定是否初始化!");
			return m_ptr;
		};

		operator T*() const throw()
		{
			return m_ptr;
		}

		T& operator*() const throw()
		{
			return *m_ptr;
		}

		/// 此代码来自CComPtr
		T** operator&() throw()
		{
			//DMASSERT(m_ptr==NULL);
			return &m_ptr;
		}

		bool operator!() const throw()
		{
			return (NULL == m_ptr);
		}

		bool operator<(T* ptr) const throw()
		{
			return m_ptr < ptr;
		}
		bool operator!=(T* ptr) const
		{
			return !operator==(ptr);
		}
		bool operator==(T* ptr) const throw()
		{
			return m_ptr == ptr;
		}

		T* operator=(T* lp) throw()
		{
			if(*this != lp)
			{
				if (m_ptr)
				{
					m_ptr->Release();
				}
				m_ptr = lp;
				if(m_ptr)
				{
					m_ptr->AddRef();
				}
			}
			return *this;
		}

		T* operator=(const DMSmartPtrT<T>& lp) throw()
		{
			if (*this != lp)
			{
				if(m_ptr)
				{
					m_ptr->Release();
				}
				m_ptr = lp;
				if (m_ptr)
				{
					m_ptr->AddRef();
				}
			}
			return *this;	
		}

		bool isNull()const {return 0 == m_ptr;}
		bool isValid()const {return 0 != m_ptr;}

		T * get(void) const {return m_ptr;}

		/// <summary>
		///		重新附加一个对象指针，原引用计数减1，新对象指针引用计数+1
		/// </summary>
		/// <param name="ptr">对象指针</param>
		/// <returns>无</returns>
		void reset(T* ptr=0)
		{
			if (0 != ptr)
			{
				ptr->AddRef();
			}

			if (0 != m_ptr)
			{
				m_ptr->Release();
			}

			m_ptr = ptr;
		}

		/// <summary>
		///		释放附加对象指针，并把自身设置为NULL,
		/// </summary>
		/// <remarks>
		///     注意使用的是.而不是->,如A.Release()
		///  </remarks>
		/// <returns>无</returns>
		void Release() throw()
		{
			T* pTemp = m_ptr;
			if (pTemp)
			{
				m_ptr = NULL;
				pTemp->Release();
			}
		}

		/// <summary>
		///		附加一个对象指针，不会增加引用计数
		/// </summary>
		/// <param name="p2">对象指针</param>
		/// <returns>无</returns>
		void Attach(T* p2) throw()
		{
			if (m_ptr)
				m_ptr->Release();
			m_ptr = p2;
		}

		/// <summary>
		///		脱离附加的对象指针，附加的对象引用计数不变，此时要注意外部释放
		/// </summary>
		/// <returns>附加的对象指针</returns>
		T* Detach() throw()
		{
			T* pt = m_ptr;
			m_ptr = NULL;
			return pt;
		}

		/// <summary>
		///		拷出对象指针
		/// </summary>
		/// <param name="ppT">对象指针</param>
		/// <returns>S_OK表示成功</returns>
		HRESULT CopyTo(OUT T** ppT) throw()
		{
			if (ppT == NULL)
				return E_POINTER;
			*ppT = m_ptr;
			if (m_ptr)
			{
				m_ptr->AddRef();
			}

			return S_OK;
		}

	private:
		T                 *m_ptr;
	};
}// namespace DM


