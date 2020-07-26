#include "stdafx.h"
#include "CMiddleBossState_Fire.h"
#include "CBoss_MiddleBoss.h"
#include "CStateMgr.h"
#include "CMiddleBossState_Idle.h"
#include "CScene.h"
#include "CStateMgr.h"
#include "CSceneMgr.h"
#include "CMonsterSpawner.h"
#include "CWizardFire.h"


CMiddleBossState_Fire::CMiddleBossState_Fire(CBoss_MiddleBoss& _rOwner)
	:
	CState(_rOwner)
{
}


CMiddleBossState_Fire::~CMiddleBossState_Fire()
{
}

void CMiddleBossState_Fire::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	_anim_info stAnimInfo;
	stAnimInfo.iState = 2;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 2.f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 4;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.DirectDirectionToTarget();

	CObj* pPlayer = TO_WOL(m_rOwner.GetGameWorld()).GetPlayer();

	TO_WOL(m_rOwner.GetGameWorld()).GetListUsedMonsterSkills().emplace_back(
		new CWizardFire(
			m_rOwner.GetGameWorld(),
			pPlayer->GetX() - 300, pPlayer->GetY() - 300,
			300, 300));
	TO_WOL(m_rOwner.GetGameWorld()).GetListUsedMonsterSkills().emplace_back(
		new CWizardFire(
			m_rOwner.GetGameWorld(),
			pPlayer->GetX() + 300, pPlayer->GetY() - 300,
			-300, 300));
	TO_WOL(m_rOwner.GetGameWorld()).GetListUsedMonsterSkills().emplace_back(
		new CWizardFire(
			m_rOwner.GetGameWorld(),
			pPlayer->GetX() + 300, pPlayer->GetY() + 300,
			-300, -300));
	TO_WOL(m_rOwner.GetGameWorld()).GetListUsedMonsterSkills().emplace_back(
		new CWizardFire(
			m_rOwner.GetGameWorld(),
			pPlayer->GetX() - 300, pPlayer->GetY() + 300,
			300, -300));
}

int CMiddleBossState_Fire::Update(float _fDeltaTime)
{
	m_rOwner.DirectDirectionToTarget();

	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Idle(m_rOwner));
	}

	return 0;
}

void CMiddleBossState_Fire::LateUpdate(void)
{
}

void CMiddleBossState_Fire::OnExited(void)
{
}
