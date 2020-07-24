#include "stdafx.h"
#include "CPlayerState_Damage.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CPlayerState_Run.h"
#include "CPlayerState_Idle.h"
#include "CPlayerState_Death.h"


CPlayerState_Damage::CPlayerState_Damage(CPlayerWOL & _rOwner, POINT _ptCollisionPoint)
	:
	CState(_rOwner),
	m_ptCollisionPoint(_ptCollisionPoint)
{
}

CPlayerState_Damage::~CPlayerState_Damage()
{
	
}

void CPlayerState_Damage::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(PLAYER::STATE_DAMAGE, true);
	m_rOwner.SetToXY(m_rOwner.GetX() - m_ptCollisionPoint.x, m_rOwner.GetY() - m_ptCollisionPoint.y);
}

int CPlayerState_Damage::Update(float _fDeltaTime)
{
	if (m_rOwner.GetAnimProgress() >= 0.0f) {
		float fT = (m_rOwner.GetAnimProgress() - 0.0f) / 1.0f;
		m_rOwner.SetSpeed(cfPlayerKnockbackSpeed * (1.f - fT) + 0.f * fT);
	}
	m_rOwner.MoveByDeltaTime(_fDeltaTime);

	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		m_rOwner.UpdateSkillKey();
		float fNewToX = 0.f;
		float fNewToY = 0.f;

		if (m_rOwner.IsDead()) {
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Death(m_rOwner));
		}
		else if (m_rOwner.IsMoveKeyPressed(fNewToX, fNewToY)) {
			m_rOwner.SetToXY(fNewToX, fNewToY);
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Run(m_rOwner));
		}
		else
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Idle(m_rOwner));
	}
	return 0;
}

void CPlayerState_Damage::LateUpdate(void)
{
}

void CPlayerState_Damage::OnExited(void)
{
}
