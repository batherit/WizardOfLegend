#pragma once
#include "CState.h"

class CPlayerWOL;

class CDashSkillState :
	public CState<CPlayerWOL>
{
public:
	CDashSkillState(CPlayerWOL& _rOwner);
	virtual ~CDashSkillState();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
};

