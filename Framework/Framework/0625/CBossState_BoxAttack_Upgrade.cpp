#include "stdafx.h"
#include "CBossState_BoxAttack_Upgrade.h"
#include "CBoss_Boss.h"
#include "CBossState_Idle.h"
#include "CStateMgr.h"
#include "CStonePillarGenerator.h"



CBossState_BoxAttack_Upgrade::CBossState_BoxAttack_Upgrade(CBoss_Boss & _rOwner)
	:
	CState(_rOwner)
{

}

CBossState_BoxAttack_Upgrade::~CBossState_BoxAttack_Upgrade()
{

}

void CBossState_BoxAttack_Upgrade::OnLoaded(void)
{
	_anim_info stAnimInfo;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 0.8f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 2;
	stAnimInfo.iState = 2;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetBoxAttackUsing(false);
}

int CBossState_BoxAttack_Upgrade::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		if (!m_bIsBoxGenerated) {
			m_rOwner.GenerateBox(0.f, 360.f, 8);
			m_bIsBoxGenerated = true;
			return 0;
		}
		if ((m_fElapsedTime += _fDeltaTime) >= 1.0f) {
			if (!m_rOwner.ThrowBox(0.5f, 2500.f, true)) {
				m_rOwner.SetBoxAttackUsing(false);
				m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
				
				return 0;
			}
			return 1;
		}
	}
	return 0;
}

void CBossState_BoxAttack_Upgrade::LateUpdate(void)
{
}

void CBossState_BoxAttack_Upgrade::OnExited(void)
{
	m_rOwner.ClearBox();
}
