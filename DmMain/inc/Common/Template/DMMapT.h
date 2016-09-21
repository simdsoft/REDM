//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMMapT.h 
// File Des: 统一的Map模板
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
	///		统一的Map模板
	/// </summary>
	/// <remarks>
	///		由子类在析构时调用RemoveAll!
	/// </remarks>
	template<class TKey, class TObj>
	class DMMapT
	{
	public:
		DMMapT(){}
		~DMMapT(){/*RemoveAll必须由重载函数的子类调用，不然重载函数无效*/}

		/// -------------------------------------------------
		/// @brief	key是否已存在
		/// @param[in]		 key    用于查找Map的key
		/// @return true:存在，false:不存在
		bool IsKeyExist(const TKey &key)
		{
			return NULL != m_Map.Lookup(key);
		}
	
		/// -------------------------------------------------
		/// @brief 通过key获取obj
		/// @param[in]		 key    用于查找Map的key
		/// @param[out]		 obj    用于返回Map的obj
		/// @remark 安全获取方式
		/// @return true:获取成功,false:获取失败
		bool GetObjByKey(const TKey &key, TObj &obj)
		{
			bool bRet = false;
			do 
			{
				if (!IsKeyExist(key))
				{
					break;
				}
				obj = m_Map[key];
				bRet = true;
			} while (false);
			return bRet;
		}

		/// -------------------------------------------------
		/// @brief 通过key获取obj
		/// @param[in]		 key    用于查找Map的key
		/// @remark 不安全方式
		/// @return 返回对象
		TObj& GetObjByKey(const TKey &key)
		{
			return m_Map[key];
		}
 
		/// -------------------------------------------------
		/// @brief 设置Key
		/// @param[in]		 key    用于传入Map的key
		/// @param[in]		 obj    用于传入Map的obj
		/// @return true:成功,false:key已存在
		bool AddKey(const TKey &key, const TObj &obj)
		{
			bool bRet = false;
			do 
			{
				if (true == IsKeyExist(key))
				{
					break;
				}
				m_Map[key] = obj;
				bRet = true;
			} while (false);
			return bRet;
		}

		/// -------------------------------------------------
		/// @brief 强制设置Key
		/// @param[in]		 key    用于传入Map的key
		/// @param[in]		 obj    用于传入Map的obj
		/// @remark key已存在则覆盖key
		/// @return 无
		void SetKey(const TKey &key, const TObj &obj)
		{
			m_Map[key] = obj;
		}

		/// -------------------------------------------------
		/// @brief 删除key
		/// @param[in]		 key    用于传入Map的key
		/// @return true:移除成功,false:移除失败
		bool RemoveKey(const TKey &key)
		{
			bool bRet = false;
			do 
			{
				if (!IsKeyExist(key))
				{
					break;
				}

				PreMapKeyRemove(m_Map[key]);//调用删除前预处理函数

				m_Map.RemoveKey(key);
				bRet = true;
			} while (false);
			return bRet;
		}

		/// -------------------------------------------------
		/// @brief 删除所有key
		/// @return 无
		void RemoveAll()
		{
			POSITION pos = m_Map.GetStartPosition();
			while(pos)
			{
				DM::CMap<TKey,TObj>::CPair *p = m_Map.GetNext(pos);
				PreMapKeyRemove(p->m_value);
			}

			m_Map.RemoveAll();
		}

		/// -------------------------------------------------
		/// @brief 获取数目
		/// @return 数目
		size_t GetCount()
		{
			return m_Map.GetCount();
		}
		/// -------------------------------------------------
		/// @brief 可重载，外部重载
		/// @remark 外部来重载预处理删除项
		/// @return 无
		virtual void PreMapKeyRemove(const TObj &obj)
		{
			// 外部来重载预处理删除项
		}

	public:
		DM::CMap<TKey, TObj>     m_Map;
	};


}//namespace DM