#include "stdafx.h"
#include "CWizardState_Death.h"
#include "CMonster_Wizard.h"
#include "CCoin.h"



CWizardState_Death::CWizardState_Death(CMonster_Wizard & _rOwner)
	:
	CState(_rOwner)
{
}

CWizardState_Death::~CWizardState_Death()
{
}

void CWizardState_Death::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(WIZARD::STATE_DEATH);

	int iRandDashSountIndex = rand() % 3;
	if (iRandDashSountIndex == 0)			CSoundMgr::Get_Instance()->PlaySound(TEXT("ENEMY_DIED.mp3"), CSoundMgr::MONSTER);
	else if (iRandDashSountIndex == 1)		CSoundMgr::Get_Instance()->PlaySound(TEXT("ENEMY_DIED_2.mp3"), CSoundMgr::MONSTER);
	else if (iRandDashSountIndex == 2)		CSoundMgr::Get_Instance()->PlaySound(TEXT("ENEMY_DIED_3.mp3"), CSoundMgr::MONSTER);
}

int CWizardState_Death::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		m_rOwner.SetValid(false);
		TO_WOL(m_rOwner.GetGameWorld()).GetListParticles().emplace_back(
			new CCoin(m_rOwner.GetGameWorld(),
				m_rOwner.GetX(), m_rOwner.GetY() + (m_rOwner.GetHeight() >> 1),
				8 + GetNumberMinBetweenMax(-3, 2))
		);
		
		return 1;
	}

	return 0;
}

void CWizardState_Death::LateUpdate(void)
{
}

void CWizardState_Death::OnExited(void)
{
}
