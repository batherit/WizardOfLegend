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
	m_rOwner.ConfirmValidSkill();
}

int CPlayerState_Attack::Update(float _fDeltaTime)
{
	//if (!m_rOwner.GetUsingSkill()) return 1;
	if (m_rOwner.GetUsingSkill()->Update(_fDeltaTime) == 1) {
		float fNewToX = 0.f;
		float fNewToY = 0.f;
		//m_rOwner.m_debug = true;
		//m_rOwner.SetUsingSkill(nullptr);
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

//bool CPlayerState_Attack::IsMutable(void)
//{
//	return false;
//}

void CPlayerState_Attack::OnExited(void)
{
	// 완전히 다른 상태로 전이.
	m_rOwner.GetUsingSkill()->OnExited();
	m_rOwner.SetUsingSkill(nullptr);
	//m_rOwner.SetNextSkill(nullptr);
	//m_rOwner.SetSkillConfirmed(true);
}
