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

