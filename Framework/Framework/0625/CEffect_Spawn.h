#pragma once
#include "CObj.h"
class CEffect_Spawn :
	public CObj
{
public:
	CEffect_Spawn(CGameWorld& _rGameWorld, CObj* _pOwner, EFFECT_SPAWN::E_TYPE _eType);
	virtual ~CEffect_Spawn();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;

private:
	CObj* m_pOwner = nullptr;
	HDC m_hDCSpawnSprite = nullptr;
	float m_fOffsetX = 0.f;
	float m_fOffsetY = 0.f;
};

