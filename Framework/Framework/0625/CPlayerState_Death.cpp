#include "stdafx.h"
#include "CPlayerState_Death.h"
#include "CPlayerWOL.h"


CPlayerState_Death::CPlayerState_Death(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
}

CPlayerState_Death::~CPlayerState_Death()
{
}

void CPlayerState_Death::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(PLAYER::STATE_DEATH);
}

int CPlayerState_Death::Update(float _fDeltaTime)
{
	m_rOwner.UpdateAnim(_fDeltaTime);

	return 0;
}

void CPlayerState_Death::LateUpdate(void)
{
}