#include "stdafx.h"
#include "CGaiaArmorSkillState.h"
#include "CBitmapMgr.h"
#include "CPlayerWOL.h"
#include "CGaiaArmor.h"




CGaiaArmorSkillState::CGaiaArmorSkillState(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
	m_fEndCooltime = 14.0f;
	m_fCurCooltime = m_fEndCooltime;
	m_hDCState[STATE_HDC::STATE_HDC_SKILLBAR] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("GAIA_ARMOR_SKILLBAR"));
	m_hDCState[STATE_HDC::STATE_HDC_ICON] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("GAIA_ARMOR_ICON"));
	m_hDCState[STATE_HDC::STATE_HDC_DESC] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("GAIA_ARMOR_EX"));
	m_hDCState[STATE_HDC::STATE_HDC_CARD] = nullptr;
}

CGaiaArmorSkillState::~CGaiaArmorSkillState()
{
}

void CGaiaArmorSkillState::OnLoaded(void)
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

	TO_WOL(m_rOwner.GetGameWorld()).GetListUsedPlayerSkills().emplace_back(
		new CGaiaArmor(m_rOwner.GetGameWorld(), &m_rOwner));
	// ��Ÿ�� �ʱ�ȭ
	m_fCurCooltime = 0.f;
}

int CGaiaArmorSkillState::Update(float _fDeltaTime)
{
	m_rOwner.UpdateSkillKey();
	// �ִϸ��̼Ǹ� �ߵ��ϰ� ���� �ϴ� �� ����;;^��^V
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CGaiaArmorSkillState::LateUpdate(void)
{
}

void CGaiaArmorSkillState::OnExited(void)
{
}

int CGaiaArmorSkillState::AlwaysUpdate(float _fDeltaTime)
{
	if ((m_fCurCooltime += (_fDeltaTime)) > m_fEndCooltime) m_fCurCooltime = m_fEndCooltime;
	return 0;
}
