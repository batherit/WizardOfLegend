#pragma once
#include "CObj.h"

template<typename T> class CState;
class CPlayerWOL;

class CItem_DroppedCard :
	public CObj
{
public:
	CItem_DroppedCard(CGameWorld& _rGameWorld, float _fX, float _fY, CState<CPlayerWOL>* _pSkillState);
	virtual ~CItem_DroppedCard();

public:
	virtual int Update(float _fDeltaTime) override;
	void Render(HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);
	bool Used(void);
	
private:
	CState<CPlayerWOL>* m_pSkillState = nullptr;
	HDC m_hDCKey = nullptr;
};

