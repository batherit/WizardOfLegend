#include "stdafx.h"
#include "CUI_PlayerBar.h"
#include "UI_Gauge.h"
#include "CBitmapMgr.h"



CUI_PlayerBar::CUI_PlayerBar(CGameWorld & _rGameWorld, CObj* _pPlayer)
	:
	CObj(_rGameWorld, 210.f, 85.f, PLAYER_BAR_WIDTH, PLAYER_BAR_HEIGHT),
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
}

void CUI_PlayerBar::Release(void)
{
	DeleteSafe(m_pHpBar);
	DeleteSafe(m_pManaBar);
}
