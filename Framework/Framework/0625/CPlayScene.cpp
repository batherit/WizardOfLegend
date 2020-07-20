#include "stdafx.h"
#include "CPlayScene.h"
#include "CMapLoader.h"
#include "CPlayerWOL.h"
#include "CMapObjsGroup.h"
#include "CSpace.h"
#include "CStateMgr.h"
#include "CPlayerState_Spawn.h"
#include "CMonster_SwordMan.h"
#include "CMonsterSpawner.h"
#include "CPlayerSpawner.h"
#include "CSpawnerGenerator.h"


CPlayScene::CPlayScene(CGameWorld& _rGameWorld, const char* _szMapDirectory)
	:
	CScene(_rGameWorld),
	m_szMapDirectory(_szMapDirectory),
	m_pPlayer(TO_WOL(_rGameWorld).GetPlayer())
{
	ResetScene();
}


CPlayScene::~CPlayScene()
{
	Release();
}

void CPlayScene::ResetScene(void)
{
	Release();
	m_pMapLoader = new CMapLoader(m_rGameWorld, m_szMapDirectory);
	const pair<float, float> pairSpawnPoint = m_pMapLoader->GetSpawnPoint()->GetXY();
	m_listSpawners.emplace_back(new CPlayerSpawner(m_rGameWorld, m_pPlayer, pairSpawnPoint.first, pairSpawnPoint.second));
	//TO_PLAYER_WOL(m_pPlayer)->Respawn(pairSpawnPoint.first, pairSpawnPoint.second);
	m_vecObjsToRender.reserve(100);
	m_vecObjsToRender.clear();
}

int CPlayScene::Update(float _fDeltaTime)
{
	if(CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_RBUTTON)) {
		CObj* pObj = new CMonsterSpawner(m_rGameWorld, m_listMonsters, m_pPlayer->GetX() + 100, m_pPlayer->GetY(), SPAWN::TYPE_SWORDMAN, -1);
		m_listSpawners.emplace_back(pObj);
	}

	for (auto& pObj : m_listSpawnerGenerators) {
		if (pObj->Update(_fDeltaTime) == 1) {
			DeleteSafe(pObj);
		}
	}

	m_vecObjsToRender.emplace_back(m_pPlayer);
	for (auto& pObj : m_pMapLoader->GetDoors()) {
		m_vecObjsToRender.emplace_back(pObj);
	}
	for (auto& pObj : m_listMonsters) {
		pObj->Update(_fDeltaTime);
		m_vecObjsToRender.emplace_back(pObj);
	}
	for (auto& pObj : m_listSpawners) {
		pObj->Update(_fDeltaTime);
	}
	m_pPlayer->Update(_fDeltaTime);
	
	return 0;
}

void CPlayScene::LateUpdate(void)
{
	for (auto& pGroup : m_pMapLoader->GetTriggersGroups()) {
		if (IsCollided(m_pPlayer->GetRect(), pGroup->GetRect())) {
			for (auto& pObj : pGroup->GetMapObjs()) {
				if (IsCollided(m_pPlayer->GetRect(), pObj->GetRect())) {
					m_listSpawnerGenerators.emplace_back(new CSpawnerGenerator(m_rGameWorld, m_listSpawners, m_listMonsters, pGroup->GetGroupID()));
				}
			}
		}
	}

	m_listSpawnerGenerators.remove_if([](auto& pObj) { return pObj == nullptr; });
	CollectGarbageObjects(m_listMonsters);
	CollectGarbageObjects(m_listSpawners);
}

void CPlayScene::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	g_iRenderCount = 0;

	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(0)) {
		pObj->Render(_hdc, _pCamera);
	}

	// y�� ����
	sort(m_vecObjsToRender.begin(), m_vecObjsToRender.end(), [](CObj* pObj1, CObj* pObj2) {
		return pObj1->GetBottom() < pObj2->GetBottom();
	});
	for (auto& pObj : m_vecObjsToRender) {
		pObj->Render(_hdc, _pCamera);
	}
	m_vecObjsToRender.clear();

	for (auto& pObj : m_listSpawners) {
		pObj->Render(_hdc, _pCamera);
	}

	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(1)) {
		pObj->Render(_hdc, _pCamera);
	}
}

void CPlayScene::Release(void)
{
	DeleteSafe(m_pMapLoader);
	DeleteListSafe(m_listMonsters);
	DeleteListSafe(m_listSpawners);
	m_vecObjsToRender.clear();
}
