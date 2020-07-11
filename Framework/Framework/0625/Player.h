#pragma once
#include "CObj.h"

class CGunBarrel;
class CKeyMgr;

class CPlayer final : public CObj
{
public:
	CPlayer(CGameWorld& _rGameWorld);
	CPlayer(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth = ciPlayerSize, size_t _iHeight = ciPlayerSize, float _fSpeed = cfPlayerSpeed);
	virtual ~CPlayer();

public:
	virtual void Ready(void)				override;
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)			override;
	virtual void Render(const HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)				override;

private:
	CKeyMgr* m_pKeyMgr = nullptr;
	list<CObj*> m_listShield;
	CGunBarrel* m_pGunBarrel = nullptr;
	float m_fVelocityY = 0.f;
};

