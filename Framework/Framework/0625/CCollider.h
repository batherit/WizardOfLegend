#pragma once
#include "CObj.h"

class CMapLoader;

class CCollider :
	public CObj
{
public:
	CCollider(FILE* _fpIn, CGameWorld& _rGameWorld, COLLIDER::E_SHAPE _eShape = COLLIDER::SHAPE_RECT);
	CCollider(CGameWorld& _rGameWorld, CObj* _pOwner, float _fOffsetX, float _fOffsetY, size_t _iWidth, size_t _iHeight, COLLIDER::E_SHAPE _eShape = COLLIDER::SHAPE_RECT);
	CCollider();
	~CCollider();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;

private:
	CObj* m_pOwner = nullptr;
	float m_fOffsetX = 0.f;
	float m_fOffsetY = 0.f;
};

