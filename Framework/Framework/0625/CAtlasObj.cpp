#include "stdafx.h"
#include "CAtlasObj.h"
#include "CCamera2D.h"
#include "CMapLoader.h"
#include "CAtlasLoader.h"


CAtlasObj::CAtlasObj(FILE* _fpIn, CGameWorld & _rGameWorld, CMapLoader & _rMap)
	:	
	CObj(_rGameWorld, 0.f, 0.f),
	m_rMap(_rMap)
{
	int iDummy;
	_pivot_point stPivotPoint;
	fscanf_s(_fpIn, " %d %d %d", &iDummy, &stPivotPoint.iRow, &stPivotPoint.iCol);
	fscanf_s(_fpIn, " %d %d %d %d %d %d %d",
		&m_stAtlasObjInfo.iAtlasID,
		&m_stAtlasObjInfo.rcOutputArea.left,
		&m_stAtlasObjInfo.rcOutputArea.top,
		&m_stAtlasObjInfo.rcOutputArea.right,
		&m_stAtlasObjInfo.rcOutputArea.bottom,
		&m_stAtlasObjInfo.iCoveredWidthTiles,
		&m_stAtlasObjInfo.iCoveredHeightTiles
	);

	const _map_structure_info& rMapStructureInfo = _rMap.GetMapStructureInfo();
	RECT rc = {
		stPivotPoint.iCol * rMapStructureInfo.iTileWidth,
		stPivotPoint.iRow * rMapStructureInfo.iTileHeight,
		(stPivotPoint.iCol + m_stAtlasObjInfo.iCoveredWidthTiles) * rMapStructureInfo.iTileWidth,
		(stPivotPoint.iRow + m_stAtlasObjInfo.iCoveredHeightTiles) * rMapStructureInfo.iTileHeight
	};
	SetX((rc.right + rc.left) >> 1);
	SetY((rc.bottom + rc.top) >> 1);
	SetWidth(rc.right - rc.left);
	SetHeight(rc.bottom - rc.top);
}

CAtlasObj::~CAtlasObj()
{
}

void CAtlasObj::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// ����� Ÿ���� ���� ��Ʋ�� ��Ʈ���� �����´�.
	//HDC memdc = CreateCompatibleDC(_hdc);

	//HBITMAP m_bitmapOldAtlas = (HBITMAP)SelectObject(memdc, m_rMapRenderInfo.vecAtlasLoaders[m_stAtlasObjInfo.iAtlasID]->GetBitmap());

	// �׸� ������ �����´�.
	RECT rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	const vector<CAtlasLoader*>& vecAtloasLoaders = m_rMap.GetAtlasLoaders();

	StretchBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first + 1,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second + 1,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		vecAtloasLoaders[m_stAtlasObjInfo.iAtlasID]->GetMemDC(),
		m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.top,
		m_stAtlasObjInfo.rcOutputArea.right - m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.bottom - m_stAtlasObjInfo.rcOutputArea.top,
		SRCCOPY);
	//DeleteDC(memdc);
}
