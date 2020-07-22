#include "stdafx.h"
#include "CSwordManState_Idle.h"
#include "CSwordManState_Run.h"
#include "CMonster_SwordMan.h"
#include "CStateMgr.h"


CSwordManState_Idle::CSwordManState_Idle(CMonster_SwordMan& _rOwner)
	:
	CState(_rOwner)
{
}


CSwordManState_Idle::~CSwordManState_Idle()
{
}

void CSwordManState_Idle::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(SWORDMAN::STATE_IDLE);
	m_rOwner.SetSpeed(0.f);
}

int CSwordManState_Idle::Update(float _fDeltaTime)
{
	if (m_rOwner.GetTarget()) {
		m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Run(m_rOwner));
	}

	m_rOwner.UpdateAnim(_fDeltaTime);
	return 0;
}

void CSwordManState_Idle::LateUpdate(void)
{
}

void CSwordManState_Idle::OnExited(void)
{
}
