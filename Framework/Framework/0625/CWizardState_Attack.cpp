#include "stdafx.h"
#include "CWizardState_Attack.h"
#include "CMonster_Wizard.h"
#include "CStateMgr.h"
#include "CWizardState_Run.h"
#include "CWizardState_Idle.h"



CWizardState_Attack::CWizardState_Attack(CMonster_Wizard & _rOwner)
	:
	CState(_rOwner)
{
}

CWizardState_Attack::~CWizardState_Attack()
{
}

void CWizardState_Attack::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(WIZARD::STATE_ATTACK, true);
	m_rOwner.DirectDirectionToTarget();
}

int CWizardState_Attack::Update(float _fDeltaTime)
{
	m_rOwner.DirectDirectionToTarget();
	/*if (!bAttackOk && m_rOwner.GetAnimProgress() >= 0.75f) {
		TO_WOL(m_rOwner.GetGameWorld()).GetListUsedMonsterSkills().emplace_back(
			new CArcherArrow(
				m_rOwner.GetGameWorld(),
				m_rOwner.GetX(), m_rOwner.GetY(),
				m_rOwner.GetToX(), m_rOwner.GetToY(),
				m_rOwner.GetArcherDir()));
		bAttackOk = true;
	}*/
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {


		if (m_rOwner.GetTarget()) {
			if (m_rOwner.IsAttackable()) {
				m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Attack(m_rOwner));
			}
			else {
				m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Run(m_rOwner));
			}
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Idle(m_rOwner));
		}
	}

	return 0;
}

void CWizardState_Attack::LateUpdate(void)
{
}
