#pragma once
#include "CState.h"

class CBoss_MiddleBoss;

class CMiddleBossState_IceCrystal :
	public CState<CBoss_MiddleBoss>
{
public:
	CMiddleBossState_IceCrystal(CBoss_MiddleBoss& _rOwner);
	virtual ~CMiddleBossState_IceCrystal();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
	virtual int AlwaysUpdate(float _fDeltaTime) override;
};

