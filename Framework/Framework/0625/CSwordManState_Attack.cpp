#include "stdafx.h"
#include "CSwordManState_Attack.h"
#include "CSwordManState_Idle.h"
#include "CSwordManState_Run.h"
#include "CMonster_SwordMan.h"
#include "CStateMgr.h"
#include "CSwordManAttack.h"



CSwordManState_Attack::CSwordManState_Attack(CMonster_SwordMan & _rOwner)
	:
	CState(_rOwner)
{
}

CSwordManState_Attack::~CSwordManState_Attack()
{
}

void CSwordManState_Attack::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(SWORDMAN::STATE_ATTACK, true);
	m_rOwner.DirectDirectionToTarget();
}

int CSwordManState_Attack::Update(float _fDeltaTime)
{
	if (!bAttackOk && m_rOwner.GetAnimProgress() >= 0.5f) {
		TO_WOL(m_rOwner.GetGameWorld()).GetListUsedMonsterSkills().emplace_back(
			new CSwordManAttack(m_rOwner.GetGameWorld()
				, m_rOwner.GetX() + m_rOwner.GetToX() * cfSwordManNormalAttackDist
				, m_rOwner.GetY() + m_rOwner.GetToY() * cfSwordManNormalAttackDist
				, m_rOwner.GetToX(), m_rOwner.GetToY(), m_rOwner.GetSwordManDir()));
		bAttackOk = true;
	}
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		if (m_rOwner.GetTarget()) {
			if (m_rOwner.IsAttackable()) {
				m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Attack(m_rOwner));
			}
			else {
				m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Run(m_rOwner));
			}
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CSwordManState_Idle(m_rOwner));
		}
	}

	return 0;
}

void CSwordManState_Attack::LateUpdate(void)
{
}

void CSwordManState_Attack::OnExited(void)
{
}
