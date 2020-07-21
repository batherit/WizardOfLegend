#pragma once
#include "CObj.h"
class CUI_Gauge :
	public CObj
{
public:
	//CUI_Gauge(CGameWorld& _rGameWorld, CObj* _pOwner, size_t _iMaxWidth, size_t _iMaxHeight, float _fMax, float _fCurrent, float _fOffsetX = 0.f, float _fOffsetY = 0.f, UI::E_GAUGE _eDir = UI::GAUGE_HORIZONTAL);
	CUI_Gauge(CGameWorld& _rGameWorld, CObj* _pOwner, RECT _rcMaxGaugeRect, float _fMax, float _fCurrent, float _fOffsetX = 0.f, float _fOffsetY = 0.f, UI::E_GAUGE _eDir = UI::GAUGE_HORIZONTAL, UI::E_PIVOT _ePivot = UI::PIVOT_LEFT);
	~CUI_Gauge();

public:
	void SetCurrentGauge(float _fCurrent);
	virtual int Update(float _fDeltaTime);
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera = nullptr) override;
	virtual void Release(void) { m_pOwner = nullptr; }
	void SetHDC(const HDC& _hDCGauge) { m_hDCGauge = _hDCGauge; }

private:
	CObj* m_pOwner;
	RECT m_rcMaxGaugeRect;
	float m_fOffsetX = 0.f;
	float m_fOffsetY = 0.f;
	//size_t m_iMaxWidth = 0.f;
	//size_t m_iMaxHeight = 0.f;
	UI::E_GAUGE m_eDir = UI::GAUGE_HORIZONTAL;
	UI::E_PIVOT m_ePivot = UI::PIVOT_LEFT;
	TCHAR szBuf[64];
	float m_fMax = 0.f;
	float m_fCurrent = 0.f;
	HDC m_hDCGauge = nullptr;
};

