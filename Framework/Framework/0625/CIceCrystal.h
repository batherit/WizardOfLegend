#pragma once
#include "CObj.h"

class CCamera2D;

class CIceCrystal :
	public CObj
{
public:
	CIceCrystal(CGameWorld& _rGameWorld, CObj* _pOwner);
	CIceCrystal(CGameWorld& _rGameWorld, float _fX, float _fY); // 소환용
	virtual ~CIceCrystal();

public:
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera);
	virtual void Release(void);

	float GetDegree(void) const { return m_fDegree; }

private:
	bool m_bIsSignatureMode = false;
	int m_iIceCrystalNum = 3;
	float m_fMinRotSpeed = ICE_CRYSTAL_MIN_ROT_SPEED;
	float m_fMaxRotSpeed = ICE_CRYSTAL_MAX_ROT_SPEED;
	CObj* m_pOwner = nullptr;
	CObj* m_pIceCrystal[6] = { nullptr, };
	float m_fOffset = 0.f;		// 부모와 자식간 거리
	float m_fDegree = 0.f;		// 회전각
	float m_fPeriodDegree = 0.f;
	int m_iPeriod = 1;
	float m_fLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
};

