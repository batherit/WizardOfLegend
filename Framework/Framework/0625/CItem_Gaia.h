#pragma once
#include "CObj.h"

template<typename T> class CState;
class CPlayerWOL;

class CItem_Gaia :
	public CObj
{
public:
	CItem_Gaia(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CItem_Gaia();

public:
	virtual int Update(float _fDeltaTime) override;
	void Render(HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);
	bool Used(void);

private:
	CState<CPlayerWOL>* m_pSkillState;
	HDC m_hDCKey = nullptr;
};

