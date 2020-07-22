#pragma once
#include "CState.h"

class CMonster_SwordMan;

class CSwordManState_Spawn :
	public CState<CMonster_SwordMan>
{
public:
	CSwordManState_Spawn(CMonster_SwordMan& _rOwner);
	virtual ~CSwordManState_Spawn();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
};

