#include "DmMainAfx.h"
#include "DUIFontPool.h"

namespace DM
{
	DUIFontPool::DUIFontPool()
	{
		wcscpy_s(m_szDefFontFace, L"宋体");
		::GetObjectW(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONTW), &m_lfDefault);
		m_lfDefault.lfHeight = -14;// 负值
		wcscpy_s(m_lfDefault.lfFaceName, countof(m_lfDefault.lfFaceName), m_szDefFontFace);
	}

	DUIFontPool::~DUIFontPool()
	{
		RemoveAll();
	}

	// font="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位
	bool DUIFontPool::SetDefaultFont(const CStringA& strFont)
	{
		bool bRet = false;
		do 
		{
			LOGFONTW lf = {0};
			if (false == GetLogFont(strFont,&lf))
			{
				break;
			}
			memcpy(&m_lfDefault, &lf, sizeof(LOGFONTW));
			wcscpy_s(m_szDefFontFace, countof(m_lfDefault.lfFaceName), m_lfDefault.lfFaceName);
			bRet = true;
		} while (false);
		return bRet;
	}

	IDMFontPtr DUIFontPool::GetFont(const CStringA& strFont)
	{
		IDMFontPtr pFont = NULL;
		do 
		{
			// 解析并创建
			LOGFONTW lf={0};
			CStringA szFont = strFont;
			szFont.Trim();
			szFont.MakeLower();
			if (false == GetLogFont(szFont,&lf))
			{
				if (GetObjByKey(szFont,pFont))
				{
					break;// 已找到
				}
				g_pDMRender->CreateFont(&m_lfDefault,&pFont); // 默认字体
				AddKey(szFont, pFont);
				break;// 已找到
			}
			
			CStringA szKey  = GetFontKey(&lf); // 生成标准key
			if (GetObjByKey(szKey,pFont))
			{
				break;// 已找到
			}

			g_pDMRender->CreateFont(&lf,&pFont);
			if (pFont)
			{
				AddKey(szKey, pFont);// 加入标准key
			}
		} while (false);
		return pFont;
	}

	CStringA DUIFontPool::GetFontKey(const LPLOGFONTW lpLogFont)
	{
		CStringA szKey;
		CStringA strFaceName = DMW2A(lpLogFont->lfFaceName);strFaceName.MakeLower();
		szKey.Format("weight:%d,charset:%d,underline:%d,italic:%d,strike:%d,size:%d,face:%s",lpLogFont->lfWeight,lpLogFont->lfCharSet,lpLogFont->lfUnderline,lpLogFont->lfItalic,lpLogFont->lfStrikeOut,lpLogFont->lfHeight,strFaceName);
		return szKey;
	}

	bool DUIFontPool::GetLogFont(const CStringA& strFont,LPLOGFONTW lpLogFont)
	{
		bool bRet = false;
		do 
		{
			CStringA szFont = strFont;
			szFont.Trim();
			szFont.MakeLower();
			if (szFont.IsEmpty()||NULL==lpLogFont)// 为NULL
			{
				break;
			}

			// 解析并创建
			bool bItalic=0,bUnderline=0,bStrike=0;   
			CStringW strFace; int lFontSize=0;int lWightSize=0;int lCharSet=0;

			//------------------------------------------
			int nPosBegin = szFont.Find("face:");                     
			if(nPosBegin!=-1)                                         
			{                                                         
				nPosBegin+=5;                                             
				int nPosEnd=szFont.Find(";",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.Find(",",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.GetLength();
				strFace=DMA2W(szFont.Mid(nPosBegin,nPosEnd-nPosBegin), CP_UTF8);
			} 
			else
			{
				strFace = m_lfDefault.lfFaceName; 
			}

			//------------------------------------------
			nPosBegin=szFont.Find("underline:");                    
			if(nPosBegin!=-1)                                         
			{                                                         
				bUnderline=szFont.Mid(nPosBegin+10,1)!="0";             
			}
			else
			{
				bUnderline=!!m_lfDefault.lfUnderline;
			}

			//------------------------------------------
			nPosBegin=szFont.Find("italic:");                       
			if(nPosBegin!=-1)                                         
			{                                                         
				bItalic=szFont.Mid(nPosBegin+7,1)!="0";                 
			}
			else
			{
				bItalic=!!m_lfDefault.lfItalic;
			}

			//------------------------------------------
			nPosBegin=szFont.Find("strike:");                       
			if(nPosBegin!=-1)                                         
			{                                                         
				bStrike=szFont.Mid(nPosBegin+7,1)!="0";                 
			}
			else
			{
				bStrike=!!m_lfDefault.lfStrikeOut;
			}

			//------------------------------------------
			nPosBegin=szFont.Find("size:");
			if (nPosBegin!=-1)
			{
				nPosBegin+=5;                                             
				int nPosEnd=szFont.Find(";",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.Find(",",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.GetLength();
				CStringA strFontSize=szFont.Mid(nPosBegin,nPosEnd-nPosBegin);
				lFontSize = DMABS(m_lfDefault.lfHeight);
				DMAttributeDispatch::ParseInt(strFontSize,lFontSize);
			}
			else
			{
				lFontSize = DMABS(m_lfDefault.lfHeight);
			}

			//------------------------------------------
			nPosBegin=szFont.Find("weight:");
			if (nPosBegin!=-1)
			{
				nPosBegin+=7;                                             
				int nPosEnd=szFont.Find(";",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.Find(",",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.GetLength();
				CStringA strWeightSize=szFont.Mid(nPosBegin,nPosEnd-nPosBegin);
				lWightSize = DMABS(m_lfDefault.lfWeight);
				DMAttributeDispatch::ParseInt(strWeightSize,lWightSize);
			}
			else
			{
				lWightSize = DMABS(m_lfDefault.lfWeight);
			}
			//------------------------------------------
			nPosBegin=szFont.Find("charset:");
			if (nPosBegin!=-1)
			{
				nPosBegin+=8;  
				int nPosEnd=szFont.Find(";",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.Find(",",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.GetLength();
				CStringA strCharSet=szFont.Mid(nPosBegin,nPosEnd-nPosBegin);
				lCharSet = DMABS(m_lfDefault.lfCharSet);
				DMAttributeDispatch::ParseInt(strCharSet,lCharSet);
			}
			else
			{
				lCharSet = DMABS(m_lfDefault.lfCharSet);
			}

			//------------------------------------------
			memcpy(lpLogFont, &m_lfDefault, sizeof(LOGFONTW));
			if (0 != lFontSize)
			{
				lpLogFont->lfHeight = -DMABS(lFontSize);// 负值
			}
			if (0 != lWightSize)
			{
				lpLogFont->lfWeight  = DMABS(lWightSize);
			}

			lpLogFont->lfCharSet	 = DMABS(lCharSet);// charset可为0
			lpLogFont->lfUnderline   = (false != bUnderline);
			lpLogFont->lfItalic      = (false != bItalic);
			lpLogFont->lfStrikeOut   = (false != bStrike);
			lpLogFont->lfQuality	 = CLEARTYPE_NATURAL_QUALITY;
			if (!strFace.IsEmpty())
			{
				wcscpy_s(lpLogFont->lfFaceName,countof(lpLogFont->lfFaceName),strFace);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	/// @brief 预处理移除项
	/// @return
	void DUIFontPool::PreMapKeyRemove(const IDMFontPtr &obj)
	{
		obj->Release(); // 如引用计数为0，则自动delete this
	}

}//namespace DM