#include "stdafx.h"
#include "CMonster_Archer.h"
#include "CStateMgr.h"
#include "CCamera2D.h"
#include "CSpawnerGenerator.h"
#include "CBitmapMgr.h"
#include "CArcherState_Idle.h"
#include "CArcherState_Damage.h"
#include "CArcherBow.h"
#include "CUI_DamageText.h"
#include "CSpace.h"
#include "CCollider.h"
#include "CHitEffect.h"
#include "CPlayerWOL.h"



CMonster_Archer::CMonster_Archer(CGameWorld & _rGameWorld, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, 0.f, 0.f, ARCHER_OUTPUT_WIDTH, ARCHER_OUTPUT_HEIGHT),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetObjType(OBJ::TYPE_MONSTER);
	SetInitInfo();
}

CMonster_Archer::CMonster_Archer(CGameWorld & _rGameWorld, float _fX, float _fY, int _iGroupID, CObj * _pTarget, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, _fX, _fY, ARCHER_OUTPUT_WIDTH, ARCHER_OUTPUT_HEIGHT),
	m_pTarget(_pTarget),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetObjType(OBJ::TYPE_MONSTER);
	m_iGroupID = _iGroupID;
	SetInitInfo();
}

CMonster_Archer::~CMonster_Archer()
{
	Release();
}

int CMonster_Archer::Update(float _fDeltaTime)
{
	// 유효하지 않은 상태로 컨펌되면 false를 반환한다.
	if (!m_pStateMgr->ConfirmValidState()) return 1;

	if (GetToX() > 0.f) m_eArcherDir = ARCHER::DIR_RIGHT;
	else m_eArcherDir = ARCHER::DIR_LEFT;
	m_pStateMgr->Update(_fDeltaTime);

	if (m_pBowObj && m_stAnimInfo.iState == ARCHER::STATE_ATTACK) m_pBowObj->Update(_fDeltaTime);

	return 0;
}

void CMonster_Archer::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
	m_pColliders[COLLIDER::TYPE_WALL]->LateUpdate();
}

void CMonster_Archer::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

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

	if (m_pBowObj && m_stAnimInfo.iState == ARCHER::STATE_ATTACK) m_pBowObj->Render(_hdc, _pCamera);
}

void CMonster_Archer::Release(void)
{
	DeleteSafe(m_pStateMgr);
	DeleteSafe(m_pBowObj);
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
		stAnimInfo.fTotalTime = 2.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 4;
		m_pBowObj->Ready();
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
		TO_WOL(GetGameWorld()).GetListUIs().emplace_back(new CUI_DamageText(GetGameWorld(), GetX(), GetY(), _ptCollisionPoint, _fDamageAmount));
		if (IsDead() && m_pSpawnerGenerator) m_pSpawnerGenerator->DecreaseSpawnedMonstersNum();
		GetStateMgr()->SetNextState(new CArcherState_Damage(*this, _ptCollisionPoint), true);
	}
}

bool CMonster_Archer::GoToAttackableLocation(float _fDeltaTime)
{
	// 공격 가능한 범위에 있다면 true 아니면 false를 반환
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = GetX() - pairPlayerXY.first;	// 플레이어와 반대 방향
	float fToY = GetY() - pairPlayerXY.second;	// 플레이어와 반대 방향

	float fLength = GetVectorLength(fToX, fToY);

	if (IsAttackable()) {
		return true;
	}

	if (fLength > ARCHER_ATTACKABLE_RANGE_OUT) {
		// 공격 가능 위치보다 멀리 있을 경우, 플레이어에게 달려간다.
		fToX *= -1.f;
		fToY *= -1.f;
	}
	SetToXY(fToX, fToY);
	MoveByDeltaTime(_fDeltaTime);

	return false;
}

bool CMonster_Archer::IsAttackable(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();

	float fLength = GetVectorLength(fToX, fToY);

	if (ARCHER_ATTACKABLE_RANGE_IN <= fLength && fLength <= ARCHER_ATTACKABLE_RANGE_OUT) {
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

void CMonster_Archer::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT& _rcCollisionRect)
{
	switch (_pCollider->GetObjType())
	{
	case OBJ::TYPE_PLAYER_SKILL:
		if (!_pCollider->IsRegisteredCollidedObj(this)) {
			int iDamage = _pCollider->GetDamageWithOffset();
			Attacked(iDamage, _ptCollisionPoint);
			CObj* pPlayer = TO_WOL(GetGameWorld()).GetPlayer();

			if (!TO_PLAYER_WOL(pPlayer)->IsSignatureMode() && !TO_PLAYER_WOL(pPlayer)->IsSignatureSkillUsing()) {
				pPlayer->IncreaseMana(iDamage);
				if (pPlayer->IsManaFulled()) {
					CSoundMgr::Get_Instance()->PlaySound(TEXT("ULT_ON.mp3"), CSoundMgr::SKILL);
					TO_PLAYER_WOL(pPlayer)->SetSignatureMode(true);
				}
			}
			_pCollider->RegisterCollidedObj(this);
		}
		break;
	default:
		break;
	}
}

void CMonster_Archer::SetInitInfo(void)
{
	Release();
	SetRenderLayer(1);
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(GetGameWorld(), this, 0.f, 67.f, 70.f, 50.f);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
	m_pStateMgr = new CStateMgr<CMonster_Archer>(GetGameWorld(), *this);
	// TODO : 아쳐의 Idle 상태만들 것
	m_pStateMgr->SetNextState(new CArcherState_Idle(*this));
	m_fMaxHp = ARCHER_MAX_HP;
	m_fHp = m_fMaxHp;
	m_pBowObj = new CArcherBow(GetGameWorld(), *this);
	m_eState = ARCHER::STATE_IDLE;
	m_eArcherDir = ARCHER::DIR_RIGHT;
	m_hDCKeyAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_LEFT"));
	m_hDCKeyAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_RIGHT"));
	m_hDCBowAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_LEFT"));
	m_hDCBowAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_RIGHT"));
}
