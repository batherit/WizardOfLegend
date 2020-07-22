#include "stdafx.h"
#include "CUI_Money.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"



CUI_Money::CUI_Money(CGameWorld & _rGameWorld, float _fX, float _fY, CObj & _rOwner)
	:
	CObj(_rGameWorld, _fX, _fY, UI_MONEY_OUTPUT_WIDTH, UI_MONEY_OUTPU_HEIGHT),
	m_rOwner(_rOwner)
{
	m_hDCKey = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("UI_MONEY"));
}

CUI_Money::~CUI_Money()
{
}

int CUI_Money::Update(float _fDeltaTime)
{
	swprintf_s(m_szMoney, TEXT("%d"), m_rOwner.GetMoney());

	return 0;
}

void CUI_Money::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	GdiTransparentBlt(_hdc,
		rcDrawArea.left,			// ��� ������ǥ X
		rcDrawArea.top,			// ��� ������ǥ Y
		rcDrawArea.right - rcDrawArea.left,
		rcDrawArea.bottom - rcDrawArea.top,
		m_hDCKey,
		0,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));

	HFONT hFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���"));
	HFONT hOldFont = (HFONT)SelectObject(_hdc, hFont);		// ��Ʈ ����
	SetTextColor(_hdc, RGB(240, 240, 240));					// �ؽ�Ʈ ���� ����
	SetBkMode(_hdc, TRANSPARENT);							// �ؽ�Ʈ ������ ó��

	// �ؽ�Ʈ ���
	TextOut(_hdc, GetX() + 30, GetY() - 15, m_szMoney, lstrlen(m_szMoney));
	SelectObject(_hdc, hOldFont);
	DeleteObject(hFont);

	g_iRenderCount++;
}