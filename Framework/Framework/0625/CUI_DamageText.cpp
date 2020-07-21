#include "stdafx.h"
#include "CUI_DamageText.h"
#include "CCamera2D.h"



CUI_DamageText::CUI_DamageText(CGameWorld & _rGameWorld, float _fX, float _fY, POINT ptCollisionPoint, int _iDamage)
	:
	CObj(_rGameWorld, _fX, _fY, 0.f, 0.f)
{
	swprintf_s(m_szDamage, TEXT("%d"), _iDamage);
	if (_fX - ptCollisionPoint.x > 0) {
		// ���ʿ��� �浹�� �Ͼ ���, ���ڴ� ���������� Ƣ��� �Ѵ�.
		m_ptPoint0.x = _fX;
		m_ptPoint0.y = _fY;
		m_ptPoint1.x = _fX + 50;
		m_ptPoint1.y = _fY - 300;
		m_ptPoint2.x = _fX + 100;
		m_ptPoint2.y = _fY - 100;
	}
	else {
		// ���� ���
		m_ptPoint0.x = _fX;
		m_ptPoint0.y = _fY;
		m_ptPoint1.x = _fX - 50;
		m_ptPoint1.y = _fY - 300;
		m_ptPoint2.x = _fX - 100;
		m_ptPoint2.y = _fY - 100;
	}
}

CUI_DamageText::~CUI_DamageText()
{
	Release();
}

int CUI_DamageText::Update(float _fDeltaTime)
{
	DO_IF_IS_VALID_OBJ(this) {
		m_fElapsedTime += _fDeltaTime;
		float fT = m_fElapsedTime / UI_DAMAGE_TEXT_KEEP_TIME;
		if (fT >= 1.f) {
			SetValid(false);
			return 1;
		}
		Clamp(&fT, 0.f, 1.f);
		m_ptCurrentPoint.x = (1.f - fT) * (1.f - fT) * m_ptPoint0.x + 2.f * fT * (1.f - fT)*m_ptPoint1.x + fT * fT * m_ptPoint2.x;
		m_ptCurrentPoint.y = (1.f - fT) * (1.f - fT) * m_ptPoint0.y + 2.f * fT * (1.f - fT)*m_ptPoint1.y + fT * fT * m_ptPoint2.y;
		m_fCurrentSize = (1.f - fT) * (1.f - fT) * UI_DAMAGE_TEXT_MIN_SIZE + 2.f * fT * (1.f - fT)*UI_DAMAGE_TEXT_MAX_SIZE + fT * fT * UI_DAMAGE_TEXT_MIN_SIZE;
	}
	return 0;
}

void CUI_DamageText::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	HFONT hFont = CreateFont(m_fCurrentSize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
	HFONT hOldFont = (HFONT)SelectObject(_hdc, hFont);		// ��Ʈ ����
	SetTextColor(_hdc, RGB(240, 240, 240));					// �ؽ�Ʈ ���� ����
	SetBkMode(_hdc, TRANSPARENT);							// �ؽ�Ʈ ������ ó��

	// �ؽ�Ʈ ���
	pair<float, float> pairCurrentPoint = _pCamera->GetScreenPoint(m_ptCurrentPoint.x, m_ptCurrentPoint.y);

	TextOut(_hdc, pairCurrentPoint.first, pairCurrentPoint.second, m_szDamage, lstrlen(m_szDamage));
	SelectObject(_hdc, hOldFont);
	DeleteObject(hFont);
}
