#include "stdafx.h"
#include "CCollider.h"
#include "CCamera2D.h"

CCollider::CCollider(CGameWorld & _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight, COLLIDER::E_TYPE _eType)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight)
{
}

CCollider::~CCollider()
{
}

void CCollider::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 그릴 영역을 가져온다.
	RECT rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	// 내부가 빈
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hBrush);

	// 빨간 선을 지닌
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(235, 120, 100));
	HPEN hOldPen = (HPEN)SelectObject(_hdc, hPen);

	// 직사각형을 그린다.
	Rectangle(_hdc, pairLeftTop.first, pairLeftTop.second, pairRightBottom.first + 1, pairRightBottom.second + 1);

	SelectObject(_hdc, hOldBrush);
	SelectObject(_hdc, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}
