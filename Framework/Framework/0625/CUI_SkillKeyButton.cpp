#include "stdafx.h"
#include "CUI_SkillKeyButton.h"
#include "CState.h"
#include "CUI_SkillKeyBinding.h"
#include "CPlayerWOL.h"



CUI_SkillKeyButton::CUI_SkillKeyButton(CGameWorld & _rGameWorld, CUI_SkillKeyBinding * _pSkillKeyBinding, float _fX, float _fY, CPlayerWOL* _pPlayer, SKILL::E_KEY _eSkillKeyType)
	:
	CObj(_rGameWorld, _fX, _fY, UI_SKILLKEY_BINDING_BUTTON_WIDTH, UI_SKILLKEY_BINDING_BUTTON_HEIGHT),
	m_pPlayer(_pPlayer),
	m_eSkillKeyType(_eSkillKeyType),
	m_pSkillKeyBinding(_pSkillKeyBinding)
{
}

CUI_SkillKeyButton::~CUI_SkillKeyButton()
{
}

int CUI_SkillKeyButton::Update(float _fDeltaTime)
{
	if (m_pSkillKeyBinding->IsVisible()) {
		POINT ptClientCursor = GetClientCursorPoint();
		if (IsPointInRect(GetRect(), ptClientCursor)) {
			m_pSkillKeyBinding->UpdateSkillInfo(m_eSkillKeyType);
			if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
				m_pSkillKeyBinding->SelectButton(m_eSkillKeyType);
			}
		}
	}

	return 0;
}

void CUI_SkillKeyButton::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (m_pSkillKeyBinding->IsVisible()) {
		if (m_pPlayer->GetSkill(m_eSkillKeyType)) {
			RECT& rcDrawArea = GetRect();

			BitBlt(_hdc,
				rcDrawArea.left,			// 출력 시작좌표 X
				rcDrawArea.top,			// 출력 시작좌표 Y
				rcDrawArea.right - rcDrawArea.left + 1,
				rcDrawArea.bottom - rcDrawArea.top + 1,
				m_pPlayer->GetSkill(m_eSkillKeyType)->GetStateHDC(STATE_HDC::STATE_HDC_ICON),
				0,
				0,
				SRCCOPY);
		}
	}
}
