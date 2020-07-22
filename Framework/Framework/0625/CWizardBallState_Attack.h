#pragma once
#include "CState.h"

class CMonster_WizardBall;

class CWizardBallState_Attack :
	public CState<CMonster_WizardBall>
{
public:
	CWizardBallState_Attack(CMonster_WizardBall& _rOwner);
	virtual ~CWizardBallState_Attack();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	float m_fElapsedTime = 0.f;
	bool m_bIsAttacking = true;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

