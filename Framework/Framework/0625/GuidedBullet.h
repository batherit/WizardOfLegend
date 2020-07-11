#pragma once
#include "Bullet.h"
class CGuidedBullet :
	public CBullet
{
public:
	CGuidedBullet(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, size_t _iWidth = ciBulletSize, size_t _iHeight = ciBulletSize, float _fSpeed = cfBulletSpeed, float _fDamage = cfBulletDamage);
	~CGuidedBullet();

public:
	virtual void Ready(void)				override;
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)			override;
	virtual void Render(const HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)				override;
};

