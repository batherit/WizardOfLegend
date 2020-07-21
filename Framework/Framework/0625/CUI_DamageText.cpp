#include "stdafx.h"
#include "CUI_DamageText.h"



CUI_DamageText::CUI_DamageText(CGameWorld & _rGameWorld, float _fX, float _fY, int _iDamage)
	:
	CObj(_rGameWorld, _fX, _fY, 100, 100)
{
	swprintf_s(m_szDamage, TEXT("%d"), _iDamage);
}

CUI_DamageText::~CUI_DamageText()
{
	Release();
}

int CUI_DamageText::Update(float _fDeltaTime)
{
	return 0;
}

void CUI_DamageText::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���"));
	HFONT hOldFont = (HFONT)SelectObject(_hdc, hFont);		// ��Ʈ ����
	SetTextColor(_hdc, RGB(240, 240, 240));					// �ؽ�Ʈ ���� ����
	SetBkMode(_hdc, TRANSPARENT);							// �ؽ�Ʈ ������ ó��

	// �ؽ�Ʈ ���
	TextOut(_hdc, m_fX, m_fY, m_szDamage, lstrlen(m_szDamage));
	SelectObject(_hdc, hOldFont);
	DeleteObject(hFont);
}
