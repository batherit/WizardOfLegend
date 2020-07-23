#pragma once
#include "CObj.h"

template<typename T> class CState;
class CPlayerWOL;

class CUI_Cooltime :
	public CObj
{
public:
	CUI_Cooltime(CGameWorld& _rGameWorld, float _fX, float _fY, CState<CPlayerWOL>*& _prSkillState);
	virtual ~CUI_Cooltime();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;

private:
	TCHAR m_szCount[32] = L"";
	CState<CPlayerWOL>*& m_prSkillState;
	SKILL_COOLTIME::E_TYPE m_eCooltimeType = SKILL_COOLTIME::TYPE_END;
};

