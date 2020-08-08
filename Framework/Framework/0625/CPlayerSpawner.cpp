#include "stdafx.h"
#include "CPlayerSpawner.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"


CPlayerSpawner::CPlayerSpawner(CGameWorld & _rGameWorld, CObj * _pPlayer, float _fX, float _fY, float _fTimeToDelay)
	:
	CSpawner(_rGameWorld, _fX, _fY),
	m_pPlayer(_pPlayer),
	m_fTimeToDelay(_fTimeToDelay)
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
	SetX(_fX); SetY(_fY - 225); // 225�� ������

	SetNewAnimInfo(stAnimInfo);

	CSoundMgr::Get_Instance()->PlaySound(TEXT("TELEPORT.mp3"), CSoundMgr::PLAYER);
}

CPlayerSpawner::~CPlayerSpawner()
{
}

int CPlayerSpawner::Update(float _fDeltaTime)
{
	if ((m_fElapsedTime += _fDeltaTime) >= m_fTimeToDelay) {
		if (UpdateAnim(_fDeltaTime) == 1) {
			SetValid(false);
			return 1;
		}
	}
	
	return 0;
}

void CPlayerSpawner::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	//RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	//if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first + 1,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second + 1,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		m_hDCSpawnSprite,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}
