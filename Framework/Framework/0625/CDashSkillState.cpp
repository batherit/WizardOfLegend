#include "stdafx.h"
#include "CDashSkillState.h"
#include "CPlayerWOL.h"
#include "CBitmapMgr.h"



CDashSkillState::CDashSkillState(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
	
	m_hDCState[STATE_HDC::STATE_HDC_SKILLBAR] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("DASH_SKILLBAR"));
	m_hDCState[STATE_HDC::STATE_HDC_ICON] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("DASH_ICON"));
	m_hDCState[STATE_HDC::STATE_HDC_DESC] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("DASH_EX"));
}

CDashSkillState::~CDashSkillState()
{
}

void CDashSkillState::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(PLAYER::STATE_DASH);
	m_rOwner.SetSpeed(cfPlayerDashSpeed);
}

int CDashSkillState::Update(float _fDeltaTime)
{
	if (m_rOwner.GetToX() == 0.f && m_rOwner.GetToY() == 0.f) {
		OBJ::E_DIRECTION _eDir = m_rOwner.GetDirType();
		m_rOwner.SetToXY(cfDeltaX[_eDir], cfDeltaY[_eDir]);
	}

	// 감속 조정
	if (m_rOwner.GetAnimProgress() >= 0.4f) {
		float fT = (m_rOwner.GetAnimProgress() - 0.4f) / 0.6f;
		m_rOwner.SetSpeed(cfPlayerDashSpeed * (1.f - fT) + 0.f * fT);
	}
	m_rOwner.MoveByDeltaTime(_fDeltaTime);

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CDashSkillState::LateUpdate(void)
{
}

void CDashSkillState::OnExited(void)
{
}
