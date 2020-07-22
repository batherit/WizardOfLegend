#pragma once
#include "CObj.h"

class CCamera2D;

class CHitEffect :
	public CObj
{
public:
	CHitEffect(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CHitEffect();

	virtual int Update(float _fDeltaTime);
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera);

private:
	HDC m_hDCKeyAtlas = nullptr;
};

