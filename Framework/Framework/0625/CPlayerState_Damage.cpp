#include "stdafx.h"
#include "CPlayerState_Damage.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CPlayerState_Run.h"
#include "CPlayerState_Idle.h"
#include "CPlayerState_Death.h"


CPlayerState_Damage::CPlayerState_Damage(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
}

CPlayerState_Damage::~CPlayerState_Damage()
{
	
}

void CPlayerState_Damage::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewState(PLAYER::STATE_DAMAGE);
}

int CPlayerState_Damage::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		float fNewToX = 0.f;
		float fNewToY = 0.f;

		if (m_rOwner.IsDead()) {
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Death(m_rOwner));
		}
		else if (m_rOwner.IsMoveKeyPressed(fNewToX, fNewToY)) {
			m_rOwner.SetToXY(fNewToX, fNewToY);
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Run(m_rOwner));
		}
		else
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Idle(m_rOwner));
	}
	return 0;
}

void CPlayerState_Damage::LateUpdate(void)
{
}
