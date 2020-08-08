#pragma once
#include "CState.h"

class CBoss_Boss;

class CBossState_JumpAttack_Upgrade :
	public CState<CBoss_Boss>
{
public:
	CBossState_JumpAttack_Upgrade(CBoss_Boss& _rOwner);
	virtual ~CBossState_JumpAttack_Upgrade();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	enum E_STATE { STATE_READY, STATE_JUMP, STATE_FALLING, STATE_ATTACK, STATE_END };

private:
	E_STATE m_eState = STATE_READY;
	float m_fTickTime = 0.f;
	float m_fInitX = 0.f;
	float m_fInitY = 0.f;
	float m_fFallX = 0.f;
	float m_fFallY = 0.f;
	CObj* m_pTempWallCollider = nullptr;
};


