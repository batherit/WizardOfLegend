#pragma once
#include "CState.h"

class CMonster_Archer;

class CArcherState_Attack :
	public CState<CMonster_Archer>
{
public:
	CArcherState_Attack(CMonster_Archer& _rOwner);
	virtual ~CArcherState_Attack();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	bool bAttackOk = false;
};

