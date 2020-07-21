#pragma once
#include "CObj.h"
class CArcherBow :
	public CObj
{
public:
	CArcherBow(CGameWorld& _rGameWorld, CObj& _rOwner);
	virtual ~CArcherBow();

public:
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;
	int GetAnimState(void) const;


private:
	CObj& m_rOwner;
	HDC m_hDCKeyAtlas[ARCHER::DIR_END] = { nullptr, };
};

