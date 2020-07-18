#include "stdafx.h"
#include "CPlayScene.h"
#include "CMapLoader.h"
#include "CPlayerWOL.h"
#include "CMapObjsGroup.h"
#include "CSpace.h"


CPlayScene::CPlayScene(CGameWorld& _rGameWorld, const char* _szMapDirectory)
	:
	CScene(_rGameWorld)
{
	m_pMapLoader = new CMapLoader(_rGameWorld, _szMapDirectory);
	m_pPlayer = TO_WOL(_rGameWorld).GetPlayer();
	const pair<float, float> pairSpawnPoint = m_pMapLoader->GetSpawnPoint()->GetXY();
	m_pPlayer->SetXY(pairSpawnPoint.first, pairSpawnPoint.second);
	m_vecObjsToRender.reserve(100);
	m_vecObjsToRender.clear();
}


CPlayScene::~CPlayScene()
{
	Release();
}

int CPlayScene::Update(float _fDeltaTime)
{
	switch (m_eState) {
	case PLAY_SCENE::STATE_PLAYER_SPAWN:
	{
		// TODO : 스폰 애니메이션 진행. => 애니메이션 완료 후 게임 플레이 상태로 전환
		m_eState = PLAY_SCENE::STATE_PLAY;
	}
		break;
	case PLAY_SCENE::STATE_PLAY:
		m_vecObjsToRender.emplace_back(m_pPlayer);
		for (auto& pObj : m_pMapLoader->GetDoors()) {
			m_vecObjsToRender.emplace_back(pObj);
		}
		m_pPlayer->Update(_fDeltaTime);
		break;
	}

	return 0;
}

void CPlayScene::LateUpdate(void)
{
}

void CPlayScene::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	g_iRenderCount = 0;
	//rcDrawArea;
	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(0)) {
		//// 그릴 영역을 가져온다.
		//RECT rcDrawArea = GetRect();

		//// 그릴 영역을 스크린 좌표로 변환한다.
		//pair<float, float> pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
		//pair<float, float> pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);
		//RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
		//if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;
		pObj->Render(_hdc, _pCamera);
		break;
	}

	// y축 정렬
	sort(m_vecObjsToRender.begin(), m_vecObjsToRender.end(), [](CObj* pObj1, CObj* pObj2) {
		return pObj1->GetBottom() < pObj2->GetBottom();
	});
	for (auto& pObj : m_vecObjsToRender) {
		pObj->Render(_hdc, _pCamera);
	}
	m_vecObjsToRender.clear();

	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(1)) {
		pObj->Render(_hdc, _pCamera);
	}
}

void CPlayScene::Release(void)
{
	DeleteSafe(m_pMapLoader);
}
