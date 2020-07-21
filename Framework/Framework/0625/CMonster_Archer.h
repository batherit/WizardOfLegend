#pragma once
#include "CObj.h"

template<typename T> class CStateMgr;
class CSpawnerGenerator;

class CMonster_Archer :
	public CObj
{
public:
	// 스폰용 아쳐 생성자
	CMonster_Archer(CGameWorld& _rGameWorld, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	// 테스트용 아쳐 생성자
	CMonster_Archer(CGameWorld& _rGameWorld, float _fX, float _fY, int _iGroupID = -1, CObj* _pTarget = nullptr, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	virtual ~CMonster_Archer();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

public:
	void SetTarget(CObj* pTarget) { m_pTarget = pTarget; }
	CObj* GetTarget(void) const { return m_pTarget; }
	CStateMgr<CMonster_Archer>* GetStateMgr(void) const { return m_pStateMgr; }
	void SetNewStateAnim(ARCHER::E_STATE _eNewState, bool _bReset = false);
	virtual void Attacked(float _fDamageAmount, POINT _ptCollisionPoint);
	//void Spawn(float _fX, float _fY);
	//bool IsSpawning(void) { return m_pSpawnEffect != nullptr; }
	bool GoToTarget(float _fDeltaTime);
	bool IsAttackable(void);
	bool DirectDirectionToTarget(void);
	ARCHER::E_DIRECTION GetArcherDir(void) const { return m_eArcherDir; }


private:
	void SetInitInfo(void);

private:
	CObj* m_pTarget = nullptr;
	CStateMgr<CMonster_Archer>* m_pStateMgr = nullptr;
	HDC m_hDCKeyAtlas[ARCHER::DIR_END];
	HDC m_hDCBowAtlas[ARCHER::DIR_END];
	ARCHER::E_STATE m_eState = ARCHER::STATE_END;
	ARCHER::E_DIRECTION m_eArcherDir = ARCHER::DIR_END;
	CSpawnerGenerator* m_pSpawnerGenerator = nullptr;
};

