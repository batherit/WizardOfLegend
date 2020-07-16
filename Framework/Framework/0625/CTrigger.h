#pragma once
#include "CObj.h"

class CMapLoader;

class CTrigger :
	public CObj
{
public:
	CTrigger(FILE* _fpIn, CGameWorld& _rGameWorld, COLLIDER::E_TYPE _eType = COLLIDER::TYPE_RECT);
	~CTrigger();

public:
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
};

