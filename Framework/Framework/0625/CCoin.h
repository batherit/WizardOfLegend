#pragma once
#include "CObj.h"
class CCoin :
	public CObj
{
public:
	CCoin(CGameWorld& _rGameWorld, float _fX, float _fY, int iMoneyAmount);
	virtual ~CCoin();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera);

private:
	HDC m_hDCKey = nullptr;
	float m_fElapsedTime = 0.f;
	CObj* m_pPlayer = nullptr;
};

