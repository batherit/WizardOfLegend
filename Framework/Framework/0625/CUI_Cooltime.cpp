#include "stdafx.h"
#include "CUI_Cooltime.h"
#include "CState.h"



CUI_Cooltime::CUI_Cooltime(CGameWorld & _rGameWorld, float _fX, float _fY, CState<CPlayerWOL>*& _prSkillState)
	:
	CObj(_rGameWorld, _fX, _fY, UI_SKILLBAR_ICON_OUTPUT_WIDTH, UI_SKILLBAR_ICON_OUTPUT_HEIGHT),
	m_prSkillState(_prSkillState)
{
	if(m_prSkillState) m_eCooltimeType = m_prSkillState->GetCooltimeType();
}

CUI_Cooltime::~CUI_Cooltime()
{
}

int CUI_Cooltime::Update(float _fDeltaTime)
{
	return 0;
}

void CUI_Cooltime::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (m_prSkillState) {
		RECT& rcDrawArea = GetRect();

		GdiTransparentBlt(_hdc,
			rcDrawArea.left,			// ��� ������ǥ X
			rcDrawArea.top,			// ��� ������ǥ Y
			rcDrawArea.right - rcDrawArea.left,
			rcDrawArea.bottom - rcDrawArea.top,
			m_prSkillState->GetStateIcon(),
			0,
			0,
			m_iWidth,
			m_iHeight,
			RGB(255, 0, 255));

		switch (m_prSkillState->GetCooltimeType()) {
		case SKILL_COOLTIME::TYPE_NORMAL: 
		
			break;
		case SKILL_COOLTIME::TYPE_COUNT:
		{
			HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
			HFONT hOldFont = (HFONT)SelectObject(_hdc, hFont);		// ��Ʈ ����
			SetTextColor(_hdc, RGB(0, 0, 0));					// �ؽ�Ʈ ���� ����
			SetBkMode(_hdc, TRANSPARENT);							// �ؽ�Ʈ ������ ó��

			// �ؽ�Ʈ ���
			swprintf_s(m_szCount, TEXT("%d"), static_cast<int>(m_prSkillState->GetCurCooltime()));
			TextOut(_hdc, GetX()-10, GetY()-10, m_szCount, lstrlen(m_szCount));
			SelectObject(_hdc, hOldFont);
			DeleteObject(hFont);

			hFont = CreateFont(26, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
			hOldFont = (HFONT)SelectObject(_hdc, hFont);		// ��Ʈ ����
			SetTextColor(_hdc, RGB(255, 255, 255));					// �ؽ�Ʈ ���� ����
			SetBkMode(_hdc, TRANSPARENT);							// �ؽ�Ʈ ������ ó��

			// �ؽ�Ʈ ���
			swprintf_s(m_szCount, TEXT("%d"), static_cast<int>(m_prSkillState->GetCurCooltime()));
			TextOut(_hdc, GetX() - 10, GetY() - 10, m_szCount, lstrlen(m_szCount));
			SelectObject(_hdc, hOldFont);
			DeleteObject(hFont);
		}
			break;
		default:
			// �ƹ��͵� ǥ������ �ʴ´�.
			break;
		}
		

		g_iRenderCount++;
	}
}
