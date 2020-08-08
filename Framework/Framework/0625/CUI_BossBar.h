#pragma once
#include "CObj.h"

class CUI_Gauge;

class CUI_BossBar :
	public CObj
{
public:
	CUI_BossBar(CGameWorld& _rGameWorld, CObj* _pBoss);
	virtual ~CUI_BossBar();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;
	virtual void Release(void) override;

private:
	HDC m_hDCBossBar;
	CObj* m_pBoss = nullptr;
	CUI_Gauge* m_pHpBar = nullptr;
};

