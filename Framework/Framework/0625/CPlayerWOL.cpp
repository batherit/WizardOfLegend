#include "stdafx.h"
#include "CPlayerWOL.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"


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
}

void CPlayerWOL::Ready(void)
{
}

int CPlayerWOL::Update(float _fDeltaTime)
{
	CKeyMgr::GetInstance()->Update();
	float fNewToX = 0.f;
	float fNewToY = 0.f;

	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_W)) {
		m_eDir = OBJ::DIR_UP;
		fNewToX += cfDeltaX[m_eDir];
		fNewToY += cfDeltaY[m_eDir];
	}

	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_S)) {
		m_eDir = OBJ::DIR_DOWN;
		fNewToX += cfDeltaX[m_eDir];
		fNewToY += cfDeltaY[m_eDir];
	}

	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_A)) {
		m_eDir = OBJ::DIR_LEFT;
		fNewToX += cfDeltaX[m_eDir];
		fNewToY += cfDeltaY[m_eDir];
	}

	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_D)) {
		m_eDir = OBJ::DIR_RIGHT;
		fNewToX += cfDeltaX[m_eDir];
		fNewToY += cfDeltaY[m_eDir];
	}
	SetToXY(fNewToX, fNewToY);
	if (GetToX() == 0.f && GetToY() == 0.f) SetNewState(PLAYER::STATE_IDLE);
	else SetNewState(PLAYER::STATE_RUN);
	MoveByDeltaTime(_fDeltaTime);

	UpdateAnim(_fDeltaTime);
	return 0;
}

void CPlayerWOL::LateUpdate(void)
{
}

void CPlayerWOL::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	//RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	//if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first + 1,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second + 1,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas[m_eDir],
		GetAnimX(),
		GetAnimY(),
		m_iWidth,									
		m_iHeight,									
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CPlayerWOL::Release(void)
{
}

void CPlayerWOL::SetInitInfo(void)
{
	SetNewState(PLAYER::STATE_IDLE);
	SetSpeed(cfPlayerRunSpeed);
	SetWidth(PLAYER_OUTPUT_WITDH);
	SetHeight(PLAYER_OUTPUT_HEIGHT);
	m_eDir = OBJ::DIR_DOWN;
	m_hDCKeyAtlas[OBJ::DIR_DOWN] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_FRONT"));
	m_hDCKeyAtlas[OBJ::DIR_UP] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_BACK"));
	m_hDCKeyAtlas[OBJ::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_RIGHT"));
	m_hDCKeyAtlas[OBJ::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_LEFT"));
}

void CPlayerWOL::SetNewState(PLAYER::E_STATE _eNewState)
{
	if (_eNewState == m_eState) return;

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
		stAnimInfo.fTotalTime = 0.6f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 10;
		break;
	case PLAYER::STATE_DASH:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.7;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		break;
	case PLAYER::STATE_ATTACK1:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.35;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		break;
	case PLAYER::STATE_ATTACK2:
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.35;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		break;
	case PLAYER::STATE_DAMAGE:
		stAnimInfo.iCountToRepeat = 3;
		stAnimInfo.fTotalTime = 0.35;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 2;
		break;
	case PLAYER::STATE_DEATH:
		m_eDir = OBJ::DIR_DOWN;
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.35;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 7;
		break;
	default:
		break;
	}

	SetNewAnimInfo(stAnimInfo);
}
