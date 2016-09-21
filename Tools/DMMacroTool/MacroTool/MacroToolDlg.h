
#pragma once
#include "afxwin.h"

class CMacroToolDlg : public CDialog
{
public:
	CMacroToolDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_MACROTOOL_DIALOG };
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	HICON				m_hIcon;

public:
	CEdit m_MacroEdit;
	CEdit m_CovertEdit;
	CButton m_CovertBtn;
	afx_msg void OnBnClickedConvertBtn();
	CButton m_Check;
};
