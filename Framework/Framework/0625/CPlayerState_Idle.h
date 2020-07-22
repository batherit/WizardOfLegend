#pragma once
#include "CState.h"

class CPlayerWOL;

class CPlayerState_Idle :
	public CState<CPlayerWOL>
{
public:
	CPlayerState_Idle(CPlayerWOL& _rOwner);
	virtual ~CPlayerState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

