#pragma once

class CObj;
template<typename T>
void DeleteSafe(T& rObj) {
	if (rObj) {
		delete rObj;
		rObj = nullptr;
	}
}

template<typename T>
void DeleteListSafe(list<T>& _list) {
	for (auto& pObj : _list) { DeleteSafe(pObj); }
	_list.clear();
}

template<typename T>
void DeleteVectorSafe(vector<T>& _vector) {
	for (auto& pObj : _vector) { DeleteSafe(pObj); }
	_vector.clear();
	_vector.shrink_to_fit();
}

bool IsCollided(const RECT& _rRect1, const RECT& _rRect2);
bool IsCollided(const CObj* _pObj1, const CObj* _pObj2);
bool IsCollided(const CObj* _pObj1, const CObj* _pObj2, RECT& _rCollidedPoint);
bool IsPointInRect(const RECT& _rRect, const POINT& _rPoint);
POINT GetClientCursorPoint(void);
float GetRandomFloat(void);
int GetNumberMinBetweenMax(int _iMin, int _iMax);
float GetNumberMinBetweenMax(float _fMin, float _fMax);
template<typename T>
void Clamp(T* _pValue, T _Min, T _Max) {
	if (*_pValue < _Min)		*_pValue = _Min;
	else if (*_pValue > _Max)	*_pValue = _Max;
}
template<typename T>
T Clamp(T _Value, T _Min, T _Max) {
	Clamp(&_Value, _Min, _Max);
	return _Value;
}

void PushObjectInRect(CObj& _pObj, const RECT& _rRect);
bool MyIntersectRect(const RECT & _rRect1, const RECT & _rRect2, RECT* _pCollidedPoint = nullptr);

template<typename T>
void CollectGarbageObjects(list<T*>& _list)
{
	for (auto& pObj : _list) { DO_IF_IS_NOT_VALID_OBJ(pObj) { DeleteSafe(pObj); } }
	_list.remove_if([](auto& pObj) { return pObj == nullptr; });
}

void NormalizeVector(float& _fToX, float& _fToY);
float GetVectorLength(const float& _fToX, const float& _fToY);