#include "stdafx.h"
#include "CMiddleBossState_Run.h"
#include "CMiddleBossState_Idle.h"
#include "CMiddleBossState_Summon.h"
#include "CBoss_MiddleBoss.h"
#include "CStateMgr.h"
#include "CMiddleBossState_Fire.h"
#include "CMiddleBossState_IceCrystal.h"
#include "CMiddleBossState_FireDragon.h"


CMiddleBossState_Run::CMiddleBossState_Run(CBoss_MiddleBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CMiddleBossState_Run::~CMiddleBossState_Run()
{
	
}

void CMiddleBossState_Run::OnLoaded(void)
{
	_anim_info stAnimInfo;
	stAnimInfo.iState = 1;
	stAnimInfo.iCountToRepeat = 0;
	stAnimInfo.fTotalTime = 1.5f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 4;

	m_rOwner.SetNewAnimInfo(stAnimInfo);


	m_rOwner.SetSpeed(MIDDLE_BOSS_RUN_SPEED);

	m_fDistAdjustmentTime = GetNumberMinBetweenMax(1.5f, 2.f);
	m_fElapsedTime = 0.f;

	float fRadian = GetNumberMinBetweenMax(0.f, TWO_PI);
	m_rOwner.SetToX(cosf(fRadian));
	m_rOwner.SetToY(sinf(fRadian));
}

int CMiddleBossState_Run::Update(float _fDeltaTime)
{
	
	if ((m_fElapsedTime += _fDeltaTime) > m_fDistAdjustmentTime) {

		if (m_rOwner.GetListMonsters().size() > 0) {
			if (rand() % 10 < 7) {
				int iRandAttack = rand() % 3;
				if (iRandAttack == 0) m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Fire(m_rOwner));
				else if (iRandAttack == 1) m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_FireDragon(m_rOwner));
				else m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_FireDragon(m_rOwner));
			}
			else m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Run(m_rOwner));
		}
		else {
			if(rand() % 10 < 3) m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_FireDragon(m_rOwner));
			else {
				int iRandAttack = rand() % 3;
				if (iRandAttack == 0) m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Fire(m_rOwner));
				else if (iRandAttack == 1) m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_FireDragon(m_rOwner));
				else if (iRandAttack == 2) m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_FireDragon(m_rOwner));
			}
		}
	}
	else {
		if (!m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Idle(m_rOwner));
		}
		else m_rOwner.RunToProperLocation(_fDeltaTime);
	}

	m_rOwner.UpdateAnim(_fDeltaTime);

	return 0;
}

void CMiddleBossState_Run::LateUpdate(void)
{
}

void CMiddleBossState_Run::OnExited(void)
{
}
