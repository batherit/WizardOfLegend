#pragma once
#include "CState.h"

class CMonster_WizardBall;

class CWizardBallState_Aiming :
	public CState<CMonster_WizardBall>
{
public:
	CWizardBallState_Aiming(CMonster_WizardBall& _rOwner);
	virtual ~CWizardBallState_Aiming();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	float m_fElapsedTime = 0.f;

	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
};

