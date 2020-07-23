#include "stdafx.h"
#include "CIceCrystal.h"
#include "CIceCrystalChild.h"



CIceCrystal::CIceCrystal(CGameWorld & _rGameWorld, CObj * _pOwner)
	:
	CObj(_rGameWorld, 0.f, 0.f, 0.f, 0.f),	// 아이스 크리스탈 중앙은 형체가 없다.
	m_pOwner(_pOwner),
	m_fLifeTime(5.f)
{
	SetSpeed(ICE_CRYSTAL_MIN_ROT_SPEED);
	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
		for (int i = 0; i < 3; i++) {
			m_pIceCrystal[i] = new CIceCrystalChild(_rGameWorld, this);
			TO_WOL(_rGameWorld).GetListUsedPlayerSkills().emplace_back(m_pIceCrystal[i]);
		}
	}
	else SetValid(false);
}

CIceCrystal::CIceCrystal(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, 0.f, 0.f),
	m_pOwner(nullptr),
	m_fLifeTime(5.f)
{
	SetSpeed(ICE_CRYSTAL_MIN_ROT_SPEED);
}

CIceCrystal::~CIceCrystal()
{
}

int CIceCrystal::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;

	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}

	if ((m_fElapsedTime += _fDeltaTime) > m_fLifeTime) {
		for (int i = 0; i < 3; i++) {
			m_pIceCrystal[i]->SetValid(false);
		}
		SetValid(false);
		return 1;
	}
	float fT = m_fElapsedTime / m_fLifeTime;
	Clamp(&fT, 0.f, 1.f);
	m_fSpeed = (1.f - fT) * (1.f - fT) * ICE_CRYSTAL_MIN_ROT_SPEED + 2.f * fT * (1.f - fT) * ICE_CRYSTAL_MAX_ROT_SPEED + fT * fT * ICE_CRYSTAL_MIN_ROT_SPEED;
	m_fOffset = (1.f - fT) * (1.f - fT) * ICE_CRYSTAL_MIN_DIST + 2.f * fT * (1.f - fT) * ICE_CRYSTAL_MAX_DIST + fT * fT * ICE_CRYSTAL_MIN_DIST;
	m_fDegree += _fDeltaTime * m_fSpeed;
	if (m_fDegree >= 360.f) m_fDegree -= 360.f;

	float m_fChildDegree = 0.f;
	for (int i = 0; i < 3; i++) {
		m_fChildDegree = m_fDegree + 120.f * i;
		if (m_fChildDegree >= 360.f) m_fChildDegree -= 360.f;
		m_pIceCrystal[i]->SetXY(GetX() + cosf(TO_RADIAN(m_fChildDegree)) * m_fOffset, GetY() + sinf(TO_RADIAN(m_fChildDegree)) * m_fOffset);
	}

	return 0;
}

void CIceCrystal::LateUpdate(void)
{
}

void CIceCrystal::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 본체는 그릴 것이 없다.
	for (int i = 0; i < 3; i++) {
		m_pIceCrystal[i]->Render(_hdc, _pCamera);
	}
}

void CIceCrystal::Release(void)
{
	// 가비지 콜렉터가 자식들 수거해감.
}
