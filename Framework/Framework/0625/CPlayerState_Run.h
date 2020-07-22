#pragma once
#include "CState.h"

class CPlayerWOL;

class CPlayerState_Run :
	public CState<CPlayerWOL>
{
public:
	CPlayerState_Run(CPlayerWOL& _rOwner);
	virtual ~CPlayerState_Run();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
};

