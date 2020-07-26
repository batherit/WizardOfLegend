#pragma once
#include "CState.h"

class CBoss_MiddleBoss;

class CMiddleBossState_Run :
	public CState<CBoss_MiddleBoss>
{
public:
	CMiddleBossState_Run(CBoss_MiddleBoss& _rOwner);
	virtual ~CMiddleBossState_Run();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;

private:
	float m_fDistAdjustmentTime = 0.f;
	float m_fElapsedTime = 0.f;
	float m_fToX = 0.f;
	float m_fToY = 0.f;
};

