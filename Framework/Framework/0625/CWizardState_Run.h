#pragma once
#include "CState.h"

class CMonster_Wizard;

class CWizardState_Run :
	public CState<CMonster_Wizard>
{
public:
	CWizardState_Run(CMonster_Wizard& _rOwner);
	virtual ~CWizardState_Run();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
};

