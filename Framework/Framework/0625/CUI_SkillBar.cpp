#include "stdafx.h"
#include "CUI_SkillBar.h"
#include "CBitmapMgr.h"
#include "CUI_Cooltime.h"
#include "CPlayerWOL.h"


CUI_SkillBar::CUI_SkillBar(CGameWorld & _rGameWorld, CObj * _pPlayer)
	:
	CObj(_rGameWorld, (UI_SKILLBAR_OUTPUT_WIDTH >> 1) + 50, WINCY - 100.f, UI_SKILLBAR_OUTPUT_WIDTH, UI_SKILLBAR_OUTPUT_HEIGHT),
	m_pPlayer(_pPlayer)
{
	m_hDCSkillBar = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("UI_SKILLBAR"));

	// 플레이어의 스킬바와 동기화
	for (int i = 0; i < SKILL::KEY_END; i++) {
		// 77 => 슬롯 간 간격
		m_pCooltimeUI[i] = new CUI_Cooltime(_rGameWorld, GetRect().left + 35 + 77 * i, GetRect().top + 90, TO_PLAYER_WOL(m_pPlayer)->GetSkill(static_cast<SKILL::E_KEY>(i)));
	}
}

CUI_SkillBar::~CUI_SkillBar()
{
	Release();
}

int CUI_SkillBar::Update(float _fDeltaTime)
{
	for (int i = 0; i < SKILL::KEY_END; i++) {
		m_pCooltimeUI[i]->Update(_fDeltaTime);
	}
	return 0;
}

void CUI_SkillBar::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	GdiTransparentBlt(_hdc,
		rcDrawArea.left,			// 출력 시작좌표 X
		rcDrawArea.top,			// 출력 시작좌표 Y
		rcDrawArea.right - rcDrawArea.left,
		rcDrawArea.bottom - rcDrawArea.top,
		m_hDCSkillBar,
		0,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));

	for (int i = 0; i < SKILL::KEY_END; i++) {
		m_pCooltimeUI[i]->Render(_hdc, _pCamera);
	}
}

void CUI_SkillBar::Release(void)
{
	m_pPlayer = nullptr;
	for (int i = 0; SKILL::KEY_END; i++) {
		m_pCooltimeUI[i] = nullptr;
	}
}
