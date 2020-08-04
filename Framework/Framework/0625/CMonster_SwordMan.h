#pragma once
#include "CObj.h"

template<typename T> class CStateMgr;
class CSpawnerGenerator;

class CMonster_SwordMan :
	public CObj
{
public:
	CMonster_SwordMan(CGameWorld& _rGameWorld, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	CMonster_SwordMan(CGameWorld& _rGameWorld, float _fX, float _fY, int _iGroupID = -1, CObj* _pTarget = nullptr, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	virtual ~CMonster_SwordMan();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

public:
	void SetTarget(CObj* pTarget) { m_pTarget = pTarget; }
	CObj* GetTarget(void) const { return m_pTarget; }
	CStateMgr<CMonster_SwordMan>* GetStateMgr(void) const { return m_pStateMgr; }
	void SetNewStateAnim(SWORDMAN::E_STATE _eNewState, bool _bReset = false);
	virtual void Attacked(float _fDamageAmount, POINT _ptCollisionPoint);
	//void Spawn(float _fX, float _fY);
	//bool IsSpawning(void) { return m_pSpawnEffect != nullptr; }
	bool GoToTarget(float _fDeltaTime);
	bool IsAttackable(void);
	bool DirectDirectionToTarget(void);
	SWORDMAN::E_DIRECTION GetSwordManDir(void) const { return m_eSwordManDir; }
	virtual void ReactToCollider(CObj * _pCollider, POINT& _ptCollisionPoint) override;


private:
	void SetInitInfo(void);

private:
	CObj* m_pTarget = nullptr;
	CStateMgr<CMonster_SwordMan>* m_pStateMgr = nullptr;
	HDC  m_hDCKeyAtlas[SWORDMAN::DIR_END];
	SWORDMAN::E_STATE m_eState = SWORDMAN::STATE_END;
	SWORDMAN::E_DIRECTION m_eSwordManDir = SWORDMAN::DIR_END;
	CSpawnerGenerator* m_pSpawnerGenerator = nullptr;
};

