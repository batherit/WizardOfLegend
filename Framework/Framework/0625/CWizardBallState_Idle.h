#pragma once
#include "CState.h"

class CMonster_WizardBall;

class CWizardBallState_Idle :
	public CState<CMonster_WizardBall>
{
public:
	CWizardBallState_Idle(CMonster_WizardBall& _rOwner);
	virtual ~CWizardBallState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
};

