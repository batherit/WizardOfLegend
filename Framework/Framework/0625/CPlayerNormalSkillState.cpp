#include "stdafx.h"
#include "CPlayerNormalSkillState.h"
#include "CWOL_World.h"
#include "CCamera2D.h"
#include "CPlayerWOL.h"
#include "CPlayerNormalAttack.h"
#include "CStateMgr.h"
#include "CPlayerState_Dash.h"
#include "CPlayerState_Run.h"
#include "CPlayerState_Idle.h"
#include "CBitmapMgr.h"



CPlayerNormalSkillState::CPlayerNormalSkillState(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
	m_iComboCount = 0;
	m_pCamera = TO_WOL(m_rOwner.GetGameWorld()).GetCamera();
	m_hDCState[STATE_HDC::STATE_HDC_SKILLBAR] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("NORMAL_ATTACK_SKILLBAR"));
	m_hDCState[STATE_HDC::STATE_HDC_ICON] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("NORMAL_ATTACK_ICON"));
	m_hDCState[STATE_HDC::STATE_HDC_DESC] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("NORMAL_ATTACK_EX"));
}

CPlayerNormalSkillState::~CPlayerNormalSkillState()
{
	m_pCamera = nullptr;
}

void CPlayerNormalSkillState::OnLoaded(void)
{
	if (m_iComboCount >= 3) return;

	float fLength = 0;
	SetAttackDirection(&fLength);
	if (fLength >= 100) m_fPlayerAttackSpeed = cfPlayerAttackSpeed;
	else m_fPlayerAttackSpeed = 0.f;

	switch (m_rOwner.GetLastAttackState()) {
	case PLAYER::STATE_ATTACK1:
		m_rOwner.SetNewStateAnim(PLAYER::STATE_ATTACK2);
		break;
	case PLAYER::STATE_ATTACK2:
		m_rOwner.SetNewStateAnim(PLAYER::STATE_ATTACK1);
		break;
	}

	TO_WOL(m_rOwner.GetGameWorld()).GetListUsedPlayerSkills().emplace_back(
		new CPlayerNormalAttack(m_rOwner.GetGameWorld()
			, m_rOwner.GetX() + m_rOwner.GetToX() * cfPlayerNormalAttackDist
			, m_rOwner.GetY() + m_rOwner.GetToY() * cfPlayerNormalAttackDist
			, m_rOwner.GetToX(), m_rOwner.GetToY()
			, m_rOwner.GetLastAttackState()));

	m_iComboCount++;
}

int CPlayerNormalSkillState::Update(float _fDeltaTime)
{
	//if (m_iComboCount < 3)
	

	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		m_iComboCount = 0;
		return 1;
	}
m_rOwner.UpdateSkillKey();
	if (m_rOwner.GetAnimProgress() >= 0.0f) {
		float fT = (m_rOwner.GetAnimProgress() - 0.0f) / 1.0f;
		m_rOwner.SetSpeed(m_fPlayerAttackSpeed * (1.f - fT) + 0.f * fT);
	}
	m_rOwner.MoveByDeltaTime(_fDeltaTime);

	return 0;
}

void CPlayerNormalSkillState::LateUpdate(void)
{
}


void CPlayerNormalSkillState::SetAttackDirection(float * _pLength)
{
	POINT ptCursorPoint = GetClientCursorPoint();
	pair<float, float> pairCursorPoint = m_pCamera->GetWorldPoint(ptCursorPoint.x, ptCursorPoint.y);

	float fToX = pairCursorPoint.first - m_rOwner.GetX();
	float fToY = pairCursorPoint.second - m_rOwner.GetY();
	if (_pLength) *_pLength = GetVectorLength(fToX, fToY);
	m_rOwner.SetToXY(fToX, fToY);

	float fDegree = GetPositiveDegreeByVector(m_rOwner.GetToX(), m_rOwner.GetToY());
	m_rOwner.SetDirType(GetDirByDegree(fDegree));
}

void CPlayerNormalSkillState::OnExited(void)
{
	m_iComboCount = 0;
}
