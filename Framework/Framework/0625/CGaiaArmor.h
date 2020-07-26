#pragma once
#include "CObj.h"
class CGaiaArmor :
	public CObj
{
public:
	CGaiaArmor(CGameWorld& _rGameWorld, CObj* _pOwner);
	virtual ~CGaiaArmor();

public:
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera);
	virtual void Release(void);

	float GetDegree(void) const { return m_fDegree; }

private:
	int m_iGaiaArmorNum = 8;
	//float m_fMinRotSpeed = ICE_CRYSTAL_MIN_ROT_SPEED;
	//float m_fMaxRotSpeed = ICE_CRYSTAL_MAX_ROT_SPEED;
	CObj* m_pOwner = nullptr;
	CObj* m_pGaiaArmor[8] = { nullptr, };
	float m_fDegree = 0.f;		// È¸Àü°¢
	float m_fLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
};

