#pragma once
#include "CObj.h"

class CGaiaArmor;

class CGaiaArmorChild :
	public CObj
{
public:
	CGaiaArmorChild(CGameWorld& _rGameWorld, CGaiaArmor* _pGaiaArmorParent);
	virtual ~CGaiaArmorChild();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

private:
	int GetSpriteIndex(void);

private:
	CGaiaArmor* m_pGaiaArmorParent = nullptr;
	HDC m_hDCKeyAtlas = nullptr;
};

