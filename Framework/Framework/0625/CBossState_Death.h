#pragma once
#include "CState.h"

class CBoss_Boss;

class CBossState_Death :
	public CState<CBoss_Boss>
{
public:
	CBossState_Death(CBoss_Boss& _rOwner);
	virtual ~CBossState_Death();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
};

