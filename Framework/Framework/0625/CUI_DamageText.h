#pragma once
#include "CObj.h"
class CUI_DamageText :
	public CObj
{
public:
	CUI_DamageText(CGameWorld& _rGameWorld, float _fX, float _fY, POINT ptCollisionPoint, int _iDamage);
	virtual ~CUI_DamageText();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera);

private:
	TCHAR m_szDamage[32];
	float m_fElapsedTime = 0.f;
	POINT m_ptPoint0;
	POINT m_ptPoint1;
	POINT m_ptPoint2;
	POINT m_ptCurrentPoint;
	float m_fCurrentSize = UI_DAMAGE_TEXT_MIN_SIZE;
};

