#pragma once
#include "CObj.h"

class CUI_Gauge;

class CUI_MiddleBossBar :
	public CObj
{
public:
	CUI_MiddleBossBar(CGameWorld& _rGameWorld, CObj* _pMiddleBoss);
	virtual ~CUI_MiddleBossBar();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;
	virtual void Release(void) override;

private:
	HDC m_hDCMiddleBossBar;
	CObj* m_pMiddleBoss = nullptr;
	CUI_Gauge* m_pHpBar = nullptr;
};

