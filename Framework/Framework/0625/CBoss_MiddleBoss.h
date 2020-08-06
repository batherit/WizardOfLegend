#pragma once
#include "CObj.h"

template<typename T> class CStateMgr;
class CSpawnerGenerator;
class CUI_MiddleBossBar;

class CBoss_MiddleBoss :
	public CObj
{
public:
	// 스폰용 위자드 생성자
	CBoss_MiddleBoss(CGameWorld& _rGameWorld, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	// 테스트용 위자드 생성자
	CBoss_MiddleBoss(CGameWorld& _rGameWorld, float _fX, float _fY, int _iGroupID = -1, CObj* _pTarget = nullptr, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	virtual ~CBoss_MiddleBoss();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;
	virtual void ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT& _rcCollisionRect) override;

public:
	void SetTarget(CObj* pTarget) { m_pTarget = pTarget; }
	CObj* GetTarget(void) const { return m_pTarget; }
	CStateMgr<CBoss_MiddleBoss>* GetStateMgr(void) const { return m_pStateMgr; }
	void SetNewStateAnim(WIZARD::E_STATE _eNewState, bool _bReset = false);
	virtual void Attacked(float _fDamageAmount, POINT _ptCollisionPoint);
	bool GoToAttackableLocation(float _fDeltaTime);
	void RunToProperLocation(float _fDeltaTime);
	bool IsAttackable(void);
	bool DirectDirectionToTarget(void);
	MONSTER::E_DIRECTION GetMiddelBossDir(void) const { return m_eMiddleBossDir; }
	int GetSummonedMonstersNum(void) const { return m_listMonsters.size(); }
	list<CObj*>& GetListMonsters(void) { return m_listMonsters; }

private:
	void SetInitInfo(void);

private:
	CObj* m_pTarget = nullptr;
	CStateMgr<CBoss_MiddleBoss>* m_pStateMgr = nullptr;
	HDC m_hDCKeyAtlas[MONSTER::DIR_END];
	MIDDLE_BOSS::E_STATE m_eState = MIDDLE_BOSS::STATE_END;
	MONSTER::E_DIRECTION m_eMiddleBossDir = MONSTER::DIR_END;
	CSpawnerGenerator* m_pSpawnerGenerator = nullptr;
	list<CObj*> m_listMonsters;
	float m_fInitX = 0.f;
	float m_fInitY = 0.f;
	CObj* m_pMiddleBossBarUI = nullptr;
};

