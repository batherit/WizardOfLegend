#pragma once
#include "CObj.h"
class CUI_Money :
	public CObj
{
public:
	CUI_Money(CGameWorld& _rGameWorld, float _fX, float _fY, CObj& _rOwner);
	virtual ~CUI_Money();

	virtual int Update(float _fDeltaTime);
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera);

private:
	TCHAR m_szMoney[32];
	CObj& m_rOwner;
	HDC m_hDCKey = nullptr;
};

