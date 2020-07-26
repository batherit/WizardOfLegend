#pragma once
#include "CState.h"

class CBoss_MiddleBoss;

class CMiddleBossState_FireDragon :
	public CState<CBoss_MiddleBoss>
{
public:
	CMiddleBossState_FireDragon(CBoss_MiddleBoss& _rOwner);
	virtual ~CMiddleBossState_FireDragon();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	// CState을(를) 통해 상속됨
	virtual void OnExited(void) override;

private:
	void SetAttackDirection(float * _pLength);

private:
	CCamera2D* m_pCamera = nullptr;
	int m_iCount = 30;
	float m_fPeriod = 0.12f;
	float m_fElapsedTime = 0.f;
	PLAYER::E_STATE m_eAttackType = PLAYER::STATE_ATTACK1;
	CObj* m_pPlayer = nullptr;
};

