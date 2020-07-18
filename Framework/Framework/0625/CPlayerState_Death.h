#pragma once
#include "CState.h"

class CPlayerWOL;

class CPlayerState_Death :
	public CState<CPlayerWOL>
{
public:
	CPlayerState_Death(CPlayerWOL& _rOwner);
	virtual ~CPlayerState_Death();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

