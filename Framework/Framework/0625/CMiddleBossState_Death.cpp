#include "stdafx.h"
#include "CMiddleBossState_Death.h"
#include "CBoss_MiddleBoss.h"
#include "CCoin.h"



CMiddleBossState_Death::CMiddleBossState_Death(CBoss_MiddleBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CMiddleBossState_Death::~CMiddleBossState_Death()
{
}

void CMiddleBossState_Death::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	
	_anim_info stAnimInfo;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 1.f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 4;
	stAnimInfo.iState = 5;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
	
	CSoundMgr::Get_Instance()->PlaySound(TEXT("WIN.mp3"), CSoundMgr::UI);
}

int CMiddleBossState_Death::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		m_rOwner.SetValid(false);
		TO_WOL(m_rOwner.GetGameWorld()).GetListParticles().emplace_back(
			new CCoin(m_rOwner.GetGameWorld(),
				m_rOwner.GetX(), m_rOwner.GetY() + (m_rOwner.GetHeight() >> 1),
				100 + GetNumberMinBetweenMax(-10, 10))
		);

		return 1;
	}

	return 0;
}

void CMiddleBossState_Death::LateUpdate(void)
{
}

void CMiddleBossState_Death::OnExited(void)
{
}
