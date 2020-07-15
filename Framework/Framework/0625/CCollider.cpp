#include "stdafx.h"
#include "CCollider.h"
#include "CCamera2D.h"
#include "CMapLoader.h"

CCollider::CCollider(FILE* _fpIn, CGameWorld & _rGameWorld, CMapLoader & _rMap, COLLIDER::E_TYPE _eType)
	:
	CObj(_rGameWorld, 0.f, 0.f, 0, 0),
	m_rMap(_rMap)
{
	int iDummy;
	_pivot_point stPivotPoint;
	fscanf_s(_fpIn, " %d %d %d", &iDummy, &stPivotPoint.iRow, &stPivotPoint.iCol);
	const _map_structure_info& rMapStructureInfo = _rMap.GetMapStructureInfo();
	RECT rc = {
		stPivotPoint.iCol * rMapStructureInfo.iTileWidth,
		stPivotPoint.iRow * rMapStructureInfo.iTileHeight,
		(stPivotPoint.iCol + 1) * rMapStructureInfo.iTileWidth,
		(stPivotPoint.iRow + 1) * rMapStructureInfo.iTileHeight
	};
	SetX((rc.right + rc.left) >> 1);
	SetY((rc.bottom + rc.top) >> 1);
	SetWidth(rc.right - rc.left);
	SetHeight(rc.bottom - rc.top);
}

CCollider::~CCollider()
{
}

void CCollider::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// �׸� ������ �����´�.
	RECT rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	// ���ΰ� ��
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hBrush);

	// ���� ���� ����
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(235, 120, 100));
	HPEN hOldPen = (HPEN)SelectObject(_hdc, hPen);

	// ���簢���� �׸���.
	Rectangle(_hdc, pairLeftTop.first, pairLeftTop.second, pairRightBottom.first + 1, pairRightBottom.second + 1);

	SelectObject(_hdc, hOldBrush);
	SelectObject(_hdc, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}
