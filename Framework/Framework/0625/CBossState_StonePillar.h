#pragma once
#include "CState.h"

class CBoss_Boss;

class CBossState_StonePillar :
	public CState<CBoss_Boss>
{
private:
	enum E_STATE { STATE_READY1, STATE_READY2, STATE_ATTACK, STATE_END };
public:
	CBossState_StonePillar(CBoss_Boss& _rOwner);
	virtual ~CBossState_StonePillar();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	E_STATE m_eState = STATE_READY1;
	float m_fElapsedTime = 0.f;
	float m_fTickTime = 0.f;
};

