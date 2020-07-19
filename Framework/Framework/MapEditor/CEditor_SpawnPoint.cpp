#include "stdafx.h"
#include "CEditor_SpawnPoint.h"
#include "CCamera2D.h"
#include "CSpace.h"


CEditor_SpawnPoint::CEditor_SpawnPoint(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo, int _iX, int _iY)
	:
	CEditor_Obj(_rGameWorld, _rMapRenderInfo, _iX / _rMapRenderInfo.stMapStructureInfo.iTileWidth, _iY / _rMapRenderInfo.stMapStructureInfo.iTileHeight, MAP_OBJ::TYPE_SPAWN_POINT)
{
	m_ptSpawnPoint.x = _iX;
	m_ptSpawnPoint.y = _iY;
}

CEditor_SpawnPoint::CEditor_SpawnPoint(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo)
	:
	CEditor_Obj(_rGameWorld, _rMapRenderInfo)
{
}

CEditor_SpawnPoint::~CEditor_SpawnPoint()
{
}

RECT CEditor_SpawnPoint::GetRect(void) const
{
	RECT rc;
	/*rc.left = m_stPivotPoint.iCol * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.top = m_stPivotPoint.iRow * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	rc.right = (m_stPivotPoint.iCol + 1) * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.bottom = (m_stPivotPoint.iRow + 1) * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;*/
	rc.left = m_ptSpawnPoint.x - (m_rMapRenderInfo.stMapStructureInfo.iTileWidth >> 1);
	rc.top = m_ptSpawnPoint.y - (m_rMapRenderInfo.stMapStructureInfo.iTileHeight >> 1);
	rc.right = m_ptSpawnPoint.x + (m_rMapRenderInfo.stMapStructureInfo.iTileWidth >> 1);
	rc.bottom = m_ptSpawnPoint.y + (m_rMapRenderInfo.stMapStructureInfo.iTileHeight >> 1);
	return rc;
}

RECT CEditor_SpawnPoint::GetRowColRect(void) const
{
	RECT rc;
	rc.left = m_ptSpawnPoint.x / m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.top = m_ptSpawnPoint.y / m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	rc.right = rc.left;
	rc.bottom = rc.top;

	return rc;
}

void CEditor_SpawnPoint::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 그릴 영역을 가져온다.
	RECT rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(m_rGameWorld.GetViewSpace()->GetRect(), rcCollider)) return;

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

	if (g_bDebugShowGroup) {
		TCHAR szMode[32];
		swprintf_s(szMode, TEXT("G%d"), m_iGroupID);
		TextOut(_hdc, pairLeftTop.first, pairLeftTop.second, szMode, lstrlen(szMode));
	}
}

void CEditor_SpawnPoint::SaveInfo(FILE * _fpOut)
{
	CEditor_Obj::SaveInfo(_fpOut);
	fprintf_s(_fpOut, "%d %d \n",
		m_ptSpawnPoint.x,
		m_ptSpawnPoint.y
	);
}

void CEditor_SpawnPoint::LoadInfo(FILE * _fpIn)
{
	CEditor_Obj::LoadInfo(_fpIn);
	fscanf_s(_fpIn, "%d %d",
		&m_ptSpawnPoint.x,
		&m_ptSpawnPoint.y
	);
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
