#pragma once
#include "CObj.h"

template<typename T> class CStateMgr;
template<typename T> class CState;

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
	void UpdateSkillKey(void);
	virtual void Attacked(float _fDamageAmoun, POINT _ptCollisionPointt);
	//void Spawn(float _fX, float _fY);		// �����͸� �����ϸ� ����
	//bool IsSpawning(void) { return m_pSpawnEffect != nullptr; }
	//void Respawn(float _fX, float _fY);		// �����͸� �ʱ� �����ͷ� �����ϰ� ����.
	void SetInitInfo(void);
	CState<CPlayerWOL>* GetUsingSkill(void) { return m_pUsingSkill; }

private:
	CStateMgr<CPlayerWOL>* m_pStateMgr = nullptr;
	HDC m_hDCKeyAtlas[OBJ::DIR_END];
	PLAYER::E_STATE m_eState = PLAYER::STATE_END;
	PLAYER::E_STATE m_eLastAttackState = PLAYER::STATE_ATTACK1;
	CState<CPlayerWOL>* m_pSkills[SKILL::KEY_END];
	CState<CPlayerWOL>* m_pUsingSkill = nullptr;
};

