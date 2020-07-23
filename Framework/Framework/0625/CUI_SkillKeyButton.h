#pragma once
#include "CObj.h"

template<typename T> class CState;
class CPlayerWOL;
class CUI_SkillKeyBinding;

class CUI_SkillKeyButton :
	public CObj
{
public:
	CUI_SkillKeyButton(CGameWorld& _rGameWorld, CUI_SkillKeyBinding* _pSkillKeyBinding, float _fX, float _fY, CPlayerWOL* _pPlayer, SKILL::E_KEY _eSkillKeyType);
	virtual ~CUI_SkillKeyButton();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;
	
private:
	CPlayerWOL* m_pPlayer = nullptr;
	SKILL::E_KEY m_eSkillKeyType = SKILL::KEY_END;
	CUI_SkillKeyBinding * m_pSkillKeyBinding = nullptr;
};

