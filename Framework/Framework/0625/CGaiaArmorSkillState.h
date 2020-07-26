#pragma once
#include "CState.h"

class CPlayerWOL;

class CGaiaArmorSkillState :
	public CState<CPlayerWOL>
{
public:
	CGaiaArmorSkillState(CPlayerWOL& _rOwner);
	virtual ~CGaiaArmorSkillState();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
	virtual int AlwaysUpdate(float _fDeltaTime) override;
};

