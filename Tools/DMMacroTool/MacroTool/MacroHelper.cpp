#include "StdAfx.h"
#include "MacroHelper.h"

CMacroHelper::CMacroHelper(CString szMacro, CString szContent, bool bDelMark)
{
	m_szContent = szContent;
	m_szMacro = szMacro;
	m_bDeleteMark = bDelMark;
}

CMacroHelper::~CMacroHelper(void)
{
}

/// @brief 这里至少要保证定义宏中有#define xxx()\，不然也不用解析了
/// @return 
void CMacroHelper::ParseMacro(vector<stMACRO>&VecMacro)
{
	int nStart = 0;
	int nFind = nStart = m_szMacro.Find(L"#define",nStart);
	bool bFind = false;
	while (nFind!=-1)
	{
		bFind = true;
		CString szFind = m_szMacro.Mid(nStart, nFind-nStart);
		if (!szFind.IsEmpty())
		{
			int nLineFind = szFind.Find('\\');//找到Def后的连行符
			stMACRO st;
			st.szDef = szFind.Left(nLineFind);
			st.szContent = szFind.Right(szFind.GetLength()-nLineFind-1);
			st.szDef.Remove('\\');
			st.szContent.Remove('\\');
			st.szDef.Trim();
			st.szContent.Trim();
			VecMacro.push_back(st);	
		}

		nStart = nFind;
		nFind = m_szMacro.Find(L"#define", nStart+1);
	}

	if (bFind)//说明已存在#define
	{
		CString szFind = m_szMacro.Mid(nStart, m_szMacro.GetLength()-nStart);
		if (!szFind.IsEmpty())
		{
			int nLineFind = szFind.Find('\\');
			stMACRO st;
			st.szDef = szFind.Left(nLineFind);
			st.szContent = szFind.Right(szFind.GetLength()-nLineFind-1);
			st.szDef.Remove('\\');
			st.szContent.Remove('\\');
			st.szDef.Trim();
			st.szContent.Trim();
			VecMacro.push_back(st);	
		}
	}
}

void CMacroHelper::ParseDot(vector<CString>&VecParam, CString sz)
{
	int nStart = 0;
	int nFind = sz.Find(L",",nStart);
	CString szFind;
	while (nFind!=-1)// 先查逗号
	{
		szFind = sz.Mid(nStart, nFind-nStart);

		szFind.Trim();
		if (!szFind.IsEmpty())
		{
			VecParam.push_back(szFind);	
		}
		nStart = nFind+1;
		nFind = sz.Find(',', nStart);
	}
	szFind = sz.Mid(nStart, sz.GetLength()-nStart);
	szFind.Trim();
	VecParam.push_back(szFind);	
}

void CMacroHelper::ModifyContent(vector<stMACRO>&VecMacro)
{
	int nCount = VecMacro.size();
	for (int i=0; i<nCount; i++)
	{
		stMACRO &item = VecMacro[i];
		ModifyContent(item);
	}
}

void CMacroHelper::ModifyContent(stMACRO &Item)
{
	// 取#define xxx()的xxx部分
	int nStart = Item.szDef.Find(L"#define");
	if (nStart!=-1)
	{
		nStart += 7;
	}
	int nEnd =Item.szDef.Find('(', nStart);
	if (nEnd == -1)
	{
		nEnd = Item.szDef.GetLength();
	}
	CString szDefMain = Item.szDef.Mid(nStart, nEnd-nStart);
	szDefMain.Trim();
	int nRightEnd =Item.szDef.ReverseFind(')');
	CString szDefOther = Item.szDef.Mid(nEnd+1,nRightEnd-nEnd-1);

	vector<CString>VecParamDest;
	if (!szDefOther.IsEmpty())
	{
		ParseDot(VecParamDest, szDefOther);
	}

	if (szDefOther.IsEmpty())
	{
		CString szReplace = szDefMain+L"()";
		m_szContent.Replace(szReplace, Item.szContent);
	}
	else
	{
		CString szContentBack = Item.szContent;
		int nFind = m_szContent.Find(szDefMain,0);
		while(nFind!=-1)
		{
			nEnd=  m_szContent.Find('(',nFind);//找到对应前括号
			nRightEnd = m_szContent.Find(')',nFind);//找到对应后括号
			CString szDot = m_szContent.Mid(nEnd+1,nRightEnd-nEnd-1);
			vector<CString>VecParamSrc;
			ParseDot(VecParamSrc, szDot);
			Item.szContent = szContentBack;// 如果循环，第二次Item.szContent是已被替换的了
			for (int i=0; i<VecParamSrc.size();i++)
			{
				Item.szContent.Replace(VecParamDest[i], VecParamSrc[i]);
			}
			CString szReplace = m_szContent.Mid(nFind, nRightEnd-nFind+1);
			m_szContent.Replace(szReplace, Item.szContent);
			nFind = m_szContent.Find(szDefMain,nRightEnd+1);
		}
	}
}

void CMacroHelper::DeleteMark()
{
	int nStart = 0;
	int nFind = m_szContent.Find(L"\r\n",nStart);
	CString szFind;
	while (nFind!=-1)// 先查逗号
	{
		szFind = m_szContent.Mid(nStart, nFind-nStart);
		szFind.Trim();
		if (!szFind.IsEmpty())
		{
			if (-1!=szFind.Find(L"//"))
			{
				m_szContent.Replace(szFind, L"");
			}
		}

		nStart = nFind+2;
		nFind = m_szContent.Find(L"\r\n", nStart);
	}

	szFind = m_szContent.Mid(nStart, m_szContent.GetLength()-nStart);
	if (!szFind.IsEmpty())
	{
		if (-1!=szFind.Find(L"//"))
		{
			m_szContent.Replace(szFind, L"");
		}
	}
}

CString CMacroHelper::Convert()
{
	vector<stMACRO> VecMacro;
	ParseMacro(VecMacro);
	ModifyContent(VecMacro);

	if (m_bDeleteMark)
	{
		DeleteMark();
	}

	// 恢复else if
	CString szElse = L"else";
	CString szIf = L"if";
	int nElseFind = m_szContent.Find(szElse);
	while (-1!=nElseFind)
	{
		int nIf = m_szContent.Find(szIf, nElseFind);
		if (-1 == nIf)
		{
			break;
		}

		// 判断else if中间是不是只有空格
		CString szContent = m_szContent.Mid(nElseFind+4,nIf-nElseFind-4);
		szContent.Trim();
		szContent.Replace(L"\r\n",L'');
		if (szContent.IsEmpty())
		{
			CString szReplace = m_szContent.Mid(nElseFind,nIf-nElseFind+2);
			m_szContent.Replace(szReplace,L"else if");
		}
		nElseFind = m_szContent.Find(szElse, nElseFind+4);
	}


	return m_szContent;
}
