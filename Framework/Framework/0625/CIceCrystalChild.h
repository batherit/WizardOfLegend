#pragma once
#include "CObj.h"

class CIceCrystal;

class CIceCrystalChild :
	public CObj
{
public:
	CIceCrystalChild(CGameWorld& _rGameWorld, CIceCrystal* _pIceCrystalParent);
	virtual ~CIceCrystalChild();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

private:
	int GetSpriteIndex(void);

private:
	CIceCrystal* m_pIceCrystalParent = nullptr;
	HDC m_hDCKeyAtlas = nullptr;
};

