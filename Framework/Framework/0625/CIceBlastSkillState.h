#pragma once
#include "CState.h"

class CPlayerWOL;

class CIceBlastSkillState :
	public CState<CPlayerWOL>
{
public:
	CIceBlastSkillState(CPlayerWOL& _rOwner);
	virtual ~CIceBlastSkillState();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;
	virtual int AlwaysUpdate(float _fDeltaTime) override;
};

