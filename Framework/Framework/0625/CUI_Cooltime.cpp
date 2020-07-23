#include "stdafx.h"
#include "CUI_Cooltime.h"
#include "CState.h"
#include "UI_Gauge.h"
#include "CBitmapMgr.h"



CUI_Cooltime::CUI_Cooltime(CGameWorld & _rGameWorld, float _fX, float _fY, CState<CPlayerWOL>*& _prSkillState)
	:
	CObj(_rGameWorld, _fX, _fY, UI_SKILLBAR_ICON_OUTPUT_WIDTH, UI_SKILLBAR_ICON_OUTPUT_HEIGHT),
	m_prSkillState(_prSkillState)
{
	if(m_prSkillState) m_eCooltimeType = m_prSkillState->GetCooltimeType();
	m_pCooltimeGauge = new CUI_Gauge(_rGameWorld, nullptr, GetRect(), 0.f, 0.f);
	m_pCooltimeGauge->SetHDC(CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("COOLTIME_SHADOW")));
}

CUI_Cooltime::~CUI_Cooltime()
{
	DeleteSafe(m_pCooltimeGauge);
}

int CUI_Cooltime::Update(float _fDeltaTime)
{
	if (m_prSkillState) {
		m_pCooltimeGauge->SetMax(m_prSkillState->GetEndCooltime());
		m_pCooltimeGauge->SetCurrentGauge(m_prSkillState->GetEndCooltime() - m_prSkillState->GetCurCooltime());
	}
	return 0;
}

void CUI_Cooltime::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (m_prSkillState) {
		RECT& rcDrawArea = GetRect();

		GdiTransparentBlt(_hdc,
			rcDrawArea.left,			// 출력 시작좌표 X
			rcDrawArea.top,			// 출력 시작좌표 Y
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
			m_pCooltimeGauge->Render(_hdc, _pCamera);
			break;
		case SKILL_COOLTIME::TYPE_COUNT:
		{
			HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
			HFONT hOldFont = (HFONT)SelectObject(_hdc, hFont);		// 폰트 설정
			SetTextColor(_hdc, RGB(0, 0, 0));					// 텍스트 색상 설정
			SetBkMode(_hdc, TRANSPARENT);							// 텍스트 배경색의 처리

			// 텍스트 출력
			swprintf_s(m_szCount, TEXT("%d"), static_cast<int>(m_prSkillState->GetCurCooltime()));
			TextOut(_hdc, GetX()-10, GetY()-10, m_szCount, lstrlen(m_szCount));
			SelectObject(_hdc, hOldFont);
			DeleteObject(hFont);

			hFont = CreateFont(26, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
			hOldFont = (HFONT)SelectObject(_hdc, hFont);		// 폰트 설정
			SetTextColor(_hdc, RGB(255, 255, 255));					// 텍스트 색상 설정
			SetBkMode(_hdc, TRANSPARENT);							// 텍스트 배경색의 처리

			// 텍스트 출력
			swprintf_s(m_szCount, TEXT("%d"), static_cast<int>(m_prSkillState->GetCurCooltime()));
			TextOut(_hdc, GetX() - 10, GetY() - 10, m_szCount, lstrlen(m_szCount));
			SelectObject(_hdc, hOldFont);
			DeleteObject(hFont);
		}
			break;
		default:
			// 아무것도 표시하지 않는다.
			break;
		}
		

		g_iRenderCount++;
	}
}
