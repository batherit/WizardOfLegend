#pragma once
#include "CState.h"

class CMonster_SwordMan;

class CSwordManState_Attack :
	public CState<CMonster_SwordMan>
{
public:
	CSwordManState_Attack(CMonster_SwordMan& _rOwner);
	virtual ~CSwordManState_Attack();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	bool bAttackOk = false;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

