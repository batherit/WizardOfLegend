#include "stdafx.h"
#include "CAtlasLoader.h"


CAtlasLoader::CAtlasLoader(int _iID, _atlas_loader_info& _stAtlasInfo)
{
	m_iID = _iID;
	m_stAtlasInfo = _stAtlasInfo;

	TCHAR szConvDir[256] = TEXT(""); // �ʱ�ȭ �����ָ� ��ȯ�� ����� �ȵ�.
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, 
		_stAtlasInfo.szAtlasFileDirectory, strlen(_stAtlasInfo.szAtlasFileDirectory), szConvDir, 256);
	m_bitmapAtlas = (HBITMAP)LoadImage(NULL, szConvDir, 0, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);

	m_iStretchedAtlasWidth = _stAtlasInfo.fAtlasRatio * _stAtlasInfo.iAtlasWidth;
	m_iStretchedAtlasHeight = _stAtlasInfo.fAtlasRatio * _stAtlasInfo.iAtlasHeight;

	switch (_stAtlasInfo.eLoaderType)
	{
	case ATLAS_LOADER::TYPE_UNIFORM:
	{
		int iRow = _stAtlasInfo.iAtlasHeight / _stAtlasInfo.iTileHeight;
		int iCol = _stAtlasInfo.iAtlasWidth / _stAtlasInfo.iTileWidth;

		// ��� ���� ����.
		_atlas_obj_info stAtlasObjInfo;
		for (int i = 0; i < iRow; i++) {
			for (int j = 0; j < iCol; j++) {
				stAtlasObjInfo.iAtlasID = _iID;
				stAtlasObjInfo.rcOutputArea.left = j * _stAtlasInfo.iTileWidth;
				stAtlasObjInfo.rcOutputArea.top = i * _stAtlasInfo.iTileHeight;
				stAtlasObjInfo.rcOutputArea.right = (j + 1) * _stAtlasInfo.iTileWidth;
				stAtlasObjInfo.rcOutputArea.bottom = (i + 1) * _stAtlasInfo.iTileHeight;
				stAtlasObjInfo.iCoveredHeightTiles = 1;
				stAtlasObjInfo.iCoveredWidthTiles = 1;

				m_vecAtlasObjInfos.emplace_back(stAtlasObjInfo);
			}
		}
	}
		break;
	case ATLAS_LOADER::TYPE_NON_UNIFORM:
		// TODO : 
		break;
	default:
		break;
	}
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
	LONG lStartX;
	LONG lStartY;
	LONG lWidth;
	LONG lHeight;

	for (auto& info : m_vecAtlasObjInfos) {
		lStartX = info.rcOutputArea.left * m_stAtlasInfo.fAtlasRatio;
		lStartY = info.rcOutputArea.top * m_stAtlasInfo.fAtlasRatio;
		lWidth = (info.rcOutputArea.right - info.rcOutputArea.left) * m_stAtlasInfo.fAtlasRatio;
		lHeight = (info.rcOutputArea.bottom - info.rcOutputArea.top) * m_stAtlasInfo.fAtlasRatio;
		
		// �ð�������� �׸� �׸���.
		MoveToEx(_hdc, lStartX, lStartY, nullptr);
		LineTo(_hdc, lStartX + lWidth, lStartY);
		LineTo(_hdc, lStartX + lWidth, lStartY + lHeight);
		LineTo(_hdc, lStartX, lStartY + lHeight);
		LineTo(_hdc, lStartX, lStartY);
	}
}

_atlas_obj_info CAtlasLoader::GetDetectedTileRowCol(const POINT& _ptClicked)
{
	_atlas_obj_info stTileInfo;
	RECT rcStretched;

	if (IsVisible()) {
		for (auto& info : m_vecAtlasObjInfos) {
			rcStretched.left = info.rcOutputArea.left * m_stAtlasInfo.fAtlasRatio;
			rcStretched.top = info.rcOutputArea.top * m_stAtlasInfo.fAtlasRatio;
			rcStretched.right = info.rcOutputArea.right * m_stAtlasInfo.fAtlasRatio;
			rcStretched.bottom = info.rcOutputArea.bottom * m_stAtlasInfo.fAtlasRatio;
			if (IsPointInRect(rcStretched, _ptClicked)) {
				return info;
			}
		}
	}
	
	return stTileInfo;
}
