#pragma once
#include "CState.h"

class CMonster_Wizard;

class CWizardState_Damage :
	public CState<CMonster_Wizard>
{
public:
	CWizardState_Damage(CMonster_Wizard& _rOwner, POINT _ptCollisionPoint);
	virtual ~CWizardState_Damage();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	POINT m_ptCollisionPoint;
};

