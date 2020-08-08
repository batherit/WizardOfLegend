#include "stdafx.h"
#include "CBoss_MiddleBoss.h"
#include "CStateMgr.h"
#include "CCamera2D.h"
#include "CUI_DamageText.h"
#include "CSpawnerGenerator.h"
#include "CCollider.h"
#include "CBitmapMgr.h"
#include "CMiddleBossState_Idle.h"
#include "CMiddleBossState_Death.h"
#include "CUI_MiddleBossBar.h"
#include "CPlayerWOL.h"
#include "CSpace.h"



CBoss_MiddleBoss::CBoss_MiddleBoss(CGameWorld & _rGameWorld, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, 0.f, 0.f, MIDDLE_BOSS_WIDTH, MIDDLE_BOSS_HEIGHT),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetObjType(OBJ::TYPE_MONSTER);
	SetInitInfo();
	m_fInitX = 0.f;
	m_fInitY = 0.f;
	m_pMiddleBossBarUI = new CUI_MiddleBossBar(_rGameWorld, this);
	TO_WOL(GetGameWorld()).GetListUIs().emplace_back(m_pMiddleBossBarUI);
}

CBoss_MiddleBoss::CBoss_MiddleBoss(CGameWorld & _rGameWorld, float _fX, float _fY, int _iGroupID, CObj * _pTarget, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, _fX, _fY, MIDDLE_BOSS_WIDTH, MIDDLE_BOSS_HEIGHT),
	m_pTarget(_pTarget),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetObjType(OBJ::TYPE_MONSTER);
	m_iGroupID = _iGroupID;
	SetInitInfo();
	m_fInitX = _fX;
	m_fInitY = _fY;
	m_pMiddleBossBarUI = new CUI_MiddleBossBar(_rGameWorld, this);
	TO_WOL(GetGameWorld()).GetListUIs().emplace_back(m_pMiddleBossBarUI);
}

CBoss_MiddleBoss::~CBoss_MiddleBoss()
{
	Release();
	m_pMiddleBossBarUI->SetValid(false);
}

int CBoss_MiddleBoss::Update(float _fDeltaTime)
{
	// 유효하지 않은 상태로 컨펌되면 false를 반환한다.
	if (!m_pStateMgr->ConfirmValidState()) return 1;

	if (GetToX() > 0.f) m_eMiddleBossDir = MONSTER::DIR_RIGHT;
	else m_eMiddleBossDir = MONSTER::DIR_LEFT;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CBoss_MiddleBoss::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
	m_pColliders[COLLIDER::TYPE_WALL]->LateUpdate();
	m_listMonsters.remove_if([](auto& _pObj) { return !IS_VALID_OBJ(_pObj); });
}

void CBoss_MiddleBoss::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		m_hDCKeyAtlas[m_eMiddleBossDir],
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CBoss_MiddleBoss::Release(void)
{
	DeleteSafe(m_pStateMgr);
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
}

void CBoss_MiddleBoss::SetNewStateAnim(WIZARD::E_STATE _eNewState, bool _bReset)
{
	// 사용하지 않을 함수
	return;
}

void CBoss_MiddleBoss::Attacked(float _fDamageAmount, POINT _ptCollisionPoint)
{
	if (!IsDead()) {
		int iRandSountIndex = rand() % 2;
		if (iRandSountIndex == 0)			CSoundMgr::Get_Instance()->PlaySound(TEXT("HIT_SOUND_NORMAL_1.mp3"), CSoundMgr::MONSTER);
		else if (iRandSountIndex == 1)		CSoundMgr::Get_Instance()->PlaySound(TEXT("HIT_SOUND_NORMAL_2.mp3"), CSoundMgr::MONSTER);
		CObj::Attacked(_fDamageAmount, _ptCollisionPoint);
		TO_WOL(GetGameWorld()).GetListUIs().emplace_back(new CUI_DamageText(GetGameWorld(), GetX(), GetY(), _ptCollisionPoint, _fDamageAmount));
		if (IsDead() && m_pSpawnerGenerator) {
			m_pMiddleBossBarUI->SetVisible(false);
			m_pSpawnerGenerator->DecreaseSpawnedMonstersNum();
			GetStateMgr()->SetNextState(new CMiddleBossState_Death(*this), true);
		}
		//GetStateMgr()->SetNextState(new CWizardState_Damage(*this, _ptCollisionPoint), true);
	}
}

void CBoss_MiddleBoss::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT& _rcCollisionRect) {
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

bool CBoss_MiddleBoss::GoToAttackableLocation(float _fDeltaTime)
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

	if (fLength > WIZARD_ATTACKABLE_RANGE_OUT) {
		// 공격 가능 위치보다 멀리 있을 경우, 플레이어에게 달려간다.
		fToX *= -1.f;
		fToY *= -1.f;
	}
	SetToXY(fToX, fToY);
	MoveByDeltaTime(_fDeltaTime);

	return false;
}

void CBoss_MiddleBoss::RunToProperLocation(float _fDeltaTime)
{
	if (!m_pTarget) return;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = GetX() - pairPlayerXY.first;	// 플레이어와 반대 방향
	float fToY = GetY() - pairPlayerXY.second;	// 플레이어와 반대 방향

	float fLength = GetVectorLength(fToX, fToY);


	if (fLength > MIDDLE_BOSS_ATTACKABLE_RANGE_OUT) {
		// 공격 가능 위치보다 멀리 있을 경우, 플레이어에게 달려간다.
		fToX *= -1.f;
		fToY *= -1.f;
		SetToXY(fToX, fToY);
	}

	MoveByDeltaTime(_fDeltaTime);

	/*fToX = m_fInitX - GetX();
	fToY = 

	if ()*/
	
}

bool CBoss_MiddleBoss::IsAttackable(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();

	float fLength = GetVectorLength(fToX, fToY);

	if (fLength <= MIDDLE_BOSS_ATTACKABLE_RANGE_OUT) {
		return true;
	}
	return false;
}

bool CBoss_MiddleBoss::DirectDirectionToTarget(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();
	SetToXY(fToX, fToY);
	return true;
}

void CBoss_MiddleBoss::SetInitInfo(void)
{
	Release();
	SetRenderLayer(1);
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(GetGameWorld(), this, 0.f, 67.f, 70.f, 50.f);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
	m_pStateMgr = new CStateMgr<CBoss_MiddleBoss>(GetGameWorld(), *this);
	// TODO : 위자드의 Idle 상태만들 것
	m_pStateMgr->SetNextState(new CMiddleBossState_Idle(*this));
	m_fMaxHp = MIDDLE_BOSS_MAX_HP;//WIZARD_MAX_HP;
	m_fHp = m_fMaxHp;
	m_eMiddleBossDir = MONSTER::DIR_RIGHT;
	m_hDCKeyAtlas[MONSTER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("MIDDLE_BOSS_LEFT"));
	m_hDCKeyAtlas[MONSTER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("MIDDLE_BOSS_RIGHT"));
}
