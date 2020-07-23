#pragma once
#include "CObj.h"
class CUI_Image :
	public CObj
{
public:
	CUI_Image(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight);
	virtual ~CUI_Image();

public:
	void SetHDC(const HDC& _hdc) { m_hDCImage = _hdc; }
	void Clear(void) { m_hDCImage = nullptr; }
	void Render(HDC& _hdc, CCamera2D* pCamera);

public:
	HDC m_hDCImage = nullptr;
};

