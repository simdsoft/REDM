
#include "stdafx.h"
#include "MacroTool.h"
#include "MacroToolDlg.h"
#include "MacroHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMacroToolDlg::CMacroToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMacroToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMacroToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MACRO_EDIT, m_MacroEdit);
	DDX_Control(pDX, IDC_CONVERT_EDIT, m_CovertEdit);
	DDX_Control(pDX, IDC_CONVERT_BTN, m_CovertBtn);
	DDX_Control(pDX, IDC_DelNote, m_Check);
}

BEGIN_MESSAGE_MAP(CMacroToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONVERT_BTN, &CMacroToolDlg::OnBnClickedConvertBtn)
END_MESSAGE_MAP()

BOOL CMacroToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);	
	// TODO

	return TRUE; 
}

void CMacroToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CMacroToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMacroToolDlg::OnBnClickedConvertBtn()
{
	CString szMacro; 
	m_MacroEdit.GetWindowText(szMacro);
	CString szCovert;
	m_CovertEdit.GetWindowText(szCovert);
	if (szCovert.IsEmpty()||szMacro.IsEmpty())
	{
		return;
	}

	CMacroHelper helper(szMacro, szCovert,m_Check.GetCheck());
	m_CovertEdit.SetWindowText(helper.Convert());

	
}
