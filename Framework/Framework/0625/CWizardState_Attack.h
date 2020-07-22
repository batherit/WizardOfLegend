#pragma once
#include "CState.h"

class CMonster_Wizard;

class CWizardState_Attack :
	public CState<CMonster_Wizard>
{
public:
	CWizardState_Attack(CMonster_Wizard& _rOwner);
	virtual ~CWizardState_Attack();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	bool bAttackOk = false;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

