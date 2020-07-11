#pragma once
#include "CObj.h"
class CBullet : public CObj
{
public:
	CBullet(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, size_t _iWidth = ciBulletSize, size_t _iHeight = ciBulletSize, float _fSpeed = cfBulletSpeed, float _fDamage = cfBulletDamage);
	virtual ~CBullet();

public:
	virtual void Ready(void)				override;
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)			override;
	virtual void Render(const HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)				override;

	void SetToX(float fToX) { m_fToX = fToX; }
	void SetToY(float fToY) { m_fToY = fToY; }
	float GetToX(void) const { return m_fToX; }
	float GetToY(void) const { return m_fToY; }

public:
	float GetDamage(void) const { return m_fDamage; }

private:
	float m_fDamage;
};

