#pragma once
#include "CState.h"

class CPlayerWOL;

class CPlayerState_Damage :
	public CState<CPlayerWOL>
{
public:
	CPlayerState_Damage(CPlayerWOL& _rOwner, POINT _ptCollisionPoint);
	virtual ~CPlayerState_Damage();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	POINT m_ptCollisionPoint;
};

