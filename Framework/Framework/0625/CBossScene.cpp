#include "stdafx.h"
#include "CBossScene.h"
#include "CMapLoader.h"
#include "CPlayerWOL.h"
#include "CUI_PlayerBar.h"
#include "CUI_SkillBar.h"
#include "CUI_SkillKeyBinding.h"
#include "CUI_Minimap.h"
#include "CUI_Money.h"
#include "CPlayerSpawner.h"
#include "CSpawnerGenerator.h"
#include "CMapObjsGroup.h"
#include "CCamera2D.h"
#include "CSpace.h"




CBossScene::CBossScene(CGameWorld & _rGameWorld, const char * _szMapDirectory)
	:
	CScene(_rGameWorld),
	m_szMapDirectory(_szMapDirectory),
	m_pPlayer(TO_WOL(_rGameWorld).GetPlayer())
{
	//ResetScene();
}

CBossScene::~CBossScene()
{
	Release();
}

void CBossScene::ResetScene(void)
{
	Release();
	m_pMapLoader = new CMapLoader(m_rGameWorld, m_szMapDirectory);
	const pair<float, float> pairSpawnPoint = m_pMapLoader->GetSpawnPoint()->GetXY();
	m_rGameWorld.GetListObjs().emplace_back(m_pPlayer); // 전역 objs 리스트에 집어넣는다. => 충돌체크하기 위함.
	m_pPlayerBarUI = new CUI_PlayerBar(m_rGameWorld, m_pPlayer);
	m_pSkillBarUI = new CUI_SkillBar(m_rGameWorld, m_pPlayer);
	m_pSkillKeyBindingUI = new CUI_SkillKeyBinding(m_rGameWorld, m_pPlayer);
	m_pMinimapUI = new CUI_Minimap(m_rGameWorld, m_pMapLoader, m_pPlayer);
	m_pMoneyUI = new CUI_Money(m_rGameWorld, (WINCX >> 1) - 100, WINCY - 50, *m_pPlayer);
	m_listSpawners.emplace_back(new CPlayerSpawner(m_rGameWorld, m_pPlayer, pairSpawnPoint.first, pairSpawnPoint.second, 0.05f));
	m_vecObjsToRender.reserve(100);
	m_vecObjsToRender.clear();
}

int CBossScene::Update(float _fDeltaTime)
{
	for (auto& pObj : m_listSpawnerGenerators) {
		if (pObj->Update(_fDeltaTime) == 1) {
			m_pMapLoader->UnactivateDoors(pObj->GetGroupID());
			DeleteSafe(pObj);
		}
	}

	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pObj->Update(_fDeltaTime);
	}

	for (auto& pObj : m_listSpawners) {
		pObj->Update(_fDeltaTime);
	}

	m_pPlayerBarUI->Update(_fDeltaTime);
	m_pSkillBarUI->Update(_fDeltaTime);
	m_pSkillKeyBindingUI->Update(_fDeltaTime);
	m_pMinimapUI->Update(_fDeltaTime);
	m_pMoneyUI->Update(_fDeltaTime);

	return 0;
}

void CBossScene::LateUpdate(void)
{
	//// 트리거 박스와의 충돌 체크
	for (auto& pGroup : m_pMapLoader->GetTriggersGroups()) {
		DO_IF_IS_NOT_VALID_OBJ(pGroup) continue;	// 트리거는 중복 발동이 되면 안되므로 이것을 넣어준다.

		CObj* pCollider = m_pPlayer->GetCollider(COLLIDER::TYPE_WALL);	// COLLIDER::TYPE_WALL 채널에 반응한다.
		if (!pCollider) continue;									// 벽 충돌체가 없으면 넘어간다.
		pCollider->LateUpdate();

		if (IsCollided(pCollider->GetRect(), pGroup->GetRect())) {
			for (auto& pObj : pGroup->GetMapObjs()) {
				if (IsCollided(pCollider->GetRect(), pObj->GetRect())) {
					// TODO : 그룹 삭제에 대한 코드를 변경해야됨.
					m_listSpawnerGenerators.emplace_back(new CSpawnerGenerator(m_rGameWorld, m_listSpawners/*, m_listMonsters*/, pGroup->GetGroupID()));
					m_pMapLoader->ActivateDoors(pGroup->GetGroupID());
					pGroup->SetValid(false);
					break;
				}
			}
		}
	}
	CollectGarbageObjects(m_pMapLoader->GetTriggersGroups());// 충돌한 트리거를 제거

	// 맵벽과 오브젝트의 충돌 체크
	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		CObj* pCollider = pObj->GetCollider(COLLIDER::TYPE_WALL);	// COLLIDER::TYPE_WALL 채널에 반응한다.
		if (!pCollider) continue;									// 벽 충돌체가 없으면 넘어간다.
		pCollider->LateUpdate();
		for (auto& pGroup : m_pMapLoader->GetCollidersGroups()) {
			if (IsCollided(pCollider->GetRect(), pGroup->GetRect())) {
				RECT rcCollidedRect;
				for (auto& pWall : pGroup->GetMapObjs()) {
					// 충돌이 일어났다면, 위치를 조정한다.
					if (IntersectRect(&rcCollidedRect, &pCollider->GetRect(), &pWall->GetRect())) {
						switch (GetDirByDegree(
							GetPositiveDegreeByVector(pCollider->GetX() - pWall->GetX(), pCollider->GetY() - pWall->GetY())
							, pWall->GetWidth(), pWall->GetHeight())) {
						case OBJ::DIR_RIGHT:
							pObj->MoveTo((rcCollidedRect.right - rcCollidedRect.left) + max(0, pWall->GetRect().right - rcCollidedRect.right), 0.f);
							break;
						case OBJ::DIR_LEFT:
							pObj->MoveTo(-(rcCollidedRect.right - rcCollidedRect.left) - max(0, pWall->GetRect().left - rcCollidedRect.left), 0.f);
							break;
						case OBJ::DIR_DOWN:
							pObj->MoveTo(0.f, (rcCollidedRect.bottom - rcCollidedRect.top) + max(0, pWall->GetRect().bottom - rcCollidedRect.bottom));
							break;
						case OBJ::DIR_UP:
							pObj->MoveTo(0.f, -(rcCollidedRect.bottom - rcCollidedRect.top) - max(0, pWall->GetRect().top - rcCollidedRect.top));
							break;
						}
						pCollider->LateUpdate();

						// pObj가 벽에 충돌했을시 반응. Ex. 벽과 충돌하면 사라진다던지..
						pObj->ReactToCollider(pWall,
							POINT{
							((rcCollidedRect.right + rcCollidedRect.left) >> 1),
							((rcCollidedRect.bottom + rcCollidedRect.top) >> 1) }, rcCollidedRect);
					}
				}
			}
		}
	}

	// 맵문과 오브젝트의 충돌 체크
	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		CObj* pCollider = pObj->GetCollider(COLLIDER::TYPE_WALL);	// COLLIDER::TYPE_WALL 채널에 반응한다.
		if (!pCollider) continue;									// 벽 충돌체가 없으면 넘어간다.
		pCollider->LateUpdate();

		RECT rcCollidedRect;
		for (auto& pDoor : m_pMapLoader->GetDoors()) {
			// 충돌이 일어났다면, 위치를 조정한다.
			if (IntersectRect(&rcCollidedRect, &pCollider->GetRect(), &pDoor->GetRect())) {
				switch (GetDirByDegree(
					GetPositiveDegreeByVector(pCollider->GetX() - pDoor->GetX(), pCollider->GetY() - pDoor->GetY())
					, pDoor->GetWidth(), pDoor->GetHeight())) {
				case OBJ::DIR_RIGHT:
					pObj->MoveTo((rcCollidedRect.right - rcCollidedRect.left) + max(0, pDoor->GetRect().right - rcCollidedRect.right), 0.f);
					break;
				case OBJ::DIR_LEFT:
					pObj->MoveTo(-(rcCollidedRect.right - rcCollidedRect.left) - max(0, pDoor->GetRect().left - rcCollidedRect.left), 0.f);
					break;
				case OBJ::DIR_DOWN:
					pObj->MoveTo(0.f, (rcCollidedRect.bottom - rcCollidedRect.top) + max(0, pDoor->GetRect().bottom - rcCollidedRect.bottom));
					break;
				case OBJ::DIR_UP:
					pObj->MoveTo(0.f, -(rcCollidedRect.bottom - rcCollidedRect.top) - max(0, pDoor->GetRect().top - rcCollidedRect.top));
					break;
				}
				pCollider->LateUpdate();

				// pObj가 벽에 충돌했을시 반응. Ex. 벽과 충돌하면 사라진다던지..
				pObj->ReactToCollider(pDoor,
					POINT{
					((rcCollidedRect.right + rcCollidedRect.left) >> 1),
					((rcCollidedRect.bottom + rcCollidedRect.top) >> 1) }, rcCollidedRect);
			}
		}
	}

	CObj* pCollider1 = nullptr;
	CObj* pCollider2 = nullptr;
	RECT rcCollidedRect;
	auto& listObjs = m_rGameWorld.GetListObjs();
	int listObjsSize = listObjs.size();
	CObj* pObj1 = nullptr;
	CObj* pObj2 = nullptr;
	for (auto iter1 = listObjs.begin(); iter1 != listObjs.end(); ++iter1) {
		for (auto iter2 = iter1; iter2 != listObjs.end(); ++iter2) {
			pObj1 = (*iter1);
			pObj2 = (*iter2);
			if (pObj1 == pObj2) continue;
			if (!(pCollider1 = pObj1->GetCollider(COLLIDER::TYPE_DAMAGED))) continue;
			if (!(pCollider2 = pObj2->GetCollider(COLLIDER::TYPE_DAMAGED))) continue;
			pCollider1->LateUpdate();
			pCollider2->LateUpdate();
			if (IntersectRect(&rcCollidedRect, &pCollider1->GetRect(), &pCollider2->GetRect())) {
				pObj1->ReactToCollider(pObj2,
					POINT{
					((rcCollidedRect.right + rcCollidedRect.left) >> 1),
					((rcCollidedRect.bottom + rcCollidedRect.top) >> 1) }, rcCollidedRect);
				pObj2->ReactToCollider(pObj1,
					POINT{
					((rcCollidedRect.right + rcCollidedRect.left) >> 1),
					((rcCollidedRect.bottom + rcCollidedRect.top) >> 1) }, rcCollidedRect);
			}
			else {
				pObj1->EraseCollidedObj(pObj2);
				pObj2->EraseCollidedObj(pObj1);
			}
		}
	}
	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		pObj->UpdateCollidedObjs();
	}

	m_listSpawnerGenerators.remove_if([](auto& pObj) { return pObj == nullptr; });
	CollectGarbageObjects(m_rGameWorld.GetListObjs());
	CollectGarbageObjects(m_listSpawners);
}

void CBossScene::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	g_iRenderCount = 0;

	for (auto& pObj : m_pMapLoader->GetDoors()) {
		m_vecObjsToRender.emplace_back(pObj);
	}

	for (auto& pObj : m_rGameWorld.GetListObjs()) {
		m_vecObjsToRender.emplace_back(pObj);
	}
	/*for (auto& pObj : m_listMonsters) {
		m_vecObjsToRender.emplace_back(pObj);
	}
	*/

	RECT rcScreenRect;
	for (auto& pGroup : m_pMapLoader->GetAtlasObjsGroups(2)) {
		rcScreenRect = _pCamera->GetScreenRect(pGroup->GetRect());
		if (IsCollided(m_rGameWorld.GetViewSpace()->GetRect(), rcScreenRect)) {
			for (auto& pObj : pGroup->GetMapObjs()) {
				m_vecObjsToRender.emplace_back(pObj);
			}
		}
	}


	for (auto& pObj : m_pMapLoader->GetAtlasObjsGroups(0)) {
		rcScreenRect = _pCamera->GetScreenRect(pObj->GetRect());
		if (IsCollided(m_rGameWorld.GetViewSpace()->GetRect(), rcScreenRect)) {
			pObj->Render(_hdc, _pCamera);
		}
	}

	// y축 정렬
	sort(m_vecObjsToRender.begin(), m_vecObjsToRender.end(), [](CObj* pObj1, CObj* pObj2) {
		if (pObj1->GetRenderLayer() < pObj2->GetRenderLayer()) {
			return true;
		}
		else if (pObj1->GetRenderLayer() == pObj2->GetRenderLayer()) {
			return pObj1->GetBottom() < pObj2->GetBottom();
		}
		return false;
	});
	for (auto& pObj : m_vecObjsToRender) {
		pObj->Render(_hdc, _pCamera);
	}
	m_vecObjsToRender.clear();

	for (auto& pObj : m_listSpawners) {
		pObj->Render(_hdc, _pCamera);
	}
	//for (auto&pObj : m_listHitEffects) {
	//	pObj->Render(_hdc, _pCamera);
	//}

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

void CBossScene::Release(void)
{
	DeleteSafe(m_pPlayerBarUI);
	DeleteSafe(m_pSkillBarUI);
	DeleteSafe(m_pSkillKeyBindingUI);
	DeleteSafe(m_pMinimapUI);
	DeleteSafe(m_pMoneyUI);
	DeleteSafe(m_pMapLoader);
	DeleteListSafe(m_listSpawnerGenerators);
	DeleteListSafe(m_listSpawners);
	m_vecObjsToRender.clear();

	// 전역 Objs 리스트에 플레이어가 있다면 빼내고, 리스트를 파괴한다. 
	auto& vecObjs = m_rGameWorld.GetListObjs();
	auto iter = find(vecObjs.begin(), vecObjs.end(), m_pPlayer);
	if (iter != vecObjs.end()) {
		vecObjs.erase(iter);
	}
	DeleteListSafe(vecObjs);
}