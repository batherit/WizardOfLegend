#include "stdafx.h"
#include "CMonster_WizardBall.h"
#include "CStateMgr.h"
#include "CSpace.h"
#include "CCamera2D.h"
#include "CBitmapMgr.h"
#include "CWizardBallState_Idle.h"
#include "CUI_DamageText.h"
#include "CSpawnerGenerator.h"
#include "CWizardBallState_Damage.h"
#include "CCollider.h"
#include "CPlayerWOL.h"


CMonster_WizardBall::CMonster_WizardBall(CGameWorld & _rGameWorld, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, 0.f, 0.f, WIZARD_BALL_OUTPUT_WIDTH, WIZARD_BALL_OUTPUT_HEIGHT),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetObjType(OBJ::TYPE_MONSTER);
	SetInitInfo();
}

CMonster_WizardBall::CMonster_WizardBall(CGameWorld & _rGameWorld, float _fX, float _fY, int _iGroupID, CObj * _pTarget, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, _fX, _fY, WIZARD_BALL_OUTPUT_WIDTH, WIZARD_BALL_OUTPUT_HEIGHT),
	m_pTarget(_pTarget),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetObjType(OBJ::TYPE_MONSTER);
	m_iGroupID = _iGroupID;
	SetInitInfo();
}

CMonster_WizardBall::~CMonster_WizardBall()
{
	Release();
}

int CMonster_WizardBall::Update(float _fDeltaTime)
{
	// 유효하지 않은 상태로 컨펌되면 false를 반환한다.
	if (!m_pStateMgr->ConfirmValidState()) return 1;

	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CMonster_WizardBall::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
	m_pColliders[COLLIDER::TYPE_WALL]->LateUpdate();
}

void CMonster_WizardBall::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CMonster_WizardBall::Release(void)
{
}

void CMonster_WizardBall::Attacked(float _fDamageAmount, POINT _ptCollisionPoint)
{
	if (!IsDead()) {
		CObj::Attacked(_fDamageAmount, _ptCollisionPoint);
		TO_WOL(GetGameWorld()).GetListUIs().emplace_back(new CUI_DamageText(GetGameWorld(), GetX(), GetY(), _ptCollisionPoint, _fDamageAmount));
		if (IsDead() && m_pSpawnerGenerator) m_pSpawnerGenerator->DecreaseSpawnedMonstersNum();
		GetStateMgr()->SetNextState(new CWizardBallState_Damage(*this, _ptCollisionPoint), true);
	}
}

void CMonster_WizardBall::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint)
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

bool CMonster_WizardBall::GoToTarget (float _fDeltaTime)
{
	if (!m_pTarget) return false;

	// 지정된 타겟에게 다가간다.
	if (!DirectDirectionToTarget()) return false;
	MoveByDeltaTime(_fDeltaTime);

	return true;
}

bool CMonster_WizardBall::IsAttackable(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();

	float fLength = GetVectorLength(fToX, fToY);

	if (fLength <= WIZARD_BALL_ATTACKABLE_RANGE) {
		return true;
	}
	return false;
}

bool CMonster_WizardBall::DirectDirectionToTarget(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();
	SetToXY(fToX, fToY);
	return true;
}

int CMonster_WizardBall::GetSpriteIndex(void)
{
	float fDegree = GetPositiveDegreeByVector(m_fToX, m_fToY);

	if (348.75f <= fDegree || fDegree < 11.25f) return 1;
	for (int i = 0; i < 15; i++) {
		if (11.25f + 22.5f * i <= fDegree && fDegree < 11.25f + 22.5f * (i + 1)) {
			return 2 + i;
		}
	}
	return 0;
}

void CMonster_WizardBall::SetInitInfo(void)
{
	Release();
	SetDamage(15);
	SetDamageOffset(2);
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(GetGameWorld(), this, 0.f, 0.f, 80.f, 80.f);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
	m_pStateMgr = new CStateMgr<CMonster_WizardBall>(GetGameWorld(), *this);
	// TODO : 아쳐의 Idle 상태만들 것
	m_pStateMgr->SetNextState(new CWizardBallState_Idle(*this));
	m_fMaxHp = WIZARD_BALL_MAX_HP;
	m_fHp = m_fMaxHp;
	m_eState = WIZARD_BALL::STATE_IDLE;
	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_BALL"));
}
