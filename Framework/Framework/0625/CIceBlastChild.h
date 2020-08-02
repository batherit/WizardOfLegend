#pragma once
#include "CObj.h"
class CIceBlastChild :
	public CObj
{
public:
	CIceBlastChild(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CIceBlastChild();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

private:
	HDC m_hDCKeyAtlas = nullptr;
	float m_fLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
	bool m_bDestroyAnimInit = false;
};

