#pragma once
#include "CObj.h"

class CGameWorld;
class CCamera2D;

class CUI_Cursor :
	public CObj
{
public:
	CUI_Cursor(CGameWorld& _rGameWorld, const HDC& _rhDC);
	virtual ~CUI_Cursor();

public:
	virtual void Ready() override;
	virtual int Update(float fDeltaTime) override;
	virtual void LateUpdate() override;
	virtual void Render(HDC & _hdc, CCamera2D* _pCamera) override;
	virtual void Release() override;

private:
	const HDC m_rhDC;
};

