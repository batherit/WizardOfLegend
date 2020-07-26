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




CBoss_MiddleBoss::CBoss_MiddleBoss(CGameWorld & _rGameWorld, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, 0.f, 0.f, MIDDLE_BOSS_WIDTH, MIDDLE_BOSS_HEIGHT),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetInitInfo();
	m_fInitX = 0.f;
	m_fInitY = 0.f;
}

CBoss_MiddleBoss::CBoss_MiddleBoss(CGameWorld & _rGameWorld, float _fX, float _fY, int _iGroupID, CObj * _pTarget, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, _fX, _fY, MIDDLE_BOSS_WIDTH, MIDDLE_BOSS_HEIGHT),
	m_pTarget(_pTarget),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	m_iGroupID = _iGroupID;
	SetInitInfo();
	m_fInitX = _fX;
	m_fInitY = _fY;
}

CBoss_MiddleBoss::~CBoss_MiddleBoss()
{
	Release();
}

int CBoss_MiddleBoss::Update(float _fDeltaTime)
{
	// ��ȿ���� ���� ���·� ���ߵǸ� false�� ��ȯ�Ѵ�.
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
	// ������� ���� �Լ�
	return;
}

void CBoss_MiddleBoss::Attacked(float _fDamageAmount, POINT _ptCollisionPoint)
{
	if (!IsDead()) {
		CObj::Attacked(_fDamageAmount, _ptCollisionPoint);
		TO_WOL(GetGameWorld()).GetListUIs().emplace_back(new CUI_DamageText(GetGameWorld(), GetX(), GetY(), _ptCollisionPoint, _fDamageAmount));
		if (IsDead() && m_pSpawnerGenerator) {
			m_pSpawnerGenerator->DecreaseSpawnedMonstersNum();
			GetStateMgr()->SetNextState(new CMiddleBossState_Death(*this), true);
		}
		//GetStateMgr()->SetNextState(new CWizardState_Damage(*this, _ptCollisionPoint), true);
	}
}

bool CBoss_MiddleBoss::GoToAttackableLocation(float _fDeltaTime)
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

	if (fLength > WIZARD_ATTACKABLE_RANGE_OUT) {
		// ���� ���� ��ġ���� �ָ� ���� ���, �÷��̾�� �޷�����.
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
	float fToX = GetX() - pairPlayerXY.first;	// �÷��̾�� �ݴ� ����
	float fToY = GetY() - pairPlayerXY.second;	// �÷��̾�� �ݴ� ����

	float fLength = GetVectorLength(fToX, fToY);


	if (fLength > MIDDLE_BOSS_ATTACKABLE_RANGE_OUT) {
		// ���� ���� ��ġ���� �ָ� ���� ���, �÷��̾�� �޷�����.
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
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(GetGameWorld(), this, 0.f, 67.f, 70.f, 50.f);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
	m_pStateMgr = new CStateMgr<CBoss_MiddleBoss>(GetGameWorld(), *this);
	// TODO : ���ڵ��� Idle ���¸��� ��
	m_pStateMgr->SetNextState(new CMiddleBossState_Idle(*this));
	m_fMaxHp = 100.f;//WIZARD_MAX_HP;
	m_fHp = m_fMaxHp;
	m_eMiddleBossDir = MONSTER::DIR_RIGHT;
	m_hDCKeyAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("MIDDLE_BOSS_LEFT"));
	m_hDCKeyAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("MIDDLE_BOSS_RIGHT"));
}
