#include "stdafx.h"
#include "CBoss_Boss.h"
#include "CStateMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CUI_DamageText.h"
#include "CSpawnerGenerator.h"
#include "CPlayerWOL.h"
#include "CCollider.h"
#include "CBitmapMgr.h"
#include "CBossState_Idle.h"
#include "CBossState_Death.h"
#include "CBoxAttack.h"
#include "CUI_BossBar.h"
#include "CStonePillar.h"
#include "CStonePillarGenerator.h"



CBoss_Boss::CBoss_Boss(CGameWorld & _rGameWorld, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, 0.f, 0.f, BOSS_WIDTH, BOSS_HEIGHT),
	m_pSpawnerGenerator(_pSpawnerGenerator)
{
	SetObjType(OBJ::TYPE_MONSTER);
	SetInitInfo();
	m_pBossBarUI = new CUI_BossBar(_rGameWorld, this);
	TO_WOL(GetGameWorld()).GetListUIs().emplace_back(m_pBossBarUI);
	CSoundMgr::Get_Instance()->PlayBGM(TEXT("BOSS_BGM.mp3"));
}

CBoss_Boss::CBoss_Boss(CGameWorld & _rGameWorld, float _fX, float _fY, int _iGroupID, CObj * _pTarget, CSpawnerGenerator * _pSpawnerGenerator)
	:
	CObj(_rGameWorld, _fX, _fY, BOSS_WIDTH, BOSS_HEIGHT),
		m_pTarget(_pTarget),
		m_pSpawnerGenerator(_pSpawnerGenerator)
	{
		SetObjType(OBJ::TYPE_MONSTER);
		m_iGroupID = _iGroupID;
		SetInitInfo();
		m_pBossBarUI = new CUI_BossBar(_rGameWorld, this);
		TO_WOL(GetGameWorld()).GetListUIs().emplace_back(m_pBossBarUI);
		CSoundMgr::Get_Instance()->PlayBGM(TEXT("BOSS_BGM.mp3"));
}

CBoss_Boss::~CBoss_Boss()
{
	Release();
	m_pBossBarUI->SetValid(false);
}

int CBoss_Boss::Update(float _fDeltaTime)
{
	// 유효하지 않은 상태로 컨펌되면 false를 반환한다.
	if (!m_pStateMgr->ConfirmValidState()) return 1;

	if (GetToX() > 0.f) m_eBossDir = MONSTER::DIR_RIGHT;
	else m_eBossDir = MONSTER::DIR_LEFT;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CBoss_Boss::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
	if(m_pColliders[COLLIDER::TYPE_WALL]) m_pColliders[COLLIDER::TYPE_WALL]->LateUpdate();
}

void CBoss_Boss::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		(m_bIsBoxAttackUsing ? m_hDCAttackKeyAtlas : m_hDCKeyAtlas[m_eBossDir]),
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CBoss_Boss::Release(void)
{
	DeleteSafe(m_pStateMgr);
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
}

void CBoss_Boss::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT& _rcCollisionRect)
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

void CBoss_Boss::Attacked(float _fDamageAmount, POINT _ptCollisionPoint)
{
	if (!IsDead()) {
		int iRandSountIndex = rand() % 2;
		if (iRandSountIndex == 0)			CSoundMgr::Get_Instance()->PlaySound(TEXT("HIT_SOUND_NORMAL_1.mp3"), CSoundMgr::MONSTER);
		else if (iRandSountIndex == 1)		CSoundMgr::Get_Instance()->PlaySound(TEXT("HIT_SOUND_NORMAL_2.mp3"), CSoundMgr::MONSTER);
		CObj::Attacked(_fDamageAmount, _ptCollisionPoint);
		TO_WOL(GetGameWorld()).GetListUIs().emplace_back(new CUI_DamageText(GetGameWorld(), GetX(), GetY(), _ptCollisionPoint, _fDamageAmount));
		if (IsDead() && m_pSpawnerGenerator) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
			m_pBossBarUI->SetVisible(false);
			m_pSpawnerGenerator->DecreaseSpawnedMonstersNum();
			GetStateMgr()->SetNextState(new CBossState_Death(*this), true);
		}
		
		//GetStateMgr()->SetNextState(new CWizardState_Damage(*this, _ptCollisionPoint), true);
	}
}

bool CBoss_Boss::DirectDirectionToTarget(void)
{
	if (!m_pTarget) return false;

	pair<float, float> pairPlayerXY = m_pTarget->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();
	SetToXY(fToX, fToY);
	return true;
}

void CBoss_Boss::GenerateBox(float _fStartDegree, float _fRangeDegree, int _iNum)
{
	ClearBox();
	// _fStartDegree에서부터 _fRangeDegree만큼 _iNum개의 박스를 생성한다.
	m_iBoxNum = _iNum;

	float _fIntervalDegree =  _fRangeDegree / _iNum ;
	
	for (int i = 0; i < _iNum; i++) {
		CObj* pSkill = new CBoxAttack(GetGameWorld(),
			GetX() + 300.f * cosf(TO_RADIAN(_fStartDegree + _fIntervalDegree * i)),
			GetY() + 300.f * sinf(TO_RADIAN(_fStartDegree + _fIntervalDegree * i))
		);
		GetGameWorld().GetListObjs().emplace_back(pSkill);
		m_pBox[i] = pSkill;
	}
}

bool CBoss_Boss::ThrowBox(float _fDelay, float _fSpeed, bool _bGenerateStonePillar)
{
	if (m_iBoxIndex < m_iBoxNum) {
		_anim_info stAnimInfo;
		stAnimInfo.fTotalTime = _fDelay;
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.iFrameCount = 3;

		switch (GetDirByDegree(GetPositiveDegreeByVector(
		m_pTarget->GetX() - GetX(), m_pTarget->GetY() - GetY()), 1.f, 1.f, 0.f)) {
		case OBJ::DIR_UP:
			stAnimInfo.iState = 1;
			break;
		case OBJ::DIR_DOWN:
			stAnimInfo.iState = 3;
			break;
		case OBJ::DIR_LEFT:
			stAnimInfo.iState = 0;
			break;
		case OBJ::DIR_RIGHT:
			stAnimInfo.iState = 2;
			break;
		}

		switch (m_eAttackState)
		{
		case BOSS::STATE_LEFT_ATTACK:
			stAnimInfo.iStartFrameIndex = 0;
			break;
		case BOSS::STATE_RIGHT_ATTACK:
			stAnimInfo.iStartFrameIndex = 3;
			break;
		}

		SetNewAnimInfo(stAnimInfo);

		dynamic_cast<CBoxAttack*>(m_pBox[m_iBoxIndex])->ThrowBoxAttack(
			m_pTarget->GetX() - m_pBox[m_iBoxIndex]->GetX(),
			m_pTarget->GetY() - m_pBox[m_iBoxIndex]->GetY(),
			_fSpeed
		);

		if (_bGenerateStonePillar) {
			GetGameWorld().GetListObjs().emplace_back(
				new CStonePillarGenerator(GetGameWorld(),
					m_pBox[m_iBoxIndex]->GetX(), m_pBox[m_iBoxIndex]->GetY(),
					m_pBox[m_iBoxIndex]->GetToX(), m_pBox[m_iBoxIndex]->GetToY(),
					100, 0.03f, 0.1f, 0.5f)
			);
		}

		m_pBox[m_iBoxIndex] = nullptr; // 던져진 박스에 대해서는 신경쓰지 않는다.
		CSoundMgr::Get_Instance()->PlaySound(TEXT("BOSS_JUMP.mp3"), CSoundMgr::SKILL);
		m_iBoxIndex++;

		SetBoxAttackUsing(true);
		SwapAttackState();

		return true;
	}
	return false;
}

void CBoss_Boss::ClearBox(void)
{
	for (int i = 0; i < m_iBoxNum; i++) {
		DO_IF_IS_VALID_OBJ(m_pBox[i]) {
			m_pBox[i]->SetValid(false);
			
		}
		m_pBox[i] = nullptr;
	}

	m_iBoxNum = 0;
	m_iBoxIndex = 0;
}

void CBoss_Boss::SwapAttackState()
{
	m_eAttackState = (m_eAttackState == BOSS::STATE_LEFT_ATTACK ?
		BOSS::STATE_RIGHT_ATTACK : BOSS::STATE_LEFT_ATTACK);
}

void CBoss_Boss::GenerateRangeStonePillar(float _fStart, float _fEnd, float _fInterval)
{
	int iNum = (_fEnd - _fStart) / _fInterval;
	int iCnt = 0;
	int iDist = 0;
	float fIntervalRadian = 0.f;
	float fRandRadian = 0.f;

	for (int i = 0; i < iNum; i++) {
		iDist = (_fStart + _fInterval * i) ;
		iCnt = iDist/ 250.f * 8;
		fIntervalRadian = TO_RADIAN(360.f / iCnt);
		fRandRadian = TO_RADIAN(rand() % 360);
		for (int j = 0; j < iCnt; j++) {
			GetGameWorld().GetListObjs().emplace_back(
				new CStonePillar(GetGameWorld(), m_fX + iDist * cosf(fIntervalRadian * j + fRandRadian), m_fY + iDist * sinf(fIntervalRadian* j + fRandRadian), 0.2 * i, 0.6f)
			);
		}
	}
}

void CBoss_Boss::SetInitInfo(void)
{
	Release();
	SetRenderLayer(1);
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(GetGameWorld(), this, 0.f, 67.f, 70.f, 50.f);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
	m_pStateMgr = new CStateMgr<CBoss_Boss>(GetGameWorld(), *this);
	// TODO : 위자드의 Idle 상태만들 것
	m_pStateMgr->SetNextState(new CBossState_Idle(*this));
	m_fMaxHp = BOSS_MAX_HP;//WIZARD_MAX_HP;
	m_fHp = m_fMaxHp;
	m_eBossDir = MONSTER::DIR_RIGHT;
	m_hDCKeyAtlas[MONSTER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BOSS_LEFT"));
	m_hDCKeyAtlas[MONSTER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BOSS_RIGHT"));
	m_hDCAttackKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BOSS_ATTACK"));
}
