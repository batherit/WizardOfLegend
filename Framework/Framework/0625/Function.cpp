#include "stdafx.h"
#include "CObj.h"

bool IsCollided(const RECT & _rRect1, const RECT & _rRect2)
{
	if (_rRect1.left > _rRect2.right) return false;
	if (_rRect1.right < _rRect2.left) return false;
	if (_rRect1.top > _rRect2.bottom) return false;
	if (_rRect1.bottom < _rRect2.top) return false;
	return true;
}

bool IsCollided(const CObj * _pObj1, const CObj * _pObj2)
{
	if (!IS_VALID_OBJ(_pObj1) || !IS_VALID_OBJ(_pObj2)) return false;
	if (_pObj1->GetLeft() > _pObj2->GetRight()) return false;
	if (_pObj1->GetRight() < _pObj2->GetLeft()) return false;
	if (_pObj1->GetTop() > _pObj2->GetBottom()) return false;
	if (_pObj1->GetBottom() < _pObj2->GetTop()) return false;
	return true;
}

bool IsCollided(const CObj * _pObj1, const CObj * _pObj2, RECT& _rCollidedPoint)
{
	if (!IS_VALID_OBJ(_pObj1) || !IS_VALID_OBJ(_pObj2)) return false;
	return IntersectRect(&_rCollidedPoint, &(_pObj1->GetRect()), &(_pObj2->GetRect()));
}

float GetRandomFloat(void)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int GetNumberMinBetweenMax(int _iMin, int _iMax) {
	if (_iMin > _iMax) abort();

	return rand() % (_iMax - _iMin + 1) + _iMin;
}


float GetNumberMinBetweenMax(float _fMin, float _fMax) {
	if (_fMin > _fMax) abort();

	float fT = GetRandomFloat();
	return _fMin * (1.f - fT) + _fMax * fT;
}

void PushObjectInRect(CObj & _pObj, const RECT & _rRect)
{
	if (_pObj.GetLeft() <= _rRect.left)		_pObj.SetX(_rRect.left + (_pObj.GetWidth() >> 1));
	if (_pObj.GetTop() <= _rRect.top)		_pObj.SetY(_rRect.top + (_pObj.GetHeight() >> 1));
	if (_pObj.GetRight() >= _rRect.right)	_pObj.SetX(_rRect.right - (_pObj.GetWidth() >> 1));
	if (_pObj.GetBottom() >= _rRect.bottom)	_pObj.SetY(_rRect.bottom - (_pObj.GetHeight() >> 1));
}

bool MyIntersectRect(const RECT & _rRect1, const RECT & _rRect2, RECT* _pCollidedPoint)
{
	if (!IsCollided(_rRect1, _rRect2)) return false;

	if (_rRect1.left <= _rRect2.right) {
		if (_rRect1.left <= _rRect2.left) {
			if (_pCollidedPoint)
				_pCollidedPoint->left = _rRect2.left;
		}
		else {
			if (_pCollidedPoint)
				_pCollidedPoint->left = _rRect1.left;
		}
	}
	if (_rRect1.right >= _rRect2.left) {
		if (_rRect1.right >= _rRect2.right) {
			if (_pCollidedPoint)
				_pCollidedPoint->right = _rRect2.right;
		}
		else {
			if (_pCollidedPoint)
				_pCollidedPoint->right = _rRect1.right;
		}
	}
	if (_rRect1.top <= _rRect2.bottom) {
		if (_rRect1.top <= _rRect2.top) {
			if (_pCollidedPoint)
				_pCollidedPoint->top = _rRect2.top;
		}
		else {
			if (_pCollidedPoint)
				_pCollidedPoint->top = _rRect1.top;
		}
	}
	if (_rRect1.bottom >= _rRect2.top) {
		if (_rRect1.bottom >= _rRect2.bottom) {
			if (_pCollidedPoint)
				_pCollidedPoint->bottom = _rRect2.bottom;
		}
		else {
			if (_pCollidedPoint)
				_pCollidedPoint->bottom = _rRect1.bottom;
		}
	}

	return true;
}

void NormalizeVector(float & _fToX, float & _fToY)
{
	float fVectorLength = sqrtf(_fToX *_fToX + _fToY *_fToY);
	_fToX /= fVectorLength;
	_fToY /= fVectorLength;
}

float GetVectorLength(const float & _fToX, const float & _fToY)
{
	return sqrtf(_fToX * _fToX + _fToY * _fToY);
}

void CollectGarbageObjects(list<CObj*>& _list)
{
	for (auto& pObj : _list) { DO_IF_IS_NOT_VALID_OBJ(pObj) { DeleteSafe(pObj); } }
	_list.remove_if([](auto& pObj) { return pObj == nullptr; });
}
