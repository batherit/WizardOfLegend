#pragma once
#include "CObj.h"
class CPlayerWOL :
	public CObj
{
public:
	CPlayerWOL(CGameWorld& _rGameWorld);
	CPlayerWOL(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth = ciPlayerSize, size_t _iHeight = ciPlayerSize, float _fSpeed = cfPlayerRunSpeed);
	virtual ~CPlayerWOL();

public:
	virtual void Ready(void)							override;
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

private:
	void SetInitInfo(void);
	void SetNewState(PLAYER::E_STATE _eNewState);

private:
	OBJ::E_DIRECTION m_eDir = OBJ::DIR_DOWN;
	HDC m_hDCKeyAtlas[OBJ::DIR_END];
	PLAYER::E_STATE m_eState = PLAYER::STATE_END;
};

