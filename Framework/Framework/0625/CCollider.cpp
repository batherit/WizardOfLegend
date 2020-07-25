#include "stdafx.h"
#include "CCollider.h"
#include "CCamera2D.h"
#include "CMapLoader.h"

CCollider::CCollider(FILE* _fpIn, CGameWorld & _rGameWorld, COLLIDER::E_SHAPE _eShape /*= COLLIDER::TYPE_RECT*/)
	:
	CObj(_rGameWorld, 0.f, 0.f, 0, 0)
{
	// 0) 오브젝트 타입과 그룹 아이디
	CObj::LoadMapData(_fpIn);

	// 2) 오브젝트가 타일맵에서 차지하는 면적 정보
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

CCollider::CCollider(CGameWorld & _rGameWorld, CObj* _pOwner, float _fOffsetX, float _fOffsetY, size_t _iWidth, size_t _iHeight, COLLIDER::E_SHAPE _eShape)
	:
	CObj(_rGameWorld, 0.f, 0.f, _iWidth, _iHeight),
	m_pOwner(_pOwner),
	m_fOffsetX(_fOffsetX),
	m_fOffsetY(_fOffsetY)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX() + _fOffsetX);
		SetY(m_pOwner->GetY() + _fOffsetY);
	}
}

CCollider::~CCollider()
{
}

int CCollider::Update(float _fDeltaTime)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX() + m_fOffsetX);
		SetY(m_pOwner->GetY() + m_fOffsetY);
	}
	return 0;
}

void CCollider::LateUpdate(void)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX() + m_fOffsetX);
		SetY(m_pOwner->GetY() + m_fOffsetY);
	}
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
	MoveToEx(_hdc, pairLeftTop.first, pairLeftTop.second, nullptr);
	LineTo(_hdc, pairRightBottom.first, pairRightBottom.second);
	MoveToEx(_hdc, pairRightBottom.first, pairLeftTop.second, nullptr);
	LineTo(_hdc, pairLeftTop.first, pairRightBottom.second);

	SelectObject(_hdc, hOldBrush);
	SelectObject(_hdc, hOldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	/*TCHAR szMode[32];
	swprintf_s(szMode, TEXT("GN : %d"), m_iGroupID);
	TextOut(_hdc, pairLeftTop.first, pairLeftTop.second, szMode, lstrlen(szMode));*/
}
