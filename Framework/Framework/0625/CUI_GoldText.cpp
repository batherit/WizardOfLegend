#include "stdafx.h"
#include "CUI_GoldText.h"
#include "CCamera2D.h"



CUI_GoldText::CUI_GoldText(CGameWorld & _rGameWorld, float _fX, float _fY, int _iMoneyAmount)
	:
	CObj(_rGameWorld, _fX, _fY, 0.f, 0.f)
{
	swprintf_s(m_szMoney, TEXT("+%d"), _iMoneyAmount);
	SetSpeed(100.f);
	SetToXY(0.f, -1.f);
	CSoundMgr::Get_Instance()->PlaySound(TEXT("MONEY_TAKE.mp3"), CSoundMgr::UI);
}

CUI_GoldText::~CUI_GoldText()
{
}

int CUI_GoldText::Update(float _fDeltaTime)
{
	if ((m_fElapsedTime += _fDeltaTime) > 1.0f) {
		SetValid(false);
		return 1;
	}
	MoveByDeltaTime(_fDeltaTime);
	return 0;
}

void CUI_GoldText::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	HFONT hFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("고딕"));
	HFONT hOldFont = (HFONT)SelectObject(_hdc, hFont);		// 폰트 설정
	SetTextColor(_hdc, RGB(240, 209, 125));					// 텍스트 색상 설정
	SetBkMode(_hdc, TRANSPARENT);							// 텍스트 배경색의 처리

	// 텍스트 출력
	pair<float, float> pairCurrentPoint = _pCamera->GetScreenPoint(GetX(), GetY());

	TextOut(_hdc, pairCurrentPoint.first, pairCurrentPoint.second, m_szMoney, lstrlen(m_szMoney));
	SelectObject(_hdc, hOldFont);
	DeleteObject(hFont);
}
