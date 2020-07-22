#include "stdafx.h"
#include "CWizardBallState_Attack.h"
#include "CMonster_WizardBall.h"
#include "CStateMgr.h"
#include "CWizardBallState_Run.h"
#include "CWizardBallState_Idle.h"



CWizardBallState_Attack::CWizardBallState_Attack(CMonster_WizardBall & _rOwner)
	:
	CState(_rOwner)
{
}

CWizardBallState_Attack::~CWizardBallState_Attack()
{
}

void CWizardBallState_Attack::OnLoaded(void)
{
	m_rOwner.SetSpeed(WIZARD_BALL_ATTACK_SPEED);
}

int CWizardBallState_Attack::Update(float _fDeltaTime)
{
	if ((m_fElapsedTime += _fDeltaTime) >= 0.25f) {
		if (m_bIsAttacking) {
			_anim_info stAnimInfo;
			stAnimInfo.iState = m_rOwner.GetAnimInfo().iState;
			stAnimInfo.bIsReversePlay = true;
			stAnimInfo.fTotalTime = 0.7f;
			stAnimInfo.iCountToRepeat = 1;
			stAnimInfo.iStartFrameIndex = 3;
			stAnimInfo.iFrameCount = 4;
			m_rOwner.SetNewAnimInfo(stAnimInfo);
			m_rOwner.SetSpeed(WIZARD_BALL_BALLAST_SPEED);	
			m_fElapsedTime = 0.f;
			m_bIsAttacking = false;
		
			return 0;
		}
			
	}
	if (!m_bIsAttacking) {
		if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
			m_rOwner.GetAnimInfo().iState = 0;
			if ((m_fElapsedTime += _fDeltaTime) >= 2.6f) {
				if (m_rOwner.GetTarget()) {
					m_rOwner.GetStateMgr()->SetNextState(new CWizardBallState_Run(m_rOwner));
					return 1;
				}
				else {
					m_rOwner.GetStateMgr()->SetNextState(new CWizardBallState_Idle(m_rOwner));
					return 1;
				}
			}
		}
	}
	
	m_rOwner.MoveByDeltaTime(_fDeltaTime);
	return 0;
}

void CWizardBallState_Attack::LateUpdate(void)
{
}

void CWizardBallState_Attack::OnExited(void)
{
}
