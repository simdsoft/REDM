#include "StdAfx.h"
#include "MacroHelper.h"

bool CMacroHelper::LoadFile(CStringW strPath,CStringW strStart)
{
	bool bRet = false;
	do 
	{
		if (!PathFileExists(strPath))
		{
			break;
		}
		DWORD dwSize = DM::GetFileSizeW((wchar_t*)(LPCWSTR)strPath);
		DMBufT<byte>pBuf; pBuf.Allocate(dwSize);
		DWORD dwRead = 0;
		DM::GetFileBufW((wchar_t*)(LPCWSTR)strPath,(void**)&pBuf,dwSize,dwRead);
		CStringA straBuf((const char*)pBuf.get(),dwSize);
		CStringW strBuf = DMA2W(straBuf);

		int nStart = 0;
		int nFind = nStart = strBuf.Find(strStart,nStart);

		bool bFind = false;
		while (-1 != nFind)
		{
			bFind = true;
			CStringW strFind = strBuf.Mid(nStart, nFind-nStart);
			if (!strFind.IsEmpty())
			{
				AddMacroItem(strFind);
			}

			nStart = nFind;
			nFind = strBuf.Find(L"#define", nStart+1);
		}

		if (bFind)//说明已存在#define
		{
			CStringW strFind = strBuf.Mid(nStart, strBuf.GetLength()-nStart);
			if (!strFind.IsEmpty())
			{
				AddMacroItem(strFind);
			}
		}

		bRet = true;
	} while (false);
	return bRet;
}

CStringW CMacroHelper::Convert(CStringW strMacro)
{
	CStringWList strList;
	int nCount = SplitStringT(strMacro,L'\n',strList);
	for (int i=0; i<nCount; i++)
	{
		CStringW strTemp = strList[i];
		strTemp.Trim();
		strTemp.Replace(L" ",L"");
		int nLeft = strTemp.FindChar(L'(');// 找到对应前括号
		int nRight = strTemp.FindChar(L')');// 找到对应后括号
		if (-1 != nLeft&&-1 != nRight)
		{
			CStringW strMainDef = strTemp.Left(nLeft);
			CStringW strDot = strTemp.Mid(nLeft+1,nRight-nLeft-1);// 取得括号中的内容
			int nDotCount = 0;
			CStringWList strDotList;
			TermSpace(strMainDef);
			TermSpace(strDot);
			if (!strDot.IsEmpty())
			{
				nDotCount = SplitStringT(strDot,L',',strDotList);// 将括号中的内容按逗号分离
			}
			PMACRO pst = FindMacroItem(strMainDef,nDotCount);
			if (pst)
			{
				strTemp = pst->strContent;
				for (int k=0; k<nDotCount; k++)
				{
					strTemp.Replace(pst->strParamList[k],strDotList[k]);
				}
				strList[i] = strTemp;// 转换后，赋值 
			}
		}
	}
	
	CStringW strOut;
	for (int i=0; i<nCount; i++)
	{
		strOut += strList[i];
		strOut += L'\n';
	}

	CStringW szElse = L"else";
	CStringW szIf = L"if";
	int nElseFind = strOut.Find(szElse);
	while (-1!=nElseFind)
	{
		int nIf = strOut.Find(szIf, nElseFind);
		if (-1 == nIf)
		{
			break;
		}

		// 判断else if中间是不是只有空格
		CStringW strContent = strOut.Mid(nElseFind+4,nIf-nElseFind-4);
		wchar_t szChar = 0x09;
		strContent.Remove(szChar);
		strContent.Trim();
		strContent.Replace(L"\r\n",L'');
		strContent.Replace(L"\n",L'');
		strContent.Replace(L"\r",L'');
		if (strContent.IsEmpty())
		{
			CStringW szReplace = strOut.Mid(nElseFind,nIf-nElseFind+2);
			strOut.Replace(szReplace,L"else if");
		}
		nElseFind = strOut.Find(szElse, nElseFind+4);
	}

	return strOut;
}

bool CMacroHelper::AddMacroItem(CStringW strFind)
{
	bool bRet = false;
	do 
	{
		if (strFind.IsEmpty())
		{
			break;
		}

		int nLineFind = strFind.FindChar('\\');//找到Def后的连行符

		PMACRO pst = new MACRO;
		pst->strDef = strFind.Left(nLineFind);
		pst->strContent = strFind.Right(strFind.GetLength()-nLineFind-1);
		
		pst->strDef.Replace(L"#define",L"");
		pst->strDef.Remove('\\');
		pst->strContent.Remove('\\');
		TermSpace(pst->strDef);
		pst->strContent.Trim();

		int nLeft = pst->strDef.FindChar(L'(');// 找到对应前括号
		int nRight = pst->strDef.FindChar(L')');// 找到对应后括号
		if (-1!=nLeft && -1!=nRight)
		{
			pst->strMainDef = pst->strDef.Left(nLeft);
			TermSpace(pst->strMainDef);
			pst->strDot = pst->strDef.Mid(nLeft+1,nRight-nLeft-1);// 取得括号中的内容
			TermSpace(pst->strDot);
			if (!pst->strDot.IsEmpty())
			{
				SplitStringT(pst->strDot,L',',pst->strParamList);// 将括号中的内容按逗号分离
			}

			m_VecMacro.push_back(pst);
		}
		
		bRet = true;
	} while (false);
	return bRet;

}

PMACRO CMacroHelper::FindMacroItem(CStringW strMainDef,int nParamCount)
{
	PMACRO pst = NULL;
	do 
	{
		if (strMainDef.IsEmpty())
		{
			break;
		}

		int nCount = (int)m_VecMacro.size();
		if (0 == nCount)
		{
			break;
		}

		for (int i=0; i<nCount; i++)
		{
			CStringW strTempMainDef = m_VecMacro[i]->strMainDef;
			if (0 == strMainDef.CompareNoCase(strTempMainDef))
			{
				if (nParamCount == m_VecMacro[i]->strParamList.GetCount())
				{
					pst = m_VecMacro[i];
					break;
				}
			}
		}
	} while (false);
	return pst;
}

void CMacroHelper::TermSpace(CStringW& str)
{
	str.Trim();
	str.Replace(L" ",L"");
	str.Replace(L"	",L"");

}
