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
// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
	virtual int AlwaysUpdate(float _fDeltaTime) override;
	virtual bool IsMutable(void);

private:
	void SetAttackDirection(float * _pLength);

private:
	CCamera2D* m_pCamera = nullptr;
	bool m_bCoolTimeStop = false;
};	

