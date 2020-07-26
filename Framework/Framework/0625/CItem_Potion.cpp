#include "stdafx.h"
#include "CItem_Potion.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"



CItem_Potion::CItem_Potion(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, SHOP_POTION_WIDTH, SHOP_POTION_HEIGHT)
{
	m_hDCKey = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("POTION"));
}

CItem_Potion::~CItem_Potion()
{
	Release();
}

int CItem_Potion::Update(float _fDeltaTime)
{
	CObj* pPlayer = TO_WOL(GetGameWorld()).GetPlayer();
	float fDist = GetVectorLength(pPlayer->GetX() - GetX(), pPlayer->GetY() - GetY());

	if (fDist < 200.0f) {
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_F)) {
			if (Used()) {
				CSoundMgr::Get_Instance()->PlaySound(TEXT("GET_SKILL.mp3"), CSoundMgr::UI);
				SetValid(false);
			}
		}
	}
	return 0;
}

void CItem_Potion::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKey,
		0,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CItem_Potion::Release(void)
{
}

bool CItem_Potion::Used(void)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return false;

	CObj* pPlayer = TO_WOL(GetGameWorld()).GetPlayer();
	if (pPlayer->GetMoney() >= 100) {
		pPlayer->SetMoney(pPlayer->GetMoney() - 100);
		TO_WOL(GetGameWorld()).GetPlayer()->SetHP(PLAYER_MAX_HP);
		return true;
	}
	return false;
}
