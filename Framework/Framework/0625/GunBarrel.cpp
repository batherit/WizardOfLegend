#include "stdafx.h"
#include "GunBarrel.h"
#include "CCamera2D.h"

CGunBarrel::CGunBarrel(CGameWorld& _rGameWorld, CObj * _pOwner, float _fDegree, float _fDistance, float _fSpeed)
	:
	m_pOwner(nullptr),
	CObj(_rGameWorld, 0.f, 0.f, 0, 0, 0.f, 0.f, _fSpeed, nullptr),
	m_fDistance(_fDistance),
	m_fDegree(_fDegree)
{
	if (_pOwner) {
		m_pOwner = _pOwner;
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}
}

CGunBarrel::~CGunBarrel()
{
	Release();
}

int CGunBarrel::Update(float _fDeltaTime)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}
	return 0;
}

void CGunBarrel::Render(const HDC & _hdc, CCamera2D* _pCamera)
{
	if (_pCamera) {
		pair<float, float> pairStart = _pCamera->TransformPoint(GetStartX(), GetStartY());
		pair<float, float> pairEnd = _pCamera->TransformPoint(GetEndX(), GetEndY());

		MoveToEx(_hdc, pairStart.first, pairStart.second, nullptr);
		LineTo(_hdc, pairEnd.first, pairEnd.second);
	}
	else {
		MoveToEx(_hdc, GetStartX(), GetStartY(), nullptr);
		LineTo(_hdc, GetEndX(), GetEndY());
	}
}

void CGunBarrel::RotateCW(float _fDeltaTime)
{
	 m_fDegree += GetSpeed() * _fDeltaTime; m_fDegree = CLAMP_DEGREE(m_fDegree); 
}

void CGunBarrel::RotateCCW(float _fDeltaTime)
{
	m_fDegree -= GetSpeed() * _fDeltaTime; m_fDegree = CLAMP_DEGREE(m_fDegree);
}
