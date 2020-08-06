#pragma once
#include "CObj.h"
class CBoxAttack :
	public CObj
{
public:
	CBoxAttack(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CBoxAttack();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;
	virtual void ReactToCollider(CObj * _pCollider, POINT& _ptCollisionPoint, RECT& _rcCollisionRect) override;

public: 
	void ThrowBoxAttack(float _fToX, float _fToY, float _fSpeed);

private:
	bool m_bIsBoxThrowing = false;
	HDC m_hDCKeyAtlas = nullptr;
	float m_fYToReach = 0.f;
	float m_fElapsedTime = 0.f;
};

