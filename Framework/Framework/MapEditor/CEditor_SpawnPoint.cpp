#include "stdafx.h"
#include "CEditor_SpawnPoint.h"
#include "CCamera2D.h"


CEditor_SpawnPoint::CEditor_SpawnPoint(const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol)
	:
	CEditor_Obj(_rMapRenderInfo, _iPivotRow, _iPivotCol, MAP_OBJ::TYPE_SPAWN_POINT)
{
}

CEditor_SpawnPoint::CEditor_SpawnPoint(const _map_render_info & _rMapRenderInfo)
	:
	CEditor_Obj(_rMapRenderInfo)
{
}

CEditor_SpawnPoint::~CEditor_SpawnPoint()
{
}

RECT CEditor_SpawnPoint::GetRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.top = m_stPivotPoint.iRow * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	rc.right = (m_stPivotPoint.iCol + 1) * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.bottom = (m_stPivotPoint.iRow + 1) * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	return rc;
}

RECT CEditor_SpawnPoint::GetRowColRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol;
	rc.top = m_stPivotPoint.iRow;
	rc.right = m_stPivotPoint.iCol;
	rc.bottom = m_stPivotPoint.iRow;

	return rc;
}

void CEditor_SpawnPoint::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 그릴 영역을 가져온다.
	RECT rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	// 내부가 빈
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hBrush);

	// 푸른 선을 지닌
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(150, 255, 255));
	HPEN hOldPen = (HPEN)SelectObject(_hdc, hPen);

	// 네모 동그라미를 그린다.
	Rectangle(_hdc, pairLeftTop.first, pairLeftTop.second, pairRightBottom.first + 1, pairRightBottom.second + 1);
	Ellipse(_hdc, pairLeftTop.first, pairLeftTop.second, pairRightBottom.first + 1, pairRightBottom.second + 1);

	SelectObject(_hdc, hOldBrush);
	SelectObject(_hdc, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	TCHAR szMode[32];
	swprintf_s(szMode, TEXT("GN : %d"), m_iGroupID);
	TextOut(_hdc, pairLeftTop.first, pairLeftTop.second, szMode, lstrlen(szMode));
}

void CEditor_SpawnPoint::SaveInfo(FILE * _fpOut)
{
	CEditor_Obj::SaveInfo(_fpOut);
}

void CEditor_SpawnPoint::LoadInfo(FILE * _fpIn)
{
	CEditor_Obj::LoadInfo(_fpIn);
}

void CEditor_SpawnPoint::MakeMapData(FILE * _fpOut)
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
