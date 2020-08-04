#pragma once
#include "CObj.h"

class CBitmapObj;

class CDoor :
	public CObj
{
public:
	CDoor(FILE* _fpIn, CGameWorld& _rGameWorld);
	~CDoor();

public:
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;

private:
	TCHAR* m_tDoorBitmapKey = L"";
	HDC m_hMemdc;
	const CBitmapObj* m_pBitmapObj;
	int iWidth = 0;
	int iHeight = 0;
};

