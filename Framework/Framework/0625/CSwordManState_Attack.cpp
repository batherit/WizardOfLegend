#include "stdafx.h"
#include "CSwordManState_Attack.h"
#include "CSwordManState_Idle.h"
#include "CSwordManState_Run.h"
#include "CMonster_SwordMan.h"
#include "CStateMgr.h"



CSwordManState_Attack::CSwordManState_Attack(CMonster_SwordMan & _rOwner)
	:
	CState(_rOwner)
{
}

CSwordManState_Attack::~CSwordManState_Attack()
{
}

void CSwordManState_Attack::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(SWORDMAN::STATE_ATTACK, true);
	m_rOwner.DirectDirectionToTarget();
}

int CSwordManState_Attack::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		if (m_rOwner.GetTarget()) {
			if (m_rOwner.IsAttackable()) {
				m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Attack(m_rOwner));
			}
			else {
				m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Run(m_rOwner));
			}
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Idle(m_rOwner));
		}
	}

	return 0;
}

void CSwordManState_Attack::LateUpdate(void)
{
}
