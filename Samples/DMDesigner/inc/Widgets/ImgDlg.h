// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ImgDlg.h
// File mark:   
// File summary:新建或编辑img,支持多个img组合
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-22
// ----------------------------------------------------------------
#pragma once

class ImgDlg : public DMHDialog
{
public:
	ImgDlg(bool bEditMode = false);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP()

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);
	void OnDropFiles(HDROP hDropInfo);
 
	DMCode OnCheckChange(DMEventArgs *pEvt);
	DMCode OnEditChange(DMEventArgs *pEvt);
	DMCode OnSaveBtn();
	DMCode OnClearBtn();
	DMCode OnOpenDir(); 

	DMCode UpdateXml();

	static int CALLBACK  BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData);
 
protected:
	DMCode OnOK();

private:  
	bool							m_bEditMode;
	DUIStatic*                      m_pTitle;
	DUIStatic*                      m_pImgPath;
	DUImgEditor*                    m_pImgEditor;

	DUIEdit*                        m_pImgName;
	DUIEdit*                        m_pImgDir;
	DUIEdit*                        m_pImgFileName;
	DUIEdit*                        m_pImgFileAfx;
	DUIRichEdit*                    m_pXml;

	// 多图拼接使用
	DUICheckBox*					m_pbMultImg;
	DUICheckBox*					m_pbVert;
	DUIButton*                      m_pSave;
	DUIButton*                      m_pClean;

	// xml
	DMXmlDocument                   m_ImgDoc;
	DMXmlNode                       m_ImgNode;

	CStringW                        m_strDirName;///< 主题包文件夹名如"themes\\theme0"
	CStringW                        m_strDir;    ///< 主题包所在的路径如"e:\\themes\\theme0\\"
	CStringW                        m_strTitle;  ///< 
	static  ImgDlg*                 ms_pthis; 	
};