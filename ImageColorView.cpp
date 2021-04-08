
// ImageColorView.cpp: CImageColorView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageColor.h"
#endif

#include "ImageColorDoc.h"
#include "ImageColorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageColorView

IMPLEMENT_DYNCREATE(CImageColorView, CView)

BEGIN_MESSAGE_MAP(CImageColorView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_EQUAL_IMAGE, &CImageColorView::OnEqualImage)
	ON_COMMAND(IDM_ADD_IMAGE, &CImageColorView::OnAddImage)
	ON_COMMAND(IDM_REVERSE_IMAGE, &CImageColorView::OnReverseImage)
	ON_COMMAND(IDM_BW_IMAGE, &CImageColorView::OnBwImage)
	ON_COMMAND(IDM_SIZEUP_IMAGE, &CImageColorView::OnSizeupImage)
	ON_COMMAND(IDM_SIZEDOWN_IMAGE, &CImageColorView::OnSizedownImage)
	ON_COMMAND(IDM_ROTATE_IMAGE, &CImageColorView::OnRotateImage)
	ON_COMMAND(IDM_EMBOSS_IMAGE, &CImageColorView::OnEmbossImage)
	ON_COMMAND(IDM_LPF_IMAGE, &CImageColorView::OnLpfImage)
	ON_COMMAND(IDM_HPF_IMAGE, &CImageColorView::OnHpfImage)
	ON_COMMAND(IDM_ERO_IMAGE, &CImageColorView::OnEroImage)
	ON_COMMAND(IDM_DILATION_IMAGE, &CImageColorView::OnDilationImage)
END_MESSAGE_MAP()

// CImageColorView 생성/소멸

CImageColorView::CImageColorView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CImageColorView::~CImageColorView()
{
}

BOOL CImageColorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CImageColorView 그리기

void CImageColorView::OnDraw(CDC* pDC)
{
	//CImageColorDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	//// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	//unsigned char R, G, B;
	//for (int i = 0; i < pDoc->inH; i++)
	//	for (int k = 0; k < pDoc->inW; k++) {
	//		R = pDoc->inImageR[i][k];
	//		G = pDoc->inImageG[i][k];
	//		B = pDoc->inImageB[i][k];
	//		pDC->SetPixel(k + 5, i + 5, RGB(R, G, B));
	//	}
	//for (int i = 0; i < pDoc->outH; i++)
	//	for (int k = 0; k < pDoc->outW; k++) {
	//		R = pDoc->outImageR[i][k];
	//		G = pDoc->outImageG[i][k];
	//		B = pDoc->outImageB[i][k];
	//		pDC->SetPixel(k + 5 + pDoc->inW + 5, i + 5, RGB(R, G, B));
	//	}

	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에원시데이터에대한그리기코드를추가합니다.
	int i, k;
	unsigned char R, G, B;

	/// 성능 개선을 위한 더블 버퍼링 
	////////////////////
	// 메모리 DC 선언
	CDC memDC;
	CBitmap* pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->inW, pDoc->inH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->inW, pDoc->inH, WHITENESS); // 흰색으로 초기화

	// 메모리 DC에 그리기
	for (i = 0; i < pDoc->inH; i++) {
		for (k = 0; k < pDoc->inW; k++) {
			R = pDoc->inImageR[i][k];
			G = pDoc->inImageG[i][k];
			B = pDoc->inImageB[i][k];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(5, 5, pDoc->inW, pDoc->inH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	///////////////////

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->outW, pDoc->outH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->outW, pDoc->outH, WHITENESS); // 흰색으로 초기화

	// 메모리 DC에 그리기
	for (i = 0; i < pDoc->outH; i++) {
		for (k = 0; k < pDoc->outW; k++) {
			R = pDoc->outImageR[i][k];
			G = pDoc->outImageG[i][k];
			B = pDoc->outImageB[i][k];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(pDoc->inW + 10, 5, pDoc->outW, pDoc->outH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

}


// CImageColorView 인쇄

BOOL CImageColorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageColorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageColorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CImageColorView 진단

#ifdef _DEBUG
void CImageColorView::AssertValid() const
{
	CView::AssertValid();
}

void CImageColorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageColorDoc* CImageColorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageColorDoc)));
	return (CImageColorDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageColorView 메시지 처리기


void CImageColorView::OnEqualImage()
{
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnEqualImage();
	Invalidate(TRUE);
}


void CImageColorView::OnAddImage()
{
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnAddImage();
	Invalidate(TRUE);
}


void CImageColorView::OnReverseImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnReverseImage();
	Invalidate(TRUE);
}


void CImageColorView::OnBwImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnBwImage();
	Invalidate(TRUE);
}


void CImageColorView::OnSizeupImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnSizeupImage();
	Invalidate(TRUE);
}


void CImageColorView::OnSizedownImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnSizedownImage();
	Invalidate(TRUE);
}


void CImageColorView::OnRotateImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnRotateImage();
	Invalidate(TRUE);
}


void CImageColorView::OnEmbossImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnEmbossImage();
	Invalidate(TRUE);
}


void CImageColorView::OnLpfImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnLpfImage();
	Invalidate(TRUE);
}


void CImageColorView::OnHpfImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnHpfImage();
	Invalidate(TRUE);
}


void CImageColorView::OnEroImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnEroImage();
	Invalidate(TRUE);
}


void CImageColorView::OnDilationImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageColorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	pDoc->OnDilationImage();
	Invalidate(TRUE);
}
