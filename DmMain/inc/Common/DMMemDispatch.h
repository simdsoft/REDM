//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMMemPool.h 
// File Des: 保证cstring等堆分配都在主dll中进行
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-31	1.0		
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		保证在dll的堆上分配内存
	/// </summary>
	class DM_EXPORT DMMemDispatch
	{
	public:
		static void *DM_malloc(size_t size);
		static void *DM_realloc(void* memblock, size_t size);
		static void *DM_calloc(size_t num,size_t size);
		static void DM_free(void* memblock);
		static void* Get_CString_InitData();

	};

	/// <summary>
	///	模拟New[],iNum指定T类型的个数
	/// </summary>
	template <class T>
	T* DMNewT(int iNum)
	{	
		T* ptr = NULL;
		do 
		{
			if (0>=iNum
				||NULL == (ptr = (T*)DMMemDispatch::DM_malloc(iNum*sizeof(T)+12)))
			{
				break;
			}
			memcpy(ptr,"RedM",4);// 前4字节赋为magic字
			*((int*)ptr+1) = iNum;// 前8字节赋值为创建对象的个数
			*((int*)ptr+2) = sizeof(T);// 前12字节赋值为T对象的大小
			ptr = (T*)((int*)ptr+3);// new操作符返回的地址是开辟的连续空间的向后移12个字节之后的地址
			for (int i=0; i<iNum; i++)
			{
				//placement new在用户指定的内存位置上构建新的对象，这个构建过程不需要额外分配内存，只需要调用对象的构造函数即可
				new (ptr+i)T(); //new表达式（placeement new） 形式(place_address)type(initializer-list)
			}
		} while (false);
		return ptr;
	}

	/// <summary>
	///	模拟Delete[],两种状态,1.基类有虚表,这时DMNewT转换的基类指针和子类一致,2.基类无虚表,基类指针=子类指针-虚表(4字节)
	/// T:可为基类
	/// </summary>
	template <class T>
	void DMDelT(T* ptr)
	{
		do 
		{
			if (NULL == ptr)
			{
				break;
			}
			if (0 != memcmp((byte*)ptr-12,"RedM",4))
			{
				if (0 == memcmp((byte*)ptr-16,"RedM",4))// 父类没有虚表，子类有
				{
					ptr = (T*)((byte*)ptr - 4);
				}
				else
				{
					break;// 传入参数错误!
				}
			}

			int iNum = *((int*)ptr-2);
			int iSize = *((int*)ptr-1);
			for (int i=0; i<iNum; i++)
			{
				((T*)((byte*)ptr+i*iSize))->~T();//在释放前自动调用析构函数
			}
			DMMemDispatch::DM_free((int*)ptr - 3);
		} while (false);
	}

}//namespace DM