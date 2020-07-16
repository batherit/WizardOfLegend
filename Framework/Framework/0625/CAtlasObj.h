#pragma once
#include "CObj.h"

class CMapLoader;

class CAtlasObj :
	public CObj
{
public:
	CAtlasObj(FILE* _fpIn, CGameWorld& _rGameWorld);
	~CAtlasObj();

public:
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;

private:
	static const TCHAR* ctBitmapKey[3];

private:
	int m_iAtlasID;
	RECT m_rcOutputArea;
	HDC m_hMemdc;
};

