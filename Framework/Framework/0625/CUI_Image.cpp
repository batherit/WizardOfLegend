#include "stdafx.h"
#include "CUI_Image.h"



CUI_Image::CUI_Image(CGameWorld & _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight)
{
}

CUI_Image::~CUI_Image()
{
}

void CUI_Image::Render(HDC & _hdc, CCamera2D * pCamera)
{
	if (m_hDCImage) {
		BitBlt(_hdc,
			GetRect().left, GetRect().top,
			GetRect().right - GetRect().left,
			GetRect().bottom - GetRect().top,
			m_hDCImage,
			0, 0, SRCCOPY);
	}
}
