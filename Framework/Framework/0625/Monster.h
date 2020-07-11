#pragma once
#include "CObj.h"

class CUI_Gauge;

class CMonster :
	public CObj
{
public:
	CMonster(CGameWorld& _rGameWorld);
	CMonster(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth = ciMonsterSize, size_t _iHeight = ciMonsterSize, float _fSpeed = cfMonsterSpeed, float _fHp = cfMonsterHp);
	virtual ~CMonster();

public:
	virtual void Ready(void)				override;
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)			override;
	virtual void Render(const HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)				override;

public:
	void TakeDamage(float _fDamage);
	bool IsDied(void);
	void SetReverseDirectionOfCollision(OBJ::E_COLLIDED _CollidedPoint, float _fCollidedDepth);

	float GetToX(void) const { return m_fToX; }
	float GetToY(void) const { return m_fToY; }
	void SetToX(float _fToX) { m_fToX = _fToX; NormalizeVector(m_fToX, m_fToY); }
	void SetToY(float _fToY) { m_fToY = _fToY; NormalizeVector(m_fToX, m_fToY); }
	void SetToXnY(float _fToX, float _fToY) { m_fToX = _fToX; m_fToY = _fToY; NormalizeVector(m_fToX, m_fToY); }

private:
	float m_fHp;
	CUI_Gauge* m_pHpGauge;
};

