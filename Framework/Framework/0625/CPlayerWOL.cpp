#include "stdafx.h"
#include "CPlayerWOL.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CStateMgr.h"
#include "CPlayerState_Idle.h"
#include "CPlayerState_Damage.h"
#include "CPlayerState_Spawn.h"
#include "CEffect_Spawn.h"


CPlayerWOL::CPlayerWOL(CGameWorld & _rGameWorld)
	:
	CObj(_rGameWorld, 0, 0, ciPlayerSize, ciPlayerSize, 0.f, 0.f, cfPlayerRunSpeed, Rectangle)
{
	SetInitInfo();
}

CPlayerWOL::CPlayerWOL(CGameWorld & _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight, float _fSpeed)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, 0.f, 0.f, _fSpeed, Rectangle)
{
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
	// ��ȿ���� ���� ���·� ���ߵǸ� false�� ��ȯ�Ѵ�.
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	if (m_pSpawnEffect) {
		if (1 == m_pSpawnEffect->Update(_fDeltaTime))
			DeleteSafe(m_pSpawnEffect);
	}
	m_pStateMgr->Update(_fDeltaTime);
	return 0;
}

void CPlayerWOL::LateUpdate(void)
{
	m_pStateMgr->LateUpdate();
}

void CPlayerWOL::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		pairRightBottom.first - pairLeftTop.first + 1,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second + 1,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		m_hDCKeyAtlas[m_eDir],
		GetAnimX(),
		GetAnimY(),
		m_iWidth,									
		m_iHeight,									
		RGB(255, 0, 255));
	g_iRenderCount++;

	if (m_pSpawnEffect) m_pSpawnEffect->Render(_hdc, _pCamera);
}

void CPlayerWOL::Release(void)
{
	DeleteSafe(m_pStateMgr);
}

void CPlayerWOL::SetInitInfo(void)
{
	DeleteSafe(m_pStateMgr);
	m_pStateMgr = new CStateMgr<CPlayerWOL>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CPlayerState_Spawn(*this));
	m_fMaxHp = cfPlayerMaxHp;
	m_fHp = m_fMaxHp;
	SetSpeed(cfPlayerRunSpeed);
	SetWidth(PLAYER_OUTPUT_WITDH);
	SetHeight(PLAYER_OUTPUT_HEIGHT);
	m_eDir = OBJ::DIR_DOWN;
	m_hDCKeyAtlas[OBJ::DIR_DOWN] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("PLAYER_FRONT"));
	m_hDCKeyAtlas[OBJ::DIR_UP] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("PLAYER_BACK"));
	m_hDCKeyAtlas[OBJ::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("PLAYER_RIGHT"));
	m_hDCKeyAtlas[OBJ::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("PLAYER_LEFT"));
}

void CPlayerWOL::Respawn(void)
{
	DeleteSafe(m_pSpawnEffect);
	m_pSpawnEffect = new CEffect_Spawn(GetGameWorld(), this, EFFECT_SPAWN::EFFECT_SPAWN_PLAYER);
	m_pStateMgr->SetNextState(new CPlayerState_Spawn(*this));
	m_fMaxHp = cfPlayerMaxHp;
	m_fHp = m_fMaxHp;
	SetSpeed(cfPlayerRunSpeed);
	m_eDir = OBJ::DIR_DOWN;
}

void CPlayerWOL::SetNewStateAnim(PLAYER::E_STATE _eNewState, bool _bReset /*= false*/)
{
	// ���� ���°� ������ ��, ������ ������ �ʴ´ٸ� �ش� �Լ��� �����Ѵ�.
	if (_eNewState == m_eState && !_bReset) return;

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
		stAnimInfo.fTotalTime = 0.25f;
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

void CPlayerWOL::Attacked(float _fDamageAmount)
{
	// TODO : ���� �������� �����ִ� UI�� ����!
	if (!IsDead()) {
		CObj::Attacked(_fDamageAmount);
		GetStateMgr()->SetNextState(new CPlayerState_Damage(*this));
	}
}

void CPlayerWOL::Spawn(void)
{
	// TODO : ���� �������� �Ѿ�� �ʿ�(0719 ����!)
}