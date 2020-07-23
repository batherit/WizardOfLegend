#pragma once
#include "CObj.h"

class CUI_Cooltime;

class CUI_SkillBar :
	public CObj
{
public:
	CUI_SkillBar(CGameWorld& _rGameWorld, CObj* _pPlayer);
	virtual ~CUI_SkillBar();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;
	virtual void Release(void) override;

private:
	HDC m_hDCSkillBar;
	CObj* m_pPlayer = nullptr;
	CUI_Cooltime* m_pCooltimeUI[SKILL::KEY_END];
};

