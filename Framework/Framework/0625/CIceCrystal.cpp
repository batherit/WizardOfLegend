#include "stdafx.h"
#include "CIceCrystal.h"
#include "CIceCrystalChild.h"
#include "CPlayerWOL.h"



CIceCrystal::CIceCrystal(CGameWorld & _rGameWorld, CObj * _pOwner)
	:
	CObj(_rGameWorld, 0.f, 0.f, 0.f, 0.f),	// 아이스 크리스탈 중앙은 형체가 없다.
	m_pOwner(_pOwner),
	m_fLifeTime(5.f)
{
	//m_pColliders[COLLIDER::TYPE_DAMAGED] = this;

	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());

		if (TO_PLAYER_WOL(_pOwner)->IsSignatureMode()) {
			m_iIceCrystalNum = 6;
			m_iPeriod = 2;
			m_fMinRotSpeed = ICE_CRYSTAL_MIN_ROT_SPEED * 2.f;
			m_fMaxRotSpeed = ICE_CRYSTAL_MAX_ROT_SPEED * 2.f;
			TO_PLAYER_WOL(_pOwner)->SetSignatureMode(false);
			TO_PLAYER_WOL(_pOwner)->SetIsSignatureSkillUsing(true);
			TO_WOL(_rGameWorld).TemporarilyAdjustWorldTimeSpeed(0.6f, 0.2f);
			CSoundMgr::Get_Instance()->PlaySound(TEXT("ULT_USE.mp3"), CSoundMgr::SKILL);
			m_bIsSignatureMode = true;
			
		}
		else {
			m_iIceCrystalNum = 3;
			m_iPeriod = 1;
			m_fMinRotSpeed = ICE_CRYSTAL_MIN_ROT_SPEED;
			m_fMaxRotSpeed = ICE_CRYSTAL_MAX_ROT_SPEED;
		}

		for (int i = 0; i < m_iIceCrystalNum; i++) {
			m_pIceCrystal[i] = new CIceCrystalChild(_rGameWorld, this);
			TO_WOL(_rGameWorld).GetListUsedPlayerSkills().emplace_back(m_pIceCrystal[i]);
		}
		CSoundMgr::Get_Instance()->PlaySound(TEXT("ICE_KRYSTAL_START.mp3"), CSoundMgr::SKILL);
	}
	else SetValid(false);
}

CIceCrystal::CIceCrystal(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, 0.f, 0.f),
	m_pOwner(nullptr),
	m_fLifeTime(5.f)
{
	//m_pColliders[COLLIDER::TYPE_DAMAGED] = this;

	m_iIceCrystalNum = 3;
	m_iPeriod = 1;
	m_fMinRotSpeed = ICE_CRYSTAL_MIN_ROT_SPEED;
	m_fMaxRotSpeed = ICE_CRYSTAL_MAX_ROT_SPEED;

	for (int i = 0; i < m_iIceCrystalNum; i++) {
		m_pIceCrystal[i] = new CIceCrystalChild(_rGameWorld, this);
		TO_WOL(_rGameWorld).GetListUsedMonsterSkills().emplace_back(m_pIceCrystal[i]);
	}
	CSoundMgr::Get_Instance()->PlaySound(TEXT("ICE_KRYSTAL_START.mp3"), CSoundMgr::SKILL);
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
		for (int i = 0; i < m_iIceCrystalNum; i++) {
			m_pIceCrystal[i]->SetValid(false);
		}
		SetValid(false);
		if(m_bIsSignatureMode && TO_PLAYER_WOL(m_pOwner)) TO_PLAYER_WOL(m_pOwner)->SetIsSignatureSkillUsing(false);
		return 1;
	}


	m_fPeriodDegree += _fDeltaTime * (180.f/ m_fLifeTime) ;
	if (m_fPeriodDegree >= 360.f) m_fPeriodDegree -= 360.f;
	float fT = (-cosf(TO_RADIAN(m_fPeriodDegree * m_iPeriod)) * 0.5f + 0.5f);

	Clamp(&fT, 0.f, 1.f);
	m_fSpeed = (1.f - fT) * (1.f - fT) * m_fMinRotSpeed + 2.f * fT * (1.f - fT) * m_fMaxRotSpeed + fT * fT * m_fMinRotSpeed;
	m_fOffset = (1.f - fT) * (1.f - fT) * ICE_CRYSTAL_MIN_DIST + 2.f * fT * (1.f - fT) * ICE_CRYSTAL_MAX_DIST + fT * fT * ICE_CRYSTAL_MIN_DIST;


	m_fDegree += m_fSpeed * _fDeltaTime;
	if (m_fDegree >= 360.f) m_fDegree -= 360.f;
	float m_fChildDegree = 0.f;
	for (int i = 0; i < m_iIceCrystalNum; i++) {
		m_fChildDegree = m_fDegree + (360.f / m_iIceCrystalNum) * i;
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
	// 본체는 그릴 것이 없다. 자식만 그린다.
	for (int i = 0; i < m_iIceCrystalNum; i++) {
		m_pIceCrystal[i]->Render(_hdc, _pCamera);
	}
}

void CIceCrystal::Release(void)
{
	// 가비지 콜렉터가 자식들 수거해감.
}
