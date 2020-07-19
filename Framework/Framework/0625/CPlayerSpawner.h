#pragma once
#include "CSpawner.h"

class CPlayerSpawner :
	public CSpawner
{
public:
	CPlayerSpawner(CGameWorld& _rGameWorld, CObj* _pPlayer, float _fX, float _fY);
	virtual ~CPlayerSpawner();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D*) override;

private:
	CObj* m_pPlayer = nullptr;
};

