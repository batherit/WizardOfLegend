#pragma once
#include "CState.h"

class CMonster_SwordMan;

class CSwordManState_Spawn :
	public CState<CMonster_SwordMan>
{
public:
	CSwordManState_Spawn(CMonster_SwordMan& _rOwner);
	virtual ~CSwordManState_Spawn();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

