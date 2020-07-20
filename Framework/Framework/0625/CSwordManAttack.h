#pragma once
#include "CObj.h"
class CSwordManAttack :
	public CObj
{
public:
	CSwordManAttack(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, SWORDMAN::E_DIRECTION _eSwordManDir);
	virtual ~CSwordManAttack();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

private:
	HDC m_hDCKeyAtlas = nullptr;
	
};

