#pragma once
#include "CObj.h"

template<typename T> class CStateMgr;
class CSpawnerGenerator;
// class CUI_BossBar;

class CBoss_Boss :
	public CObj
{
public:
	// ������ ���ڵ� ������
	CBoss_Boss(CGameWorld& _rGameWorld, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	// �׽�Ʈ�� ���ڵ� ������
	CBoss_Boss(CGameWorld& _rGameWorld, float _fX, float _fY, int _iGroupID = -1, CObj* _pTarget = nullptr, CSpawnerGenerator* _pSpawnerGenerator = nullptr);
	virtual ~CBoss_Boss();

public:
	virtual int Update(float _fDeltaTime)				override;
	virtual void LateUpdate(void)						override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera)	override;
	virtual void Release(void)							override;
	virtual void ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT& _rcCollisionRect) override;

public:
	void SetTarget(CObj* pTarget) { m_pTarget = pTarget; }
	CObj* GetTarget(void) const { return m_pTarget; }
	CStateMgr<CBoss_Boss>* GetStateMgr(void) const { return m_pStateMgr; }
	virtual void Attacked(float _fDamageAmount, POINT _ptCollisionPoint);
	bool DirectDirectionToTarget(void);
	MONSTER::E_DIRECTION GetBossDir(void) const { return m_eBossDir; }
	void SetBoxAttackUsing(bool _bIsBoxAttackUsing) { m_bIsBoxAttackUsing = _bIsBoxAttackUsing; }
	bool IsBoxAttackUsing(void) { return m_bIsBoxAttackUsing; }
	void SetAttackState(BOSS::E_STATE _eAttackState) { m_eAttackState = _eAttackState; }
	BOSS::E_STATE GetAttackState(void) const { return m_eAttackState; }
	void GenerateBox(float _fStartDegree, float _fRangeDegree, int _iNum);
	bool ThrowBox(float _fDelay = 0.8f, float _fSpeed = 1950.f, bool _bGenerateStonePillar = false);	// �ڽ��� ���ȴٸ� true, �ڽ��� ��� ������ �������� false
	void ClearBox(void);
	void SwapAttackState();
	void GenerateRangeStonePillar(float _fStart, float _fEnd, float _fInterval);

private:
	void SetInitInfo(void);

private:
	CObj* m_pTarget = nullptr;
	CStateMgr<CBoss_Boss>* m_pStateMgr = nullptr;
	bool m_bIsBoxAttackUsing = false;
	HDC m_hDCAttackKeyAtlas;
	HDC m_hDCKeyAtlas[MONSTER::DIR_END];
	BOSS::E_STATE m_eAttackState = BOSS::STATE_LEFT_ATTACK;
	MONSTER::E_DIRECTION m_eBossDir = MONSTER::DIR_END;
	CSpawnerGenerator* m_pSpawnerGenerator = nullptr;
	CObj* m_pBossBarUI = nullptr;
	
	int m_iBoxNum = 0;
	int m_iBoxIndex = 0;
	CObj* m_pBox[20] = { nullptr, };
};

