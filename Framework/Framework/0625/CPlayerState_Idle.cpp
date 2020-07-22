#include "stdafx.h"
#include "CPlayerState_Idle.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CPlayerState_Run.h"
#include "CPlayerState_Dash.h"
#include "CPlayerState_Attack.h"


CPlayerState_Idle::CPlayerState_Idle(CPlayerWOL& _rOwner)
	:
	CState(_rOwner)
{

}


CPlayerState_Idle::~CPlayerState_Idle()
{
}

void CPlayerState_Idle::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(PLAYER::STATE_IDLE);
	m_rOwner.SetSpeed(0.f);
}

int CPlayerState_Idle::Update(float _fDeltaTime)
{
	float fNewToX = 0.f;
	float fNewToY = 0.f;

	if (m_rOwner.IsMoveKeyPressed(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		if (m_rOwner.GetToX() != 0.f || m_rOwner.GetToY() != 0.f)
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Run(m_rOwner));
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_SPACE)) {
		m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Dash(m_rOwner));
	}

	m_rOwner.UpdateSkillKey();
	/*if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
		m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Attack(m_rOwner));
	}*/

	m_rOwner.UpdateAnim(_fDeltaTime);

	return 0;
}

void CPlayerState_Idle::LateUpdate(void)
{
}
