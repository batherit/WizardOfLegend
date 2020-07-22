#pragma once
#include "CState.h"

class CMonster_SwordMan;

class CSwordManState_Death :
	public CState<CMonster_SwordMan>
{
public:
	CSwordManState_Death(CMonster_SwordMan& _rOwner);
	virtual ~CSwordManState_Death();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

