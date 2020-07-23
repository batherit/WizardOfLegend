#include "stdafx.h"
#include "CFireDragonSkillState.h"
#include "CCamera2D.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CFireDragon.h"
#include "CBitmapMgr.h"



CFireDragonSkillState::CFireDragonSkillState(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
	m_pCamera = TO_WOL(m_rOwner.GetGameWorld()).GetCamera();

	// 쿨타임 관련 세팅
	m_fEndCooltime = 8.f;
	m_fCurCooltime = m_fEndCooltime;
	m_eCooltimeType = SKILL_COOLTIME::TYPE_COUNT;
	m_hDCState[STATE_HDC::STATE_HDC_SKILLBAR] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("FIRE_DRAGON_SKILLBAR"));
	m_hDCState[STATE_HDC::STATE_HDC_ICON] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("FIRE_DRAGON_ICON"));
	m_hDCState[STATE_HDC::STATE_HDC_DESC] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("FIRE_DRAGON_EX"));
}

CFireDragonSkillState::~CFireDragonSkillState()
{
	m_pCamera = nullptr;
}

void CFireDragonSkillState::OnLoaded(void)
{
	m_bCoolTimeStop = true;		// 쿨타임 업데이트를 잠시 멈춘다.
	
	
	// IsMutable이 상태 사용 가능 여부를 체크하므로 OnLoaded는 항상 유효한 상태이다.

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

	m_fCurCooltime -= 1.f;
}

int CFireDragonSkillState::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		if (static_cast<int> (m_fCurCooltime) > 0) {
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

			m_fCurCooltime -= 1.f;
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
		// 기존 시간보다 3배빠르게 쿨타임이 채워지도록 함.
		if ((m_fCurCooltime += (_fDeltaTime * 3.0f)) > m_fEndCooltime) m_fCurCooltime = m_fEndCooltime;

	return 0;
}

bool CFireDragonSkillState::IsMutable(void)
{
	return static_cast<int>(m_fCurCooltime) > 0;
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

