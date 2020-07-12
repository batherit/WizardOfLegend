#include "stdafx.h"
#include "CSpace.h"



CSpace::CSpace(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight)
{
}

CSpace::~CSpace()
{
	Release();
}

void CSpace::LateUpdate(void)
{
}

bool CSpace::IsObjInside(const CObj * rObj, OBJ::E_COLLIDED* _pCollidedPoint, float* _pCollidedDepth) const
{
	if (rObj->GetTop() < GetTop()) {
		if (_pCollidedPoint) *_pCollidedPoint = OBJ::COL_TOP;
		if (_pCollidedDepth) *_pCollidedDepth = rObj->GetTop() - GetTop();
		return false;
	}
	if (rObj->GetBottom() > GetBottom()) {
		if (_pCollidedPoint) *_pCollidedPoint = OBJ::COL_BOTTOM;
		if (_pCollidedDepth) *_pCollidedDepth = rObj->GetBottom() - GetBottom();
		return false;
	}
	if (rObj->GetLeft() < GetLeft()) {
		if (_pCollidedPoint) *_pCollidedPoint = OBJ::COL_LEFT;
		if (_pCollidedDepth) *_pCollidedDepth = rObj->GetLeft() - GetLeft();
		return false;
	}
	if (rObj->GetRight() > GetRight()) {
		if (_pCollidedPoint) *_pCollidedPoint = OBJ::COL_RIGHT;
		if (_pCollidedDepth) *_pCollidedDepth = rObj->GetRight() - GetRight();
		return false;
	}
	if (_pCollidedPoint) *_pCollidedPoint = OBJ::COL_END;
	return true;
}
