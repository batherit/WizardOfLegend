#include "stdafx.h"
#include "CItem_Gaia.h"
#include "CGaiaArmorSkillState.h"
#include "CPlayerWOL.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"



CItem_Gaia::CItem_Gaia(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, SHOP_GAIA_WIDTH, SHOP_GAIA_HEIGHT),
	m_pSkillState(new CGaiaArmorSkillState(*TO_PLAYER_WOL(TO_WOL(_rGameWorld).GetPlayer())))
{
	m_hDCKey = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("GAIA_ARMOR_CARD"));
}

CItem_Gaia::~CItem_Gaia()
{
}

int CItem_Gaia::Update(float _fDeltaTime)
{
	CObj* pPlayer = TO_WOL(GetGameWorld()).GetPlayer();
	float fDist = GetVectorLength(pPlayer->GetX() - GetX(), pPlayer->GetY() - GetY());

	if (fDist < 200.0f) {
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_F)) {
			if (Used()) {
				m_pSkillState = nullptr;
				CSoundMgr::Get_Instance()->PlaySound(TEXT("GET_SKILL.mp3"), CSoundMgr::UI);
				SetValid(false);
			}
		}
	}
	return 0;
}

void CItem_Gaia::Render(HDC & _hdc, CCamera2D * _pCamera)
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

void CItem_Gaia::Release(void)
{
	DeleteSafe(m_pSkillState);
}

bool CItem_Gaia::Used(void)
{
	
	DO_IF_IS_NOT_VALID_OBJ(this) return false;

	CObj* pPlayer = TO_WOL(GetGameWorld()).GetPlayer();
	if (pPlayer->GetMoney() >= 150) {
		pPlayer->SetMoney(pPlayer->GetMoney() - 150);
		if (TO_PLAYER_WOL(TO_WOL(GetGameWorld()).GetPlayer())->AquireSkillState(m_pSkillState)) {
			return true;
		}
		else {
			DeleteSafe(m_pSkillState);
			return true;
		}
	}
	return false;
}
