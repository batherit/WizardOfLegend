#include "stdafx.h"
#include "CPlayerState_Run.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CPlayerState_Idle.h"
#include "CPlayerState_Dash.h"
#include "CPlayerState_Attack.h"


CPlayerState_Run::CPlayerState_Run(CPlayerWOL& _rOwner)
	:
	CState(_rOwner)
{
}


CPlayerState_Run::~CPlayerState_Run()
{
	
}

void CPlayerState_Run::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(PLAYER::STATE_RUN);
	m_rOwner.SetSpeed(cfPlayerRunSpeed);
}

int CPlayerState_Run::Update(float _fDeltaTime)
{
	float fNewToX = 0.f;
	float fNewToY = 0.f;

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_SPACE)) {
		m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Dash(m_rOwner));
	}
	
	/*if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
		m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Attack(m_rOwner));
	}*/
	m_rOwner.UpdateSkillKey();

	if (m_rOwner.IsMoveKeyPressed(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		if (fNewToY != 0.f)
			m_rOwner.SetDirType((fNewToY > 0.f ? OBJ::DIR_DOWN : OBJ::DIR_UP));
		if (fNewToX != 0.f)
			m_rOwner.SetDirType((fNewToX > 0.f ? OBJ::DIR_RIGHT : OBJ::DIR_LEFT));
	}
	else {
		m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Idle(m_rOwner));
	}
	
	m_rOwner.UpdateAnim(_fDeltaTime);

	return 0;
}

void CPlayerState_Run::LateUpdate(void)
{
}
