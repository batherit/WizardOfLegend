#pragma once
#include "CState.h"

class CMonster_WizardBall;

class CWizardBallState_Attack :
	public CState<CMonster_WizardBall>
{
public:
	CWizardBallState_Attack(CMonster_WizardBall& _rOwner);
	virtual ~CWizardBallState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	float m_fElapsedTime = 0.f;
	bool m_bIsAttacking = true;
};

