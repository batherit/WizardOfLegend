#include "stdafx.h"
#include "CSwordManState_Run.h"
#include "CSwordManState_Idle.h"
#include "CSwordManState_Attack.h"
#include "CMonster_SwordMan.h"
#include "CStateMgr.h"



CSwordManState_Run::CSwordManState_Run(CMonster_SwordMan & _rOwner)
	:
	CState(_rOwner)
{
}

CSwordManState_Run::~CSwordManState_Run()
{
}

void CSwordManState_Run::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(SWORDMAN::STATE_RUN);
	m_rOwner.SetSpeed(cfSwordManRunSpeed);
}

int CSwordManState_Run::Update(float _fDeltaTime)
{
	if (m_rOwner.IsAttackable()) {
		m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Attack(m_rOwner));
	}
	else {
		if(!m_rOwner.GoToTarget(_fDeltaTime)) {
			m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Idle(m_rOwner));
		}
	}
	
	m_rOwner.UpdateAnim(_fDeltaTime);

	return 0;
}

void CSwordManState_Run::LateUpdate(void)
{
}

void CSwordManState_Run::OnExited(void)
{
}
