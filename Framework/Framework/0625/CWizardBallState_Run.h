#pragma once
#include "CState.h"

class CMonster_WizardBall;

class CWizardBallState_Run :
	public CState<CMonster_WizardBall>
{
public:
	CWizardBallState_Run(CMonster_WizardBall& _rOwner);
	virtual ~CWizardBallState_Run();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

