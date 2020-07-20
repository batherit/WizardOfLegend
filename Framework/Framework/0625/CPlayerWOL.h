#pragma once
#include "CObj.h"

template<typename T> class CStateMgr;

class CPlayerWOL :
	public CObj
{
public:
	CPlayerWOL(CGameWorld& _rGameWorld);
	CPlayerWOL(CGameWorld& _rGameWorld, float _fX, float _fY);
	virtual ~CPlayerWOL();

public:
	virtual void Ready(void)							override;
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

public:
	CStateMgr<CPlayerWOL>* GetStateMgr(void) const { return m_pStateMgr; }
	void SetNewStateAnim(PLAYER::E_STATE _eNewState, bool _bReset = false);
	const PLAYER::E_STATE GetLastAttackState(void) const { return m_eLastAttackState; }
	bool IsMoveKeyPressed(float& _fToX, float& _fToY);
	virtual void Attacked(float _fDamageAmoun, POINT _ptCollisionPointt);
	//void Spawn(float _fX, float _fY);		// 데이터를 유지하며 스폰
	//bool IsSpawning(void) { return m_pSpawnEffect != nullptr; }
	//void Respawn(float _fX, float _fY);		// 데이터를 초기 데이터로 리셋하고 스폰.
	void SetInitInfo(void);

private:
	CStateMgr<CPlayerWOL>* m_pStateMgr = nullptr;
	HDC m_hDCKeyAtlas[OBJ::DIR_END];
	PLAYER::E_STATE m_eState = PLAYER::STATE_END;
	PLAYER::E_STATE m_eLastAttackState = PLAYER::STATE_ATTACK1;
};

