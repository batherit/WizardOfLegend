#include "stdafx.h"
#include "CWizardState_Idle.h"
#include "CMonster_Wizard.h"
#include "CStateMgr.h"
#include "CWizardState_Run.h"



CWizardState_Idle::CWizardState_Idle(CMonster_Wizard & _rOwner)
	:
	CState(_rOwner)
{
}

CWizardState_Idle::~CWizardState_Idle()
{
	
}

void CWizardState_Idle::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(WIZARD::STATE_IDLE);
	m_rOwner.SetSpeed(0.f);
}

int CWizardState_Idle::Update(float _fDeltaTime)
{
	if (m_rOwner.GetTarget()) {
		// TODO : WizardState를 만들어야 한다.
		m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Run(m_rOwner));
	}

	m_rOwner.UpdateAnim(_fDeltaTime);
	return 0;
}

void CWizardState_Idle::LateUpdate(void)
{
}
