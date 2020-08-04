#include "stdafx.h"
#include "CMiddleBossState_IceCrystal.h"
#include "CBoss_MiddleBoss.h"
#include "CIceCrystal.h"
#include "CStateMgr.h"
#include "CMiddleBossState_Idle.h"




CMiddleBossState_IceCrystal::CMiddleBossState_IceCrystal(CBoss_MiddleBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CMiddleBossState_IceCrystal::~CMiddleBossState_IceCrystal()
{
}

void CMiddleBossState_IceCrystal::OnLoaded(void)
{
	_anim_info stAnimInfo;
	stAnimInfo.iState = 3;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 2.5f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 4;
	m_rOwner.SetNewAnimInfo(stAnimInfo);
}

int CMiddleBossState_IceCrystal::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		// TODO : 여기에 아이스 크리스탈 스킬 생성해주면 됩니더ㅓㅓㄷㄷ..
		m_rOwner.GetGameWorld().GetListObjs().emplace_back(
			new CIceCrystal(m_rOwner.GetGameWorld(), m_rOwner.GetX() - 300, m_rOwner.GetY() - 300));
		m_rOwner.GetGameWorld().GetListObjs().emplace_back(
			new CIceCrystal(m_rOwner.GetGameWorld(), m_rOwner.GetX() + 300, m_rOwner.GetY() - 300));
		m_rOwner.GetGameWorld().GetListObjs().emplace_back(
			new CIceCrystal(m_rOwner.GetGameWorld(), m_rOwner.GetX(), m_rOwner.GetY() + 300));
		m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Idle(m_rOwner));
		return 1;
	}
	return 0;
}

void CMiddleBossState_IceCrystal::LateUpdate(void)
{
}

void CMiddleBossState_IceCrystal::OnExited(void)
{
}

int CMiddleBossState_IceCrystal::AlwaysUpdate(float _fDeltaTime)
{
	return 0;
}
