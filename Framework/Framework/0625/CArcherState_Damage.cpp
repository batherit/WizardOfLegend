#include "stdafx.h"
#include "CArcherState_Damage.h"
#include "CMonster_Archer.h"
#include "CStateMgr.h"
#include "CArcherState_Run.h"
#include "CArcherState_Idle.h"
#include "CArcherState_Death.h"



CArcherState_Damage::CArcherState_Damage(CMonster_Archer & _rOwner, POINT _ptCollisionPoint)
	:
	CState(_rOwner),
	m_ptCollisionPoint(_ptCollisionPoint)
{
}

CArcherState_Damage::~CArcherState_Damage()
{
}

void CArcherState_Damage::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(ARCHER::STATE_DAMAGE, true);
	m_rOwner.SetToXY(m_rOwner.GetX() - m_ptCollisionPoint.x, m_rOwner.GetY() - m_ptCollisionPoint.y);
}

int CArcherState_Damage::Update(float _fDeltaTime)
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
			// TODO : 아쳐의 죽음 상태를 만들 것
			m_rOwner.GetStateMgr()->SetNextState(new CArcherState_Death(m_rOwner));
		}
		else if (m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CArcherState_Run(m_rOwner));
		}
		else
			m_rOwner.GetStateMgr()->SetNextState(new CArcherState_Idle(m_rOwner));
	}
	return 0;
}

void CArcherState_Damage::LateUpdate(void)
{
}
