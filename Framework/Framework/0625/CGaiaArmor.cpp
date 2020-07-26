#include "stdafx.h"
#include "CGaiaArmor.h"
#include "CGaiaArmorChild.h"



CGaiaArmor::CGaiaArmor(CGameWorld & _rGameWorld, CObj * _pOwner)
	:
	CObj(_rGameWorld, 0.f, 0.f, 0.f, 0.f),	// 아이스 크리스탈 중앙은 형체가 없다.
	m_pOwner(_pOwner),
	m_fLifeTime(10.f)
{
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
	SetSpeed(0.f);

	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());


		for (int i = 0; i < m_iGaiaArmorNum; i++) {
			m_pGaiaArmor[i] = new CGaiaArmorChild(_rGameWorld, this);
			TO_WOL(_rGameWorld).GetListUsedPlayerSkills().emplace_back(m_pGaiaArmor[i]);
		}
		CSoundMgr::Get_Instance()->PlaySound(TEXT("GAIA_ARMOR_START.mp3"), CSoundMgr::SKILL);
	}
	else SetValid(false);
}


CGaiaArmor::~CGaiaArmor()
{
}

int CGaiaArmor::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;


	if ((m_fElapsedTime += _fDeltaTime) <= 1.f) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY() + 20 * (1.f - m_fElapsedTime));
	}
	else if (m_fElapsedTime <= 2.f) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
		float fT = m_fElapsedTime - 1.f;
		SetSpeed(fT * GAIA_ARMOR_MAX_ROT_SPEED);
	}
	else {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}

	if (m_fElapsedTime > m_fLifeTime) {
		for (int i = 0; i < m_iGaiaArmorNum; i++) {
			m_pGaiaArmor[i]->SetValid(false);
		}
		SetValid(false);
		return 1;
	}

	m_fDegree += m_fSpeed * _fDeltaTime;
	if (m_fDegree >= 360.f) m_fDegree -= 360.f;
	float m_fChildDegree = 0.f;
	for (int i = 0; i < m_iGaiaArmorNum; i++) {
		m_fChildDegree = m_fDegree + (360.f / m_iGaiaArmorNum) * i;
		if (m_fChildDegree >= 360.f) m_fChildDegree -= 360.f;
		m_pGaiaArmor[i]->SetXY(GetX() + cosf(TO_RADIAN(m_fChildDegree)) * GAIA_ARMOR_OFFSET, GetY() + sinf(TO_RADIAN(m_fChildDegree)) * GAIA_ARMOR_OFFSET);
	}


	return 0;
}

void CGaiaArmor::LateUpdate(void)
{
}

void CGaiaArmor::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	// 본체는 그릴 것이 없다. 자식만 그린다.
	for (int i = 0; i < m_iGaiaArmorNum; i++) {
		m_pGaiaArmor[i]->Render(_hdc, _pCamera);
	}
}

void CGaiaArmor::Release(void)
{
	// 가비지 콜렉터가 자식들 수거해감.
}
