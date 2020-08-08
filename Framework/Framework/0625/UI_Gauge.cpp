#include "stdafx.h"
#include "UI_Gauge.h"


//CUI_Gauge::CUI_Gauge(CGameWorld& _rGameWorld, CObj* _pOwner, size_t _iMaxWidth, size_t _iMaxHeight, float _fMax, float _fCurrent, float _fOffsetX /*= 0.f*/, float _fOffsetY /*= 0.f*/, UI::E_GAUGE _eDir /*= UI::GAUGE_HORIZONTAL*/)
//	:
//	m_pOwner(nullptr),
//	CObj(_rGameWorld, 0.f, 0.f, _iMaxWidth, _iMaxHeight),
//	m_fMax(_fMax),
//	m_fCurrent(_fCurrent),
//	m_fOffsetX(_fOffsetX),
//	m_fOffsetY(_fOffsetY),
//	m_iMaxWidth(_iMaxWidth),
//	m_iMaxHeight(_iMaxHeight),
//	m_eDir(_eDir)
//{
//	if (_pOwner) {
//		m_pOwner = _pOwner;
//		SetX(m_pOwner->GetX() + m_fOffsetX);
//		SetY(m_pOwner->GetY() + m_fOffsetY);
//	}
//	SetCurrentGauge(m_fCurrent);
//}

CUI_Gauge::CUI_Gauge(CGameWorld & _rGameWorld, CObj * _pOwner, RECT _rcMaxGaugeRect, float _fMax, float _fCurrent, float _fOffsetX, float _fOffsetY, UI::E_GAUGE _eDir, UI::E_PIVOT _ePivot)
	:
	m_pOwner(_pOwner),
	CObj(_rGameWorld, 0.f, 0.f, 0.f, 0.f),
	m_rcMaxGaugeRect(_rcMaxGaugeRect),
	m_fMax(_fMax),
	m_fCurrent(_fCurrent),
	m_fOffsetX(_fOffsetX),
	m_fOffsetY(_fOffsetY),
	//m_iMaxWidth(_rcGaugeRect.right - _rcGaugeRect.left),
	//m_iMaxHeight(_rcGaugeRect.bottom - _rcGaugeRect.top),
	m_eDir(_eDir),
	m_ePivot(_ePivot)
{
	SetCurrentGauge(_fCurrent);
}

CUI_Gauge::~CUI_Gauge()
{
	Release();
}

void CUI_Gauge::SetCurrentGauge(float _fCurrent)
{
	m_fCurrent = Clamp(_fCurrent, 0.f, m_fMax);
	float fT = m_fCurrent / m_fMax;
	//swprintf_s(szBuf, 64, TEXT("%d / %d"), static_cast<int>(m_fCurrent), static_cast<int>(m_fMax));

	RECT rcCurGaugeRect = m_rcMaxGaugeRect;
	switch (m_eDir)
	{ //SetWidth(0.f * (1.f - fT) + m_iMaxWidth * fT);
	case UI::GAUGE_HORIZONTAL: 
		switch (m_ePivot)
		{
		case UI::PIVOT_LEFT:
			rcCurGaugeRect.left = m_rcMaxGaugeRect.left;
			rcCurGaugeRect.right = m_rcMaxGaugeRect.left + (m_rcMaxGaugeRect.right - m_rcMaxGaugeRect.left) * fT;
			break;
		default:
			break;
		}
		break;
	default: 
		break;
	}
	SetRect(rcCurGaugeRect);
}

int CUI_Gauge::Update(float _fDeltaTime)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX() + m_fOffsetX);
		SetY(m_pOwner->GetY() + m_fOffsetY);
	}

	return 0;
}

void CUI_Gauge::Render(HDC & _hdc, CCamera2D* _pCamera)
{
	if (m_fCurrent > 0) {
		RECT& rcDrawArea = GetRect();

		BitBlt(_hdc,
			rcDrawArea.left,			// 출력 시작좌표 X
			rcDrawArea.top,			// 출력 시작좌표 Y
			rcDrawArea.right - rcDrawArea.left,
			rcDrawArea.bottom - rcDrawArea.top,
			m_hDCGauge,
			0,
			0,
			SRCCOPY);
	}
}
