#include "stdafx.h"
#include "CBossState_Idle.h"
#include "CBoss_Boss.h"
#include "CWOL_World.h"
#include "CBossState_Death.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CStateMgr.h"
#include "CBossState_BoxAttack.h"
#include "CBossState_StonePillar.h"
#include "CBossState_JumpAttack.h"
#include "CBossState_BoxAttack_Upgrade.h"
#include "CBossState_JumpAttack_Upgrade.h"


CBossState_Idle::CBossState_Idle(CBoss_Boss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Idle::~CBossState_Idle()
{
}

void CBossState_Idle::OnLoaded(void)
{
	m_fBruisingTime = GetNumberMinBetweenMax(0.5f, 1.f);
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

int CBossState_Idle::Update(float _fDeltaTime)
{
	if ((m_fElapsedTime += _fDeltaTime) > m_fBruisingTime) {
		int iSkillIndex = rand() % 3;
		if (m_rOwner.GetHP() >= m_rOwner.GetMaxHp() * 0.5f) {
			if (iSkillIndex == 0)
				m_rOwner.GetStateMgr()->SetNextState(new CBossState_BoxAttack(m_rOwner));
			else if(iSkillIndex == 1)
				m_rOwner.GetStateMgr()->SetNextState(new CBossState_StonePillar(m_rOwner));
			else if(iSkillIndex == 2)
				m_rOwner.GetStateMgr()->SetNextState(new CBossState_JumpAttack(m_rOwner));
		}
		else {
			if (iSkillIndex == 0)
				m_rOwner.GetStateMgr()->SetNextState(new CBossState_BoxAttack_Upgrade(m_rOwner));
			else if (iSkillIndex == 1)
				m_rOwner.GetStateMgr()->SetNextState(new CBossState_StonePillar(m_rOwner));
			else if (iSkillIndex == 2)
				m_rOwner.GetStateMgr()->SetNextState(new CBossState_JumpAttack_Upgrade(m_rOwner));
		}
	}

	m_rOwner.UpdateAnim(_fDeltaTime);
	return 0;
}

void CBossState_Idle::LateUpdate(void)
{
}

void CBossState_Idle::OnExited(void)
{
}
