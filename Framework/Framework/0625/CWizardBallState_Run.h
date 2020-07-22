#pragma once
#include "CState.h"

class CMonster_WizardBall;

class CWizardBallState_Run :
	public CState<CMonster_WizardBall>
{
public:
	CWizardBallState_Run(CMonster_WizardBall& _rOwner);
	virtual ~CWizardBallState_Run();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

