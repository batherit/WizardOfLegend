#pragma once
#include "CState.h"

class CMonster_SwordMan;

class CSwordManState_Damage :
	public CState<CMonster_SwordMan>
{
public:
	CSwordManState_Damage(CMonster_SwordMan& _rOwner, POINT _ptCollisionPoint);
	virtual ~CSwordManState_Damage();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	POINT m_ptCollisionPoint;
};

