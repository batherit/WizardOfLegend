#pragma once
#include "CObj.h"

template<typename T> class CStateMgr;

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

public:
	CStateMgr<CPlayerWOL>* GetStateMgr(void) const { return m_pStateMgr; }
	void SetNewState(PLAYER::E_STATE _eNewState);
	const PLAYER::E_STATE GetLastAttackState(void) const { return m_eLastAttackState; }
	bool IsMoveKeyPressed(float& _fToX, float& _fToY);
	virtual void Attacked(float _fDamageAmount);

private:
	void SetInitInfo(void);

private:
	CStateMgr<CPlayerWOL>* m_pStateMgr = nullptr;
	HDC m_hDCKeyAtlas[OBJ::DIR_END];
	PLAYER::E_STATE m_eState = PLAYER::STATE_END;
	PLAYER::E_STATE m_eLastAttackState = PLAYER::STATE_ATTACK1;
};

