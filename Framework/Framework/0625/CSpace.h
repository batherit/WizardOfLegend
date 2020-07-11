#pragma once
#include "CObj.h"

class CSpace :
	public CObj
{
public:
	CSpace(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight, list<CObj*>* _pBulletList);
	virtual ~CSpace();

public:
	virtual void Ready(void) {};
	virtual int Update(float _fDeltaTime) { return 0; };
	virtual void LateUpdate(void);
	virtual void Render(const HDC& _hdc) { m_pDrawFunc(_hdc, GetLeft(), GetTop(), GetRight(), GetBottom()); }
	virtual void Release(void) { }

public:
	bool IsObjInside(const CObj* rObj, OBJ::E_COLLIDED* _pCollided = nullptr, float* _pCollidedDepth = nullptr) const;
};

