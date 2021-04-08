// ValueDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageColor.h"
#include "ValueDlg.h"
#include "afxdialogex.h"


// CValueDlg 대화 상자

IMPLEMENT_DYNAMIC(CValueDlg, CDialog)

CValueDlg::CValueDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_contant1(0)
{

}

CValueDlg::~CValueDlg()
{
}

void CValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_contant1);
}


BEGIN_MESSAGE_MAP(CValueDlg, CDialog)
END_MESSAGE_MAP()


// CValueDlg 메시지 처리기
