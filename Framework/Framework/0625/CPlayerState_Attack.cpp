#include "stdafx.h"
#include "CPlayerState_Attack.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CPlayerState_Run.h"
#include "CPlayerState_Idle.h"
#include "CPlayerState_Dash.h"
#include "CCamera2D.h"
#include "CPlayerNormalAttack.h"



CPlayerState_Attack::CPlayerState_Attack(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
}

CPlayerState_Attack::~CPlayerState_Attack()
{
	m_iComboCount = 0;
	m_pCamera = nullptr;
}

void CPlayerState_Attack::OnLoaded(void)
{
	m_iComboCount++;
	m_pCamera = TO_WOL(m_rOwner.GetGameWorld()).GetCamera();

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
}

int CPlayerState_Attack::Update(float _fDeltaTime)
{
	if (m_iComboCount < 3) {
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
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
	}
	// ���� ����
	if (m_rOwner.GetAnimProgress() >= 0.0f) {
		float fT = (m_rOwner.GetAnimProgress() - 0.0f) / 1.0f;
		m_rOwner.SetSpeed(m_fPlayerAttackSpeed * (1.f - fT) + 0.f * fT);
	}
	m_rOwner.MoveByDeltaTime(_fDeltaTime);

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_SPACE)) {
		m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Dash(m_rOwner));
	}


	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		float fNewToX = 0.f;
		float fNewToY = 0.f;

		if (m_rOwner.IsMoveKeyPressed(fNewToX, fNewToY)) {
			m_rOwner.SetToXY(fNewToX, fNewToY);
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Run(m_rOwner));
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Idle(m_rOwner));
		}
	}
	return 0;
}

void CPlayerState_Attack::LateUpdate(void)
{
}

void CPlayerState_Attack::SetAttackDirection(float* _pLength)
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
