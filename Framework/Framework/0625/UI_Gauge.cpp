#include "stdafx.h"
#include "UI_Gauge.h"


CUI_Gauge::CUI_Gauge(CGameWorld& _rGameWorld, CObj* _pOwner, size_t _iMaxWidth, size_t _iMaxHeight, float _fMax, float _fCurrent, float _fOffsetX /*= 0.f*/, float _fOffsetY /*= 0.f*/, UI::E_GAUGE _eDir /*= UI::GAUGE_HORIZONTAL*/)
	:
	m_pOwner(nullptr),
	CObj(_rGameWorld, 0.f, 0.f, _iMaxWidth, _iMaxHeight),
	m_fMax(_fMax),
	m_fCurrent(_fCurrent),
	m_fOffsetX(_fOffsetX),
	m_fOffsetY(_fOffsetY),
	m_iMaxWidth(_iMaxWidth),
	m_iMaxHeight(_iMaxHeight),
	m_eDir(_eDir)
{
	if (_pOwner) {
		m_pOwner = _pOwner;
		SetX(m_pOwner->GetX() + m_fOffsetX);
		SetY(m_pOwner->GetY() + m_fOffsetY);
	}
	SetCurrentGauge(m_fCurrent);
}

CUI_Gauge::~CUI_Gauge()
{
	Release();
}

void CUI_Gauge::SetCurrentGauge(float _fCurrent)
{
	m_fCurrent = Clamp(_fCurrent, 0.f, m_fMax);
	float fT = m_fCurrent / m_fMax;
	swprintf_s(szBuf, 64, TEXT("%d / %d"), static_cast<int>(m_fCurrent), static_cast<int>(m_fMax));

	switch (m_eDir)
	{
	case UI::GAUGE_HORIZONTAL: SetWidth(0.f * (1.f - fT) + m_iMaxWidth * fT);
		break;
	case UI::GAUGE_VERTICAL: SetHeight(0.f * (1.f - fT) + m_iMaxHeight * fT);
		break;
	default: SetWidth(0.f * (1.f - fT) + m_iMaxWidth * fT);
		break;
	}
}

int CUI_Gauge::Update(float _fDeltaTime)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX() + m_fOffsetX);
		SetY(m_pOwner->GetY() + m_fOffsetY);
	}

	return 0;
}

void CUI_Gauge::Render(const HDC & _hdc)
{
	if (m_fCurrent > 0) {
		m_pDrawFunc(_hdc, GetLeft(), GetTop(), GetRight(), GetBottom());
		TextOut(_hdc, GetLeft(), GetY() - 30.f, szBuf, lstrlen(szBuf));
	}
}
