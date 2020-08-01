#include "stdafx.h"
#include "CPlayerSpawner.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"


CPlayerSpawner::CPlayerSpawner(CGameWorld & _rGameWorld, CObj * _pPlayer, float _fX, float _fY)
	:
	CSpawner(_rGameWorld, _fX, _fY),
	m_pPlayer(_pPlayer)
{
	m_pPlayer->SetX(GetX());
	m_pPlayer->SetY(GetY());

	_anim_info stAnimInfo;
	stAnimInfo.iState = 0;
	m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_PLAYER"));
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 0.5f;
	stAnimInfo.iStartFrameIndex = 0;
	stAnimInfo.iFrameCount = 8;
	SetWidth(225); SetHeight(730);
	SetX(_fX); SetY(_fY - 225); // 225은 오프셋

	SetNewAnimInfo(stAnimInfo);

	CSoundMgr::Get_Instance()->PlaySound(TEXT("TELEPORT.mp3"), CSoundMgr::PLAYER);
}

CPlayerSpawner::~CPlayerSpawner()
{
}

int CPlayerSpawner::Update(float _fDeltaTime)
{
	if ((m_fElapsedTime += _fDeltaTime) >= m_fTimeToDelay) {
		if (!m_bIsAnimStarted) {
			CSoundMgr::Get_Instance()->PlaySound(TEXT("CARD_SUMMON.mp3"), CSoundMgr::MONSTER);
			m_bIsAnimStarted = true;
		}

		if (UpdateAnim(_fDeltaTime) == 1) {
			SetValid(false);
			return 1;
		}
		if (!m_bIsSpawend && GetAnimProgress() >= 0) {
			m_bIsSpawend = true;
		}
	}
	
	return 0;
}

void CPlayerSpawner::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		m_hDCSpawnSprite,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}
