#include "stdafx.h"
#include "CIceCrystalSkillState.h"
#include "CBitmapMgr.h"
#include "CPlayerWOL.h"
#include "CIceCrystal.h"



CIceCrystalSkillState::CIceCrystalSkillState(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
	m_fEndCooltime = 5.0f;
	m_fCurCooltime = m_fEndCooltime;
	m_hDCStateIcon = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ICE_CRYSTAL_SKILLBAR"));
}

CIceCrystalSkillState::~CIceCrystalSkillState()
{
}

void CIceCrystalSkillState::OnLoaded(void)
{
	_anim_info stAnimInfo;
	switch (m_rOwner.GetLastAttackState()) {
	case PLAYER::STATE_ATTACK1:
		stAnimInfo.iState = PLAYER::STATE_ATTACK1;
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.1f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		m_rOwner.SetLastAttackState(PLAYER::STATE_ATTACK2);
		break;
	case PLAYER::STATE_ATTACK2:
		stAnimInfo.iState = PLAYER::STATE_ATTACK2;
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.1f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		m_rOwner.SetLastAttackState(PLAYER::STATE_ATTACK1);
		break;
	}
	m_rOwner.SetNewAnimInfo(stAnimInfo);

	// TODO : 여기에 아이스 크리스탈 스킬 생성해주면 됩니더ㅓㅓㄷㄷ..
	TO_WOL(m_rOwner.GetGameWorld()).GetListUsedPlayerSkills().emplace_back(
		new CIceCrystal(m_rOwner.GetGameWorld(), &m_rOwner));

	// 쿨타임 초기화
	m_fCurCooltime = 0.f;
}

int CIceCrystalSkillState::Update(float _fDeltaTime)
{
	// 애니메이션만 발동하고 딱히 하는 건 없음;;^ㅅ^V
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CIceCrystalSkillState::LateUpdate(void)
{
}

void CIceCrystalSkillState::OnExited(void)
{
}

int CIceCrystalSkillState::AlwaysUpdate(float _fDeltaTime)
{
	if ((m_fCurCooltime += (_fDeltaTime)) > m_fEndCooltime) m_fCurCooltime = m_fEndCooltime;
	return 0;
}