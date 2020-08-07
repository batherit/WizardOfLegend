#include "stdafx.h"
#include "CBossState_Idle.h"
#include "CBoss_Boss.h"
#include "CWOL_World.h"
#include "CBossState_Death.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CStateMgr.h"
#include "CBossState_BoxAttack.h"
#include "CBossState_StonePillar.h"


CBossState_Idle::CBossState_Idle(CBoss_Boss & _rOwner)
	:
	CState(_rOwner)
{
	m_pCamera = TO_WOL(m_rOwner.GetGameWorld()).GetCamera();
}

CBossState_Idle::~CBossState_Idle()
{
	m_pCamera = nullptr;
}

void CBossState_Idle::OnLoaded(void)
{
	_anim_info stAnimInfo;
	stAnimInfo.iState = 0;
	stAnimInfo.fTotalTime = 0.f;
	stAnimInfo.iFrameCount = 1;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iCountToRepeat = 0;

	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(0.f);
}

int CBossState_Idle::Update(float _fDeltaTime)
{
	m_rOwner.UpdateAnim(_fDeltaTime);

	RECT& rcDrawArea = m_rOwner.GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float>& pairLeftTop = m_pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = m_pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(m_rOwner.GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return 0;

	// TODO : 첫 공격을 하는 상태를 집어넣는다.
	m_rOwner.GetStateMgr()->SetNextState(new CBossState_StonePillar(m_rOwner));

	return 0;
}

void CBossState_Idle::LateUpdate(void)
{
}

void CBossState_Idle::OnExited(void)
{
}
