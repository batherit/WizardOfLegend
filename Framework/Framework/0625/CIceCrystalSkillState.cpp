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
	m_hDCState[STATE_HDC::STATE_HDC_SKILLBAR] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ICE_CRYSTAL_SKILLBAR"));
	m_hDCState[STATE_HDC::STATE_HDC_ICON] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ICE_CRYSTAL_ICON"));
	m_hDCState[STATE_HDC::STATE_HDC_DESC] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ICE_CRYSTAL_EX"));
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

	// TODO : ���⿡ ���̽� ũ����Ż ��ų �������ָ� �˴ϴ��ää���..
	TO_WOL(m_rOwner.GetGameWorld()).GetListUsedPlayerSkills().emplace_back(
		new CIceCrystal(m_rOwner.GetGameWorld(), &m_rOwner));
	
	// ��Ÿ�� �ʱ�ȭ
	m_fCurCooltime = 0.f;
}

int CIceCrystalSkillState::Update(float _fDeltaTime)
{
	// �ִϸ��̼Ǹ� �ߵ��ϰ� ���� �ϴ� �� ����;;^��^V
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