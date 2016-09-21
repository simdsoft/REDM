//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMArray.h 
// File Des: 统一的Array模板
// File Summary: 子类在析构时调用RemoveAll!
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{	
	/// <summary>
	///		统一的Array模板
	/// </summary>
	/// <remarks>
	///		由子类在析构时调用RemoveAll!
	/// </remarks>
	template<class TObj>
	class DMArrayT
	{
	public:
		DMArrayT(){};
		~DMArrayT(){/*RemoveAll必须由重载函数的子类调用，不然重载函数无效*/}

		/// -------------------------------------------------
		/// @brief obj是否存在
		/// @param[in]		 obj    用于查找的obj
		/// @return DM_INVALID_VALUE(-1)表示不存在
		int FindObj(const TObj &obj)
		{
			int count = (int)m_DMArray.GetCount();
			for (int i=0; i<count; i++)
			{
				if (EqualArrayObj(obj, m_DMArray[i]))
				{
					return i;
				}
			}

			return DM_INVALID_VALUE;
		}

		/// -------------------------------------------------
		/// @brief 取得obj,
		/// @param[in]  iElement		指定项的index
		/// @param[out] obj			    返回指定项的obj
		/// @remark 安全获取方式，推荐
		/// @return false:获取失败,true:获取成功
		bool GetObj(size_t iElement, TObj &obj)
		{
			bool bRet = false;
			do 
			{
				if (iElement>=GetCount())
				{
					break;
				}

				obj = m_DMArray[iElement];
				bRet = true;
			} while (false);
			return true;
		}

		/// -------------------------------------------------
		/// @brief 取得obj
		/// @param[in] iElement		   指定项的index
		/// @remark 不安全获取方式
		/// @return 获取的对象
		TObj &GetObj(size_t iElement)
		{
			return m_DMArray[iElement];
		}

		/// -------------------------------------------------
		/// @brief 加入obj
		/// @param[in] obj			   加入指定项的obj
		/// @return true:加入成功,false:obj已存在
		bool AddObj(const TObj &obj)
		{
			bool bRet = false;
			do 
			{
				if (DM_INVALID_VALUE != FindObj(obj))/// obj已存在
				{
					break;
				}
				m_DMArray.Add(obj);
				bRet = true;
			} while (false);
			return bRet;
		}

		/// -------------------------------------------------
		/// @brief 移除obj
		/// @param[in] iElement		指定项的index
		/// @return true:移除成功,false:移除成功
		bool RemoveObj(size_t iElement)
		{
			bool bRet = false;
			do 
			{
				if (iElement>=GetCount())
				{
					break;
				}
				PreArrayObjRemove(m_DMArray[iElement]);
				m_DMArray[iElement] = NULL;//lzlong add
				m_DMArray.RemoveAt(iElement);
				bRet = true;
			} while (false);
			return bRet;
		}

		/// -------------------------------------------------
		/// @brief 移除obj
		/// @param[in] obj		指定项obj
		/// @return true:移除成功,false:移除成功
		bool RemoveObj(TObj &obj)
		{
			bool bRet = false;
			do 
			{
				int iFind = FindObj(obj);
				if (-1 == iFind)//obj不存在
				{
					break;
				}
				PreArrayObjRemove(obj);
				obj = NULL;//lzlong add 
				m_DMArray.RemoveAt(iFind);
				bRet = true;
			} while (false);
			return bRet;
		}

		/// -------------------------------------------------
		/// @brief 移除所有obj
		/// @return 无
		void RemoveAll()
		{
			int count = (int)m_DMArray.GetCount();
			for (int i=count-1; i>=0; i--)
			{
				PreArrayObjRemove(m_DMArray[i]);
				m_DMArray[i] = NULL; //lzlong add 
			}

			m_DMArray.RemoveAll();
		}

		/// -------------------------------------------------
		/// @brief 获取数目
		/// @return 数目
		size_t GetCount()
		{
			return m_DMArray.GetCount();
		}
		
		/// -------------------------------------------------
		/// @brief 可重载
		/// @param[in] objsrc		需要比较的项
		/// @param[in] objdest		Array中轮流比较的项
		/// @remark 比较函数，由子类重载，默认直接==
		/// @return true:相等,false:不相等
		virtual bool EqualArrayObj(const TObj &objsrc, const TObj &objdest)
		{
			/// 外部子类重载
			if (objsrc == objdest)
			{
				return true;
			}
			return false;
		}

		/// -------------------------------------------------
		/// @brief 可重载
		/// @param[in] obj		指定项
		/// @remark 在移除前的预处理函数，用于子类重载
		/// @return 无
		virtual void PreArrayObjRemove(const TObj &obj)
		{
			/// 外部子类重载
		}

	public:
		DM::CArray<TObj>         m_DMArray;
	};

}//namespace DM