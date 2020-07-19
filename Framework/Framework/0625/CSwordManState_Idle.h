#pragma once
#include "CState.h"

class CMonster_SwordMan;

class CSwordManState_Idle :
	public CState<CMonster_SwordMan>
{
public:
	CSwordManState_Idle(CMonster_SwordMan& _rOwner);
	virtual ~CSwordManState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

