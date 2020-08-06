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
#include "CPlayerWOL.h"



CMonster_Wizard::CMonster_Wizard(CGameWorld & _rGameWorld, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, 0.f, 0.f, WIZARD_OUTPUT_WIDTH, WIZARD_OUTPUT_HEIGHT),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetObjType(OBJ::TYPE_MONSTER);
	SetInitInfo();
}

CMonster_Wizard::CMonster_Wizard(CGameWorld & _rGameWorld, float _fX, float _fY, int _iGroupID, CObj * _pTarget, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, _fX, _fY, WIZARD_OUTPUT_WIDTH, WIZARD_OUTPUT_HEIGHT),
	m_pTarget(_pTarget),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetObjType(OBJ::TYPE_MONSTER);
	m_iGroupID = _iGroupID;
	SetInitInfo();
}

CMonster_Wizard::~CMonster_Wizard()
{
	Release();
}

int CMonster_Wizard::Update(float _fDeltaTime)
{
	// ��ȿ���� ���� ���·� ���ߵǸ� false�� ��ȯ�Ѵ�.
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
	m_listWizardBalls.remove_if([](auto& _pObj) { return !IS_VALID_OBJ(_pObj); });
}

void CMonster_Wizard::Render(HDC & _hdc, CCamera2D * _pCamera)
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
	// ���� ���°� ������ ��, ������ ������ �ʴ´ٸ� �ش� �Լ��� �����Ѵ�.
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
	SetRenderLayer(1);
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(GetGameWorld(), this, 0.f, 67.f, 70.f, 50.f);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
	m_pStateMgr = new CStateMgr<CMonster_Wizard>(GetGameWorld(), *this);
	// TODO : ���ڵ��� Idle ���¸��� ��
	m_pStateMgr->SetNextState(new CWizardState_Idle(*this));
	m_fMaxHp = WIZARD_MAX_HP;
	m_fHp = m_fMaxHp;
	m_eWizardDir = MONSTER::DIR_RIGHT;
	m_hDCKeyAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_LEFT"));
	m_hDCKeyAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_RIGHT"));
}

void CMonster_Wizard::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT& _rcCollisionRect)
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