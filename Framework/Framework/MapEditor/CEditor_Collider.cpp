#include "stdafx.h"
#include "CEditor_Collider.h"
#include "CCamera2D.h"
#include "CSpace.h"


CEditor_Collider::CEditor_Collider(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol)
	:
	CEditor_Obj(_rGameWorld, _rMapRenderInfo, _iPivotRow, _iPivotCol, MAP_OBJ::TYPE_COLLIDER)
{
}

CEditor_Collider::CEditor_Collider(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo)
	:
	CEditor_Obj(_rGameWorld, _rMapRenderInfo)
{
}


CEditor_Collider::~CEditor_Collider()
{
}

RECT CEditor_Collider::GetRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.top = m_stPivotPoint.iRow * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	rc.right = (m_stPivotPoint.iCol + 1) * m_rMapRenderInfo.stMapStructureInfo.iTileWidth;
	rc.bottom = (m_stPivotPoint.iRow + 1) * m_rMapRenderInfo.stMapStructureInfo.iTileHeight;
	return rc;
}

RECT CEditor_Collider::GetRowColRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol;
	rc.top = m_stPivotPoint.iRow;
	rc.right = m_stPivotPoint.iCol;
	rc.bottom = m_stPivotPoint.iRow;

	return rc;
}

void CEditor_Collider::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// �׸� ������ �����´�.
	RECT rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(m_rGameWorld.GetViewSpace()->GetRect(), rcCollider)) return;

	// ���ΰ� ��
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hBrush);

	// ���� ���� ����
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(235, 120, 100));
	HPEN hOldPen = (HPEN)SelectObject(_hdc, hPen);

	// ������ �׷��� ���簢���� �׸���.
	Rectangle(_hdc, pairLeftTop.first, pairLeftTop.second, pairRightBottom.first + 1, pairRightBottom.second + 1);
	MoveToEx(_hdc, pairLeftTop.first, pairLeftTop.second, nullptr);
	LineTo(_hdc, pairRightBottom.first, pairRightBottom.second);
	MoveToEx(_hdc, pairRightBottom.first, pairLeftTop.second, nullptr);
	LineTo(_hdc, pairLeftTop.first, pairRightBottom.second);

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

void CEditor_Collider::SaveInfo(FILE * _fpOut)
{
	CEditor_Obj::SaveInfo(_fpOut);
}

void CEditor_Collider::LoadInfo(FILE * _fpIn)
{
	CEditor_Obj::LoadInfo(_fpIn);
}

void CEditor_Collider::MakeMapData(FILE * _fpOut)
{
	CEditor_Obj::MakeMapData(_fpOut);

	// 1) ������Ʈ�� Ÿ�ϸʿ��� �����ϴ� ���� ����
	RECT rcRect = GetRect();
	fprintf_s(_fpOut, "%d %d %d %d \n\n",
		rcRect.left,
		rcRect.top,
		rcRect.right,
		rcRect.bottom);
}
