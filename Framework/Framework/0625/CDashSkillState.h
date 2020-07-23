#pragma once
#include "CState.h"

class CPlayerWOL;

class CDashSkillState :
	public CState<CPlayerWOL>
{
public:
	CDashSkillState(CPlayerWOL& _rOwner);
	virtual ~CDashSkillState();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

