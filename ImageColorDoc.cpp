
// ImageColorDoc.cpp: CImageColorDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageColor.h"
#endif

#include "ImageColorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageColorDoc

IMPLEMENT_DYNCREATE(CImageColorDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageColorDoc, CDocument)
END_MESSAGE_MAP()


// CImageColorDoc 생성/소멸

CImageColorDoc::CImageColorDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImageColorDoc::~CImageColorDoc()
{
}

BOOL CImageColorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CImageColorDoc serialization

void CImageColorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageColorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageColorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageColorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageColorDoc 진단

#ifdef _DEBUG
void CImageColorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageColorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageColorDoc 명령
unsigned char** malloc2D(int h, int w) {
	unsigned char** p;
	p = (unsigned char**)malloc(h * sizeof(unsigned char*));
	for (int i = 0; i < h; i++)
		p[i] = (unsigned char*)malloc(w * sizeof(unsigned char));
	return p;
}
void free2DImage(unsigned char** image, int h) {
	if (image == NULL)
		return;
	for (int i = 0; i < h; i++)
		free(image[i]);
	free(image);
	image = NULL;
}

BOOL CImageColorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	// 입력 메모리 해제
	free2DImage(inImageR, inH);
	free2DImage(inImageG, inH);
	free2DImage(inImageB, inH);

	CImage bitmap; // 칼라 이미지 클래스(Good Luck~~)
	bitmap.Load(lpszPathName);
	// 중요! 입력 메모리 크기.
	inH = bitmap.GetHeight();
	inW = bitmap.GetWidth();
	// 입력 메모리 할당
	inImageR = malloc2D(inH, inW);
	inImageG = malloc2D(inH, inW);
	inImageB = malloc2D(inH, inW);
	COLORREF pixel; // 한 점(R,G,B)
	for (int i=0; i<inH; i++)
		for (int k = 0; k < inW; k++) {
			pixel = bitmap.GetPixel(k, i);
			inImageR[i][k] = (unsigned char)GetRValue(pixel);
			inImageG[i][k] = (unsigned char)GetGValue(pixel);
			inImageB[i][k] = (unsigned char)GetBValue(pixel);
		}
	return TRUE;
}


void CImageColorDoc::OnEqualImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	// ** 진짜 영상처리 알고리즘 **//
	for(int i=0; i<inH; i++)
		for (int k = 0; k < inW; k++) {
			outImageR[i][k] = inImageR[i][k];
			outImageG[i][k] = inImageG[i][k];
			outImageB[i][k] = inImageB[i][k];
		}


}

void CImageColorDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	free2DImage(inImageR, inH);
	free2DImage(inImageG, inH);
	free2DImage(inImageB, inH);

	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);

	CDocument::OnCloseDocument();
}

#include "ValueDlg.h"
void CImageColorDoc::OnAddImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	// ** 진짜 영상처리 알고리즘 **//
	CValueDlg  dlg;
	int value=0;
	if (dlg.DoModal() == IDOK)
		value = (int) dlg.m_contant1;

	int r, g, b;
	int px;
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++) {
			r = inImageR[i][k];
			g = inImageG[i][k];
			b = inImageB[i][k];

			if (r + value > 255)
				px = 255;
			else if (r + value < 0)
				px = 0;
			else
				px = r + value;
			outImageR[i][k] = px;

			if (g + value > 255)
				px = 255;
			else if (g + value < 0)
				px = 0;
			else
				px = g + value;
			outImageG[i][k] = px;

			if (b + value > 255)
				px = 255;
			else if (b + value < 0)
				px = 0;
			else
				px = b + value;
			outImageB[i][k] = px;
		}

}


void CImageColorDoc::OnReverseImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++) {
			outImageR[i][k] = (unsigned char) 255 - inImageR[i][k];
			outImageG[i][k] = (unsigned char) 255 - inImageG[i][k];
			outImageB[i][k] = (unsigned char) 255 - inImageB[i][k];
		}
}


void CImageColorDoc::OnBwImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	for (int i = 0; i < inH; i++)
		for (int k = 0; k < inW; k++) {
			if (inImageR[i][k] > 127) {
				outImageR[i][k] = 255;
			}
			else {
				outImageR[i][k] = 0;
			}
			if (inImageG[i][k] > 127) {
				outImageG[i][k] = 255;
			}
			else {
				outImageG[i][k] = 0;
			}
			if (inImageB[i][k] > 127) {
				outImageB[i][k] = 255;
			}
			else {
				outImageB[i][k] = 0;
			}
		}
}


void CImageColorDoc::OnSizeupImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	int mul = 2;
	outH = inH * mul;  outW = inW * mul;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	// ** 진짜 영상처리 알고리즘 **//
	for (int i = 0; i < outH; i++) {
		for (int k = 0; k < outW; k++) {
			outImageR[i][k] = inImageR[i / mul][k / mul];
			outImageG[i][k] = inImageG[i / mul][k / mul];
			outImageB[i][k] = inImageB[i / mul][k / mul];
		}
	}
}


void CImageColorDoc::OnSizedownImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	int div = 2;
	outH = inH / div;  outW = inW / div;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	// ** 진짜 영상처리 알고리즘 **//
	int sumR = 0, sumG = 0, sumB = 0;
	for (int i = 0; i < outH; i++) {
		for (int j = 0; j < outW; j++) {
			sumR = 0, sumG = 0, sumB = 0;
			for (int k = 0; k < div; k++) {
				for (int m = 0; m < div; m++) {
					sumR = sumR + inImageR[i * div + k][j * div + m];
					sumG = sumG + inImageG[i * div + k][j * div + m];
					sumB = sumB + inImageB[i * div + k][j * div + m];
				}
			}
			outImageR[i][j] = (unsigned char)(sumR / (float)(div * div));
			outImageG[i][j] = (unsigned char)(sumG / (float)(div * div));
			outImageB[i][j] = (unsigned char)(sumB / (float)(div * div));
		}
	}
}


void CImageColorDoc::OnRotateImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	int degree = 222;
	int center_w = inW / 2;//중심축 계산
	int center_h = inH / 2;
	int new_w;
	int new_h;
	double pi = 3.141592;
	// -degree 반시계 방향 회전
	// degree 시계 방향 회전
	double seta = pi / (180.0 / degree);
	//회전 알고리즘
	for (int i = 0; i < inH; i++) {
		for (int j = 0; j < inW; j++) {
			new_w = (i - center_h) * sin(seta) + (j - center_w) * cos(seta) + center_w;
			new_h = (i - center_h) * cos(seta) - (j - center_w) * sin(seta) + center_h;
			if (new_w < 0)		continue;
			if (new_w >= inW)	continue;
			if (new_h < 0)		continue;
			if (new_h >= inH)	continue;
			outImageR[i][j] = inImageR[new_h][new_w];
			outImageG[i][j] = inImageG[new_h][new_w];
			outImageB[i][j] = inImageB[new_h][new_w];
		}
	}
}


void CImageColorDoc::OnEmbossImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	// 평균 값 계산
	long avgR = 0, avgG = 0, avgB = 0;
	for (int i = 0; i < inH; i++) {
		for (int j = 0; j < inW; j++) {
			avgR += inImageR[i][j];
			avgG += inImageG[i][j];
			avgB += inImageB[i][j];
		}
	}
	avgR = avgR / (inH * inW);
	avgG = avgG / (inH * inW);
	avgB = avgB / (inH * inW);

	int mask[3][3] = { {-1, 0, 0},
		{0, 0, 0},
		{0, 0, 1},
	};
	long sumR = 0, sumG = 0, sumB = 0;
	for (int i = 1; i < inH - 1; i++) {
		for (int j = 1; j < inW - 1; j++) {
			sumR = 0, sumG = 0, sumB = 0;
			for (int k = 0; k < 3; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					sumR += inImageR[i - 1 + k][j - 1 + m] * mask[k][m];
					sumG += inImageG[i - 1 + k][j - 1 + m] * mask[k][m];
					sumB += inImageB[i - 1 + k][j - 1 + m] * mask[k][m];
				}
			}
			sumR += avgR, sumG += avgG, sumB += avgB;
			if (sumR > 255) {
				sumR = 255;
			}
			else if (sumR < 0) {
				sumR = 0;
			}
			if (sumG > 255) {
				sumG = 255;
			}
			else if (sumG < 0) {
				sumG = 0;
			}
			if (sumB > 255) {
				sumB = 255;
			}
			else if (sumB < 0) {
				sumB = 0;
			}
			outImageR[i][j] = (unsigned char)sumR;
			outImageG[i][j] = (unsigned char)sumG;
			outImageB[i][j] = (unsigned char)sumB;
		}
	}
}


void CImageColorDoc::OnLpfImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	long sumR = 0, sumG = 0, sumB = 0;
	for (int i = 1; i < inH - 1; i++) {
		for (int j = 1; j < inW - 1; j++) {
			sumR = 0, sumG = 0, sumB = 0;
			for (int k = 0; k < 3; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					sumR += inImageR[i - 1 + k][j - 1 + m];
					sumG += inImageG[i - 1 + k][j - 1 + m];
					sumB += inImageB[i - 1 + k][j - 1 + m];
				}
			}
			sumR /= 9, sumG /= 9, sumB /= 9;
			if (sumR > 255) {
				sumR = 255;
			}
			else if (sumR < 0) {
				sumR = 0;
			}
			if (sumG > 255) {
				sumG = 255;
			}
			else if (sumG < 0) {
				sumG = 0;
			}
			if (sumB > 255) {
				sumB = 255;
			}
			else if (sumB < 0) {
				sumB = 0;
			}
			outImageR[i][j] = (unsigned char)sumR;
			outImageG[i][j] = (unsigned char)sumG;
			outImageB[i][j] = (unsigned char)sumB;
		}
	}
}


void CImageColorDoc::OnHpfImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	int mask[3][3] = { {-1, -1, -1},
		{-1, 8, -1},
		{-1, -1, -1}
	};
	long sumR = 0, sumG = 0, sumB = 0;
	for (int i = 1; i < inH - 1; i++) {
		for (int j = 1; j < inW - 1; j++) {
			sumR = 0, sumG = 0, sumB = 0;
			for (int k = 0; k < 3; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					sumR += inImageR[i - 1 + k][j - 1 + m] * mask[k][m];
					sumG += inImageG[i - 1 + k][j - 1 + m] * mask[k][m];
					sumB += inImageB[i - 1 + k][j - 1 + m] * mask[k][m];
				}
			}
			if (sumR > 255) {
				sumR = 255;
			}
			else if (sumR < 0) {
				sumR = 0;
			}
			if (sumG > 255) {
				sumG = 255;
			}
			else if (sumG < 0) {
				sumG = 0;
			}
			if (sumB > 255) {
				sumB = 255;
			}
			else if (sumB < 0) {
				sumB = 0;
			}
			outImageR[i][j] = (unsigned char)sumR;
			outImageG[i][j] = (unsigned char)sumG;
			outImageB[i][j] = (unsigned char)sumB;
		}
	}
}


void CImageColorDoc::OnEroImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	int mask[3][3] = { {-1, -1, -1},
		{-1, 8, -1},
		{-1, -1, -1}
	};
	unsigned char minR = 255, minG = 255, minB = 255;
	for (int i = 1; i < inH - 1; i++) {
		for (int j = 1; j < inW - 1; j++) {
			minR = 255, minG = 255, minB = 255;
			for (int k = 0; k < 3; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (minR > inImageR[i - 1 + k][j - 1 + m]) {
						minR = inImageR[i - 1 + k][j - 1 + m];
					}
					if (minG > inImageG[i - 1 + k][j - 1 + m]) {
						minG = inImageG[i - 1 + k][j - 1 + m];
					}
					if (minB > inImageB[i - 1 + k][j - 1 + m]) {
						minB = inImageB[i - 1 + k][j - 1 + m];
					}
				}
			}
			outImageR[i][j] = minR;
			outImageG[i][j] = minG;
			outImageB[i][j] = minB;
		}
	}
}


void CImageColorDoc::OnDilationImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (inImageR == NULL)
		return;
	// 기존에 처리한 적이 있으면 일단 메모리 해제
	free2DImage(outImageR, outH);
	free2DImage(outImageG, outH);
	free2DImage(outImageB, outH);
	// 중요! 출력 영상 크기 --> 알고리즘에 따름
	outH = inH;  outW = inW;
	// 출력 메모리 할당.
	outImageR = malloc2D(outH, outW);
	outImageG = malloc2D(outH, outW);
	outImageB = malloc2D(outH, outW);
	int mask[3][3] = { {-1, -1, -1},
		{-1, 8, -1},
		{-1, -1, -1}
	};
	unsigned char maxR = 0, maxG = 0, maxB = 0;
	for (int i = 1; i < inH - 1; i++) {
		for (int j = 1; j < inW - 1; j++) {
			maxR = 0, maxG = 0, maxB = 0;
			for (int k = 0; k < 3; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (maxR < inImageR[i - 1 + k][j - 1 + m]) {
						maxR = inImageR[i - 1 + k][j - 1 + m];
					}
					if (maxG < inImageG[i - 1 + k][j - 1 + m]) {
						maxG = inImageG[i - 1 + k][j - 1 + m];
					}
					if (maxB < inImageB[i - 1 + k][j - 1 + m]) {
						maxB = inImageB[i - 1 + k][j - 1 + m];
					}
				}
			}
			outImageR[i][j] = maxR;
			outImageG[i][j] = maxG;
			outImageB[i][j] = maxB;
		}
	}
}
