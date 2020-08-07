#pragma once
#include "CObj.h"
class CTeleport :
	public CObj
{
public:
	CTeleport(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CTeleport();

public:
	virtual int Update(float _fDeltaTime) override;
	void Render(HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);

private:
	HDC m_hDCKey = nullptr;
};

