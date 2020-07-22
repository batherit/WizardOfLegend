#include "stdafx.h"
#include "CArcherState_Death.h"
#include "CMonster_Archer.h"
#include "CCoin.h"


CArcherState_Death::CArcherState_Death(CMonster_Archer & _rOwner)
	:
	CState(_rOwner)
{
}

CArcherState_Death::~CArcherState_Death()
{
}

void CArcherState_Death::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(ARCHER::STATE_DEATH);
}

int CArcherState_Death::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		m_rOwner.SetValid(false);
		TO_WOL(m_rOwner.GetGameWorld()).GetListParticles().emplace_back(
			new CCoin(m_rOwner.GetGameWorld(),
				m_rOwner.GetX(), m_rOwner.GetY() + (m_rOwner.GetHeight() >> 1),
				m_rOwner.GetMoney())
		);
		return 1;
	}

	return 0;
}

void CArcherState_Death::LateUpdate(void)
{
}

void CArcherState_Death::OnExited(void)
{
}
