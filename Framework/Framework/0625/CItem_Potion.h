#pragma once
#include "CObj.h"
class CItem_Potion :
	public CObj
{
public:
	CItem_Potion(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CItem_Potion();

public:
	virtual int Update(float _fDeltaTime) override;
	void Render(HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);
	bool Used(void);

private:
	HDC m_hDCKey = nullptr;
};

