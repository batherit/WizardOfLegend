#include "stdafx.h"
#include "CArcherState_Idle.h"
#include "CMonster_Archer.h"
#include "CStateMgr.h"
#include "CArcherState_Run.h"


CArcherState_Idle::CArcherState_Idle(CMonster_Archer& _rOwner)
	:
	CState(_rOwner)
{
}


CArcherState_Idle::~CArcherState_Idle()
{
}

void CArcherState_Idle::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(ARCHER::STATE_IDLE);
	m_rOwner.SetSpeed(0.f);
}

int CArcherState_Idle::Update(float _fDeltaTime)
{
	if (m_rOwner.GetTarget()) {
		// TODO : ArcherState를 만들어야 한다.
		m_rOwner.GetStateMgr()->SetNextState(new CArcherState_Run(m_rOwner));
	}

	m_rOwner.UpdateAnim(_fDeltaTime);
	return 0;
}

void CArcherState_Idle::LateUpdate(void)
{
}
