#pragma once
#include "CObj.h"
class CUI_DamageText :
	public CObj
{
public:
	CUI_DamageText(CGameWorld& _rGameWorld, float _fX, float _fY, int _iDamage);
	virtual ~CUI_DamageText();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera);

private:
	TCHAR m_szDamage[32];
};

