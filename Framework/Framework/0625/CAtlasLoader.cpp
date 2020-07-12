#include "stdafx.h"
#include "CAtlasLoader.h"


CAtlasLoader::CAtlasLoader(_atlas_info& _stAtlasInfo)
{
	m_stAtlasInfo = _stAtlasInfo;


	TCHAR szConvDir[256] = L""; // 초기화 안해주면 변환이 제대로 안됨.
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, 
		_stAtlasInfo.szAtlasFileDirectory, strlen(_stAtlasInfo.szAtlasFileDirectory), szConvDir, 256);
	m_bitmapAtlas = (HBITMAP)LoadImage(NULL, szConvDir, 0, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);

	m_iRow = _stAtlasInfo.iAtlasHeight / _stAtlasInfo.iTileHeight;
	m_iCol = _stAtlasInfo.iAtlasWidth / _stAtlasInfo.iTileWidth;
	m_iStretchedAtlasWidth = _stAtlasInfo.fAtlasRatio * _stAtlasInfo.iAtlasWidth;
	m_iStretchedAtlasHeight = _stAtlasInfo.fAtlasRatio * _stAtlasInfo.iAtlasHeight;
	m_iStretchedTileWidth = _stAtlasInfo.fAtlasRatio * _stAtlasInfo.iTileWidth;
	m_iStretchedTileHeight = _stAtlasInfo.fAtlasRatio * _stAtlasInfo.iTileHeight;
}


CAtlasLoader::~CAtlasLoader()
{
}

void CAtlasLoader::Update(float _fDeltaTime)
{
	if (IsVisible()) {
		// TODO : 블록 선택 기능을 추가합니다.
	}
}

void CAtlasLoader::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (IsVisible()) {
		RenderAtlas(_hdc, _pCamera);
		RenderGrid(_hdc, _pCamera);
	}
}

void CAtlasLoader::RenderAtlas(HDC & _hdc, CCamera2D * _pCamera)
{
	HDC memdc = CreateCompatibleDC(_hdc);
	m_bitmapOldAtlas = (HBITMAP)SelectObject(memdc, m_bitmapAtlas);

	StretchBlt(_hdc,
		0,
		0,
		m_iStretchedAtlasWidth,
		m_iStretchedAtlasHeight,
		memdc, 0, 0, m_stAtlasInfo.iAtlasWidth, m_stAtlasInfo.iAtlasHeight, SRCCOPY);
	DeleteDC(memdc);
}

void CAtlasLoader::RenderGrid(HDC & _hdc, CCamera2D * _pCamera)
{
	// 가로줄 그리기
	for (int i = 0; i < m_iRow + 1; i++) {
		MoveToEx(_hdc, GetLeft(), GetTop() + GetTileHeight() * i, nullptr);
		LineTo(_hdc, GetRight(), GetTop() + GetTileHeight() * i);
	}

	// 세로줄 그리기
	for (int i = 0; i < m_iCol + 1; i++) {
		MoveToEx(_hdc, GetLeft() + GetTileWidth() * i, GetTop(), nullptr);
		LineTo(_hdc, GetLeft() + GetTileWidth() * i, GetBottom());
	}
}
