#pragma once
#include "CState.h"

class CPlayerWOL;

class CPlayerState_Dash :
	public CState<CPlayerWOL>
{
public:
	CPlayerState_Dash(CPlayerWOL& _rOwner);
	virtual ~CPlayerState_Dash();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

