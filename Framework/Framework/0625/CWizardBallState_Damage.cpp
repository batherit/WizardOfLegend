#include "stdafx.h"
#include "CWizardBallState_Damage.h"
#include "CMonster_WizardBall.h"
#include "CStateMgr.h"
#include "CWizardBallState_Idle.h"
#include "CWizardBallState_Run.h"
#include "CCoin.h"




CWizardBallState_Damage::CWizardBallState_Damage(CMonster_WizardBall& _rOwner, POINT _ptCollisionPoint)
	:
	CState(_rOwner),
	m_ptCollisionPoint(_ptCollisionPoint)
{
}

CWizardBallState_Damage::~CWizardBallState_Damage()
{
}

void CWizardBallState_Damage::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	_anim_info stAnimInfo;
	stAnimInfo.iState = 0;
	stAnimInfo.fTotalTime = 0.5f;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iFrameCount = 2;
	stAnimInfo.iStartFrameIndex = 1;
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	//m_rOwner.SetNewStateAnim(ARCHER::STATE_DAMAGE, true);
	m_rOwner.SetToXY(m_rOwner.GetX() - m_ptCollisionPoint.x, m_rOwner.GetY() - m_ptCollisionPoint.y);

	int iRandSountIndex = rand() % 3;
	if (iRandSountIndex == 0)			CSoundMgr::Get_Instance()->PlaySound(TEXT("HIT_SOUND_NORMAL_1.mp3"), CSoundMgr::MONSTER);
	else if (iRandSountIndex == 1)		CSoundMgr::Get_Instance()->PlaySound(TEXT("HIT_SOUND_NORMAL_2.mp3"), CSoundMgr::MONSTER);
}

int CWizardBallState_Damage::Update(float _fDeltaTime)
{
	if (m_rOwner.GetAnimProgress() >= 0.0f) {
		float fT = (m_rOwner.GetAnimProgress() - 0.0f) / 1.0f;
		m_rOwner.SetSpeed(WIZARD_KNOCKBACK_SPEED * (1.f - fT) + 0.f * fT);
	}
	m_rOwner.MoveByDeltaTime(_fDeltaTime);

	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		float fNewToX = 0.f;
		float fNewToY = 0.f;

		if (m_rOwner.IsDead()) {
			m_rOwner.SetValid(false);
			TO_WOL(m_rOwner.GetGameWorld()).GetListParticles().emplace_back(
				new CCoin(m_rOwner.GetGameWorld(),
					m_rOwner.GetX(), m_rOwner.GetY() + (m_rOwner.GetHeight() >> 1),
					m_rOwner.GetMoney())
			);
			return 1;
		}
		else if (m_rOwner.GetTarget()) {
			m_rOwner.GetStateMgr()->SetNextState(new CWizardBallState_Run(m_rOwner));
		}
		else
			m_rOwner.GetStateMgr()->SetNextState(new CWizardBallState_Idle(m_rOwner));
	}
	return 0;
}

void CWizardBallState_Damage::LateUpdate(void)
{
}

void CWizardBallState_Damage::OnExited(void)
{
}
