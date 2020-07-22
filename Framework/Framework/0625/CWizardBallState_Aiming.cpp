#include "stdafx.h"
#include "CWizardBallState_Aiming.h"
#include "CMonster_WizardBall.h"
#include "CWizardBallState_Attack.h"
#include "CStateMgr.h"



CWizardBallState_Aiming::CWizardBallState_Aiming(CMonster_WizardBall & _rOwner)
	:
	CState(_rOwner)
{
}

CWizardBallState_Aiming::~CWizardBallState_Aiming()
{
}

void CWizardBallState_Aiming::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.DirectDirectionToTarget();
	
	_anim_info stAnimInfo;
	stAnimInfo.iState = m_rOwner.GetSpriteIndex();
	stAnimInfo.fTotalTime = 0.8f;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iFrameCount = 4;
	stAnimInfo.iStartFrameIndex = 0;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
}

int CWizardBallState_Aiming::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		if ((m_fElapsedTime += _fDeltaTime) >= 1.5f) {
			m_rOwner.GetStateMgr()->SetNextState(new CWizardBallState_Attack(m_rOwner));
			return 1;
		}
		m_rOwner.DirectDirectionToTarget();
		m_rOwner.GetAnimInfo().iState = m_rOwner.GetSpriteIndex();
		return 0;
	}
	return 0;
}

void CWizardBallState_Aiming::LateUpdate(void)
{
}
