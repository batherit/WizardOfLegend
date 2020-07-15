#pragma once
#include "CObj.h"

class CMapLoader;

class CCollider :
	public CObj
{
public:
	CCollider(FILE* _fpIn, CGameWorld& _rGameWorld, CMapLoader & _rMap, COLLIDER::E_TYPE _eType = COLLIDER::TYPE_RECT);
	~CCollider();

public:
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;

private:
	CMapLoader& m_rMap;
};

