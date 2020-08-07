#include "stdafx.h"
#include "CBossState_BoxAttack.h"
#include "CBoss_Boss.h"
#include "CBossState_Idle.h"
#include "CStateMgr.h"



CBossState_BoxAttack::CBossState_BoxAttack(CBoss_Boss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_BoxAttack::~CBossState_BoxAttack()
{
}

void CBossState_BoxAttack::OnLoaded(void)
{
	_anim_info stAnimInfo;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 1.f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 2;
	stAnimInfo.iState = 2;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetBoxAttackUsing(false);


}

int CBossState_BoxAttack::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		if (!m_bIsBoxGenerated) {
			m_rOwner.GenerateBox(0.f, 180.f, 3);
			m_bIsBoxGenerated = true;
			return 0;
		}
		if ((m_fElapsedTime += _fDeltaTime) >= 1.3f) {
			if (!m_rOwner.ThrowBox()) {
				m_rOwner.SetBoxAttackUsing(false);
				m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
				return 0;
			}
			return 1;
		}
	}
	return 0;
}

void CBossState_BoxAttack::LateUpdate(void)
{
}

void CBossState_BoxAttack::OnExited(void)
{
	m_rOwner.ClearBox();
}
