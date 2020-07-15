#pragma once
class CBitmapObj
{
public:
	CBitmapObj();
	~CBitmapObj();
public:
	HDC GetMemDC() { return m_hMemDC; }
	HBITMAP GetBitmap() { return m_hBitmap; }
	POINT GetWidthHeight(void) const { return POINT{ m_lWidth, m_lHeight }; }
	LONG GetWitdh(void) const { return m_lWidth; }
	LONG GetHeight(void) const { return m_lHeight; }
	void LoadBmp(const TCHAR* pFilePath);
	void Release();
private:
	HDC m_hMemDC;
	HBITMAP m_OldBitmap;
	HBITMAP m_hBitmap;

	LONG m_lWidth;
	LONG m_lHeight;
};

