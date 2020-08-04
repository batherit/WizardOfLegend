#pragma once
#include "CObj.h"
class CFireDragon :
	public CObj
{
public:
	CFireDragon(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, PLAYER::E_STATE _eAttackType);
	virtual ~CFireDragon();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;
	virtual void ReactToCollider(CObj * _pCollider, POINT& _ptCollisionPoint) override;

private:
	int GetSpriteIndex(void);

private:
	HDC m_hDCKeyAtlas = nullptr;
	float m_fInitDegree = 0.f;
	float m_fDeltaDegree = 0.f;
	float m_fCurrentDegree = 0.f;
	float m_fCycleDegree = 0.f;
	float m_fElapsedTime = 0.f;
	int m_iState = 0;
	float m_fLifeTime = 0.f; //юс╫ц©К
};

