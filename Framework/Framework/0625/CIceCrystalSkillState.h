#pragma once
#include "CState.h"

class CPlayerWOL;

class CIceCrystalSkillState :
	public CState<CPlayerWOL>
{
public:
	CIceCrystalSkillState(CPlayerWOL& _rOwner);
	virtual ~CIceCrystalSkillState();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
	virtual int AlwaysUpdate(float _fDeltaTime) override;
};

