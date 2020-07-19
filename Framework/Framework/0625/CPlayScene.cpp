#include "stdafx.h"
#include "CPlayScene.h"
#include "CMapLoader.h"
#include "CPlayerWOL.h"
#include "CMapObjsGroup.h"
#include "CSpace.h"
#include "CStateMgr.h"
#include "CPlayerState_Spawn.h"
#include "CMonster_SwordMan.h"


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
	TO_PLAYER_WOL(m_pPlayer)->Respawn(pairSpawnPoint.first, pairSpawnPoint.second);
	m_vecObjsToRender.reserve(100);
	m_vecObjsToRender.clear();
}

int CPlayScene::Update(float _fDeltaTime)
{
	if(CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_RBUTTON)) {
		CObj* pObj = new CMonster_SwordMan(m_rGameWorld, m_pPlayer);
		m_listMonsters.emplace_back(pObj);
		dynamic_cast<CMonster_SwordMan*>(pObj)->Spawn(m_pPlayer->GetX() + 150, m_pPlayer->GetY());
	}
	m_vecObjsToRender.emplace_back(m_pPlayer);
	for (auto& pObj : m_pMapLoader->GetDoors()) {
		m_vecObjsToRender.emplace_back(pObj);
	}
	for (auto& pObj : m_listMonsters) {
		pObj->Update(_fDeltaTime);
		m_vecObjsToRender.emplace_back(pObj);
	}
	m_pPlayer->Update(_fDeltaTime);
	
	return 0;
}

void CPlayScene::LateUpdate(void)
{
}

void CPlayScene::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	g_iRenderCount = 0;

	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(0)) {
		pObj->Render(_hdc, _pCamera);
	}

	// yÃà Á¤·Ä
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
