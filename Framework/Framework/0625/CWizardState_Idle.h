#pragma once
#include "CState.h"

class CMonster_Wizard;

class CWizardState_Idle :
	public CState<CMonster_Wizard>
{
public:
	CWizardState_Idle(CMonster_Wizard& _rOwner);
	virtual ~CWizardState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

