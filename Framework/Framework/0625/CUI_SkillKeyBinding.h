#pragma once
#include "CObj.h"

class CUI_SkillKeyButton;
class CUI_Image;

class CUI_SkillKeyBinding :
	public CObj
{
public:
	CUI_SkillKeyBinding(CGameWorld& _rGameWorld, CObj* _pPlayer);
	virtual ~CUI_SkillKeyBinding();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;
	virtual void Release(void) override;

public:
	int UpdateSkillInfo(SKILL::E_KEY _eSkillKeyType);
	void SelectButton(SKILL::E_KEY _eSkillKeyType);

private:
	HDC m_hDCSkillKeyBinding = nullptr;
	CObj* m_pPlayer = nullptr;
	CUI_SkillKeyButton* m_pSkillKeyButtons[SKILL::KEY_END] = { nullptr, };
	CUI_Image* m_pDesc = nullptr;

	SKILL::E_KEY m_eSkillKeyToSwap[2] = { SKILL::KEY_END, SKILL::KEY_END };
	int m_iSkillKeyToSwapIndex = 0;
};

