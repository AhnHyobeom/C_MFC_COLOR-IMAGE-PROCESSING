﻿#pragma once


// CValueDlg 대화 상자

class CValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CValueDlg)

public:
	CValueDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CValueDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	double m_contant1;
};
