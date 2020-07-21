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
	// ��ȿ���� ���� ���·� ���ߵǸ� false�� ��ȯ�Ѵ�.
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
}

void CMonster_Archer::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	//RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	//if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
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
	// ���� ���°� ������ ��, ������ ������ �ʴ´ٸ� �ش� �Լ��� �����Ѵ�.
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
	// ���� ������ ������ �ִٸ� true �ƴϸ� false�� ��ȯ
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = GetX() - pairPlayerXY.first;	// �÷��̾�� �ݴ� ����
	float fToY = GetY() - pairPlayerXY.second;	// �÷��̾�� �ݴ� ����

	float fLength = GetVectorLength(fToX, fToY);

	if (IsAttackable()) {
		return true;
	}

	if (fLength > ARCHER_ATTACKABLE_RANGE_OUT) {
		// ���� ���� ��ġ���� �ָ� ���� ���, �÷��̾�� �޷�����.
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

void CMonster_Archer::SetInitInfo(void)
{
	Release();
	m_pStateMgr = new CStateMgr<CMonster_Archer>(GetGameWorld(), *this);
	// TODO : ������ Idle ���¸��� ��
	m_pStateMgr->SetNextState(new CArcherState_Idle(*this));
	m_fMaxHp = ARCHER_MAX_HP;
	m_fHp = m_fMaxHp;
	m_pBowObj = new CArcherBow(GetGameWorld(), *this);
	m_eArcherDir = ARCHER::DIR_RIGHT;
	m_hDCKeyAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_LEFT"));
	m_hDCKeyAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_RIGHT"));
	m_hDCBowAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_LEFT"));
	m_hDCBowAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_RIGHT"));
}