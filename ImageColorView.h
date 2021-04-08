
// ImageColorView.h: CImageColorView 클래스의 인터페이스
//

#pragma once


class CImageColorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CImageColorView() noexcept;
	DECLARE_DYNCREATE(CImageColorView)

// 특성입니다.
public:
	CImageColorDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CImageColorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEqualImage();
	afx_msg void OnAddImage();
	afx_msg void OnReverseImage();
	afx_msg void OnBwImage();
	afx_msg void OnSizeupImage();
	afx_msg void OnSizedownImage();
	afx_msg void OnRotateImage();
	afx_msg void OnEmbossImage();
	afx_msg void OnLpfImage();
	afx_msg void OnHpfImage();
	afx_msg void OnEroImage();
	afx_msg void OnDilationImage();
};

#ifndef _DEBUG  // ImageColorView.cpp의 디버그 버전
inline CImageColorDoc* CImageColorView::GetDocument() const
   { return reinterpret_cast<CImageColorDoc*>(m_pDocument); }
#endif

