#include "stdafx.h"
#include "CAtlasObj.h"
#include "CAtlasLoader.h"
#include "CCamera2D.h"


CAtlasObj::CAtlasObj(const _obj_render_info & _rObjRenderInfo, int _iPivotRow, int _iPivotCol, const _atlas_obj_info& _rAtlasObjInfo)
	:
	CTileMapObj(_rObjRenderInfo, _iPivotRow, _iPivotCol),
	m_stAtlasObjInfo(_rAtlasObjInfo)
{
}


CAtlasObj::~CAtlasObj()
{
}

RECT CAtlasObj::GetRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol * m_rObjRenderInfo.iTileWidth;
	rc.top = m_stPivotPoint.iRow * m_rObjRenderInfo.iTileHeight;
	rc.right = (m_stPivotPoint.iCol + m_stAtlasObjInfo.iCoveredWidthTiles) * m_rObjRenderInfo.iTileWidth;
	rc.bottom = (m_stPivotPoint.iRow + m_stAtlasObjInfo.iCoveredHeightTiles) * m_rObjRenderInfo.iTileHeight;
	return rc;
}

RECT CAtlasObj::GetRowColRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol;
	rc.top = m_stPivotPoint.iRow;
	rc.right = m_stPivotPoint.iCol + m_stAtlasObjInfo.iCoveredWidthTiles - 1;
	rc.bottom = m_stPivotPoint.iRow + m_stAtlasObjInfo.iCoveredHeightTiles - 1;

	return rc;
}

void CAtlasObj::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 검출된 타일이 속한 아틀라스 비트맵을 가져온다.
	HDC memdc = CreateCompatibleDC(_hdc);

	HBITMAP m_bitmapOldAtlas = (HBITMAP)SelectObject(memdc, m_rObjRenderInfo.vecAtlasLoaders[m_stAtlasObjInfo.iAtlasID]->GetBitmap());

	// 그릴 영역을 가져온다.
	RECT rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	StretchBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기
		memdc,
		m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.top,
		m_stAtlasObjInfo.rcOutputArea.right - m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.bottom - m_stAtlasObjInfo.rcOutputArea.top,
		SRCCOPY);
	DeleteDC(memdc);
}
