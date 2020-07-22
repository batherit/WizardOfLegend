#include "stdafx.h"
#include "CWizardState_Run.h"
#include "CMonster_Wizard.h"
#include "CStateMgr.h"
#include "CWizardState_Idle.h"
#include "CWizardState_Attack.h"



CWizardState_Run::CWizardState_Run(CMonster_Wizard & _rOwner)
	:
	CState(_rOwner)
{
}

CWizardState_Run::~CWizardState_Run()
{
}

void CWizardState_Run::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(WIZARD::STATE_RUN);
	m_rOwner.SetSpeed(WIZARD_RUN_SPEED);
}

int CWizardState_Run::Update(float _fDeltaTime)
{
	if (m_rOwner.IsAttackable()) {
		m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Attack(m_rOwner));
	}
	else {
		if (!m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Idle(m_rOwner));
		}
		else m_rOwner.GoToAttackableLocation(_fDeltaTime);
	}

	m_rOwner.UpdateAnim(_fDeltaTime);

	return 0;
}

void CWizardState_Run::LateUpdate(void)
{
}

void CWizardState_Run::OnExited(void)
{
}
