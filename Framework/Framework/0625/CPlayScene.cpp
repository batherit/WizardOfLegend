#include "stdafx.h"
#include "CPlayScene.h"
#include "CMapLoader.h"
#include "CPlayerWOL.h"
#include "CMapObjsGroup.h"
#include "CSpace.h"
#include "CStateMgr.h"
#include "CMonster_SwordMan.h"
#include "CMonsterSpawner.h"
#include "CPlayerSpawner.h"
#include "CSpawnerGenerator.h"
#include "CUI_PlayerBar.h"
#include "CUI_SkillBar.h"
#include "CUI_SkillKeyBinding.h"
#include "CUI_Money.h"
#include "CUI_Minimap.h"
#include "CItem_DroppedCard.h"
#include "CFireDragonSkillState.h"
#include "CIceCrystalSkillState.h"
#include "CHitEffect.h"
#include "CCamera2D.h"
#include "CWizardFire.h"
#include "CItem_Potion.h"
#include "CItem_Gaia.h"
#include "CUI_Image.h"
#include "CBitmapMgr.h"


CPlayScene::CPlayScene(CGameWorld& _rGameWorld, const char* _szMapDirectory)
	:
	CScene(_rGameWorld),
	m_szMapDirectory(_szMapDirectory),
	m_pPlayer(TO_WOL(_rGameWorld).GetPlayer())
{
	ResetScene();
	CSoundMgr::Get_Instance()->PlayBGM(TEXT("DUNGEON_BGM.mp3"));
}


CPlayScene::~CPlayScene()
{
	Release();
}

void CPlayScene::ResetScene(void)
{
	//6749.f, 2373.f
	Release();
	m_pMapLoader = new CMapLoader(m_rGameWorld, m_szMapDirectory);
	const pair<float, float> pairSpawnPoint = m_pMapLoader->GetSpawnPoint()->GetXY();
	TO_PLAYER_WOL(m_pPlayer)->SetInitInfo();
	m_pPlayerBarUI = new CUI_PlayerBar(m_rGameWorld, m_pPlayer);
	m_pSkillBarUI = new CUI_SkillBar(m_rGameWorld, m_pPlayer);
	m_pSkillKeyBindingUI = new CUI_SkillKeyBinding(m_rGameWorld, m_pPlayer);
	m_pMinimapUI = new CUI_Minimap(m_rGameWorld, m_pMapLoader, m_pPlayer);
	m_pMoneyUI = new CUI_Money(m_rGameWorld, (WINCX >> 1) - 100, WINCY - 50, *m_pPlayer);
	m_listSpawners.emplace_back(new CPlayerSpawner(m_rGameWorld, m_pPlayer, pairSpawnPoint.first, pairSpawnPoint.second));
	CUI_Image* pImage = new CUI_Image(m_rGameWorld, 6749.f, 2373.f, SHOP_NPC_WIDTH, SHOP_NPC_HEIGHT);
	pImage->SetHDC(CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ITEMSHOP_NPC")));
	pImage->SetCameraUsing(true);
	m_listItems.emplace_back(pImage);
	m_listItems.emplace_back(new CItem_DroppedCard(m_rGameWorld, 2187.f, 1087.f, new CFireDragonSkillState(*TO_PLAYER_WOL(m_pPlayer))));
	m_listItems.emplace_back(new CItem_DroppedCard(m_rGameWorld, 346.5f, 2378.f, new CIceCrystalSkillState(*TO_PLAYER_WOL(m_pPlayer))));
	m_listItems.emplace_back(new CItem_Potion(m_rGameWorld, 6547.f, 2500.f));
	m_listItems.emplace_back(new CItem_Gaia(m_rGameWorld, 6976.f, 2500.f));
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
	for (auto& pObj : m_listItems) {
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
	m_pMinimapUI->Update(_fDeltaTime);
	m_pMoneyUI->Update(_fDeltaTime);
	
	return 0;
}

void CPlayScene::LateUpdate(void)
{
	// 트리거 박스와의 충돌 체크
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


	// 1) 플레이어와 벽과의 충돌 체크
	CObj* pCollider = m_pPlayer->GetCollider(COLLIDER::TYPE_WALL);
	pCollider->LateUpdate();
	RECT rcColliderRect = pCollider->GetRect();
	for (auto* pGroup : m_pMapLoader->GetCollidersGroups()) {
		if (pGroup) {
			if (IsCollided(rcColliderRect, pGroup->GetRect())) {
				for (auto& pObj : pGroup->GetMapObjs()) {
					RECT rcCollidedPoint;
					if (IntersectRect(&rcCollidedPoint, &rcColliderRect, &pObj->GetRect())) {
						if (rcCollidedPoint.bottom - rcCollidedPoint.top > rcCollidedPoint.right - rcCollidedPoint.left) {
							if (pCollider->GetX() <= pObj->GetX()) {
								m_pPlayer->MoveTo(-(rcCollidedPoint.right - rcCollidedPoint.left), 0.f);
							}
							else if (pCollider->GetX() >= pObj->GetX()) {
								m_pPlayer->MoveTo((rcCollidedPoint.right - rcCollidedPoint.left), 0.f);
							}
						}
						else {
							if (pCollider->GetY() >= pObj->GetY()) {
								m_pPlayer->MoveTo(0.f, (rcCollidedPoint.bottom - rcCollidedPoint.top));
							}
							else if (pCollider->GetY() <= pObj->GetY()) {
								m_pPlayer->MoveTo(0.f, -(rcCollidedPoint.bottom - rcCollidedPoint.top));
							}

						}
						pCollider->LateUpdate();
						rcColliderRect = pCollider->GetRect();
					}
				}
			}
		}
	}

	// 2) 플레이어와 문과의 충돌 체크
	pCollider = m_pPlayer->GetCollider(COLLIDER::TYPE_WALL);
	pCollider->LateUpdate();
	rcColliderRect = pCollider->GetRect();
	for (auto* pObj : m_pMapLoader->GetDoors()) {
		RECT rcCollidedPoint;
		if (IntersectRect(&rcCollidedPoint, &rcColliderRect, &pObj->GetRect())) {
			if (rcCollidedPoint.bottom - rcCollidedPoint.top > rcCollidedPoint.right - rcCollidedPoint.left) {
				if (pCollider->GetX() <= pObj->GetX()) {
					m_pPlayer->MoveTo(-(rcCollidedPoint.right - rcCollidedPoint.left), 0.f);
				}
				else if (pCollider->GetX() >= pObj->GetX()) {
					m_pPlayer->MoveTo((rcCollidedPoint.right - rcCollidedPoint.left), 0.f);
				}
			}
			else {
				if (pCollider->GetY() >= pObj->GetY()) {
					m_pPlayer->MoveTo(0.f, (rcCollidedPoint.bottom - rcCollidedPoint.top));
				}
				else if (pCollider->GetY() <= pObj->GetY()) {
					m_pPlayer->MoveTo(0.f, -(rcCollidedPoint.bottom - rcCollidedPoint.top));
				}

			}
			pCollider->LateUpdate();
			rcColliderRect = pCollider->GetRect();
		}
	}


	// 3) 플레이어와 몬스터 스킬과의 충돌 체크
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


	// 1) 몬스터와 벽과의 충돌 체크
	for (auto& pMonster : m_listMonsters) {
		DO_IF_IS_VALID_OBJ(pMonster) {
			pCollider = pMonster->GetCollider(COLLIDER::TYPE_WALL);
			pCollider->LateUpdate();
			rcColliderRect = pCollider->GetRect();
			for (auto* pGroup : m_pMapLoader->GetCollidersGroups()) {
				if (pGroup) {
					if (IsCollided(rcColliderRect, pGroup->GetRect())) {
						for (auto& pObj : pGroup->GetMapObjs()) {
							RECT rcCollidedPoint;
							if (IntersectRect(&rcCollidedPoint, &rcColliderRect, &pObj->GetRect())) {
								if (rcCollidedPoint.bottom - rcCollidedPoint.top > rcCollidedPoint.right - rcCollidedPoint.left) {
									if (pCollider->GetX() <= pObj->GetX()) {
										pMonster->MoveTo(-(rcCollidedPoint.right - rcCollidedPoint.left), 0.f);
									}
									else if (pCollider->GetX() >= pObj->GetX()) {
										pMonster->MoveTo((rcCollidedPoint.right - rcCollidedPoint.left), 0.f);
									}
								}
								else {
									if (pCollider->GetY() >= pObj->GetY()) {
										pMonster->MoveTo(0.f, (rcCollidedPoint.bottom - rcCollidedPoint.top));
									}
									else if (pCollider->GetY() <= pObj->GetY()) {
										pMonster->MoveTo(0.f, -(rcCollidedPoint.bottom - rcCollidedPoint.top));
									}

								}
								pCollider->LateUpdate();
								rcColliderRect = pCollider->GetRect();
							}
						}
					}
				}
			}
		}
	}

	// 2) 몬스터와 문과의 충돌 체크
	for (auto& pMonster : m_listMonsters) {
		DO_IF_IS_VALID_OBJ(pMonster) {
			pCollider = pMonster->GetCollider(COLLIDER::TYPE_WALL);
			pCollider->LateUpdate();
			rcColliderRect = pCollider->GetRect();
			for (auto* pObj : m_pMapLoader->GetDoors()) {
				RECT rcCollidedPoint;
				if (IntersectRect(&rcCollidedPoint, &rcColliderRect, &pObj->GetRect())) {
					if (rcCollidedPoint.bottom - rcCollidedPoint.top > rcCollidedPoint.right - rcCollidedPoint.left) {
						if (pCollider->GetX() <= pObj->GetX()) {
							pMonster->MoveTo(-(rcCollidedPoint.right - rcCollidedPoint.left), 0.f);
						}
						else if (pCollider->GetX() >= pObj->GetX()) {
							pMonster->MoveTo((rcCollidedPoint.right - rcCollidedPoint.left), 0.f);
						}
					}
					else {
						if (pCollider->GetY() >= pObj->GetY()) {
							pMonster->MoveTo(0.f, (rcCollidedPoint.bottom - rcCollidedPoint.top));
						}
						else if (pCollider->GetY() <= pObj->GetY()) {
							pMonster->MoveTo(0.f, -(rcCollidedPoint.bottom - rcCollidedPoint.top));
						}

					}
					pCollider->LateUpdate();
					rcColliderRect = pCollider->GetRect();
				}
			}
		}
	}
	

	// 3) 몬스터와 플레이어 스킬과의 충돌 체크
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

						if (!TO_PLAYER_WOL(m_pPlayer)->IsSignatureMode() && !TO_PLAYER_WOL(m_pPlayer)->IsSignatureSkillUsing()) {
							m_pPlayer->IncreaseMana(pPlayerSkill->GetDamage());
							if (m_pPlayer->IsManaFulled()) {
								CSoundMgr::Get_Instance()->PlaySound(TEXT("ULT_ON.mp3"), CSoundMgr::SKILL);
								TO_PLAYER_WOL(m_pPlayer)->SetSignatureMode(true);
							}
						}
					}
				}
			}
		}
	}

	// 1) 플레이어 스킬과 벽과의 충돌 체크
	for (auto& pPlayerSkill : TO_WOL(m_rGameWorld).GetListUsedPlayerSkills()) {
		DO_IF_IS_VALID_OBJ(pPlayerSkill) {
			pPlayerSkill->LateUpdate(); // 충돌 리스트를 정리
			pCollider = pPlayerSkill->GetCollider(COLLIDER::TYPE_WALL);
			if (!pCollider) continue;

			RECT rcCollisionRect;
			for (auto& pGroup : m_pMapLoader->GetCollidersGroups()) {
				DO_IF_IS_NOT_VALID_OBJ(pPlayerSkill) break;
					if(pGroup) {
					if (IsCollided(pCollider->GetRect(), pGroup->GetRect())) {
						for (auto& pObj : pGroup->GetMapObjs()) {
							if (IntersectRect(&rcCollisionRect, &pCollider->GetRect(), &pObj->GetRect())) {
								m_listHitEffects.emplace_back(
									new CHitEffect(m_rGameWorld, 
										pPlayerSkill->GetX(),
										pPlayerSkill->GetY())
								);
								pPlayerSkill->SetValid(false);
								break;
							}
							
						}
					}
				}
			}
		}
	}

	// 2) 플레이어 스킬과 문과의 충돌 체크
	for (auto& pPlayerSkill : TO_WOL(m_rGameWorld).GetListUsedPlayerSkills()) {
		DO_IF_IS_VALID_OBJ(pPlayerSkill) {
			pPlayerSkill->LateUpdate(); // 충돌 리스트를 정리
			pCollider = pPlayerSkill->GetCollider(COLLIDER::TYPE_WALL);
			if (!pCollider) continue;

			RECT rcCollisionRect;
			for (auto& pDoor : m_pMapLoader->GetDoors()) {
				DO_IF_IS_NOT_VALID_OBJ(pPlayerSkill) break;
				if (IntersectRect(&rcCollisionRect, &pCollider->GetRect(), &pDoor->GetRect())) {
					m_listHitEffects.emplace_back(
						new CHitEffect(m_rGameWorld,
							pPlayerSkill->GetX(),
							pPlayerSkill->GetY())
					);
					pPlayerSkill->SetValid(false);
					break;
				}
			}
		}
	}

	// 1) 몬스터 스킬과 벽과의 충돌 체크
	for (auto& pMonsterSkill : TO_WOL(m_rGameWorld).GetListUsedMonsterSkills()) {
		DO_IF_IS_VALID_OBJ(pMonsterSkill) {
			pMonsterSkill->LateUpdate(); // 충돌 리스트를 정리
			pCollider = pMonsterSkill->GetCollider(COLLIDER::TYPE_WALL);
			if (!pCollider) continue;

			RECT rcCollisionRect;
			for (auto& pGroup : m_pMapLoader->GetCollidersGroups()) {
				DO_IF_IS_NOT_VALID_OBJ(pMonsterSkill) break;
				if (pGroup) {
					if (IsCollided(pCollider->GetRect(), pGroup->GetRect())) {
						for (auto& pObj : pGroup->GetMapObjs()) {
							if (IntersectRect(&rcCollisionRect, &pCollider->GetRect(), &pObj->GetRect())) {
								m_listHitEffects.emplace_back(
									new CHitEffect(m_rGameWorld,
										pMonsterSkill->GetX(),
										pMonsterSkill->GetY())
								);
								pMonsterSkill->SetValid(false);
								break;
							}

						}
					}
				}
			}
		}
	}

	// 2) 몬스터 스킬과 문과의 충돌 체크
	for (auto& pMonsterSkill : TO_WOL(m_rGameWorld).GetListUsedMonsterSkills()) {
		DO_IF_IS_VALID_OBJ(pMonsterSkill) {
			pMonsterSkill->LateUpdate(); // 충돌 리스트를 정리
			pCollider = pMonsterSkill->GetCollider(COLLIDER::TYPE_WALL);
			if (!pCollider) continue;

			RECT rcCollisionRect;
			for (auto& pDoor : m_pMapLoader->GetDoors()) {
				DO_IF_IS_NOT_VALID_OBJ(pMonsterSkill) break;
				if (IntersectRect(&rcCollisionRect, &pCollider->GetRect(), &pDoor->GetRect())) {
					m_listHitEffects.emplace_back(
						new CHitEffect(m_rGameWorld,
							pMonsterSkill->GetX(),
							pMonsterSkill->GetY())
					);
					pMonsterSkill->SetValid(false);
					break;
				}
			}
		}
	}

	//// 위자드 파이어 삭제 과정
	//CObj* pWizardFire = nullptr;
	//for (auto& pPlayerSkill : TO_WOL(m_rGameWorld).GetListUsedPlayerSkills()) {
	//	DO_IF_IS_VALID_OBJ(pPlayerSkill) {
	//		RECT rcCollisionRect;
	//		for (auto& pMonsterSkill : TO_WOL(m_rGameWorld).GetListUsedMonsterSkills()) {
	//			DO_IF_IS_VALID_OBJ(pMonsterSkill) {
	//				pWizardFire = dynamic_cast<CWizardFire*>(pMonsterSkill);
	//				if (pWizardFire) {
	//					if (IntersectRect(&rcCollisionRect, &pPlayerSkill->GetRect(), &pWizardFire->GetRect())) {
	//						m_listHitEffects.emplace_back(
	//							new CHitEffect(m_rGameWorld,
	//								pMonsterSkill->GetX(),
	//								pMonsterSkill->GetY())
	//						);
	//						pMonsterSkill->SetValid(false);
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}



	// 콜라이더 재설정과 충돌체 정리
	for (auto& pMonster : m_listMonsters) {
		DO_IF_IS_VALID_OBJ(pMonster) {
			pMonster->LateUpdate();
		}
	}

	m_pMapLoader->LateUpdate(); // 충돌한 트리거를 제거
	m_listSpawnerGenerators.remove_if([](auto& pObj) { return pObj == nullptr; });
	CollectGarbageObjects(m_listItems);
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
	for (auto& pObj : m_listItems) {
		m_vecObjsToRender.emplace_back(pObj);
	}
	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(2)) {
		m_vecObjsToRender.emplace_back(pObj);
	}

	RECT rcScreenRect;
	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(0)) {
		rcScreenRect = _pCamera->GetScreenRect(pObj->GetRect());
		if (IsCollided(m_rGameWorld.GetViewSpace()->GetRect(), rcScreenRect)) {
			pObj->Render(_hdc, _pCamera);
		}
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
	if (g_bDebugShowSpec) {
		for (auto& pObj : m_pMapLoader->GetCollidersGroups()) {
			pObj->Render(_hdc, _pCamera);
			pObj->RenderGroupArea(_hdc, _pCamera);
		}
		for (auto& pObj : m_pMapLoader->GetTriggersGroups()) {
			pObj->Render(_hdc, _pCamera);
			pObj->RenderGroupArea(_hdc, _pCamera);
		}
	}

	m_pPlayerBarUI->Render(_hdc, _pCamera);
	m_pSkillBarUI->Render(_hdc, _pCamera);
	m_pSkillKeyBindingUI->Render(_hdc, _pCamera);
	m_pMinimapUI->Render(_hdc, _pCamera);
	m_pMoneyUI->Render(_hdc, _pCamera);
}

void CPlayScene::Release(void)
{
	DeleteSafe(m_pPlayerBarUI);
	DeleteSafe(m_pSkillBarUI);
	DeleteSafe(m_pSkillKeyBindingUI);
	DeleteSafe(m_pMinimapUI);
	DeleteSafe(m_pMoneyUI);
	DeleteSafe(m_pMapLoader);
	DeleteListSafe(m_listItems);
	DeleteListSafe(m_listSpawnerGenerators);
	DeleteListSafe(m_listMonsters);
	DeleteListSafe(m_listHitEffects);
	DeleteListSafe(m_listSpawners);
	m_vecObjsToRender.clear();
}
