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
	m_fBruisingTime = GetNumberMinBetweenMax(1.f, 2.f);
	m_fElapsedTime = 0.f;

	m_rOwner.SetNewStateAnim(WIZARD::STATE_IDLE);
	m_rOwner.SetSpeed(0.f);
}

int CWizardState_Idle::Update(float _fDeltaTime)
{
	if ((m_fElapsedTime += _fDeltaTime) > m_fBruisingTime) {
		if (m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Run(m_rOwner));
		}
	}

	m_rOwner.UpdateAnim(_fDeltaTime);
	return 0;
}

void CWizardState_Idle::LateUpdate(void)
{
}

void CWizardState_Idle::OnExited(void)
{
}
