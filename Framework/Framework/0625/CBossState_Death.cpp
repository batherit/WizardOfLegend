#include "stdafx.h"
#include "CBossState_Death.h"
#include "CBoss_Boss.h"


CBossState_Death::CBossState_Death(CBoss_Boss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Death::~CBossState_Death()
{
}

void CBossState_Death::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);

	_anim_info stAnimInfo;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 1.f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 1;
	stAnimInfo.iState = 5;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetBoxAttackUsing(false);

	CSoundMgr::Get_Instance()->PlaySound(TEXT("WIN.mp3"), CSoundMgr::UI);
}

int CBossState_Death::Update(float _fDeltaTime)
{
	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		// 딱히 하는 건 없고 그냥 가만히 있음.
		return 1;
	}

	return 0;
}

void CBossState_Death::LateUpdate(void)
{
}

void CBossState_Death::OnExited(void)
{
}
