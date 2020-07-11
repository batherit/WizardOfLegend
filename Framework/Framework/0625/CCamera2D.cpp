#include "stdafx.h"
#include "CCamera2D.h"


CCamera2D::CCamera2D(CGameWorld& _rGameWorld, CObj* _pOwner /*= nullptr*/, float _fX /*= 0.f*/, float _fY /*= 0.f*/)
	:
	m_pOwner(nullptr),
	CObj(_rGameWorld, _fX, _fY, 0.f, 0.f, 0.f, 0.f, 0.f, nullptr)
{
	if (_pOwner) {
		m_pOwner = _pOwner;
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}
	else {

	}
}


CCamera2D::~CCamera2D()
{
}

int CCamera2D::Update(float _fDeltaTime)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}
	else {
		// 드래그 한만큼 이동.
		// MoveTo();
	}

	return 0;
}

RECT CCamera2D::TransformRect(RECT& _rRect)
{
	RECT rc;

	rc.right = (_rRect.right - GetX()) * m_fZoomMultiple + GetX();
	rc.top = (_rRect.top - GetY())* m_fZoomMultiple + GetY();
	rc.left = (_rRect.left - GetX()) * m_fZoomMultiple + GetX();
	rc.bottom = (_rRect.bottom - GetY())* m_fZoomMultiple  + GetY();

	return rc;
}

pair<float, float> CCamera2D::TransformPoint(float _fX, float _fY)
{
	pair<float, float> pairPoint;
	pairPoint.first = (_fX - GetX())* m_fZoomMultiple + GetX();
	pairPoint.second = (_fY - GetY())* m_fZoomMultiple + GetY();
	return pairPoint;
}
