#include "stdafx.h"
#include "CAtlasLoader.h"
#include "CBitmapObj.h"
#include "CBitmapMgr.h"


CAtlasLoader::CAtlasLoader(int _iID, _atlas_loader_info& _stAtlasInfo)
{
	m_iID = _iID;
	m_stAtlasInfo = _stAtlasInfo;

	const CBitmapObj* pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(ctBitmapKey[_stAtlasInfo.iAtlasID]);
	m_iStretchedAtlasWidth = _stAtlasInfo.fAtlasRatio * pBitmapObj->GetWitdh();
	m_iStretchedAtlasHeight = _stAtlasInfo.fAtlasRatio * pBitmapObj->GetHeight();

	switch (_stAtlasInfo.eLoaderType)
	{
	case ATLAS_LOADER::TYPE_UNIFORM:
	{
		int iRow = pBitmapObj->GetHeight() / _stAtlasInfo.iTileHeight;
		int iCol = pBitmapObj->GetWitdh() / _stAtlasInfo.iTileWidth;

		// 출력 영역 설정.
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
	m_stAtlasInfo.vecOutputArea.clear();

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
	const CBitmapObj* pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(ctBitmapKey[m_stAtlasInfo.iAtlasID]);

	StretchBlt(_hdc,
		0,
		0,
		m_iStretchedAtlasWidth,
		m_iStretchedAtlasHeight,
		GetMemDC(), 
		0, 
		0, 
		pBitmapObj->GetWitdh(),
		pBitmapObj->GetHeight(), SRCCOPY);
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
		
		// 시계방향으로 네모를 그린다.
		MoveToEx(_hdc, lStartX, lStartY, nullptr);
		LineTo(_hdc, lStartX + lWidth, lStartY);
		LineTo(_hdc, lStartX + lWidth, lStartY + lHeight);
		LineTo(_hdc, lStartX, lStartY + lHeight);
		LineTo(_hdc, lStartX, lStartY);
	}
}

const HDC & CAtlasLoader::GetMemDC(void)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return CBitmapMgr::GetInstance()->FindBitmapMemDC(ctBitmapKey[m_stAtlasInfo.iAtlasID]);
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
