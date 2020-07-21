#pragma once
#include "CState.h"

class CMonster_Archer;

class CArcherState_Damage :
	public CState<CMonster_Archer>
{
public:
	CArcherState_Damage(CMonster_Archer& _rOwner, POINT _ptCollisionPoint);
	virtual ~CArcherState_Damage();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	POINT m_ptCollisionPoint;
};

