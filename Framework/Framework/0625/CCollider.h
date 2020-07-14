#pragma once
#include "CObj.h"

class CMap;

class CCollider :
	public CObj
{
public:
	CCollider(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight, COLLIDER::E_TYPE _eType = COLLIDER::TYPE_RECT);
	~CCollider();

public:
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
};

