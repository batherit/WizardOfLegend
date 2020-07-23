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
#include "CUI_PlayerBar.h"
#include "CUI_SkillBar.h"
#include "CUI_SkillKeyBinding.h"
#include "CUI_Money.h"
#include "CHitEffect.h"


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
	TO_PLAYER_WOL(m_pPlayer)->SetInitInfo();
	m_pPlayerBarUI = new CUI_PlayerBar(m_rGameWorld, m_pPlayer);
	m_pSkillBarUI = new CUI_SkillBar(m_rGameWorld, m_pPlayer);
	m_pSkillKeyBindingUI = new CUI_SkillKeyBinding(m_rGameWorld, m_pPlayer);
	m_pMoneyUI = new CUI_Money(m_rGameWorld, (WINCX >> 1) - 100, WINCY - 50, *m_pPlayer);
	m_listSpawners.emplace_back(new CPlayerSpawner(m_rGameWorld, m_pPlayer, pairSpawnPoint.first, pairSpawnPoint.second));
	//TO_PLAYER_WOL(m_pPlayer)->Respawn(pairSpawnPoint.first, pairSpawnPoint.second);
	m_vecObjsToRender.reserve(100);
	m_vecObjsToRender.clear();
}

int CPlayScene::Update(float _fDeltaTime)
{
	/*if(CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_RBUTTON)) {
		CObj* pObj = new CMonsterSpawner(m_rGameWorld, m_listMonsters, 0.f, m_pPlayer->GetX() + 100, m_pPlayer->GetY(), SPAWN::TYPE_WIZARDBALL, -1);
		m_listSpawners.emplace_back(pObj);
	}*/

	for (auto& pObj : m_listSpawnerGenerators) {
		if (pObj->Update(_fDeltaTime) == 1) {
			m_pMapLoader->UnactivateDoors(pObj->GetGroupID());
			DeleteSafe(pObj);
		}
	}
	for (auto& pObj : m_listMonsters) {
		pObj->Update(_fDeltaTime);
	}
	for (auto& pObj : m_listSpawners) {
		pObj->Update(_fDeltaTime);
	}
	for (auto& pObj : m_listHitEffects) {
		pObj->Update(_fDeltaTime);
	}
	m_pPlayer->Update(_fDeltaTime);
	m_pPlayerBarUI->Update(_fDeltaTime);
	m_pSkillBarUI->Update(_fDeltaTime);
	m_pSkillKeyBindingUI->Update(_fDeltaTime);
	m_pMoneyUI->Update(_fDeltaTime);
	
	return 0;
}

void CPlayScene::LateUpdate(void)
{
	for (auto& pGroup : m_pMapLoader->GetTriggersGroups()) {
		if(pGroup)
		if (IsCollided(m_pPlayer->GetRect(), pGroup->GetRect())) {
			for (auto& pObj : pGroup->GetMapObjs()) {
				if (IsCollided(m_pPlayer->GetRect(), pObj->GetRect())) {
					// TODO : 그룹 삭제에 대한 코드를 변경해야됨.
					m_listSpawnerGenerators.emplace_back(new CSpawnerGenerator(m_rGameWorld, m_listSpawners, m_listMonsters, pGroup->GetGroupID()));
					m_pMapLoader->ActivateDoors(pGroup->GetGroupID());
					pGroup->SetValid(false);
					break;
				}
			}
		}
	}

	for (auto& pPlayerSkill : TO_WOL(m_rGameWorld).GetListUsedPlayerSkills()) {
		DO_IF_IS_VALID_OBJ(pPlayerSkill) {
			pPlayerSkill->LateUpdate(); // 충돌 리스트를 정리
			POINT ptCollisionPoint;
			for (auto& pMonster : m_listMonsters) {
				DO_IF_IS_VALID_OBJ(pMonster) {
					if (pPlayerSkill->CheckCollision(pMonster, &ptCollisionPoint)) {
						m_listHitEffects.emplace_back(
							new CHitEffect(m_rGameWorld, ptCollisionPoint.x, ptCollisionPoint.y)
						);

						if (!TO_PLAYER_WOL(m_pPlayer)->IsSignatureMode()) {
							m_pPlayer->IncreaseMana(pPlayerSkill->GetDamage());
							if (m_pPlayer->IsManaFulled()) TO_PLAYER_WOL(m_pPlayer)->SetSignatureMode(true);
						}
					}
				}
			}
		}
	}

	for (auto& pMonsterSkill : TO_WOL(m_rGameWorld).GetListUsedMonsterSkills()) {
		DO_IF_IS_VALID_OBJ(pMonsterSkill) {
			pMonsterSkill->LateUpdate(); // 충돌 리스트를 정리
			DO_IF_IS_VALID_OBJ(m_pPlayer) {
				POINT ptCollisionPoint;
				if (pMonsterSkill->CheckCollision(m_pPlayer, &ptCollisionPoint)) {
					m_listHitEffects.emplace_back(
						new CHitEffect(m_rGameWorld, ptCollisionPoint.x, ptCollisionPoint.y)
					);
				}
			}
		}
	}

	m_pMapLoader->LateUpdate(); // 충돌한 트리거를 제거
	m_listSpawnerGenerators.remove_if([](auto& pObj) { return pObj == nullptr; });
	CollectGarbageObjects(m_listMonsters);
	CollectGarbageObjects(m_listSpawners);
	CollectGarbageObjects(m_listHitEffects);
}

void CPlayScene::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	g_iRenderCount = 0;

	m_vecObjsToRender.emplace_back(m_pPlayer);
	for (auto& pObj : m_pMapLoader->GetDoors()) {
		m_vecObjsToRender.emplace_back(pObj);
	}
	for (auto& pObj : m_listMonsters) {
		m_vecObjsToRender.emplace_back(pObj);
	}

	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(0)) {
		pObj->Render(_hdc, _pCamera);
	}

	// y축 정렬
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
	for (auto&pObj : m_listHitEffects) {
		pObj->Render(_hdc, _pCamera);
	}

	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(1)) {
		pObj->Render(_hdc, _pCamera);
	}

	m_pPlayerBarUI->Render(_hdc, _pCamera);
	m_pSkillBarUI->Render(_hdc, _pCamera);
	m_pSkillKeyBindingUI->Render(_hdc, _pCamera);
	m_pMoneyUI->Render(_hdc, _pCamera);
}

void CPlayScene::Release(void)
{
	DeleteSafe(m_pPlayerBarUI);
	DeleteSafe(m_pSkillBarUI);
	DeleteSafe(m_pSkillKeyBindingUI);
	DeleteSafe(m_pMoneyUI);
	DeleteSafe(m_pMapLoader);
	DeleteListSafe(m_listSpawnerGenerators);
	DeleteListSafe(m_listMonsters);
	DeleteListSafe(m_listHitEffects);
	DeleteListSafe(m_listSpawners);
	m_vecObjsToRender.clear();
}
