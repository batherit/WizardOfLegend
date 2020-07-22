#pragma once
#include "CObj.h"
class CUI_GoldText :
	public CObj
{
public:
	CUI_GoldText(CGameWorld& _rGameWorld, float _fX, float _fY, int _iMoneyAmount);
	virtual ~CUI_GoldText();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera);

private:
	TCHAR m_szMoney[32];
	float m_fElapsedTime = 0.f;
};

