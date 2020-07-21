#include "stdafx.h"
#include "CMonster_Archer.h"
#include "CStateMgr.h"
#include "CCamera2D.h"
#include "CSpawnerGenerator.h"
#include "CBitmapMgr.h"
#include "CArcherState_Idle.h"
#include "CArcherState_Damage.h"



CMonster_Archer::CMonster_Archer(CGameWorld & _rGameWorld, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, 0.f, 0.f, ARCHER_OUTPUT_WIDTH, ARCHER_OUTPUT_HEIGHT),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetInitInfo();
}

CMonster_Archer::CMonster_Archer(CGameWorld & _rGameWorld, float _fX, float _fY, int _iGroupID, CObj * _pTarget, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, _fX, _fY, ARCHER_OUTPUT_WIDTH, ARCHER_OUTPUT_HEIGHT),
	m_pTarget(_pTarget),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	m_iGroupID = _iGroupID;
	SetInitInfo();
}

CMonster_Archer::~CMonster_Archer()
{
}

int CMonster_Archer::Update(float _fDeltaTime)
{
	// 유효하지 않은 상태로 컨펌되면 false를 반환한다.
	if (!m_pStateMgr->ConfirmValidState()) return 1;

	if (GetToX() > 0.f) m_eArcherDir = ARCHER::DIR_RIGHT;
	else m_eArcherDir = ARCHER::DIR_LEFT;
	m_pStateMgr->Update(_fDeltaTime);
	return 0;
}

void CMonster_Archer::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
}

void CMonster_Archer::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas[m_eArcherDir],
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CMonster_Archer::Release(void)
{
	m_pTarget = nullptr;
	DeleteSafe(m_pStateMgr);
}

void CMonster_Archer::SetNewStateAnim(ARCHER::E_STATE _eNewState, bool _bReset)
{
	// 같은 상태가 들어왔을 때, 리셋을 원하지 않는다면 해당 함수를 종료한다.
	if (_eNewState == m_eState && !_bReset) return;

	_anim_info stAnimInfo;
	m_eState = _eNewState;
	stAnimInfo.iState = _eNewState;
	switch (_eNewState) {
	case ARCHER::STATE_IDLE:
		stAnimInfo.iCountToRepeat = 0;
		stAnimInfo.fTotalTime = 0.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 1;
		break;
	case ARCHER::STATE_RUN:
		stAnimInfo.iCountToRepeat = 0;
		stAnimInfo.fTotalTime = 0.8f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 5;
		break;
	case ARCHER::STATE_ATTACK:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.5f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 4;
		break;
	case ARCHER::STATE_DAMAGE:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.25f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 2;
		break;
	case ARCHER::STATE_DEATH:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 6;
		break;
	}
	SetNewAnimInfo(stAnimInfo);
}

void CMonster_Archer::Attacked(float _fDamageAmount, POINT _ptCollisionPoint)
{
	if (!IsDead()) {
		CObj::Attacked(_fDamageAmount, _ptCollisionPoint);
		if (IsDead() && m_pSpawnerGenerator) m_pSpawnerGenerator->DecreaseSpawnedMonstersNum();
		// TODO : 아쳐 데미지 상태 만들 것.
		GetStateMgr()->SetNextState(new CArcherState_Damage(*this, _ptCollisionPoint), true);
	}
}

bool CMonster_Archer::GoToTarget(float _fDeltaTime)
{
	if (!m_pTarget) return false;

	// 지정된 타겟에게 다가간다.
	if (!DirectDirectionToTarget()) return false;
	MoveByDeltaTime(_fDeltaTime);

	return true;
}

bool CMonster_Archer::IsAttackable(void)
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

bool CMonster_Archer::DirectDirectionToTarget(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();
	SetToXY(fToX, fToY);
	return true;
}

void CMonster_Archer::SetInitInfo(void)
{
	DeleteSafe(m_pStateMgr);
	m_pStateMgr = new CStateMgr<CMonster_Archer>(GetGameWorld(), *this);
	// TODO : 아쳐의 Idle 상태만들 것
	m_pStateMgr->SetNextState(new CArcherState_Idle(*this));
	m_fMaxHp = ARCHER_MAX_HP;
	m_fHp = m_fMaxHp;
	m_eArcherDir = ARCHER::DIR_RIGHT;
	m_hDCKeyAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_LEFT"));
	m_hDCKeyAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_RIGHT"));
	m_hDCBowAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_LEFT"));
	m_hDCBowAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_RIGHT"));
}
