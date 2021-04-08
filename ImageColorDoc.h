
// ImageColorDoc.h: CImageColorDoc 클래스의 인터페이스
//


#pragma once


class CImageColorDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CImageColorDoc() noexcept;
	DECLARE_DYNCREATE(CImageColorDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CImageColorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char** inImageR = NULL;
	unsigned char** inImageG = NULL;
	unsigned char** inImageB = NULL;
	unsigned char** outImageR = NULL;
	unsigned char** outImageG = NULL;
	unsigned char** outImageB = NULL;
	int inH = 0;
	int inW = 0;
	int outH = 0;
	int outW = 0;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void OnEqualImage();
	virtual void OnCloseDocument();
	void OnAddImage();
	void OnReverseImage();
	void OnBwImage();
	void OnSizeupImage();
	void OnSizedownImage();
	void OnRotateImage();
	void OnEmbossImage();
	void OnLpfImage();
	void OnHpfImage();
	void OnEroImage();
	void OnDilationImage();
};
