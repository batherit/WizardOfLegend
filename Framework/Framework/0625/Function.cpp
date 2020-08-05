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

bool IsPointInRect(const RECT & _rRect, const POINT & _rPoint)
{
	return (_rRect.left <= _rPoint.x && _rPoint.x <= _rRect.right) && (_rRect.top <= _rPoint.y && _rPoint.y <= _rRect.bottom);
}

POINT GetClientCursorPoint(void)
{
	POINT pt;
	GetCursorPos(&pt);				// 데스크탑 기준 좌표
	ScreenToClient(g_hWND, &pt);	// 윈도우창 기준 좌표

	return pt;
}

float GetRandomFloat(void)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float GetRadianByVector(float _fToX, float _fToY)
{
	float fRadian = acosf(_fToX / GetVectorLength(_fToX, _fToY));
	if (_fToY < 0) fRadian *= -1.f;

	return fRadian;
}

float GetPositiveDegreeByVector(float _fToX, float _fToY) {
	float fDegree = TO_DEGREE(GetRadianByVector(_fToX, _fToY));
	if (fDegree < 0.f) fDegree += 360.f;
	return fDegree;
}

OBJ::E_DIRECTION GetDirByDegree(float _fDegree, float _fWidth, float _fHeight, float _fWeight) {
	Clamp(&_fWeight, 0.f, 1.f);

	float fWidthDegree = 90.f * (_fWidth / (_fWidth + _fHeight));
	float fHeightDegree = 90.f - fWidthDegree;

	if (_fWidth > _fHeight) {
		fWidthDegree = 90.f * (_fWidth * (1.f - _fWeight) + (_fWidth + _fHeight) * _fWeight)/(_fWidth + _fHeight);
		fHeightDegree = 90.f - fWidthDegree;
	}
	else if (_fWidth < _fHeight) {
		fHeightDegree = 90.f * (_fHeight * (1.f - _fWeight) + (_fWidth + _fHeight) * _fWeight)/(_fWidth + _fHeight);
		fWidthDegree = 90.f - fHeightDegree;
	}
	

	if (fHeightDegree <= _fDegree && _fDegree < fHeightDegree + 2.f * fWidthDegree) {
		return OBJ::DIR_DOWN;
	}
	else if (fHeightDegree + 2.f * fWidthDegree <= _fDegree && _fDegree < 3.f * fHeightDegree + 2.f * fWidthDegree) {
		return OBJ::DIR_LEFT;
	}
	else if (3.f * fHeightDegree + 2.f * fWidthDegree <= _fDegree && _fDegree < 3.f * fHeightDegree + 4.f * fWidthDegree) {
		return OBJ::DIR_UP;
	}
	else return OBJ::DIR_RIGHT;
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
	if (fVectorLength == 0) {
		_fToX = 0;
		_fToY = 0;
		return;
	}
	_fToX /= fVectorLength;
	_fToY /= fVectorLength;
}

float GetVectorLength(const float & _fToX, const float & _fToY)
{
	return sqrtf(_fToX * _fToX + _fToY * _fToY);
}

//void CollectGarbageObjects(list<CObj*>& _list)
//{
//	for (auto& pObj : _list) { DO_IF_IS_NOT_VALID_OBJ(pObj) { DeleteSafe(pObj); } }
//	_list.remove_if([](auto& pObj) { return pObj == nullptr; });
//}
