#pragma once
#include "CState.h"

class CBoss_Boss;

class CBossState_StonePillar_Upgrade :
	public CState<CBoss_Boss>
{
public:
	CBossState_StonePillar_Upgrade(CBoss_Boss& _rOwner);
	virtual ~CBossState_StonePillar_Upgrade();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	enum E_STATE { STATE_READY1, STATE_READY2, STATE_ATTACK, STATE_END };

private:
	E_STATE m_eState = STATE_READY1;
	float m_fElapsedTime = 0.f;
	float m_fTickTime = 0.f;
};
