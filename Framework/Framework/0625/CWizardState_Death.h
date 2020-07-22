#pragma once
#include "CState.h"

class CMonster_Wizard;

class CWizardState_Death :
	public CState<CMonster_Wizard>
{
public:
	CWizardState_Death(CMonster_Wizard& _rOwner);
	virtual ~CWizardState_Death();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

