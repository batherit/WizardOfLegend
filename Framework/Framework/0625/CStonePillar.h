#pragma once
#include "CObj.h"
class CStonePillar :
	public CObj
{
public:
	CStonePillar(CGameWorld& _rGameWorld, float _fX, float _fY, float _fDelayTime, float _fLifeTime);
	virtual ~CStonePillar();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;
	virtual void ReactToCollider(CObj * _pCollider, POINT& _ptCollisionPoint, RECT& _rcCollisionRect) override;

private:
	enum E_STATE { STATE_READY, STATE_ATTACK, STATE_IDLE, STATE_DEATH, STATE_END };

private:
	HDC m_hDCKeyAtlas = nullptr;
	float m_fDelayTime = 0.f;
	float m_fLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
	bool m_bAttackState = false;
	bool m_bDeath = false;
	E_STATE m_eState = STATE_READY;
};

