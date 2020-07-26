#include "stdafx.h"
#include "CWizardState_Attack.h"
#include "CMonster_Wizard.h"
#include "CStateMgr.h"
#include "CWizardState_Run.h"
#include "CWizardState_Idle.h"
#include "CMonster_Wizard.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CMonsterSpawner.h"
#include "CWizardFire.h"



CWizardState_Attack::CWizardState_Attack(CMonster_Wizard & _rOwner)
	:
	CState(_rOwner)
{
}

CWizardState_Attack::~CWizardState_Attack()
{
}

void CWizardState_Attack::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(WIZARD::STATE_ATTACK, true);
	m_rOwner.DirectDirectionToTarget();
	
	if (m_rOwner.GetSummonedWizardBalls() == 0) {
		m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetSpawners()->emplace_back(
			new CMonsterSpawner(m_rOwner.GetGameWorld(), *(m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetMonsters()), 0.f, m_rOwner.GetX() - 80, m_rOwner.GetY(), SPAWN::TYPE_WIZARDBALL, -1, nullptr, &m_rOwner));
		m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetSpawners()->emplace_back(
			new CMonsterSpawner(m_rOwner.GetGameWorld(), *(m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetMonsters()), 0.f, m_rOwner.GetX(), m_rOwner.GetY() + 80, SPAWN::TYPE_WIZARDBALL, -1, nullptr, &m_rOwner));
		m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetSpawners()->emplace_back(
			new CMonsterSpawner(m_rOwner.GetGameWorld(), *(m_rOwner.GetGameWorld().GetSceneManager()->GetCurScene()->GetMonsters()), 0.f, m_rOwner.GetX() + 80, m_rOwner.GetY(), SPAWN::TYPE_WIZARDBALL, -1, nullptr, &m_rOwner));
	}
	else {
		TO_WOL(m_rOwner.GetGameWorld()).GetListUsedMonsterSkills().emplace_back(
			new CWizardFire(
				m_rOwner.GetGameWorld(),
				m_rOwner.GetX(), m_rOwner.GetY(),
				m_rOwner.GetToX(), m_rOwner.GetToY()));
	}
}

int CWizardState_Attack::Update(float _fDeltaTime)
{
	m_rOwner.DirectDirectionToTarget();

	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Idle(m_rOwner));
	}

	return 0;
}

void CWizardState_Attack::LateUpdate(void)
{
}

void CWizardState_Attack::OnExited(void)
{
}
