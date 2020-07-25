#pragma once
#include "CObj.h"
class CWizardFire :
	public CObj
{
public:
	CWizardFire(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY);
	virtual ~CWizardFire();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;
	int GetAnimState(float _fToX, float _fToY) const;

private:
	HDC m_hDCKeyAtlas = nullptr;
};

