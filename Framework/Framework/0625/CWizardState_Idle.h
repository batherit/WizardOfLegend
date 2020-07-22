#pragma once
#include "CState.h"

class CMonster_Wizard;

class CWizardState_Idle :
	public CState<CMonster_Wizard>
{
public:
	CWizardState_Idle(CMonster_Wizard& _rOwner);
	virtual ~CWizardState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

