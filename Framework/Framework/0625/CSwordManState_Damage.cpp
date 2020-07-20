#include "stdafx.h"
#include "CSwordManState_Damage.h"
#include "CMonster_SwordMan.h"
#include "CSwordManState_Run.h"
#include "CSwordManState_Idle.h"
#include "CSwordManState_Death.h"
#include "CStateMgr.h"



CSwordManState_Damage::CSwordManState_Damage(CMonster_SwordMan & _rOwner, POINT _ptCollisionPoint)
	:
	CState(_rOwner),
	m_ptCollisionPoint(_ptCollisionPoint)
{
}

CSwordManState_Damage::~CSwordManState_Damage()
{
}

void CSwordManState_Damage::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(SWORDMAN::STATE_DAMAGE, true);
	m_rOwner.SetToXY(m_rOwner.GetX() - m_ptCollisionPoint.x, m_rOwner.GetY() - m_ptCollisionPoint.y);
}

int CSwordManState_Damage::Update(float _fDeltaTime)
{
	if (m_rOwner.GetAnimProgress() >= 0.0f) {
		float fT = (m_rOwner.GetAnimProgress() - 0.0f) / 1.0f;
		m_rOwner.SetSpeed(cfSwordManKnockbackSpeed * (1.f - fT) + 0.f * fT);
	}
	m_rOwner.MoveByDeltaTime(_fDeltaTime);

	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		float fNewToX = 0.f;
		float fNewToY = 0.f;

		if (m_rOwner.IsDead()) {
			m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Death(m_rOwner));
		}
		else if (m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Run(m_rOwner));
		}
		else
			m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Idle(m_rOwner));
	}
	return 0;
}

void CSwordManState_Damage::LateUpdate(void)
{
}
