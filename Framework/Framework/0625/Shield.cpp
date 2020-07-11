#include "stdafx.h"
#include "Shield.h"
#include "ScrewBall.h"


CShield::CShield(CGameWorld& _rGameWorld, CObj * _pOwner, size_t _iWidth, size_t _iHeight, float _fSpeed, float _fDamage)
	:
	m_pOwner(nullptr),
	m_pScrewBall(new CScrewBall(_rGameWorld, this, 20, 20, 10.f, CLAMP_DEGREE(0.f), ((_iWidth > _iHeight ? _iWidth : _iHeight) >> 1) + 30.f)),
	CObj(_rGameWorld, 0.f, 0.f, _iWidth, _iHeight, _fSpeed, Ellipse)
{
	if (_pOwner) {
		m_pOwner = _pOwner;
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}
}

CShield::~CShield()
{
	Release();
}

void CShield::Ready(void)
{
}

int CShield::Update(float _fDeltaTime)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
		DO_IF_IS_VALID_OBJ(m_pScrewBall) { m_pScrewBall->Update(_fDeltaTime); }
	}

	return 0;
}

void CShield::LateUpdate(void)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
		DO_IF_IS_VALID_OBJ(m_pScrewBall) { m_pScrewBall->LateUpdate(); }
	}
}

void CShield::Render(const HDC & _hdc)
{
	//Ellipse(_hdc, GetLeft(), GetTop(), GetRight(), GetBottom());
	DO_IF_IS_VALID_OBJ(m_pScrewBall) { m_pScrewBall->Render(_hdc); }
}

void CShield::Release(void)
{
	DeleteSafe(m_pScrewBall);
}
