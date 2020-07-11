#pragma once
#include "CObj.h"

class CGameWorld;

class CCamera2D :
	public CObj
{
public:
	CCamera2D(CGameWorld& _rGameWorld, CObj* _pOwner = nullptr, float _fX = 0.f, float _fY = 0.f);
	~CCamera2D();

public:
	virtual void Ready(void) {};
	virtual int Update(float _fDeltaTime);
	virtual void LateUpdate(void) {};
	virtual void Render(const HDC& _hdc) { /*카메라는 그려지는 대상이 아니다.*/ };
	virtual void Release(void) {};

public:
	RECT TransformRect(RECT& _rRect);
	pair<float, float> TransformPoint(float _fX, float _fY);

private:
	CObj* m_pOwner = nullptr;
	float m_fZoomMultiple = 1.0f;
	const float m_cfZoomSpeed = 2.0f;
};

