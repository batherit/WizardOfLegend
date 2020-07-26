#include "stdafx.h"
#include "CUI_PlayerBar.h"
#include "UI_Gauge.h"
#include "CBitmapMgr.h"



CUI_PlayerBar::CUI_PlayerBar(CGameWorld & _rGameWorld, CObj* _pPlayer)
	:
	CObj(_rGameWorld, (PLAYER_BAR_WIDTH >> 1) + 50, 85.f, PLAYER_BAR_WIDTH, PLAYER_BAR_HEIGHT),
	m_pPlayer(_pPlayer)
{
	m_hDCPlayerBar = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("UI_PLAYERBAR"));
	if (_pPlayer) {
		RECT rcGauge;
		rcGauge.left = 125;
		rcGauge.top = 57;
		rcGauge.right = rcGauge.left + HP_BAR_WIDTH;
		rcGauge.bottom = rcGauge.top + HP_BAR_HEIGHT;
		m_pHpBar = new CUI_Gauge(_rGameWorld, _pPlayer, rcGauge, _pPlayer->GetMaxHp(), _pPlayer->GetHP());
		m_pHpBar->SetHDC(CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("UI_HPBAR")));

		rcGauge.left = 125;
		rcGauge.top = 97;
		rcGauge.right = rcGauge.left + MANA_BAR_WIDTH;
		rcGauge.bottom = rcGauge.top + MANA_BAR_HEIGHT;
		m_pManaBar = new CUI_Gauge(_rGameWorld, _pPlayer, rcGauge, _pPlayer->GetMaxMana(), _pPlayer->GetMana());
		m_pManaBar->SetHDC(CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("UI_MANABAR")));
	}
}

CUI_PlayerBar::~CUI_PlayerBar()
{
	Release();
}

int CUI_PlayerBar::Update(float _fDeltaTime)
{
	if (m_pHpBar) m_pHpBar->SetCurrentGauge(m_pPlayer->GetHP());
	if (m_pManaBar) m_pManaBar->SetCurrentGauge(m_pPlayer->GetMana());
	return 0;
}

void CUI_PlayerBar::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (m_pHpBar) m_pHpBar->Render(_hdc, _pCamera);
	if (m_pManaBar) m_pManaBar->Render(_hdc, _pCamera);

	RECT& rcDrawArea = GetRect();

	GdiTransparentBlt(_hdc,
		rcDrawArea.left,			// 출력 시작좌표 X
		rcDrawArea.top,			// 출력 시작좌표 Y
		rcDrawArea.right - rcDrawArea.left,					
		rcDrawArea.bottom - rcDrawArea.top,
		m_hDCPlayerBar,
		0,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));

	HFONT hFont = CreateFont(27, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
	HFONT hOldFont = (HFONT)SelectObject(_hdc, hFont);		// 폰트 설정
	SetTextColor(_hdc, RGB(255, 255, 255));					// 텍스트 색상 설정
	SetBkMode(_hdc, TRANSPARENT);							// 텍스트 배경색의 처리

	// 텍스트 출력
	swprintf_s(m_szHpState, TEXT("%d/%d"), static_cast<int>(m_pHpBar->GetCurrent()), static_cast<int>(m_pHpBar->GetMax()));
	TextOut(_hdc, GetX() + 40, GetY() - 40, m_szHpState, lstrlen(m_szHpState));
	SelectObject(_hdc, hOldFont);
	DeleteObject(hFont);
}

void CUI_PlayerBar::Release(void)
{
	DeleteSafe(m_pHpBar);
	DeleteSafe(m_pManaBar);
}
