#pragma once
#include "CSpawner.h"

class CPlayerSpawner :
	public CSpawner
{
public:
	CPlayerSpawner(CGameWorld& _rGameWorld, CObj* _pPlayer, float _fX, float _fY, float _fTimeToDelay = 0.6f);
	virtual ~CPlayerSpawner();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D*) override;

private:
	const float m_fTimeToDelay = 0.25f;
	float m_fElapsedTime = 0.f;
	bool m_bIsAnimStarted = false;
	CObj* m_pPlayer = nullptr;
};

