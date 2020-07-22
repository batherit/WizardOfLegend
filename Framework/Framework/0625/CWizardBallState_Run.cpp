#include "stdafx.h"
#include "CWizardBallState_Run.h"
#include "CMonster_WizardBall.h"
#include "CStateMgr.h"
#include "CWizardBallState_Idle.h"
#include "CWizardBallState_Aiming.h"



CWizardBallState_Run::CWizardBallState_Run(CMonster_WizardBall & _rOwner)
	:
	CState(_rOwner)
{
}

CWizardBallState_Run::~CWizardBallState_Run()
{
}

void CWizardBallState_Run::OnLoaded(void)
{
	_anim_info stAnimInfo;
	// WIZARD_BALL.bmp를 참고할 것.
	stAnimInfo.iState = 0;
	stAnimInfo.iCountToRepeat = 0;
	stAnimInfo.fTotalTime = 0.f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 1;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(WIZARD_BALL_RUN_SPEED);
}

int CWizardBallState_Run::Update(float _fDeltaTime)
{
	if (m_rOwner.IsAttackable()) {
		m_rOwner.GetStateMgr()->SetNextState(new CWizardBallState_Aiming(m_rOwner));
	}
	else {
		if (!m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CWizardBallState_Idle(m_rOwner));
		}
		else m_rOwner.GoToTarget(_fDeltaTime);
	}

	m_rOwner.UpdateAnim(_fDeltaTime);

	return 0;
}

void CWizardBallState_Run::LateUpdate(void)
{
}

void CWizardBallState_Run::OnExited(void)
{
}
