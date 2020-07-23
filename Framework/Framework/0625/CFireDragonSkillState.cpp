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
	m_bCoolTimeStop = true;		// 쿨타임 업데이트를 잠시 멈춘다.
	
	_anim_info stAnimInfo;
	stAnimInfo.iState = 0;
	stAnimInfo.iCountToRepeat = 1;
	m_rOwner.SetNewAnimInfo(stAnimInfo); // 더미 애니메이션을 넣는다. => 사용가능횟수가 0일시 바로 스킬상태를 빠져나오게 하기 위함.

	if (static_cast<int> (m_fCoolTime) > 0) {
		float fLength = 0;
		SetAttackDirection(&fLength);

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

		m_fCoolTime -= 1.f;
	}
}

int CFireDragonSkillState::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		if (static_cast<int> (m_fCoolTime) > 0) {
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

			m_fCoolTime -= 1.f;
			return 0;
		}
		return 1;
	}
	return 0;
}

void CFireDragonSkillState::LateUpdate(void)
{
}

void CFireDragonSkillState::OnExited(void)
{
	m_bCoolTimeStop = false;
}

int CFireDragonSkillState::AlwaysUpdate(float _fDeltaTime)
{
	if(!m_bCoolTimeStop)
		if ((m_fCoolTime += (_fDeltaTime * 1.5f)) > 2.f) m_fCoolTime = 8.f;

	return 0;
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

