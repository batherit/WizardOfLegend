#include "stdafx.h"
#include "CArcherState_Run.h"
#include "CMonster_Archer.h"
#include "CArcherState_Idle.h"
#include "CArcherState_Attack.h"
#include "CStateMgr.h"



CArcherState_Run::CArcherState_Run(CMonster_Archer & _rOwner)
	:
	CState(_rOwner)
{
}

CArcherState_Run::~CArcherState_Run()
{
}

void CArcherState_Run::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(ARCHER::STATE_RUN);
	m_rOwner.SetSpeed(ARCHER_RUN_SPEED);
}

int CArcherState_Run::Update(float _fDeltaTime)
{
	if (m_rOwner.IsAttackable()) {
		m_rOwner.GetStateMgr()->SetNextState(new CArcherState_Attack(m_rOwner));
	}
	else {
		if (!m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CArcherState_Idle(m_rOwner));
		}
		else m_rOwner.GoToAttackableLocation(_fDeltaTime);
	}

	m_rOwner.UpdateAnim(_fDeltaTime);

	return 0;
}

void CArcherState_Run::LateUpdate(void)
{
}
