//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMEventSubscriber.h 
// File Des: 定义观察者模式的两种注册方式，全局函数+成员函数
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-9	1.0			
//--------------------------------------------------------
#pragma once
#include "DMEvents.h"

namespace DM
{
	enum _SLOTTYPE{SLOT_FUN,SLOT_MEMBER,SLOT_USER};
	class DM_EXPORT DMSlotFunctorBase
	{
	public:
		virtual ~DMSlotFunctorBase(){};
		virtual bool operator()(DMEventArgs *pArg)            = 0;
		virtual bool Equal(const DMSlotFunctorBase &src)const = 0;
		virtual DMSlotFunctorBase*Clone()const                = 0;
		virtual UINT GetSlotType()const                       = 0;
	};

	class DM_EXPORT DMFreeFunctionSlot:public DMSlotFunctorBase
	{
	public:
		typedef DMCode (SlotFunction)(DMEventArgs*);
		DMFreeFunctionSlot(SlotFunction* func):m_function(func)
		{
		}

		virtual bool operator()(DMEventArgs* pArg)
		{
			return DMSUCCEEDED(m_function(pArg));
		}

		virtual DMSlotFunctorBase* Clone()const 
		{
			return new DMFreeFunctionSlot(m_function);
		}

		virtual bool Equal(const DMSlotFunctorBase& src)const 
		{
			bool bRet = false;
			do 
			{
				if (SLOT_FUN != src.GetSlotType())
				{
					break;
				}

				const DMFreeFunctionSlot* psrc = static_cast<const DMFreeFunctionSlot*>(&src);
				bRet = (psrc->m_function==m_function);
			} while (false);
			return bRet;
		}

		virtual UINT GetSlotType()const {return SLOT_FUN;}
	public:
		SlotFunction*						m_function;
	};

	template<typename T>
	class DMMemberFunctionSlot:public DMSlotFunctorBase
	{
	public:
		typedef DMCode (T::*MemberFunctionType)(DMEventArgs*);
		DMMemberFunctionSlot(MemberFunctionType func, T*obj)
			:m_function(func)
			,m_object(obj)
		{
		}

		virtual bool operator()(DMEventArgs *pArg)
		{
			return DMSUCCEEDED((m_object->*m_function)(pArg));
		}

		virtual bool Equal(const DMSlotFunctorBase& src)const 
		{

			bool bRet = false;
			do 
			{
				if (SLOT_MEMBER != src.GetSlotType())
				{
					break;
				}
				const DMMemberFunctionSlot* psrc = static_cast<const DMMemberFunctionSlot*>(&src);
				bRet = (psrc->m_function==m_function && psrc->m_object==m_object);
			} while (false);
			return bRet;
		}

		virtual DMSlotFunctorBase* Clone() const 
		{
			return new DMMemberFunctionSlot(m_function,m_object);
		}

		virtual UINT GetSlotType()const {return SLOT_MEMBER;}

	protected:
		MemberFunctionType  m_function;			// 成员函数
		T*					m_object;           // this指针
	};

	template<class T>
	DMMemberFunctionSlot<T> Subscriber(DMCode (T::* pFn)(DMEventArgs*), T* pObject)
	{
		return DMMemberFunctionSlot<T>(pFn, pObject);
	}

	inline DMFreeFunctionSlot Subscriber(DMCode (*pFn)(DMEventArgs*))
	{
		return DMFreeFunctionSlot(pFn); 
	}
}//namespace DM