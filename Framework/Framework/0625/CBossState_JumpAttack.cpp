#include "stdafx.h"
#include "CBossState_JumpAttack.h"
#include "CBoss_Boss.h"
#include "CStateMgr.h"
#include "CBossState_Idle.h"
#include "CCamera2D.h"
#include "CPlayerWOL.h"


CBossState_JumpAttack::CBossState_JumpAttack(CBoss_Boss& _rOwner)
	:
	CState(_rOwner)
{
}


CBossState_JumpAttack::~CBossState_JumpAttack()
{
}

void CBossState_JumpAttack::OnLoaded(void)
{
	m_rOwner.ClearBox();
	m_rOwner.SetBoxAttackUsing(false);

	_anim_info stAnimInfo;
	stAnimInfo.iState = 3;
	stAnimInfo.fTotalTime = 0.5f;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iFrameCount = 1;
	stAnimInfo.iStartFrameIndex = 0;
	m_rOwner.SetNewAnimInfo(stAnimInfo);

	m_fInitX = m_rOwner.GetX();
	m_fInitY = m_rOwner.GetY();
}

int CBossState_JumpAttack::Update(float _fDeltaTime)
{
	switch (m_eState) {
	case STATE_READY:
		if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
			m_eState = STATE_JUMP;
			m_rOwner.GetAnimInfo().iStartFrameIndex = 1;
			m_pTempWallCollider = m_rOwner.SetCollider(nullptr, COLLIDER::TYPE_WALL);

			_anim_info stAnimInfo;
			stAnimInfo.iState = 3;
			stAnimInfo.fTotalTime = 0.5f;
			stAnimInfo.iCountToRepeat = 1;
			stAnimInfo.iFrameCount = 1;
			stAnimInfo.iStartFrameIndex = 1;
			m_rOwner.SetNewAnimInfo(stAnimInfo);
		}
		break;
	case STATE_JUMP: {
		m_rOwner.UpdateAnim(_fDeltaTime);
		m_fTickTime += _fDeltaTime;

		if (m_fTickTime >= 0.5f) {
			m_rOwner.SetX(m_rOwner.GetTarget()->GetX());
			m_rOwner.SetY(m_rOwner.GetTarget()->GetY() - (WINCY >> 1) - 2000);
			m_fTickTime = 0.f;

			_anim_info stAnimInfo;
			stAnimInfo.fTotalTime = 0.5f;
			stAnimInfo.iCountToRepeat = 1;
			stAnimInfo.iFrameCount = 2;
			stAnimInfo.iStartFrameIndex = 2;
			stAnimInfo.iState = 3;
			m_rOwner.SetNewAnimInfo(stAnimInfo);

			m_fFallY = m_rOwner.GetTarget()->GetY();			// 떨어질 지점의 y
			m_fInitY = m_fFallY - (WINCY >> 1) - 2000;			// 초기 지점의 y
			
			m_rOwner.SetX(m_rOwner.GetTarget()->GetX());		// X는 Target의 X

			m_eState = STATE_FALLING;
			return 0;
		}

		float fT = m_fTickTime / 0.2f;
		Clamp(&fT, 0.f, 1.f);
		m_rOwner.SetY(m_fInitY * (1.f - fT) + (m_fInitY - (WINCY >> 1) - 2000) * fT);
	}
		break;
	case STATE_FALLING: {
		if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
			_anim_info stAnimInfo;
			stAnimInfo.fTotalTime = 1.0f;
			stAnimInfo.iCountToRepeat = 1;
			stAnimInfo.iFrameCount = 4;
			stAnimInfo.iStartFrameIndex = 4;
			stAnimInfo.iState = 3;
			m_rOwner.SetNewAnimInfo(stAnimInfo);

			m_rOwner.SetCollider(m_pTempWallCollider, COLLIDER::TYPE_WALL);

			m_eState = STATE_ATTACK;

			float fLength = GetVectorLength(m_rOwner.GetX() - m_rOwner.GetTarget()->GetX(), m_rOwner.GetY() - m_rOwner.GetTarget()->GetY());
			if (fLength <= 400.f) {
				TO_PLAYER_WOL(m_rOwner.GetTarget())->Attacked(40,
					POINT{ static_cast<LONG>(m_rOwner.GetTarget()->GetX())
					, static_cast<LONG>(m_rOwner.GetTarget()->GetY()) });
			}

			m_rOwner.GetGameWorld().GetCamera()->Shake(1.f, 9.f, 20);

			return 0;
		}

		float fT = m_rOwner.GetAnimProgress();
		m_rOwner.SetY(m_fInitY * (1.f - fT) + m_fFallY * fT);
	}
		break;

	case STATE_ATTACK: {
		if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
			if ((m_fTickTime += _fDeltaTime) >= 0.5f) {
				m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
			}
			return 1;
		}


	}
	}
	return 0;
}

void CBossState_JumpAttack::LateUpdate(void)
{
}

void CBossState_JumpAttack::OnExited(void)
{
	if(m_pTempWallCollider) m_rOwner.SetCollider(m_pTempWallCollider, COLLIDER::TYPE_WALL);
}
