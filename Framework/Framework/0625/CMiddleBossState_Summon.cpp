#include "stdafx.h"
#include "CMiddleBossState_Summon.h"
#include "CBoss_MiddleBoss.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CMiddleBossState_Idle.h"
#include "CMonsterSpawner.h"
#include "CStateMgr.h"


CMiddleBossState_Summon::CMiddleBossState_Summon(CBoss_MiddleBoss& _rOwner)
	:
	CState(_rOwner)
{
}

CMiddleBossState_Summon::~CMiddleBossState_Summon()
{
}

void CMiddleBossState_Summon::OnLoaded(void)
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

	if (m_rOwner.GetSummonedMonstersNum() == 0) {
		SPAWN::E_TYPE eTypeToSpawn = static_cast<SPAWN::E_TYPE>(rand() % (SPAWN::TYPE_END - 1));
		m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetSpawners()->emplace_back(
			new CMonsterSpawner(m_rOwner.GetGameWorld(), *(m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetMonsters()), 0.f, m_rOwner.GetX() - 80, m_rOwner.GetY(), eTypeToSpawn, -1, nullptr, &m_rOwner));
		m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetSpawners()->emplace_back(
			new CMonsterSpawner(m_rOwner.GetGameWorld(), *(m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetMonsters()), 0.f, m_rOwner.GetX(), m_rOwner.GetY() + 80, eTypeToSpawn, -1, nullptr, &m_rOwner));
		m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetSpawners()->emplace_back(
			new CMonsterSpawner(m_rOwner.GetGameWorld(), *(m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetMonsters()), 0.f, m_rOwner.GetX() + 80, m_rOwner.GetY(), eTypeToSpawn, -1, nullptr, &m_rOwner));
	}
}

int CMiddleBossState_Summon::Update(float _fDeltaTime)
{
	m_rOwner.DirectDirectionToTarget();

	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Idle(m_rOwner));
	}

	return 0;
}

void CMiddleBossState_Summon::LateUpdate(void)
{
}

void CMiddleBossState_Summon::OnExited(void)
{
}
