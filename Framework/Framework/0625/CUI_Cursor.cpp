#include "stdafx.h"
#include "CUI_Cursor.h"



CUI_Cursor::CUI_Cursor(CGameWorld & _rGameWorld, const HDC& _rhDC)
	:
	CObj(_rGameWorld, 0.f, 0.f),
	m_rhDC(_rhDC)
{
	SetWidth(MOUSE_CURSOR_WITDH);
	SetHeight(MOUSE_CURSOR_HEIGHT);
	ShowCursor(false);
}

CUI_Cursor::~CUI_Cursor()
{
}

void CUI_Cursor::Ready()
{
}

int CUI_Cursor::Update(float fDeltaTime)
{
	POINT pos = GetClientCursorPoint();
	SetXY(float(pos.x), float(pos.y));

	return 0;
}

void CUI_Cursor::LateUpdate()
{
}

void CUI_Cursor::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT rcDrawArea = GetRect();
	GdiTransparentBlt(
		_hdc,
		rcDrawArea.left,
		rcDrawArea.top,
		rcDrawArea.right - rcDrawArea.left,
		rcDrawArea.bottom - rcDrawArea.top,
		m_rhDC,
		0, 0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
}

void CUI_Cursor::Release()
{

}
