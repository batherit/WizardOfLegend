#pragma once
#include "CState.h"

class CBoss_MiddleBoss;

class CMiddleBossState_Idle :
	public CState<CBoss_MiddleBoss>
{
public:
	CMiddleBossState_Idle(CBoss_MiddleBoss& _rOwner);
	virtual ~CMiddleBossState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;

private:
	float m_fBruisingTime = 0.f;
	float m_fElapsedTime = 0.f;
};

