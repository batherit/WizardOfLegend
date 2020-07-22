#pragma once
#include "CObj.h"

template<typename T> class CStateMgr;
class CSpawnerGenerator;

class CMonster_WizardBall :
	public CObj
{
public:
	// 스폰용 위자드볼 생성자
	CMonster_WizardBall(CGameWorld& _rGameWorld, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	// 테스트용 위자드볼 생성자
	CMonster_WizardBall(CGameWorld& _rGameWorld, float _fX, float _fY, int _iGroupID = -1, CObj* _pTarget = nullptr, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	virtual ~CMonster_WizardBall();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;

public:
	void SetTarget(CObj* pTarget) { m_pTarget = pTarget; }
	CObj* GetTarget(void) const { return m_pTarget; }
	CStateMgr<CMonster_WizardBall>* GetStateMgr(void) const { return m_pStateMgr; }
	// void SetNewStateAnim(WIZARD_BALL::E_STATE _eNewState, bool _bReset = false);
	virtual void Attacked(float _fDamageAmount, POINT _ptCollisionPoint);
	bool GoToTarget(float _fDeltaTime);
	bool IsAttackable(void);
	bool DirectDirectionToTarget(void);
	int GetSpriteIndex(void);

private:
	void SetInitInfo(void);

private:
	CObj* m_pTarget = nullptr;
	CStateMgr<CMonster_WizardBall>* m_pStateMgr = nullptr;
	WIZARD_BALL::E_STATE m_eState = WIZARD_BALL::STATE_END;
	HDC m_hDCKeyAtlas = nullptr;
	CSpawnerGenerator* m_pSpawnerGenerator = nullptr;
};

