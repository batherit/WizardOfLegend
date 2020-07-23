#pragma once
#include "CObj.h"
class CUI_SkillKeyBinding :
	public CObj
{
public:
	CUI_SkillKeyBinding();
	virtual ~CUI_SkillKeyBinding();

private:
	HDC m_hDCPlayerBar;
	CObj* m_pPlayer = nullptr;
};

