#pragma once
#include "CState.h"

class CPlayerWOL;

class CPlayerState_Attack :
	public CState<CPlayerWOL>
{
public:
	CPlayerState_Attack(CPlayerWOL& _rOwner);
	virtual ~CPlayerState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	void SetAttackDirection(float* _pLength = nullptr);

private:
	int m_iComboCount = 0;
	CCamera2D* m_pCamera;
	float m_fPlayerAttackSpeed = 0.f;
};

