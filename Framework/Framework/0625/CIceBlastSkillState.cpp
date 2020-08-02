#include "stdafx.h"
#include "CIceBlastSkillState.h"
#include "CBitmapMgr.h"
#include "CPlayerWOL.h"
#include "CIceBlast.h"
#include "CCamera2D.h"


CIceBlastSkillState::CIceBlastSkillState(CPlayerWOL & _rOwner)
	:
	CState(_rOwner)
{
	m_fEndCooltime = 5.0f;
	m_fCurCooltime = m_fEndCooltime;
	m_hDCState[STATE_HDC::STATE_HDC_SKILLBAR] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ICE_BLAST_SKILLBAR"));
	m_hDCState[STATE_HDC::STATE_HDC_ICON] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ICE_BLAST_ICON"));
	m_hDCState[STATE_HDC::STATE_HDC_DESC] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ICE_BLAST_EX"));
	m_hDCState[STATE_HDC::STATE_HDC_CARD] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ICE_BLAST_CARD"));
}

CIceBlastSkillState::~CIceBlastSkillState()
{
}

void CIceBlastSkillState::OnLoaded(void)
{
	_anim_info stAnimInfo;
	switch (m_rOwner.GetLastAttackState()) {
	case PLAYER::STATE_ATTACK1:
		stAnimInfo.iState = PLAYER::STATE_ATTACK1;
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.1f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		m_rOwner.SetLastAttackState(PLAYER::STATE_ATTACK2);
		break;
	case PLAYER::STATE_ATTACK2:
		stAnimInfo.iState = PLAYER::STATE_ATTACK2;
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.1f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		m_rOwner.SetLastAttackState(PLAYER::STATE_ATTACK1);
		break;
	}
	m_rOwner.SetNewAnimInfo(stAnimInfo);

	POINT ptCursorPoint = GetClientCursorPoint();
	pair<float, float> pairCursorPoint = TO_WOL(m_rOwner.GetGameWorld()).GetCamera()->GetWorldPoint(ptCursorPoint.x, ptCursorPoint.y);

	float fToX = pairCursorPoint.first - m_rOwner.GetX();
	float fToY = pairCursorPoint.second - m_rOwner.GetY();
	m_rOwner.SetToXY(fToX, fToY);

	float fDegree = GetPositiveDegreeByVector(m_rOwner.GetToX(), m_rOwner.GetToY());
	m_rOwner.SetDirType(GetDirByDegree(fDegree));
	// TODO : ���⿡ ���̽� ũ����Ż ��ų �������ָ� �˴ϴ��ää���..
	TO_WOL(m_rOwner.GetGameWorld()).GetListUIs().emplace_back(
		new CIceBlast(m_rOwner.GetGameWorld(), &m_rOwner, m_rOwner.GetToX(), m_rOwner.GetToY()));

	// ��Ÿ�� �ʱ�ȭ
	m_fCurCooltime = 0.f;
}

int CIceBlastSkillState::Update(float _fDeltaTime)
{
	m_rOwner.UpdateSkillKey();
	// �ִϸ��̼Ǹ� �ߵ��ϰ� ���� �ϴ� �� ����;;^��^V
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CIceBlastSkillState::LateUpdate(void)
{
}

void CIceBlastSkillState::OnExited(void)
{
}

int CIceBlastSkillState::AlwaysUpdate(float _fDeltaTime)
{
	if ((m_fCurCooltime += (_fDeltaTime)) > m_fEndCooltime) m_fCurCooltime = m_fEndCooltime;
	return 0;
}
