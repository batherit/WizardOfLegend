#include "stdafx.h"
#include "CBitmapObj.h"


CBitmapObj::CBitmapObj()
{
}


CBitmapObj::~CBitmapObj()
{
	Release();
}

void CBitmapObj::LoadBmp(const TCHAR * pFilePath)
{
	HDC hDC = GetDC(g_hWND);
	// 도화지 준비. 
	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWND, hDC);
	// 여기는 물감 준비. 
	m_hBitmap = (HBITMAP)LoadImage(nullptr, pFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// 여기가 그림을 그림. 
	m_OldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	// Bitmap의 속성을 얻는다.
	BITMAP bmp;
	HBITMAP  hBitmap;

	::GetObject(m_hBitmap, sizeof(BITMAP), (LPVOID)&bmp);
	m_lWidth = bmp.bmWidth;  // 그림의 폭
	m_lHeight = bmp.bmHeight; // 그림의 넓이
}

void CBitmapObj::Release()
{
	// 선택되어 있는 GDI 오브젝트는 지울수 있는 방법이 없음. 
	// 그래서 원래 있던놈으로 끼워 집어 넣고 내가 만든걸 지워 줘야함. 
	// 그래서 OldBitmap이라는놈까지 만든거. 
	SelectObject(m_hMemDC, m_OldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
