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
	void SetLastAttackState(PLAYER::E_STATE _eState) { m_eLastAttackState = _eState; }
	bool IsMoveKeyPressed(float& _fToX, float& _fToY);
	void UpdateSkillKey(void);
	virtual void Attacked(float _fDamageAmoun, POINT _ptCollisionPointt);
	void SetInitInfo(void);
	CState<CPlayerWOL>* GetUsingSkill(void) { return m_pUsingSkill; }
	bool ConfirmValidSkill(void);
	void SetUsingSkill(CState<CPlayerWOL>* _pSkillState) { m_pUsingSkill = _pSkillState; }
	void SetNextSkill(CState<CPlayerWOL>* _pSkillState) { 
		if (!_pSkillState) m_pNextSkill = nullptr;
		if (!m_bIsSkillConfirmed) return;
		m_pNextSkill = _pSkillState;
		m_bIsSkillConfirmed = false;
	}
	CState<CPlayerWOL>*& GetSkill(SKILL::E_KEY _eSkillKey) { return m_pSkills[_eSkillKey]; }
	bool IsSignatureMode(void) { return m_bIsSignatureMode; }
	void SetSignatureMode(bool _bIsSignatureMode) { m_bIsSignatureMode = _bIsSignatureMode; if(!_bIsSignatureMode) SetMana(0.f); }
	void SwapSkillKey(SKILL::E_KEY _eKey1, SKILL::E_KEY _eKey2);
	void SetSkillConfirmed(bool value) { m_bIsSkillConfirmed = value; }
	bool AquireSkillState(CState<CPlayerWOL>* _pSkillState);
	void SetIsSignatureSkillUsing(bool _bValue) { m_bIsSignatureSkillUsing = _bValue; }
	bool IsSignatureSkillUsing(void) { return m_bIsSignatureSkillUsing; }
	virtual void ReactToCollider(CObj * _pCollider, POINT& _ptCollisionPoint, RECT& _rcCollisionRect) override;

private:
	CStateMgr<CPlayerWOL>* m_pStateMgr = nullptr;
	HDC m_hDCKeyAtlas[OBJ::DIR_END];
	PLAYER::E_STATE m_eState = PLAYER::STATE_END;
	PLAYER::E_STATE m_eLastAttackState = PLAYER::STATE_ATTACK1;
	CState<CPlayerWOL>* m_pSkills[SKILL::KEY_END];
	CState<CPlayerWOL>* m_pUsingSkill = nullptr;
	CState<CPlayerWOL>* m_pNextSkill = nullptr;
	bool m_bIsSkillConfirmed = true;
	bool m_bIsSignatureMode = false;
	bool m_bIsSignatureSkillUsing = false;
	// 디버그용
//public:
//	bool m_debug = false;
public:
	int visitIdle = 0;
};

