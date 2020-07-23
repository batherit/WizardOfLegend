#include "stdafx.h"
#include "CUI_SkillBar.h"
#include "CBitmapMgr.h"


CUI_SkillBar::CUI_SkillBar(CGameWorld & _rGameWorld, CObj * _pPlayer)
	:
	CObj(_rGameWorld, (UI_SKILLBAR_OUTPUT_WIDTH >> 1) + 50, WINCY - 100.f, UI_SKILLBAR_OUTPUT_WIDTH, UI_SKILLBAR_OUTPUT_HEIGHT),
	m_pPlayer(_pPlayer)
{
	m_hDCSkillBar = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("UI_SKILLBAR"));
}

CUI_SkillBar::~CUI_SkillBar()
{
	Release();
}

int CUI_SkillBar::Update(float _fDeltaTime)
{
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
}

void CUI_SkillBar::Release(void)
{
}
