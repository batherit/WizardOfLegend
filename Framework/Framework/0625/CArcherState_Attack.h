#pragma once
#include "CState.h"

class CMonster_Archer;

class CArcherState_Attack :
	public CState<CMonster_Archer>
{
public:
	CArcherState_Attack(CMonster_Archer& _rOwner);
	virtual ~CArcherState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	bool bAttackOk = false;

	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
};

