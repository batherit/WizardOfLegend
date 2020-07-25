#include "stdafx.h"
#include "CPlayerState_Dash.h"
#include "CPlayerWOL.h"
#include "CStateMgr.h"
#include "CPlayerState_Idle.h"
#include "CPlayerState_Run.h"


CPlayerState_Dash::CPlayerState_Dash(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
}

CPlayerState_Dash::~CPlayerState_Dash()
{
}

void CPlayerState_Dash::OnLoaded(void)
{
	m_rOwner.SetNewStateAnim(PLAYER::STATE_DASH);
	m_rOwner.SetSpeed(cfPlayerDashSpeed);
	/*int iRandDashSountIndex = rand() % 4;
	if (iRandDashSountIndex == 0)			CSoundMgr::Get_Instance()->PlaySound(TEXT("DASH_1.mp3"), CSoundMgr::PLAYER);
	else if (iRandDashSountIndex == 1)		CSoundMgr::Get_Instance()->PlaySound(TEXT("DASH_2.mp3"), CSoundMgr::PLAYER);
	else if (iRandDashSountIndex == 2)		CSoundMgr::Get_Instance()->PlaySound(TEXT("DASH_3.mp3"), CSoundMgr::PLAYER);
	else if (iRandDashSountIndex == 3)	*/	CSoundMgr::Get_Instance()->PlaySound(TEXT("DASH_4.mp3"), CSoundMgr::PLAYER);
}

int CPlayerState_Dash::Update(float _fDeltaTime)
{
	if (m_rOwner.GetToX() == 0.f && m_rOwner.GetToY() == 0.f) {
		OBJ::E_DIRECTION _eDir = m_rOwner.GetDirType();
		m_rOwner.SetToXY(cfDeltaX[_eDir], cfDeltaY[_eDir]);
	}

	// ���� ����
	if (m_rOwner.GetAnimProgress() >= 0.7f) {
		float fT = (m_rOwner.GetAnimProgress() - 0.7f) / 0.3f;
		m_rOwner.SetSpeed(cfPlayerDashSpeed * (1.f - fT) + 0.f * fT);
	}
	m_rOwner.MoveByDeltaTime(_fDeltaTime);

	if (m_rOwner.UpdateAnim(_fDeltaTime) == 1) {
		// �� ������ �ִϸ��̼��� ���ٸ�.
		float fNewToX = 0.f;
		float fNewToY = 0.f;

		if (m_rOwner.IsMoveKeyPressed(fNewToX, fNewToY)) {
			m_rOwner.SetToXY(fNewToX, fNewToY);
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Run(m_rOwner));
		}
		else 
			m_rOwner.GetStateMgr()->SetNextState(new CPlayerState_Idle(m_rOwner));		
	}
	return 0;
}

void CPlayerState_Dash::LateUpdate(void)
{
}

void CPlayerState_Dash::OnExited(void)
{
}
