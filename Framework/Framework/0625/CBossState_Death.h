#pragma once
#include "CState.h"

class CBoss_Boss;

class CBossState_Death :
	public CState<CBoss_Boss>
{
public:
	CBossState_Death(CBoss_Boss& _rOwner);
	virtual ~CBossState_Death();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

