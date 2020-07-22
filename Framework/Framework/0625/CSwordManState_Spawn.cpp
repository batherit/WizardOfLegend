#include "stdafx.h"
#include "CSwordManState_Spawn.h"
#include "CMonster_SwordMan.h"
#include "CStateMgr.h"
#include "CSwordManState_Idle.h"



CSwordManState_Spawn::CSwordManState_Spawn(CMonster_SwordMan & _rOwner)
	:
	CState(_rOwner)
{
}

CSwordManState_Spawn::~CSwordManState_Spawn()
{
}

void CSwordManState_Spawn::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(SWORDMAN::STATE_IDLE);
}

int CSwordManState_Spawn::Update(float _fDeltaTime)
{
	if(!m_rOwner.IsSpawning())
		m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Idle(m_rOwner));
	return 0;
}

void CSwordManState_Spawn::LateUpdate(void)
{
}

void CSwordManState_Spawn::OnExited(void)
{
}

