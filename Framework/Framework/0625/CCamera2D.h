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
	virtual void Render(HDC& _hdc) {
		TCHAR szBuffer[64];
		swprintf(szBuffer, sizeof(szBuffer), L"Camera X : %f, Y : %f, Zoom : %f", GetX(), GetY(), m_fZoomMultiple);
		TextOut(_hdc, 0, 0, szBuffer, lstrlen(szBuffer));
	};
	virtual void Release(void) {};

public:
	void ZoomIn(float _fDeltaTime);
	void ZoomOut(float _fDeltaTime);
	float GetZoomMultiple(void) const { return m_fZoomMultiple; }
	void Shake(float _fShakeKeepTime, float _fSakeRadius, int _iShakeNum);

public:
	RECT GetScreenRect(RECT& _rRectW);
	pair<float, float> GetScreenPoint(float _fXW, float _fYW);
	pair<float, float> GetWorldPoint(float _fXS, float _fYS);

private:
	CObj* m_pOwner = nullptr;
	float m_fZoomMultiple = 1.f;
	const float m_cfZoomSpeed = 2.f;
	const float m_cfMaxZoomOut = 0.2f;
	const float m_cfMaxZoomIn = 2.0f;

	bool m_bIsShaking = false;
	float m_fShakeKeepTime = 0.f;
	float m_fShakeTickTime = 0.f;
	float m_fShakeElapsedTime = 0.f;
	float m_fShakeRadius = 0.f;
	float m_fShakeOffsetX = 0.f;
	float m_fShakeOffsetY = 0.f;
	float m_fOffsetDegree = 0.f;
	int m_iShakeNum = 0;
};

