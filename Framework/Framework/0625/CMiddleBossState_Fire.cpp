#include "stdafx.h"
#include "CMiddleBossState_Fire.h"
#include "CBoss_MiddleBoss.h"
#include "CStateMgr.h"
#include "CMiddleBossState_Idle.h"
#include "CScene.h"
#include "CStateMgr.h"
#include "CSceneMgr.h"
#include "CMonsterSpawner.h"
#include "CWizardFire.h"


CMiddleBossState_Fire::CMiddleBossState_Fire(CBoss_MiddleBoss& _rOwner)
	:
	CState(_rOwner)
{
	m_iCount = 3;
	m_fPeriod = 1.5f;
	m_fElapsedTime = 0.f;
}


CMiddleBossState_Fire::~CMiddleBossState_Fire()
{
}

void CMiddleBossState_Fire::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	_anim_info stAnimInfo;
	stAnimInfo.iState = 2;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 2.f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 4;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.DirectDirectionToTarget();
	
}

int CMiddleBossState_Fire::Update(float _fDeltaTime)
{
	m_rOwner.DirectDirectionToTarget();


	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		if (m_iCount > 0) {
			if ((m_fElapsedTime += _fDeltaTime) > m_fPeriod) {
				float fIntervalDegree = 360.f / 8.f;
				CObj* pPlayer = TO_WOL(m_rOwner.GetGameWorld()).GetPlayer();
				for (int i = 0; i < 8; i++) {
					TO_WOL(m_rOwner.GetGameWorld()).GetListUsedMonsterSkills().emplace_back(
						new CWizardFire(
							m_rOwner.GetGameWorld(),
							pPlayer->GetX() + cosf(TO_RADIAN(fIntervalDegree * i)) * 450.f, pPlayer->GetY() + sinf(TO_RADIAN(fIntervalDegree* i))* 450.f,
							-cosf(TO_RADIAN(fIntervalDegree* i)), -sinf(TO_RADIAN(fIntervalDegree* i))));
				}
				m_fElapsedTime = 0.f;
				m_iCount--;
			}
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CMiddleBossState_Idle(m_rOwner));
		}
		return 1;
	}
	return 0;
}

void CMiddleBossState_Fire::LateUpdate(void)
{
}

void CMiddleBossState_Fire::OnExited(void)
{
}
