#pragma once
#include "CObj.h"
class CIceBlast :
	public CObj
{
public:
	CIceBlast(CGameWorld& _rGameWorld, CObj* _pOwner, float _fToX, float _fToY);
	virtual ~CIceBlast();

public:
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void);
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera);
	virtual void Release(void);

private:
	CObj* m_pOwner = nullptr;
	float m_fSpawnTime = 0.f;
	float m_fElapsedtime = 0.f;
};

