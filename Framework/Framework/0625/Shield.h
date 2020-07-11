#pragma once
#include "CObj.h"
class CShield final :
	public CObj
{
public:
	CShield(CGameWorld& _rGameWorld, CObj* _pOwner, size_t _iWidth = ciBulletSize, size_t _iHeight = ciBulletSize, float _fSpeed = cfBulletSpeed, float _fDamage = cfBulletDamage);
	~CShield();

public:
	virtual void Ready(void)				override;
	virtual int Update(float _fDeltaTime)	override;
	virtual void LateUpdate(void)			override;
	virtual void Render(const HDC& _hdc)	override;
	virtual void Release(void)				override;

private:
	CObj* m_pOwner;
	CObj* m_pScrewBall;
};

