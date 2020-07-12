#pragma once
#include "CObj.h"
class CUI_Gauge :
	public CObj
{
public:
	CUI_Gauge(CGameWorld& _rGameWorld, CObj* _pOwner, size_t _iMaxWidth, size_t _iMaxHeight, float _fMax, float _fCurrent, float _fOffsetX = 0.f, float _fOffsetY = 0.f, UI::E_GAUGE _eDir = UI::GAUGE_HORIZONTAL);
	~CUI_Gauge();

public:
	void SetCurrentGauge(float _fCurrent);
	virtual int Update(float _fDeltaTime);
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera = nullptr) override;
	virtual void Release(void) { m_pOwner = nullptr; }

private:
	CObj* m_pOwner;
	RECT m_rcGauge;
	float m_fOffsetX;
	float m_fOffsetY;
	size_t m_iMaxWidth;
	size_t m_iMaxHeight;
	UI::E_GAUGE m_eDir;
	TCHAR szBuf[64];
	float m_fMax;
	float m_fCurrent;
};

