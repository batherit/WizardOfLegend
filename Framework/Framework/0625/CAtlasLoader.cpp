#include "stdafx.h"
#include "CAtlasLoader.h"


CAtlasLoader::CAtlasLoader(_atlas_info& _stAtlasInfo)
{
	m_stAtlasInfo = _stAtlasInfo;


	TCHAR szConvDir[256] = L""; // �ʱ�ȭ �����ָ� ��ȯ�� ����� �ȵ�.
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
		// TODO : ��� ���� ����� �߰��մϴ�.
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
		memdc, 
		0, 
		0, 
		m_stAtlasInfo.iAtlasWidth, 
		m_stAtlasInfo.iAtlasHeight, SRCCOPY);
	DeleteDC(memdc);
}

void CAtlasLoader::RenderGrid(HDC & _hdc, CCamera2D * _pCamera)
{
	// ������ �׸���
	for (int i = 0; i < m_iRow + 1; i++) {
		MoveToEx(_hdc, GetStretchedLeft(), GetStretchedTop() + GetStretchedTileHeight() * i, nullptr);
		LineTo(_hdc, GetStretchedRight(), GetStretchedTop() + GetStretchedTileHeight() * i);
	}

	// ������ �׸���
	for (int i = 0; i < m_iCol + 1; i++) {
		MoveToEx(_hdc, GetStretchedLeft() + GetStretchedTileWidth() * i, GetStretchedTop(), nullptr);
		LineTo(_hdc, GetStretchedLeft() + GetStretchedTileWidth() * i, GetStretchedBottom());
	}
}

pair<int, int> CAtlasLoader::GetDetectedTileRowCol(const POINT& _ptClicked)
{
	pair<int, int> pairRowCol(-1, -1);

	if (IsVisible()) {
		RECT rcStretchedTile;
		for (int i = 0; i < m_iRow; i++) {
			for (int j = 0; j < m_iCol; j++) {
				rcStretchedTile.left = m_iStretchedTileWidth * j;
				rcStretchedTile.top = m_iStretchedTileHeight * i;
				rcStretchedTile.right = m_iStretchedTileWidth * (j + 1);
				rcStretchedTile.bottom = m_iStretchedTileHeight * (i + 1);
				if (IsPointInRect(rcStretchedTile, _ptClicked)) {
					pairRowCol.first = i;
					pairRowCol.second = j;
					return pairRowCol;
				}
			}
		}
	}
	
	return pairRowCol;
}
