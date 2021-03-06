#include "stdafx.h"
#include "CEditor_AtlasObj.h"
#include "CAtlasLoader.h"
#include "CCamera2D.h"
#include "CSpace.h"


CEditor_AtlasObj::CEditor_AtlasObj(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol, const _atlas_obj_info& _rAtlasObjInfo)
	:
	CEditor_Obj(_rGameWorld, _rMapRenderInfo, _iPivotRow, _iPivotCol, MAP_OBJ::TYPE_ATLAS_OBJ),
	m_stAtlasObjInfo(_rAtlasObjInfo)
{
}

CEditor_AtlasObj::CEditor_AtlasObj(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo)
	:
	CEditor_Obj(_rGameWorld, _rMapRenderInfo)
{
}


CEditor_AtlasObj::~CEditor_AtlasObj()
{
}

RECT CEditor_AtlasObj::GetRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.top = m_stPivotPoint.iRow * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	rc.right = (m_stPivotPoint.iCol + m_stAtlasObjInfo.iCoveredWidthTiles) * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.bottom = (m_stPivotPoint.iRow + m_stAtlasObjInfo.iCoveredHeightTiles) * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	return rc;
}

RECT CEditor_AtlasObj::GetRowColRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol;
	rc.top = m_stPivotPoint.iRow;
	rc.right = m_stPivotPoint.iCol + m_stAtlasObjInfo.iCoveredWidthTiles - 1;
	rc.bottom = m_stPivotPoint.iRow + m_stAtlasObjInfo.iCoveredHeightTiles - 1;

	return rc;
}

void CEditor_AtlasObj::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	//HBITMAP m_bitmapOldAtlas = (HBITMAP)SelectObject(memdc, m_rMapRenderInfo.vecAtlasLoaders[m_stAtlasObjInfo.iAtlasID]->GetBitmap());

	// 그릴 영역을 가져온다.
	RECT rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(m_rGameWorld.GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first + 1,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second + 1,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_rMapRenderInfo.vecAtlasLoaders[m_stAtlasObjInfo.iAtlasID]->GetMemDC(),
		m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.top,
		m_stAtlasObjInfo.rcOutputArea.right - m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.bottom - m_stAtlasObjInfo.rcOutputArea.top,
		RGB(255, 0, 255));
	
	if (g_bDebugShowGroup) {
		TCHAR szMode[32];
		swprintf_s(szMode, TEXT("G%d"), m_iGroupID);
		TextOut(_hdc, pairLeftTop.first, pairLeftTop.second, szMode, lstrlen(szMode));
		swprintf_s(szMode, TEXT("L%d"), m_iDrawLayer);
		TextOut(_hdc, pairLeftTop.first + 20, pairLeftTop.second, szMode, lstrlen(szMode));
	}
}

void CEditor_AtlasObj::SaveInfo(FILE * _fpOut)
{
	CEditor_Obj::SaveInfo(_fpOut);
	fprintf_s(_fpOut, "%d %d %d %d %d %d %d \n",
		m_stAtlasObjInfo.iAtlasID,
		m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.top,
		m_stAtlasObjInfo.rcOutputArea.right,
		m_stAtlasObjInfo.rcOutputArea.bottom,
		m_stAtlasObjInfo.iCoveredWidthTiles,
		m_stAtlasObjInfo.iCoveredHeightTiles
	);
}

void CEditor_AtlasObj::LoadInfo(FILE * _fpIn)
{
	CEditor_Obj::LoadInfo(_fpIn);
	fscanf_s(_fpIn, " %d %d %d %d %d %d %d",
		&m_stAtlasObjInfo.iAtlasID,
		&m_stAtlasObjInfo.rcOutputArea.left,
		&m_stAtlasObjInfo.rcOutputArea.top,
		&m_stAtlasObjInfo.rcOutputArea.right,
		&m_stAtlasObjInfo.rcOutputArea.bottom,
		&m_stAtlasObjInfo.iCoveredWidthTiles,
		&m_stAtlasObjInfo.iCoveredHeightTiles
	);
}

void CEditor_AtlasObj::MakeMapData(FILE * _fpOut)
{
	CEditor_Obj::MakeMapData(_fpOut);

	// 1) 아틀라스 로더 아이디
	fprintf_s(_fpOut, "%d \n", m_stAtlasObjInfo.iAtlasID);

	// 2) 오브젝트가 타일맵에서 차지하는 면적 정보
	RECT rcRect = GetRect();
	fprintf_s(_fpOut, "%d %d %d %d \n",
		rcRect.left,
		rcRect.top,
		rcRect.right,
		rcRect.bottom);

	// 3) 오브젝트에 대응하는 아틀라스 이미지 면적 정보
	fprintf_s(_fpOut, "%d %d %d %d \n\n",
		m_stAtlasObjInfo.rcOutputArea.left,
		m_stAtlasObjInfo.rcOutputArea.top,
		m_stAtlasObjInfo.rcOutputArea.right,
		m_stAtlasObjInfo.rcOutputArea.bottom);
}
