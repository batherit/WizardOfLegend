#pragma once
#include "CObj.h"
class CStonePillarGenerator :
	public CObj
{
public:
	CStonePillarGenerator(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, float _fIntervalDist, float _fIntervalTime, float _fDelayTime, float _fLifeTime);
	virtual ~CStonePillarGenerator();


public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;
	virtual void ReactToCollider(CObj * _pCollider, POINT& _ptCollisionPoint, RECT& _rcCollisionRect) override;

private:
	const float m_fIntervalDist = 0.f;
	const float m_fIntervalTime = 0.f;
	const float m_fDelayTime = 0.f;
	const float m_fLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
};

