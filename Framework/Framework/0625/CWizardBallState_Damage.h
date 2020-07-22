#pragma once
#include "CState.h"

class CMonster_WizardBall;

class CWizardBallState_Damage :
	public CState<CMonster_WizardBall>
{
public:
	CWizardBallState_Damage(CMonster_WizardBall& _rOwner, POINT _ptCollisionPoint);
	virtual ~CWizardBallState_Damage();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	POINT m_ptCollisionPoint;
};

