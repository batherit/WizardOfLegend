#include "stdafx.h"
#include "CSpawnPoint.h"
#include "CCamera2D.h"


CSpawnPoint::CSpawnPoint(FILE * _fpIn, CGameWorld & _rGameWorld)
	:
	CObj(_rGameWorld, 0.f, 0.f, 0, 0)
{
	// 0) ������Ʈ Ÿ�԰� �׷� ���̵�
	CObj::LoadMapData(_fpIn);

	// 2) ������Ʈ�� Ÿ�ϸʿ��� �����ϴ� ���� ����
	RECT rcRect;
	fscanf_s(_fpIn, " %d %d %d %d",
		&rcRect.left,
		&rcRect.top,
		&rcRect.right,
		&rcRect.bottom);

	SetX((rcRect.right + rcRect.left) >> 1);
	SetY((rcRect.bottom + rcRect.top) >> 1);
	SetWidth(rcRect.right - rcRect.left);
	SetHeight(rcRect.bottom - rcRect.top);
}

CSpawnPoint::~CSpawnPoint()
{
}

void CSpawnPoint::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// �׸� ������ �����´�.
	RECT rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	// ���ΰ� ��
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hBrush);

	// Ǫ�� ���� ����
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(150, 255, 255));
	HPEN hOldPen = (HPEN)SelectObject(_hdc, hPen);

	// �׸� ���׶�̸� �׸���.
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
