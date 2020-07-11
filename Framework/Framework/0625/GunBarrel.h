#pragma once
#include "CObj.h"
class CGunBarrel :
	public CObj
{
public:
	CGunBarrel(CGameWorld& _rGameWorld, CObj* _pOwner, float _fDegree = 0.f, float _fDistance = 20.f, float _fSpeed = 20.f);
	~CGunBarrel();

public:
	//virtual void Ready(void) {};
	virtual int Update(float _fDeltaTime);
	//virtual void LateUpdate(void) {};
	virtual void Render(const HDC& _hdc);
	virtual void Release(void) { m_pOwner = nullptr; }

	float GetStartX(void) const { return CObj::GetX(); }
	float GetStartY(void) const { return CObj::GetY(); }
	float GetEndX(void) const { return GetStartX() + m_fDistance * DIR_X_DEGREE(m_fDegree); }
	float GetEndY(void) const { return GetStartY() + m_fDistance * DIR_Y_DEGREE(m_fDegree); }
	float GetDirX(void) const { return DIR_X_DEGREE(m_fDegree); }
	float GetDirY(void) const { return DIR_Y_DEGREE(m_fDegree); }
	float GetDistance(void) const { return m_fDistance; }

	void RotateCW(float _fDeltaTime);
	void RotateCCW(float _fDeltaTime);
	

private:
	CObj* m_pOwner;
	float m_fDegree;
	float m_fDistance;
};

