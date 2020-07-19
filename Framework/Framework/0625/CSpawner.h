#pragma once
#include "CObj.h"

class CGameWorld;
class CCamera2D;

class CSpawner :
	public CObj
{
public:
	CSpawner(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CSpawner();

	virtual int Update(float _fDeltaTime) override;

protected:
	bool m_bIsSpawend = false;
	HDC m_hDCSpawnSprite = nullptr;
	float m_fOffsetX = 0.f;
	float m_fOffsetY = 0.f;
};

