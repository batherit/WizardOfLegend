#pragma once
#include "CState.h"

class CPlayerWOL;

class CPlayerState_Spawn :
	public CState<CPlayerWOL>
{
public:
	CPlayerState_Spawn(CPlayerWOL& _rOwner);
	virtual ~CPlayerState_Spawn();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
};

