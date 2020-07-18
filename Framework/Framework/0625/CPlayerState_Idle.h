#pragma once
#include "CState.h"

class CPlayerWOL;

class CPlayerState_Idle :
	public CState<CPlayerWOL>
{
public:
	CPlayerState_Idle(CPlayerWOL& _rOwner);
	virtual ~CPlayerState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

