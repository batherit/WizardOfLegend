#pragma once
#include "CState.h"

class CBoss_Boss;
class CCamera2D;

class CBossState_Idle :
	public CState<CBoss_Boss>
{
public:
	CBossState_Idle(CBoss_Boss& _rOwner);
	virtual ~CBossState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;

private:
	CCamera2D* m_pCamera = nullptr;
};

