#pragma once
#include "CState.h"

class CPlayerWOL;

class CFireDragonSkillState :
	public CState<CPlayerWOL>
{
public:
	CFireDragonSkillState(CPlayerWOL& _rOwner);
	virtual ~CFireDragonSkillState();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	void SetAttackDirection(float * _pLength);

private:
	CCamera2D* m_pCamera = nullptr;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

