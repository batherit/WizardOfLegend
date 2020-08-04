#include "stdafx.h"
#include "CMiddleBossState_FireDragon.h"
#include "CBoss_MiddleBoss.h"
#include "CCamera2D.h"
#include "CFireDragon.h"
#include "CStateMgr.h"
#include "CMiddleBossState_Idle.h"




CMiddleBossState_FireDragon::CMiddleBossState_FireDragon(CBoss_MiddleBoss & _rOwner)
	:
	CState(_rOwner)
{
	m_pCamera = TO_WOL(m_rOwner.GetGameWorld()).GetCamera();
	m_iCount = 30;
	m_fPeriod = 0.12f;
	m_fElapsedTime = 0.f;
	m_eAttackType = PLAYER::STATE_ATTACK1;
	m_pPlayer = TO_WOL(m_rOwner.GetGameWorld()).GetPlayer();
}

CMiddleBossState_FireDragon::~CMiddleBossState_FireDragon()
{
	m_pCamera = nullptr;
}

void CMiddleBossState_FireDragon::OnLoaded(void)
{

	float fLength = 0;
	SetAttackDirection(&fLength);

	_anim_info stAnimInfo;
	stAnimInfo.iState = PLAYER::STATE_ATTACK1;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 2.f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 4;
	m_rOwner.SetNewAnimInfo(stAnimInfo);
}

int CMiddleBossState_FireDragon::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		if (m_iCount > 0) {
			if ((m_fElapsedTime += _fDeltaTime) > m_fPeriod) {
				float fLength = 0;
				SetAttackDirection(&fLength);
				m_rOwner.GetGameWorld().GetListObjs().emplace_back(
					new CFireDragon(m_rOwner.GetGameWorld()
						, m_rOwner.GetX() + m_rOwner.GetToX() * 130.f
						, m_rOwner.GetY() + m_rOwner.GetToY() * 130.f
						, m_pPlayer->GetX() - m_rOwner.GetX(), m_pPlayer->GetY() - m_rOwner.GetY()
						, m_eAttackType));
				m_pCamera->Shake(0.3f, 3.f, 5);
				switch (m_eAttackType)
				{
				case PLAYER::STATE_ATTACK1:
					m_eAttackType = PLAYER::STATE_ATTACK2;
					break;
				case PLAYER::STATE_ATTACK2:
					m_eAttackType = PLAYER::STATE_ATTACK1;
					break;
				default:
					break;
				}
				m_fElapsedTime = 0.f;
				m_iCount--;
			}
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Idle(m_rOwner));
		}
		return 1;
	}
	return 0;
}

void CMiddleBossState_FireDragon::LateUpdate(void)
{
}

void CMiddleBossState_FireDragon::OnExited(void)
{
}

void CMiddleBossState_FireDragon::SetAttackDirection(float * _pLength)
{
	float fToX = m_pPlayer->GetX() - m_rOwner.GetX();
	float fToY = m_pPlayer->GetY() - m_rOwner.GetY();
	if (_pLength) *_pLength = GetVectorLength(fToX, fToY);
	m_rOwner.SetToXY(fToX, fToY);
}