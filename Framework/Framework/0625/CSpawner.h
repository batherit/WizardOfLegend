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
};

