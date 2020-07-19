#include "stdafx.h"
#include "CPlayerState_Spawn.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CPlayerState_Idle.h"


CPlayerState_Spawn::CPlayerState_Spawn(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
}

CPlayerState_Spawn::~CPlayerState_Spawn()
{
}

void CPlayerState_Spawn::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(PLAYER::STATE_IDLE);
}

int CPlayerState_Spawn::Update(float _fDeltaTime)
{
	if(!m_rOwner.IsSpawning())
		m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Idle(m_rOwner));

	return 0;
}

void CPlayerState_Spawn::LateUpdate(void)
{
}
