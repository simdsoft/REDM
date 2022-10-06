//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMMapT.h 
// File Des: ͳһ��Mapģ��
// File Summary: ����������ʱ����RemoveAll!
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
	///		ͳһ��Mapģ��
	/// </summary>
	/// <remarks>
	///		������������ʱ����RemoveAll!
	/// </remarks>
	template<class TKey, class TObj>
	class DMMapT
	{
	public:
		DMMapT(){}
		~DMMapT(){/*RemoveAll���������غ�����������ã���Ȼ���غ�����Ч*/}

		/// -------------------------------------------------
		/// @brief	key�Ƿ��Ѵ���
		/// @param[in]		 key    ���ڲ���Map��key
		/// @return true:���ڣ�false:������
		bool IsKeyExist(const TKey &key)
		{
			return NULL != m_Map.Lookup(key);
		}
	
		/// -------------------------------------------------
		/// @brief ͨ��key��ȡobj
		/// @param[in]		 key    ���ڲ���Map��key
		/// @param[out]		 obj    ���ڷ���Map��obj
		/// @remark ��ȫ��ȡ��ʽ
		/// @return true:��ȡ�ɹ�,false:��ȡʧ��
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
		/// @brief ͨ��key��ȡobj
		/// @param[in]		 key    ���ڲ���Map��key
		/// @remark ����ȫ��ʽ
		/// @return ���ض���
		TObj& GetObjByKey(const TKey &key)
		{
			return m_Map[key];
		}
 
		/// -------------------------------------------------
		/// @brief ����Key
		/// @param[in]		 key    ���ڴ���Map��key
		/// @param[in]		 obj    ���ڴ���Map��obj
		/// @return true:�ɹ�,false:key�Ѵ���
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
		/// @brief ǿ������Key
		/// @param[in]		 key    ���ڴ���Map��key
		/// @param[in]		 obj    ���ڴ���Map��obj
		/// @remark key�Ѵ����򸲸�key
		/// @return ��
		void SetKey(const TKey &key, const TObj &obj)
		{
			m_Map[key] = obj;
		}

		/// -------------------------------------------------
		/// @brief ɾ��key
		/// @param[in]		 key    ���ڴ���Map��key
		/// @return true:�Ƴ��ɹ�,false:�Ƴ�ʧ��
		bool RemoveKey(const TKey &key)
		{
			bool bRet = false;
			do 
			{
				if (!IsKeyExist(key))
				{
					break;
				}

				PreMapKeyRemove(m_Map[key]);//����ɾ��ǰԤ������

				m_Map.RemoveKey(key);
				bRet = true;
			} while (false);
			return bRet;
		}

		/// -------------------------------------------------
		/// @brief ɾ������key
		/// @return ��
		void RemoveAll()
		{
			POSITION pos = m_Map.GetStartPosition();
			while(pos)
			{
				auto p = m_Map.GetNext(pos);
				PreMapKeyRemove(p->m_value);
			}

			m_Map.RemoveAll();
		}

		/// -------------------------------------------------
		/// @brief ��ȡ��Ŀ
		/// @return ��Ŀ
		size_t GetCount()
		{
			return m_Map.GetCount();
		}
		/// -------------------------------------------------
		/// @brief �����أ��ⲿ����
		/// @remark �ⲿ������Ԥ����ɾ����
		/// @return ��
		virtual void PreMapKeyRemove(const TObj &obj)
		{
			// �ⲿ������Ԥ����ɾ����
		}

	public:
		DM::CMap<TKey, TObj>     m_Map;
	};


}//namespace DM