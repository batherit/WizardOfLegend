#pragma once
#include "CState.h"

class CBoss_MiddleBoss;

class CMiddleBossState_Fire :
	public CState<CBoss_MiddleBoss>
{
public:
	CMiddleBossState_Fire(CBoss_MiddleBoss& _rOwner);
	virtual ~CMiddleBossState_Fire();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	

	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;

private:
	bool bAttackOk = false;
	int m_iCount = 30;
	float m_fPeriod = 0.12f;
	float m_fElapsedTime = 0.f;
};

