
#pragma once
#include <vector>
using namespace std;

struct stMACRO
{
	CString szDef;
	CString szContent;
};

class CMacroHelper
{
public:
	CMacroHelper(CString szMacro, CString szContent, bool bDelMark);
	~CMacroHelper(void);

	void ParseMacro(vector<stMACRO>&VecMacro);
	void ParseDot(vector<CString>&VecParam, CString sz);
	void ModifyContent(vector<stMACRO>&VecMacro);
	void DeleteMark();
	void ModifyContent(stMACRO &Item);

	CString Convert();

protected:
	CString         m_szMacro;
	CString         m_szContent;
	bool            m_bDeleteMark;


};
