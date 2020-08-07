#pragma once
#include "CState.h"

class CBoss_Boss;

class CBossState_BoxAttack :
	public CState<CBoss_Boss>
{
public:
	CBossState_BoxAttack(CBoss_Boss& _rOwner);
	virtual ~CBossState_BoxAttack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	float m_fElapsedTime = 0.f;
	bool m_bIsBoxGenerated = false;
};

