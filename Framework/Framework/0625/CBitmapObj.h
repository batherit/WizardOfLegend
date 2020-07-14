#pragma once
class CBitmapObj
{
public:
	CBitmapObj();
	~CBitmapObj();
public:
	HDC GetMemDC() { return m_hMemDC; }
	void LoadBmp(const TCHAR* pFilePath);
	void Release();
private:
	HDC m_hMemDC;
	HBITMAP m_OldBitmap;
	HBITMAP m_hBitmap;
};

