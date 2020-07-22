#include "stdafx.h"
#include "CFireDragonSkillState.h"
#include "CCamera2D.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CFireDragon.h"



CFireDragonSkillState::CFireDragonSkillState(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
	m_pCamera = TO_WOL(m_rOwner.GetGameWorld()).GetCamera();
}

CFireDragonSkillState::~CFireDragonSkillState()
{
	m_pCamera = nullptr;
}

void CFireDragonSkillState::OnLoaded(void)
{

	float fLength = 0;
	SetAttackDirection(&fLength);
	
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
		new CFireDragon(m_rOwner.GetGameWorld()
			, m_rOwner.GetX() + m_rOwner.GetToX() * cfPlayerNormalAttackDist
			, m_rOwner.GetY() + m_rOwner.GetToY() * cfPlayerNormalAttackDist
			, m_rOwner.GetToX(), m_rOwner.GetToY()
			, m_rOwner.GetLastAttackState()));
	m_pCamera->Shake(0.3f, 6.f, 5);
}

int CFireDragonSkillState::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		float fLength = 0;
		SetAttackDirection(&fLength);

		_anim_info stAnimInfo;
		switch (m_rOwner.GetLastAttackState()) {
		case PLAYER::STATE_ATTACK1:
			stAnimInfo.iState = PLAYER::STATE_ATTACK1;
			stAnimInfo.iCountToRepeat = 1;
			stAnimInfo.fTotalTime = 0.12f;
			stAnimInfo.iStartFrameIndex = 0;
			stAnimInfo.iFrameCount = 8;
			m_rOwner.SetLastAttackState(PLAYER::STATE_ATTACK2);
			break;
		case PLAYER::STATE_ATTACK2:
			stAnimInfo.iState = PLAYER::STATE_ATTACK2;
			stAnimInfo.iCountToRepeat = 1;
			stAnimInfo.fTotalTime = 0.12f;
			stAnimInfo.iStartFrameIndex = 0;
			stAnimInfo.iFrameCount = 8;
			m_rOwner.SetLastAttackState(PLAYER::STATE_ATTACK1);
			break;
		}
		m_rOwner.SetNewAnimInfo(stAnimInfo);

		TO_WOL(m_rOwner.GetGameWorld()).GetListUsedPlayerSkills().emplace_back(
			new CFireDragon(m_rOwner.GetGameWorld()
				, m_rOwner.GetX() + m_rOwner.GetToX() * cfPlayerNormalAttackDist
				, m_rOwner.GetY() + m_rOwner.GetToY() * cfPlayerNormalAttackDist
				, m_rOwner.GetToX(), m_rOwner.GetToY()
				, m_rOwner.GetLastAttackState()));
		m_pCamera->Shake(0.3f, 6.f, 5);
	}
	return 0;
}

void CFireDragonSkillState::LateUpdate(void)
{
}

void CFireDragonSkillState::SetAttackDirection(float * _pLength)
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

void CFireDragonSkillState::OnExited(void)
{
}
