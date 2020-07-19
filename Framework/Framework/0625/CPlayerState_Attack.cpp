#include "stdafx.h"
#include "CPlayerState_Attack.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CPlayerState_Run.h"
#include "CPlayerState_Idle.h"
#include "CPlayerState_Dash.h"
#include "CCamera2D.h"



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

	SetAttackDirection();
		
	m_rOwner.SetSpeed(0.f);
	switch (m_rOwner.GetLastAttackState()) {
	case PLAYER::STATE_ATTACK1:
		m_rOwner.SetNewStateAnim(PLAYER::STATE_ATTACK2);
		break;
	case PLAYER::STATE_ATTACK2:
		m_rOwner.SetNewStateAnim(PLAYER::STATE_ATTACK1);
		break;
	}
}

int CPlayerState_Attack::Update(float _fDeltaTime)
{
	if (m_iComboCount < 3) {
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
			SetAttackDirection();
			switch (m_rOwner.GetLastAttackState()) {
			case PLAYER::STATE_ATTACK1:
				m_rOwner.SetNewStateAnim(PLAYER::STATE_ATTACK2);
				break;
			case PLAYER::STATE_ATTACK2:
				m_rOwner.SetNewStateAnim(PLAYER::STATE_ATTACK1);
				break;
			}
			m_iComboCount++;
		}
	}

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

void CPlayerState_Attack::SetAttackDirection(void)
{
	POINT ptCursorPoint = GetClientCursorPoint();
	pair<float, float> pairCursorPoint = m_pCamera->GetWorldPoint(ptCursorPoint.x, ptCursorPoint.y);

	float fToX = pairCursorPoint.first - m_rOwner.GetX();
	float fToY = pairCursorPoint.second - m_rOwner.GetY();
	m_rOwner.SetToXY(fToX, fToY);

	float fDegree = GetPositiveRadianByVector(m_rOwner.GetToX(), m_rOwner.GetToY());
	m_rOwner.SetDirType(GetDirByDegree(fDegree));
}
