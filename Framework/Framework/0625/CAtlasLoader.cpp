#include "stdafx.h"
#include "CAtlasLoader.h"


CAtlasLoader::CAtlasLoader(_atlas_info& _stAtlasInfo)
{
	TCHAR szConvDir[256] = L""; // 초기화 안해주면 변환이 제대로 안됨.

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, 
		_stAtlasInfo.szAtlasFileDirectory, strlen(_stAtlasInfo.szAtlasFileDirectory), szConvDir, 256);

	m_bitmapAtlas = (HBITMAP)LoadImage(NULL, szConvDir, 0, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
}


CAtlasLoader::~CAtlasLoader()
{
}

void CAtlasLoader::Update(float _fDeltaTime)
{
}

void CAtlasLoader::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	HDC memdc = CreateCompatibleDC(_hdc);
	m_bitmapOldAtlas = (HBITMAP)SelectObject(memdc, m_bitmapAtlas);
	BitBlt(_hdc, 0, 0, 192, 132, memdc, 0, 0, SRCCOPY);
	DeleteDC(memdc);
}
