#include "stdafx.h"
#include "CMiddleBossState_Idle.h"
#include "CBoss_MiddleBoss.h"
#include "CStateMgr.h"
#include "CMiddleBossState_Run.h"



CMiddleBossState_Idle::CMiddleBossState_Idle(CBoss_MiddleBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CMiddleBossState_Idle::~CMiddleBossState_Idle()
{
}

void CMiddleBossState_Idle::OnLoaded(void)
{
	m_fBruisingTime = GetNumberMinBetweenMax(1.f, 2.f);
	m_fElapsedTime = 0.f;

	_anim_info stAnimInfo;
	stAnimInfo.iState = 0;
	stAnimInfo.fTotalTime = 0.f;
	stAnimInfo.iFrameCount = 1;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iCountToRepeat = 0;

	m_rOwner.SetNewAnimInfo(stAnimInfo);

	m_rOwner.SetSpeed(0.f);
}

int CMiddleBossState_Idle::Update(float _fDeltaTime)
{
	if ((m_fElapsedTime += _fDeltaTime) > m_fBruisingTime) {
		if (m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Run(m_rOwner));
		}
	}

	m_rOwner.UpdateAnim(_fDeltaTime);
	return 0;
}

void CMiddleBossState_Idle::LateUpdate(void)
{
}

void CMiddleBossState_Idle::OnExited(void)
{
}
