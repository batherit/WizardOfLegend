#include "stdafx.h"
#include "CMonster_SwordMan.h"
#include "CStateMgr.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CEffect_Spawn.h"
#include "CSwordManState_Spawn.h"
#include "CSwordManState_Idle.h"


CMonster_SwordMan::CMonster_SwordMan(CGameWorld & _rGameWorld)
	:
	CObj(_rGameWorld, 0.f, 0.f, SWORDMAN_OUTPUT_WIDTH, SWORDMAN_OUTPUT_HEIGHT)
{
	SetInitInfo();
}

CMonster_SwordMan::CMonster_SwordMan(CGameWorld& _rGameWorld, float _fX, float _fY, int _iGroupID, CObj* _pTarget /*= nullptr*/)
	:CObj(_rGameWorld, _fX, _fY, SWORDMAN_OUTPUT_WIDTH, SWORDMAN_OUTPUT_HEIGHT),
	m_pTarget(_pTarget)
{
	m_iGroupID = _iGroupID;
	SetInitInfo();
}


CMonster_SwordMan::~CMonster_SwordMan()
{
}

int CMonster_SwordMan::Update(float _fDeltaTime)
{
	// 유효하지 않은 상태로 컨펌되면 false를 반환한다.
	if (!m_pStateMgr->ConfirmValidState()) return 1;

	if (GetToX() > 0.f) m_eSwordManDir = SWORDMAN::DIR_RIGHT;
	else m_eSwordManDir = SWORDMAN::DIR_LEFT;
	m_pStateMgr->Update(_fDeltaTime);
	return 0;
}

void CMonster_SwordMan::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
}

void CMonster_SwordMan::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	//RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	//if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first + 1,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second + 1,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas[m_eSwordManDir],
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight + (m_eState == SWORDMAN::STATE_DEATH ? 80 : 0),
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CMonster_SwordMan::Release(void)
{
	m_pTarget = nullptr;
	DeleteSafe(m_pStateMgr);
}

void CMonster_SwordMan::SetNewStateAnim(SWORDMAN::E_STATE _eNewState, bool _bReset)
{
	// 같은 상태가 들어왔을 때, 리셋을 원하지 않는다면 해당 함수를 종료한다.
	if (_eNewState == m_eState && !_bReset) return;

	_anim_info stAnimInfo;
	m_eState = _eNewState;
	stAnimInfo.iState = _eNewState;
	switch (_eNewState) {
	case SWORDMAN::STATE_IDLE:
		stAnimInfo.iCountToRepeat = 0;
		stAnimInfo.fTotalTime = 0.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 1;
		break;
	case SWORDMAN::STATE_RUN:
		stAnimInfo.iCountToRepeat = 0;
		stAnimInfo.fTotalTime = 0.8f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 6;
		break;
	case SWORDMAN::STATE_ATTACK:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.5f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 2;
		break;
	case SWORDMAN::STATE_DAMAGE:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.25f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 2;
		break;
	case SWORDMAN::STATE_DEATH:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 6;
		break;
	}
	SetNewAnimInfo(stAnimInfo);
}

void CMonster_SwordMan::Attacked(float _fDamageAmount)
{
	if (!IsDead()) {
		CObj::Attacked(_fDamageAmount);
		//GetStateMgr()->SetNextState(new CSwordMan_Damage(*this));
	}
}

//void CMonster_SwordMan::Spawn(float _fX, float _fY)
//{
//	SetXY(_fX, _fY);
//	DeleteSafe(m_pSpawnEffect);
//	m_pSpawnEffect = new CEffect_Spawn(GetGameWorld(), this, EFFECT_SPAWN::EFFECT_SPAWN_SWORDMAN);
//	m_pStateMgr->SetNextState(new CSwordManState_Spawn(*this));
//	SetSpeed(0.f);
//	m_eDir = OBJ::DIR_DOWN;
//}

bool CMonster_SwordMan::IsAttackable(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();

	float fLength = GetVectorLength(fToX, fToY);

	if (fLength <= cfSwordManAttackableRange) {
		return true;
	}
	return false;
}

bool CMonster_SwordMan::DirectDirectionToTarget(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();
	SetToXY(fToX, fToY);
	return true;
}

bool CMonster_SwordMan::GoToTarget(float _fDeltaTime)
{
	if (!m_pTarget) return false;

	// 지정된 타겟에게 다가간다.
	if (!DirectDirectionToTarget()) return false;
	MoveByDeltaTime(_fDeltaTime);
	
	return true;
}

void CMonster_SwordMan::SetInitInfo(void)
{
	DeleteSafe(m_pStateMgr);
	m_pStateMgr = new CStateMgr<CMonster_SwordMan>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CSwordManState_Idle(*this));
	m_fMaxHp = cfSwordManMaxHp;
	m_fHp = m_fMaxHp;
	m_eSwordManDir = SWORDMAN::DIR_RIGHT;
	m_hDCKeyAtlas[SWORDMAN::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SWORDMAN_LEFT"));
	m_hDCKeyAtlas[SWORDMAN::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SWORDMAN_RIGHT"));
}
