#include "stdafx.h"
#include "CArcherState_Attack.h"
#include "CMonster_Archer.h"
#include "CStateMgr.h"
#include "CArcherState_Idle.h"
#include "CArcherState_Run.h"
#include "CArcherArrow.h"



CArcherState_Attack::CArcherState_Attack(CMonster_Archer & _rOwner)
	:
	CState(_rOwner)
{
}

CArcherState_Attack::~CArcherState_Attack()
{
}

void CArcherState_Attack::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	m_rOwner.SetNewStateAnim(ARCHER::STATE_ATTACK, true);
	m_rOwner.DirectDirectionToTarget();
	CSoundMgr::Get_Instance()->PlaySound(TEXT("ARCHER_AIM.mp3"), CSoundMgr::MONSTER);
}

int CArcherState_Attack::Update(float _fDeltaTime)
{
	m_rOwner.DirectDirectionToTarget();
	if (!bAttackOk && m_rOwner.GetAnimProgress() >= 0.75f) {
		m_rOwner.GetGameWorld().GetListObjs().emplace_back(
			new CArcherArrow(
				m_rOwner.GetGameWorld(),
				m_rOwner.GetX(), m_rOwner.GetY(),
				m_rOwner.GetToX(), m_rOwner.GetToY(),
				m_rOwner.GetArcherDir()));
		bAttackOk = true;
		CSoundMgr::Get_Instance()->PlaySound(TEXT("ARCHER_SHOOT.mp3"), CSoundMgr::SKILL);
	}
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		

		if (m_rOwner.GetTarget()) {
			if (m_rOwner.IsAttackable()) {
				m_rOwner.GetStateMgr()->SetNextState(new CArcherState_Attack(m_rOwner));
			}
			else {
				m_rOwner.GetStateMgr()->SetNextState(new CArcherState_Run(m_rOwner));
			}
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CArcherState_Idle(m_rOwner));
		}
	}

	return 0;
}

void CArcherState_Attack::LateUpdate(void)
{
}

void CArcherState_Attack::OnExited(void)
{
}
