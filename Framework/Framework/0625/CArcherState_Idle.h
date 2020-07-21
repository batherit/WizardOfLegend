#pragma once
#include "CState.h"

class CMonster_Archer;

class CArcherState_Idle :
	public CState<CMonster_Archer>
{
public:
	CArcherState_Idle(CMonster_Archer& _rOwner);
	virtual ~CArcherState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

