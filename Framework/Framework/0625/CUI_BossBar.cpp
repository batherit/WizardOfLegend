#include "stdafx.h"
#include "CUI_BossBar.h"
#include "CBitmapMgr.h"
#include "UI_Gauge.h"



CUI_BossBar::CUI_BossBar(CGameWorld & _rGameWorld, CObj * _pBoss)
	:
	CObj(_rGameWorld, (WINCX >> 1), 85.f, UI_BOSS_NAMEBAR_WIDTH, UI_BOSS_NAMEBAR_HEIGHT),
	m_pBoss(_pBoss)
{
	m_hDCBossBar = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BOSS_NAMEBAR"));
	if (_pBoss) {
		RECT rcGauge;
		rcGauge.left = GetRect().left + 48;
		rcGauge.top = GetRect().top + 41;
		rcGauge.right = rcGauge.left + UI_BOSS_NAMEBAR_WIDTH;
		rcGauge.bottom = rcGauge.top + UI_BOSS_NAMEBAR_HEIGHT;
		m_pHpBar = new CUI_Gauge(_rGameWorld, _pBoss, rcGauge, _pBoss->GetMaxHp(), _pBoss->GetHP());
		m_pHpBar->SetHDC(CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BOSS_HPBAR")));
	}
}

CUI_BossBar::~CUI_BossBar()
{
	Release();
}

int CUI_BossBar::Update(float _fDeltaTime)
{
	if (m_pHpBar) m_pHpBar->SetCurrentGauge(m_pBoss->GetHP());
	return 0;
}

void CUI_BossBar::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (m_bIsVisible) {
		if (m_pHpBar) m_pHpBar->Render(_hdc, _pCamera);

		RECT& rcDrawArea = GetRect();

		GdiTransparentBlt(_hdc,
			rcDrawArea.left,			// 출력 시작좌표 X
			rcDrawArea.top,			// 출력 시작좌표 Y
			rcDrawArea.right - rcDrawArea.left,
			rcDrawArea.bottom - rcDrawArea.top,
			m_hDCBossBar,
			0,
			0,
			m_iWidth,
			m_iHeight,
			RGB(255, 0, 255));
	}
}

void CUI_BossBar::Release(void)
{
	DeleteSafe(m_pHpBar);
}
