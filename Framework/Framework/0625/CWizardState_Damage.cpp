#include "stdafx.h"
#include "CWizardState_Damage.h"
#include "CMonster_Wizard.h"
#include "CStateMgr.h"
#include "CWizardState_Run.h"
#include "CWizardState_Idle.h"
#include "CWizardState_Death.h"



CWizardState_Damage::CWizardState_Damage(CMonster_Wizard & _rOwner, POINT _ptCollisionPoint)
	:
	CState(_rOwner),
	m_ptCollisionPoint(_ptCollisionPoint)
{
}

CWizardState_Damage::~CWizardState_Damage()
{
}

void CWizardState_Damage::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(WIZARD::STATE_DAMAGE, true);
	m_rOwner.SetToXY(m_rOwner.GetX() - m_ptCollisionPoint.x, m_rOwner.GetY() - m_ptCollisionPoint.y);

	int iRandSountIndex = rand() % 3;
	if (iRandSountIndex == 0)			CSoundMgr::Get_Instance()->PlaySound(TEXT("ENEMY_HITTED_WIND_1.mp3"), CSoundMgr::MONSTER);
	else if (iRandSountIndex == 1)		CSoundMgr::Get_Instance()->PlaySound(TEXT("ENEMY_HITTED_WIND_2.mp3"), CSoundMgr::MONSTER);
	else if (iRandSountIndex == 2)		CSoundMgr::Get_Instance()->PlaySound(TEXT("ENEMY_HITTED_WIND_3.mp3"), CSoundMgr::MONSTER);
}

int CWizardState_Damage::Update(float _fDeltaTime)
{
	if (m_rOwner.GetAnimProgress() >= 0.0f) {
		float fT = (m_rOwner.GetAnimProgress() - 0.0f) / 1.0f;
		m_rOwner.SetSpeed(cfSwordManKnockbackSpeed * (1.f - fT) + 0.f * fT);
	}
	m_rOwner.MoveByDeltaTime(_fDeltaTime);

	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		float fNewToX = 0.f;
		float fNewToY = 0.f;

		if (m_rOwner.IsDead()) {
			// TODO : 아쳐의 죽음 상태를 만들 것
			m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Death(m_rOwner));
		}
		else if (m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Run(m_rOwner));
		}
		else
			m_rOwner.GetStateMgr()->SetNextState(new CWizardState_Idle(m_rOwner));
	}
	return 0;
}

void CWizardState_Damage::LateUpdate(void)
{
}

void CWizardState_Damage::OnExited(void)
{
}
