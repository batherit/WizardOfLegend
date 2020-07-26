#include "stdafx.h"
#include "CPlayerWOL.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CStateMgr.h"
#include "CPlayerState_Idle.h"
#include "CPlayerState_Damage.h"
#include "CUI_DamageText.h"
#include "CPlayerState_Attack.h"
#include "CPlayerNormalSkillState.h"
#include "CDashSkillState.h"
#include "CFireDragonSkillState.h"
#include "CIceCrystalSkillState.h"
#include "CGaiaArmorSkillState.h"
#include "CCollider.h"
//#include "CPlayerState_Spawn.h"
//#include "CEffect_Spawn.h"


CPlayerWOL::CPlayerWOL(CGameWorld & _rGameWorld)
	:
	CObj(_rGameWorld, 0, 0, PLAYER_OUTPUT_WITDH, PLAYER_OUTPUT_HEIGHT, 0.f, 0.f, 0.f, Rectangle)
{
	ZeroMemory(m_pSkills, sizeof(m_pSkills));
	SetInitInfo();
}

CPlayerWOL::CPlayerWOL(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, PLAYER_OUTPUT_WITDH, PLAYER_OUTPUT_HEIGHT, 0.f, 0.f, 0.f, Rectangle)
{
	ZeroMemory(m_pSkills, sizeof(m_pSkills));
	SetInitInfo();
}

CPlayerWOL::~CPlayerWOL()
{
	Release();
}

void CPlayerWOL::Ready(void)
{
}

int CPlayerWOL::Update(float _fDeltaTime)
{
	// 유효하지 않은 상태로 컨펌되면 false를 반환한다.
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	DecreaseMana(PLAYER_MANA_REDUCTION_PER_SEC * _fDeltaTime);

	for (auto& pSkillState : m_pSkills) {
		if(pSkillState) pSkillState->AlwaysUpdate(_fDeltaTime);
	}

	m_pStateMgr->Update(_fDeltaTime);

	for (auto& pCollider : m_pColliders) {
		pCollider->Update(_fDeltaTime);
	}

	return 0;
}

void CPlayerWOL::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
	if (IsSignatureMode() && IsManaEmpty()) SetSignatureMode(false);
	for (auto& pCollider : m_pColliders) {
		pCollider->LateUpdate();
	}
}

void CPlayerWOL::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		m_hDCKeyAtlas[m_eDir],
		GetAnimX(),
		GetAnimY(),
		m_iWidth,									
		m_iHeight,									
		RGB(255, 0, 255));
	g_iRenderCount++;

	//m_pColliders[1]->Render(_hdc, _pCamera);
}

void CPlayerWOL::Release(void)
{
	DeleteSafe(m_pStateMgr);
	for (int i = 0; i < SKILL::KEY_END; i++) DeleteSafe(m_pSkills[i]);
	for (int i = 0; i < COLLIDER::TYPE_END; i++) DeleteSafe(m_pColliders[i]);
}

void CPlayerWOL::SetInitInfo(void)
{
	Release();
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(GetGameWorld(), this, 0.f, 50.f, 51, 27);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = new CCollider(GetGameWorld(), this, 0.f, -13.f, 150, 150);
	m_pSkills[SKILL::KEY_LBUTTON] = new CPlayerNormalSkillState(*this);
	m_pSkills[SKILL::KEY_SPACE] = new CDashSkillState(*this);
	m_pSkills[SKILL::KEY_Q] = new CGaiaArmorSkillState(*this);
	SetMoney(0);
	//m_pSkills[SKILL::KEY_Q] = new CFireDragonSkillState(*this);
	//m_pSkills[SKILL::KEY_R] = new CIceCrystalSkillState(*this);
	m_pStateMgr = new CStateMgr<CPlayerWOL>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CPlayerState_Idle(*this));
	m_fMaxHp = PLAYER_MAX_HP;
	m_fHp = PLAYER_MAX_HP;
	m_fMaxMana = PLAYER_MAX_MANA;
	m_fMana = 0.f;
	m_eDir = OBJ::DIR_DOWN;
	m_hDCKeyAtlas[OBJ::DIR_DOWN] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("PLAYER_FRONT"));
	m_hDCKeyAtlas[OBJ::DIR_UP] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("PLAYER_BACK"));
	m_hDCKeyAtlas[OBJ::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("PLAYER_RIGHT"));
	m_hDCKeyAtlas[OBJ::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("PLAYER_LEFT"));
}

bool CPlayerWOL::ConfirmValidSkill(void)
{
	if (!m_bIsSkillConfirmed) {
		if (!m_pNextSkill) { 
			int a = 10; 
		}
		if (m_pUsingSkill && (m_pUsingSkill != m_pNextSkill)) m_pUsingSkill->OnExited();
		m_pUsingSkill = m_pNextSkill;
		if (m_pUsingSkill) m_pUsingSkill->OnLoaded();
		
		m_pNextSkill = nullptr;
		m_bIsSkillConfirmed = true;
	}
	return true;
}

void CPlayerWOL::SwapSkillKey(SKILL::E_KEY _eKey1, SKILL::E_KEY _eKey2)
{
	CState<CPlayerWOL>* pTemp = m_pSkills[_eKey1];
	m_pSkills[_eKey1] = m_pSkills[_eKey2];
	m_pSkills[_eKey2] = pTemp;
	CSoundMgr::Get_Instance()->PlaySound(TEXT("CHANGE_SKILL.mp3"), CSoundMgr::SKILL);
}

bool CPlayerWOL::AquireSkillState(CState<CPlayerWOL>* _pSkillState)
{
	for (int i = 0; i < SKILL::KEY_END; i++) {
		if (!m_pSkills[i]) {
			m_pSkills[i] = _pSkillState;
			return true;
		}
	}
	return false;
}

void CPlayerWOL::SetNewStateAnim(PLAYER::E_STATE _eNewState, bool _bReset /*= false*/)
{
	// 같은 상태가 들어왔을 때, 리셋을 원하지 않는다면 해당 함수를 종료한다.
	//if (_eNewState == m_eState && !_bReset) return;

	_anim_info stAnimInfo;
	m_eState = _eNewState;
	stAnimInfo.iState = _eNewState;
	switch (_eNewState)
	{
	case PLAYER::STATE_IDLE:
		stAnimInfo.iCountToRepeat = 0;
		stAnimInfo.fTotalTime = 0.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 1;
		break;
	case PLAYER::STATE_RUN:
		stAnimInfo.iCountToRepeat = 0;
		stAnimInfo.fTotalTime = 0.8f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 10;
		break;
	case PLAYER::STATE_DASH:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.35f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		break;
	case PLAYER::STATE_ATTACK1:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.4f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		m_eLastAttackState = PLAYER::STATE_ATTACK1;
		break;
	case PLAYER::STATE_ATTACK2:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.4f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		m_eLastAttackState = PLAYER::STATE_ATTACK2;
		break;
	case PLAYER::STATE_DAMAGE:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.25f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 2;
		break;
	case PLAYER::STATE_DEATH:
		m_eDir = OBJ::DIR_DOWN;
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 7;
		break;
	default:
		break;
	}

	SetNewAnimInfo(stAnimInfo);
}

bool CPlayerWOL::IsMoveKeyPressed(float & _fToX, float & _fToY)
{
	_fToX = 0.f;
	_fToY = 0.f;

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_W) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_W)) {
		_fToX += cfDeltaX[OBJ::DIR_UP];
		_fToY += cfDeltaY[OBJ::DIR_UP];
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_S) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_S)) {
		_fToX += cfDeltaX[OBJ::DIR_DOWN];
		_fToY += cfDeltaY[OBJ::DIR_DOWN];
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_A) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_A)) {
		_fToX += cfDeltaX[OBJ::DIR_LEFT];
		_fToY += cfDeltaY[OBJ::DIR_LEFT];
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_D) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_D)) {
		_fToX += cfDeltaX[OBJ::DIR_RIGHT];
		_fToY += cfDeltaY[OBJ::DIR_RIGHT];
	}

	if (_fToX == 0.f && _fToY == 0.f) return false;
	return true;
}

void CPlayerWOL::UpdateSkillKey(void)
{
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {

		if (visitIdle == 2) {
			int a = 10;
		}
		if (m_pSkills[SKILL::KEY_LBUTTON] && m_pSkills[SKILL::KEY_LBUTTON]->IsMutable()) {
			SetNextSkill(m_pSkills[SKILL::KEY_LBUTTON]);
			GetStateMgr()->SetNextState(new CPlayerState_Attack(*this));
		}
		return;
	}
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_RBUTTON)) {
		if (m_pSkills[SKILL::KEY_RBUTTON] && m_pSkills[SKILL::KEY_RBUTTON]->IsMutable()) {
			SetNextSkill(m_pSkills[SKILL::KEY_RBUTTON]);
			GetStateMgr()->SetNextState(new CPlayerState_Attack(*this));
		}
		return;
	}
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_Q)) {
		if (m_pSkills[SKILL::KEY_Q] && m_pSkills[SKILL::KEY_Q]->IsMutable()) {
			SetNextSkill(m_pSkills[SKILL::KEY_Q]);
			GetStateMgr()->SetNextState(new CPlayerState_Attack(*this));
		}
		return;
	}
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_E)) {
		if (m_pSkills[SKILL::KEY_E] && m_pSkills[SKILL::KEY_E]->IsMutable()) {
			SetNextSkill(m_pSkills[SKILL::KEY_E]);
			GetStateMgr()->SetNextState(new CPlayerState_Attack(*this));
		}
		return;
	}
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_R)) {
		if (m_pSkills[SKILL::KEY_R] && m_pSkills[SKILL::KEY_R]->IsMutable()) {
			SetNextSkill(m_pSkills[SKILL::KEY_R]);
			GetStateMgr()->SetNextState(new CPlayerState_Attack(*this));
		}
		return;
	}
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_SPACE)) {
		if (m_pSkills[SKILL::KEY_SPACE] && m_pSkills[SKILL::KEY_SPACE]->IsMutable()) {
			SetNextSkill(m_pSkills[SKILL::KEY_SPACE]);
			GetStateMgr()->SetNextState(new CPlayerState_Attack(*this));
		}
		return;
	}
}

void CPlayerWOL::Attacked(float _fDamageAmount, POINT _ptCollisionPoint)
{
	// TODO : 받은 데미지를 보여주는 UI를 띄우셈! 했어오 ^ㅅ^v
	if (!IsDead()) {
		CObj::Attacked(_fDamageAmount, _ptCollisionPoint);
		TO_WOL(GetGameWorld()).GetListUIs().emplace_back(new CUI_DamageText(GetGameWorld(), GetX(), GetY(), _ptCollisionPoint, _fDamageAmount));
		GetStateMgr()->SetNextState(new CPlayerState_Damage(*this, _ptCollisionPoint), true);
	}
}
