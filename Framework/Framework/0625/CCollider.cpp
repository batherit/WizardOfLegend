#include "stdafx.h"
#include "CCollider.h"
#include "CCamera2D.h"


CCollider::CCollider(const _obj_render_info & _rObjRenderInfo, int _iPivotRow, int _iPivotCol)
	:
	CTileMapObj(_rObjRenderInfo, _iPivotRow, _iPivotCol, MAP_OBJ::TYPE_COLLIDER)
{
}


CCollider::~CCollider()
{
}

RECT CCollider::GetRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol * m_rObjRenderInfo.iTileWidth;
	rc.top = m_stPivotPoint.iRow * m_rObjRenderInfo.iTileHeight;
	rc.right = (m_stPivotPoint.iCol + 1) * m_rObjRenderInfo.iTileWidth;
	rc.bottom = (m_stPivotPoint.iRow + 1) * m_rObjRenderInfo.iTileHeight;
	return rc;
}

RECT CCollider::GetRowColRect(void) const
{
	RECT rc;
	rc.left = m_stPivotPoint.iCol;
	rc.top = m_stPivotPoint.iRow;
	rc.right = m_stPivotPoint.iCol;
	rc.bottom = m_stPivotPoint.iRow;

	return rc;
}

void CCollider::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// �׸� ������ �����´�.
	RECT rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	// ���ΰ� ��.
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hBrush);

	// ���� ��
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(235, 120, 100));
	HPEN hOldPen = (HPEN)SelectObject(_hdc, hPen);

	Rectangle(_hdc, pairLeftTop.first, pairLeftTop.second, pairRightBottom.first + 1, pairRightBottom.second + 1);

	SelectObject(_hdc, hOldBrush);
	SelectObject(_hdc, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}
