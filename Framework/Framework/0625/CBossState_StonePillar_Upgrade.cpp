#include "stdafx.h"
#include "CBossState_StonePillar_Upgrade.h"
#include "CBoss_Boss.h"
#include "CStonePillarGenerator.h"
#include "CStateMgr.h"
#include "CBossState_Idle.h"



CBossState_StonePillar_Upgrade::CBossState_StonePillar_Upgrade(CBoss_Boss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_StonePillar_Upgrade::~CBossState_StonePillar_Upgrade()
{
}

void CBossState_StonePillar_Upgrade::OnLoaded(void)
{
	m_rOwner.ClearBox();
	m_rOwner.SetBoxAttackUsing(false);

	_anim_info stAnimInfo;
	stAnimInfo.iState = 1;
	stAnimInfo.fTotalTime = 1.5f;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iFrameCount = 9;
	stAnimInfo.iStartFrameIndex = 0;
	m_rOwner.SetNewAnimInfo(stAnimInfo);
}

int CBossState_StonePillar_Upgrade::Update(float _fDeltaTime)
{
	switch (m_eState) {
	case STATE_READY1: {
		if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
			_anim_info stAnimInfo;
			stAnimInfo.iState = 2;
			stAnimInfo.fTotalTime = 0.5f;
			stAnimInfo.iCountToRepeat = 1;
			stAnimInfo.iFrameCount = 2;
			stAnimInfo.iStartFrameIndex = 0;
			m_rOwner.SetNewAnimInfo(stAnimInfo);
			m_eState = STATE_READY2;
		}
	}
					   break;
	case STATE_READY2: {
		if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
			float fToTargetDegree = GetPositiveDegreeByVector(
				m_rOwner.GetTarget()->GetX() - m_rOwner.GetX(),
				m_rOwner.GetTarget()->GetY() - m_rOwner.GetY());

			m_rOwner.GetGameWorld().GetListObjs().emplace_back(
				new CStonePillarGenerator(m_rOwner.GetGameWorld(),
					m_rOwner.GetX(), m_rOwner.GetY(),
					-1.f,0.f,
					70, 0.025f, 0.02f, 6.f)
			);
			m_rOwner.GetGameWorld().GetListObjs().emplace_back(
				new CStonePillarGenerator(m_rOwner.GetGameWorld(),
					m_rOwner.GetX(), m_rOwner.GetY(),
					0.f, -1.f,
					70, 0.02f, 0.01f, 6.f)
			);
			m_rOwner.GetGameWorld().GetListObjs().emplace_back(
				new CStonePillarGenerator(m_rOwner.GetGameWorld(),
					m_rOwner.GetX(), m_rOwner.GetY(),
					1.f, 0.f,
					70, 0.02f, 0.01f, 4.5f)
			);
			m_rOwner.GetGameWorld().GetListObjs().emplace_back(
				new CStonePillarGenerator(m_rOwner.GetGameWorld(),
					m_rOwner.GetX(), m_rOwner.GetY(),
					0.f, 1.f,
					70, 0.02f, 0.01f, 4.5f)
			);
			m_eState = STATE_ATTACK;
		}
	}
					   break;
	case STATE_ATTACK: {
		if ((m_fTickTime += _fDeltaTime) >= 0.5f) {
			float fToTargetDegree = GetPositiveDegreeByVector(
				m_rOwner.GetTarget()->GetX() - m_rOwner.GetX(),
				m_rOwner.GetTarget()->GetY() - m_rOwner.GetY());

			m_rOwner.GetGameWorld().GetListObjs().emplace_back(
				new CStonePillarGenerator(m_rOwner.GetGameWorld(),
					m_rOwner.GetX(), m_rOwner.GetY(),
					cosf(TO_RADIAN(fToTargetDegree)),
					sinf(TO_RADIAN(fToTargetDegree)),
					100, 0.05f, 0.02f, 0.6f)
			);
			m_fElapsedTime += m_fTickTime;
			m_fTickTime = 0.f;
		}

		if (m_fElapsedTime >= 4.f) {
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
			return 1;
		}
	}
					   break;
	}
	return 0;
}

void CBossState_StonePillar_Upgrade::LateUpdate(void)
{
}

void CBossState_StonePillar_Upgrade::OnExited(void)
{
	m_rOwner.SetBoxAttackUsing(false);
}
