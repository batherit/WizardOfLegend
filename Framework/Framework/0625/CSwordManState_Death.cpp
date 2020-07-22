#include "stdafx.h"
#include "CSwordManState_Death.h"
#include "CMonster_SwordMan.h"



CSwordManState_Death::CSwordManState_Death(CMonster_SwordMan & _rOwner)
	:
	CState(_rOwner)
{
}

CSwordManState_Death::~CSwordManState_Death()
{
}

void CSwordManState_Death::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(SWORDMAN::STATE_DEATH);
}

int CSwordManState_Death::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		m_rOwner.SetValid(false);
		return 1;
	}

	return 0;
}

void CSwordManState_Death::LateUpdate(void)
{
}

void CSwordManState_Death::OnExited(void)
{
}
