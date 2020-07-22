#pragma once
#include "CState.h"

class CPlayerWOL;

class CPlayerNormalSkillState :
	public CState<CPlayerWOL>
{
public:
	CPlayerNormalSkillState(CPlayerWOL& _rOwner);
	virtual ~CPlayerNormalSkillState();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;

private:
	void SetAttackDirection(float* _pLength = nullptr);

private:
	int m_iComboCount = 0;
	CCamera2D* m_pCamera = nullptr;
	float m_fPlayerAttackSpeed = 0.f;

	// CState��(��) ���� ��ӵ�
	virtual void OnExited(void) override;
};

