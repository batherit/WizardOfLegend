#pragma once
#include "CObj.h"

class CPlayerWOL;
class CUI_Gauge;
class CCamera2D;

class CUI_PlayerBar :
	public CObj
{
public:
	CUI_PlayerBar(CGameWorld& _rGameWorld, CObj* _pPlayer);
	virtual ~CUI_PlayerBar();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;
	virtual void Release(void) override;

private:
	HDC m_hDCPlayerBar;
	CObj* m_pPlayer = nullptr;
	CUI_Gauge* m_pHpBar = nullptr;
	CUI_Gauge* m_pManaBar = nullptr;
};

