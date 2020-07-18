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
	MoveByDeltaTime(_fDeltaTime);
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
		0,
		0,
		m_iWidth,									// 테스트용
		m_iHeight,									// 테스트용
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CPlayerWOL::Release(void)
{
}

void CPlayerWOL::SetInitInfo(void)
{
	SetSpeed(cfPlayerRunSpeed);
	SetWidth(PLAYER_OUTPUT_WITDH);
	SetHeight(PLAYER_OUTPUT_HEIGHT);
	m_eDir = OBJ::DIR_DOWN;
	m_hDCKeyAtlas[OBJ::DIR_DOWN] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_FRONT"));
	m_hDCKeyAtlas[OBJ::DIR_UP] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_BACK"));
	m_hDCKeyAtlas[OBJ::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_RIGHT"));
	m_hDCKeyAtlas[OBJ::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_LEFT"));
}
