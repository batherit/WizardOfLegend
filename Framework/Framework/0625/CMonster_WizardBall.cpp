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


CMonster_WizardBall::CMonster_WizardBall(CGameWorld & _rGameWorld, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, 0.f, 0.f, WIZARD_BALL_OUTPUT_WIDTH, WIZARD_BALL_OUTPUT_HEIGHT),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
}

CMonster_WizardBall::CMonster_WizardBall(CGameWorld & _rGameWorld, float _fX, float _fY, int _iGroupID, CObj * _pTarget, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, _fX, _fY, WIZARD_BALL_OUTPUT_WIDTH, WIZARD_BALL_OUTPUT_HEIGHT),
	m_pTarget(_pTarget),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	m_iGroupID = _iGroupID;
	SetInitInfo();
}

CMonster_WizardBall::~CMonster_WizardBall()
{
	Release();
}

int CMonster_WizardBall::Update(float _fDeltaTime)
{
	// ��ȿ���� ���� ���·� ���ߵǸ� false�� ��ȯ�Ѵ�.
	if (!m_pStateMgr->ConfirmValidState()) return 1;

	m_pStateMgr->Update(_fDeltaTime);
}

void CMonster_WizardBall::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
}

void CMonster_WizardBall::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
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

bool CMonster_WizardBall::GoToTarget (float _fDeltaTime)
{
	if (!m_pTarget) return false;

	// ������ Ÿ�ٿ��� �ٰ�����.
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
	m_pStateMgr = new CStateMgr<CMonster_WizardBall>(GetGameWorld(), *this);
	// TODO : ������ Idle ���¸��� ��
	m_pStateMgr->SetNextState(new CWizardBallState_Idle(*this));
	m_fMaxHp = WIZARD_BALL_MAX_HP;
	m_fHp = m_fMaxHp;
	m_eState = WIZARD_BALL::STATE_IDLE;
	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_BALL"));
}
