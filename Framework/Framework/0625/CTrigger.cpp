#include "stdafx.h"
#include "CTrigger.h"
#include "CMapLoader.h"
#include "CCamera2D.h"


CTrigger::CTrigger(FILE * _fpIn, CGameWorld & _rGameWorld, CMapLoader & _rMap, COLLIDER::E_TYPE _eType)
	:
	CObj(_rGameWorld, 0.f, 0.f, 0, 0),
	m_rMap(_rMap),
	m_iGroupID(-1)
{
	int iObjType;
	_pivot_point stPivotPoint;
	_pivot_point stEndPoint;
	fscanf_s(_fpIn, " %d %d %d %d", &iObjType, &m_iGroupID, &stPivotPoint.iRow, &stPivotPoint.iCol);
	fscanf_s(_fpIn, " %d %d", &stEndPoint.iRow, &stEndPoint.iCol);
	const _map_structure_info& rMapStructureInfo = _rMap.GetMapStructureInfo();
	RECT rc = {
		stPivotPoint.iCol * rMapStructureInfo.iTileWidth,
		stPivotPoint.iRow * rMapStructureInfo.iTileHeight,
		(stEndPoint.iCol + 1) * rMapStructureInfo.iTileWidth,
		(stEndPoint.iRow + 1) * rMapStructureInfo.iTileHeight
	};
	SetX((rc.right + rc.left) >> 1);
	SetY((rc.bottom + rc.top) >> 1);
	SetWidth(rc.right - rc.left);
	SetHeight(rc.bottom - rc.top);
}

CTrigger::~CTrigger()
{
}

void CTrigger::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// �׸� ������ �����´�.
	RECT rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	// ���ΰ� ��
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hBrush);

	// �ʷ� ���� ����
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(120, 235, 100));
	HPEN hOldPen = (HPEN)SelectObject(_hdc, hPen);

	// ���簢���� �׸���.
	Rectangle(_hdc, pairLeftTop.first, pairLeftTop.second, pairRightBottom.first + 1, pairRightBottom.second + 1);

	SelectObject(_hdc, hOldBrush);
	SelectObject(_hdc, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	TCHAR szMode[32];
	swprintf_s(szMode, TEXT("Group : %d"), m_iGroupID);
	TextOut(_hdc, pairLeftTop.first, pairLeftTop.second, szMode, lstrlen(szMode));
}