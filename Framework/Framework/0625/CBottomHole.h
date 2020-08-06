#pragma once
#include "CObj.h"
class CBottomHole :
	public CObj
{
public:
	CBottomHole(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CBottomHole();

	virtual int Update(float _fDeltaTime);
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera);
	virtual void ReactToCollider(CObj * _pCollider, POINT& _ptCollisionPoint, RECT& _rcCollisionRect) override;

private:
	HDC m_hDCKeyAtlas = nullptr;
	float m_fLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
};

