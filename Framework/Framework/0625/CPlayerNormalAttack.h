#pragma once
#include "CObj.h"
class CPlayerNormalAttack :
	public CObj
{
public:
	CPlayerNormalAttack(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, PLAYER::E_STATE _eAttackType);
	virtual ~CPlayerNormalAttack();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

private:
	HDC m_hDCKeyAtlas = nullptr;
};

