#pragma once
#include "CObj.h"

class CCamera2D;

class CIceCrystal :
	public CObj
{
public:
	CIceCrystal(CGameWorld& _rGameWorld, CObj* _pOwner);
	CIceCrystal(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CIceCrystal();

public:
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera);
	virtual void Release(void);

	float GetDegree(void) const { return m_fDegree; }

private:
	CObj* m_pOwner = nullptr;
	CObj* m_pIceCrystal[3] = { nullptr, };
	float m_fOffset = 0.f;		// �θ�� �ڽİ� �Ÿ�
	float m_fDegree = 0.f;		// ȸ����
	float m_fLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
};

