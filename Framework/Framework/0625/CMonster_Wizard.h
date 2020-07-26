#pragma once
#include "CObj.h"

template<typename T> class CStateMgr;
class CSpawnerGenerator;

class CMonster_Wizard :
	public CObj
{
public:
	// 스폰용 위자드 생성자
	CMonster_Wizard(CGameWorld& _rGameWorld, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	// 테스트용 위자드 생성자
	CMonster_Wizard(CGameWorld& _rGameWorld, float _fX, float _fY, int _iGroupID = -1, CObj* _pTarget = nullptr, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	virtual ~CMonster_Wizard();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

public:
	void SetTarget(CObj* pTarget) { m_pTarget = pTarget; }
	CObj* GetTarget(void) const { return m_pTarget; }
	CStateMgr<CMonster_Wizard>* GetStateMgr(void) const { return m_pStateMgr; }
	void SetNewStateAnim(WIZARD::E_STATE _eNewState, bool _bReset = false);
	virtual void Attacked(float _fDamageAmount, POINT _ptCollisionPoint);
	bool GoToAttackableLocation(float _fDeltaTime);
	bool IsAttackable(void);
	bool DirectDirectionToTarget(void);
	MONSTER::E_DIRECTION GetWizardDir(void) const { return m_eWizardDir; }
	int GetSummonedWizardBalls(void) const { return m_listWizardBalls.size(); }
	list<CObj*>& GetListWizardBalls(void) { return m_listWizardBalls; }

private:
	void SetInitInfo(void);

private:
	CObj* m_pTarget = nullptr;
	CStateMgr<CMonster_Wizard>* m_pStateMgr = nullptr;
	HDC m_hDCKeyAtlas[MONSTER::DIR_END];
	WIZARD::E_STATE m_eState = WIZARD::STATE_END;
	MONSTER::E_DIRECTION m_eWizardDir = MONSTER::DIR_END;
	CSpawnerGenerator* m_pSpawnerGenerator = nullptr;
	list<CObj*> m_listWizardBalls;
};

