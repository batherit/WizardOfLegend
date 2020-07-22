#include "stdafx.h"
#include "CWizardBallState_Idle.h"
#include "CMonster_WizardBall.h"
#include "CStateMgr.h"
#include "CWizardBallState_Run.h"


CWizardBallState_Idle::CWizardBallState_Idle(CMonster_WizardBall & _rOwner)
	:
	CState(_rOwner)
{
}

CWizardBallState_Idle::~CWizardBallState_Idle()
{
}

void CWizardBallState_Idle::OnLoaded(void)
{
	_anim_info stAnimInfo;

	// WIZARD_BALL.bmp를 참고할 것.
	stAnimInfo.iState = 0;
	stAnimInfo.iCountToRepeat = 0;
	stAnimInfo.fTotalTime = 0.f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 1;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
}

int CWizardBallState_Idle::Update(float _fDeltaTime)
{
	if (m_rOwner.GetTarget()) {
		// TODO : WizardState를 만들어야 한다.
		m_rOwner.GetStateMgr()->SetNextState(new CWizardBallState_Run(m_rOwner));
	}

	m_rOwner.UpdateAnim(_fDeltaTime);
	return 0;
}

void CWizardBallState_Idle::LateUpdate(void)
{
}

void CWizardBallState_Idle::OnExited(void)
{
}
