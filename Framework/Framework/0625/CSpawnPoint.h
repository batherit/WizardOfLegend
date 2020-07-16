#pragma once
#include "CObj.h"
class CSpawnPoint :
	public CObj
{
public:
	CSpawnPoint(FILE* _fpIn, CGameWorld& _rGameWorld);
	~CSpawnPoint();

public:
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
};

