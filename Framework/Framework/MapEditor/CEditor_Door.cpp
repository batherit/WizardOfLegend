#include "stdafx.h"
#include "CEditor_Door.h"
#include "CBitmapMgr.h"
#include "CBitmapObj.h"
#include "CCamera2D.h"


CEditor_Door::CEditor_Door(const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol, MAP_OBJ::E_TYPE _eDoorType)
	:
	CEditor_Obj(_rMapRenderInfo, _iPivotRow, _iPivotCol, _eDoorType)
{
	switch (m_eObjType) {
	case MAP_OBJ::LAYER_DOOR_HOR:
		m_tDoorBitmapKey = TEXT("PRISON_HOR");
		break;
	case MAP_OBJ::LAYER_DOOR_VER:
		m_tDoorBitmapKey = TEXT("PRISON_VER");
		break;
	}

	const CBitmapObj* pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(m_tDoorBitmapKey);
	m_iCoveredWidthTiles = pBitmapObj->GetWitdh() / _rMapRenderInfo.stMapStructureInfo.iTileWidth;
	m_iCoveredHeightTiles = pBitmapObj->GetHeight() / _rMapRenderInfo.stMapStructureInfo.iTileHeight;
}

CEditor_Door::CEditor_Door(const _map_render_info & _rMapRenderInfo)
	:
	CEditor_Obj(_rMapRenderInfo)
{
}

CEditor_Door::~CEditor_Door()
{
}

RECT CEditor_Door::GetRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.top = m_stPivotPoint.iRow * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	rc.right = (m_stPivotPoint.iCol + m_iCoveredWidthTiles) * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.bottom = (m_stPivotPoint.iRow + m_iCoveredHeightTiles) * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	
	return rc;
}

RECT CEditor_Door::GetRowColRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol;
	rc.top = m_stPivotPoint.iRow;
	rc.right = m_stPivotPoint.iCol + m_iCoveredWidthTiles - 1;
	rc.bottom = m_stPivotPoint.iRow + m_iCoveredHeightTiles - 1;

	return rc;
}

void CEditor_Door::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 그릴 영역을 가져온다.
	RECT rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);


	const HDC memdc = CBitmapMgr::GetInstance()->FindBitmapMemDC(m_tDoorBitmapKey);
	const CBitmapObj* pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(m_tDoorBitmapKey);

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first + 1,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second + 1,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		memdc,
		0,
		0,
		pBitmapObj->GetWitdh(),
		pBitmapObj->GetHeight(),
		RGB(255, 0, 255));

	TCHAR szMode[32];
	swprintf_s(szMode, TEXT("GN : %d"), m_iGroupID);
	TextOut(_hdc, pairLeftTop.first, pairLeftTop.second, szMode, lstrlen(szMode));
}

void CEditor_Door::SaveInfo(FILE * _fpOut)
{
	CEditor_Obj::SaveInfo(_fpOut);
	fprintf_s(_fpOut, "%d %d \n",
		m_iCoveredWidthTiles,	
		m_iCoveredHeightTiles
	);
}

void CEditor_Door::LoadInfo(FILE * _fpIn)
{
	CEditor_Obj::LoadInfo(_fpIn);
	fscanf_s(_fpIn, " %d %d",
		&m_iCoveredWidthTiles,
		&m_iCoveredHeightTiles
	);
	switch (m_eObjType) {
	case MAP_OBJ::LAYER_DOOR_HOR:
		m_tDoorBitmapKey = TEXT("PRISON_HOR");
		break;
	case MAP_OBJ::LAYER_DOOR_VER:
		m_tDoorBitmapKey = TEXT("PRISON_VER");
		break;
	}
}

void CEditor_Door::MakeMapData(FILE * _fpOut)
{
	CEditor_Obj::MakeMapData(_fpOut);

	// 1) 오브젝트가 타일맵에서 차지하는 면적 정보
	RECT rcRect = GetRect();
	fprintf_s(_fpOut, "%d %d %d %d \n\n",
		rcRect.left,
		rcRect.top,
		rcRect.right,
		rcRect.bottom);
}
