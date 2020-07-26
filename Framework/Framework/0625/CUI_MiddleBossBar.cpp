#include "stdafx.h"
#include "CUI_MiddleBossBar.h"
#include "CBitmapMgr.h"
#include "UI_Gauge.h"



CUI_MiddleBossBar::CUI_MiddleBossBar(CGameWorld & _rGameWorld, CObj * _pMiddleBoss)
	:
	CObj(_rGameWorld, (WINCX >> 1), 85.f, UI_MIDDLE_BOSS_NAMEBAR_WIDTH, UI_MIDDLE_BOSS_NAMEBAR_HEIGHT),
	m_pMiddleBoss(_pMiddleBoss)
{
	m_hDCMiddleBossBar = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("MIDDLEBOSS_NAMEBAR"));
	if (_pMiddleBoss) {
		RECT rcGauge;
		rcGauge.left = GetRect().left + 48;
		rcGauge.top = GetRect().top + 41;
		rcGauge.right = rcGauge.left + UI_MIDDLE_BOSS_NAMEBAR_WIDTH;
		rcGauge.bottom = rcGauge.top + UI_MIDDLE_BOSS_NAMEBAR_HEIGHT;
		m_pHpBar = new CUI_Gauge(_rGameWorld, _pMiddleBoss, rcGauge, _pMiddleBoss->GetMaxHp(), _pMiddleBoss->GetHP());
		m_pHpBar->SetHDC(CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BOSS_HPBAR")));
	}
}

CUI_MiddleBossBar::~CUI_MiddleBossBar()
{
	Release();
}

int CUI_MiddleBossBar::Update(float _fDeltaTime)
{
	if (m_pHpBar) m_pHpBar->SetCurrentGauge(m_pMiddleBoss->GetHP());
	return 0;
}

void CUI_MiddleBossBar::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (m_pHpBar) m_pHpBar->Render(_hdc, _pCamera);

	RECT& rcDrawArea = GetRect();

	GdiTransparentBlt(_hdc,
		rcDrawArea.left,			// 출력 시작좌표 X
		rcDrawArea.top,			// 출력 시작좌표 Y
		rcDrawArea.right - rcDrawArea.left,
		rcDrawArea.bottom - rcDrawArea.top,
		m_hDCMiddleBossBar,
		0,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
}

void CUI_MiddleBossBar::Release(void)
{
	DeleteSafe(m_pHpBar);
}
