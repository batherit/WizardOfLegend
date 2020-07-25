#include "stdafx.h"
#include "CMonster_Wizard.h"
#include "CStateMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CUI_DamageText.h"
#include "CSpawnerGenerator.h"
#include "CBitmapMgr.h"
#include "CWizardState_Damage.h"
#include "CWizardState_Idle.h"
#include "CCollider.h"



CMonster_Wizard::CMonster_Wizard(CGameWorld & _rGameWorld, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, 0.f, 0.f, WIZARD_OUTPUT_WIDTH, WIZARD_OUTPUT_HEIGHT),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetInitInfo();
}

CMonster_Wizard::CMonster_Wizard(CGameWorld & _rGameWorld, float _fX, float _fY, int _iGroupID, CObj * _pTarget, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, _fX, _fY, WIZARD_OUTPUT_WIDTH, WIZARD_OUTPUT_HEIGHT),
	m_pTarget(_pTarget),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	m_iGroupID = _iGroupID;
	SetInitInfo();
}

CMonster_Wizard::~CMonster_Wizard()
{
	Release();
}

int CMonster_Wizard::Update(float _fDeltaTime)
{
	// 유효하지 않은 상태로 컨펌되면 false를 반환한다.
	if (!m_pStateMgr->ConfirmValidState()) return 1;

	if (GetToX() > 0.f) m_eWizardDir = MONSTER::DIR_RIGHT;
	else m_eWizardDir = MONSTER::DIR_LEFT;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CMonster_Wizard::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
	m_pColliders[COLLIDER::TYPE_WALL]->LateUpdate();
}

void CMonster_Wizard::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		m_hDCKeyAtlas[m_eWizardDir],
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CMonster_Wizard::Release(void)
{
	DeleteSafe(m_pStateMgr);
}

void CMonster_Wizard::SetNewStateAnim(WIZARD::E_STATE _eNewState, bool _bReset)
{
	// 같은 상태가 들어왔을 때, 리셋을 원하지 않는다면 해당 함수를 종료한다.
	if (_eNewState == m_eState && !_bReset) return;

	_anim_info stAnimInfo;
	m_eState = _eNewState;
	stAnimInfo.iState = _eNewState;
	switch (_eNewState) {
	case WIZARD::STATE_IDLE:
		stAnimInfo.iCountToRepeat = 0;
		stAnimInfo.fTotalTime = 0.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 1;
		break;
	case WIZARD::STATE_RUN:
		stAnimInfo.iCountToRepeat = 0;
		stAnimInfo.fTotalTime = 1.5f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 4;
		break;
	case WIZARD::STATE_ATTACK:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 2.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 4;
		break;
	case WIZARD::STATE_DAMAGE:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.25f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 2;
		break;
	case WIZARD::STATE_DEATH:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 4;
		break;
	}
	SetNewAnimInfo(stAnimInfo);
}

void CMonster_Wizard::Attacked(float _fDamageAmount, POINT _ptCollisionPoint)
{
	if (!IsDead()) {
		CObj::Attacked(_fDamageAmount, _ptCollisionPoint);
		TO_WOL(GetGameWorld()).GetListUIs().emplace_back(new CUI_DamageText(GetGameWorld(), GetX(), GetY(), _ptCollisionPoint, _fDamageAmount));
		if (IsDead() && m_pSpawnerGenerator) m_pSpawnerGenerator->DecreaseSpawnedMonstersNum();
		GetStateMgr()->SetNextState(new CWizardState_Damage(*this, _ptCollisionPoint), true);
	}
}

bool CMonster_Wizard::GoToAttackableLocation(float _fDeltaTime)
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

bool CMonster_Wizard::IsAttackable(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();

	float fLength = GetVectorLength(fToX, fToY);

	if (WIZARD_ATTACKABLE_RANGE_IN <= fLength && fLength <= WIZARD_ATTACKABLE_RANGE_OUT) {
		return true;
	}
	return false;
}

bool CMonster_Wizard::DirectDirectionToTarget(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();
	SetToXY(fToX, fToY);
	return true;
}

void CMonster_Wizard::SetInitInfo(void)
{
	Release();
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(GetGameWorld(), this, 0.f, 67.f, 70.f, 50.f);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
	m_pStateMgr = new CStateMgr<CMonster_Wizard>(GetGameWorld(), *this);
	// TODO : 위자드의 Idle 상태만들 것
	m_pStateMgr->SetNextState(new CWizardState_Idle(*this));
	m_fMaxHp = WIZARD_MAX_HP;
	m_fHp = m_fMaxHp;
	m_eWizardDir = MONSTER::DIR_RIGHT;
	m_hDCKeyAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_LEFT"));
	m_hDCKeyAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_RIGHT"));
}
