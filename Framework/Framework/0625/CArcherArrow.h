#pragma once
#include "CObj.h"
class CArcherArrow :
	public CObj
{
public:
	CArcherArrow(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, ARCHER::E_DIRECTION _eArcherDir);
	virtual ~CArcherArrow();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;
	int GetAnimState(float _fToX, float _fToY) const;
	virtual void ReactToCollider(CObj * _pCollider, POINT& _ptCollisionPoint) override;


private:
	HDC m_hDCKeyAtlas[ARCHER::DIR_END] = { nullptr, };
	ARCHER::E_DIRECTION m_eArcherDir = ARCHER::DIR_END;
};

