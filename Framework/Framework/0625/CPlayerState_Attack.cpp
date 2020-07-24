#include "stdafx.h"
#include "CPlayerState_Attack.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CPlayerState_Run.h"
#include "CPlayerState_Idle.h"
#include "CPlayerState_Dash.h"
#include "CCamera2D.h"
#include "CPlayerNormalAttack.h"



CPlayerState_Attack::CPlayerState_Attack(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
}

CPlayerState_Attack::~CPlayerState_Attack()
{
}

void CPlayerState_Attack::OnLoaded(void)
{
	m_rOwner.GetUsingSkill()->OnLoaded();
}

int CPlayerState_Attack::Update(float _fDeltaTime)
{
	if (m_rOwner.GetUsingSkill()->Update(_fDeltaTime) == 1) {
		float fNewToX = 0.f;
		float fNewToY = 0.f;

		if (m_rOwner.IsMoveKeyPressed(fNewToX, fNewToY)) {
			m_rOwner.SetToXY(fNewToX, fNewToY);
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Run(m_rOwner));
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Idle(m_rOwner));
		}
		return 1;
	}
	return 0;
}

void CPlayerState_Attack::LateUpdate(void)
{
	m_rOwner.GetUsingSkill()->LateUpdate();
}

void CPlayerState_Attack::OnExited(void)
{
	m_rOwner.GetUsingSkill()->OnExited();
}
