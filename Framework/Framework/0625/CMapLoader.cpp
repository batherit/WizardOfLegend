#include "stdafx.h"
#include "CMapLoader.h"
#include "CObj.h"
#include "CAtlasObj.h"
#include "CCollider.h"
#include "CTrigger.h"
#include "CDoor.h"
#include "CSpawnPoint.h"
#include "CMapFileMgr.h"
#include "CMapObjsGroup.h"


CMapLoader::CMapLoader(CGameWorld & _rGameWorld, const char* szMapDirectory)
	:
	m_rGameWorld(_rGameWorld)
{
	ClearObjs();

	FILE* fpIn = nullptr;
	errno_t err = fopen_s(&fpIn, szMapDirectory, "rt");
	if (!err) {

		int iSize = 0;
		CObj* pObj = nullptr;
		bool bIsRegistered = false;
		for (int i = 0; i < ciMaxDrawLayerNum; i++) {
			fscanf_s(fpIn, " %d", &iSize);	
			for (int j = 0; j < iSize; j++) {
				bIsRegistered = false;
				pObj = new CAtlasObj(fpIn, _rGameWorld);
				for (auto& pGroup : m_vecAtlasObjsGroups[i]) {
					if (pGroup->GetGroupID() == pObj->GetGroupID()) {
						pGroup->RegisterMapObj(pObj);
						bIsRegistered = true;
						break;
					}
				}
				if (!bIsRegistered) {
					CMapObjsGroup* pMapObjsGroup = new CMapObjsGroup(_rGameWorld, pObj->GetGroupID());
					pMapObjsGroup->RegisterMapObj(pObj);
					m_vecAtlasObjsGroups[i].emplace_back(pMapObjsGroup);
					
				}
			}
		}
		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			bIsRegistered = false;
			pObj = new CCollider(fpIn, _rGameWorld);
			for (auto& pGroup : m_vecCollidersGroups) {
				if (pGroup->GetGroupID() == pObj->GetGroupID()) {
					pGroup->RegisterMapObj(pObj);
					bIsRegistered = true;
					break;
				}
			}
			if (!bIsRegistered) {
				CMapObjsGroup* pMapObjsGroup = new CMapObjsGroup(_rGameWorld, pObj->GetGroupID());
				pMapObjsGroup->RegisterMapObj(pObj);
				m_vecCollidersGroups.emplace_back(pMapObjsGroup);
			}
		}
		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			bIsRegistered = false;
			pObj = new CTrigger(fpIn, _rGameWorld);
			for (auto& pGroup : m_listTriggersGroups) {
				if (pGroup->GetGroupID() == pObj->GetGroupID()) {
					pGroup->RegisterMapObj(pObj);
					bIsRegistered = true;
					break;
				}
			}
			if (!bIsRegistered) {
				CMapObjsGroup* pMapObjsGroup = new CMapObjsGroup(_rGameWorld, pObj->GetGroupID());
				pMapObjsGroup->RegisterMapObj(pObj);
				m_listTriggersGroups.emplace_back(pMapObjsGroup);
			}
		}

		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			pObj = new CDoor(fpIn, _rGameWorld);
			m_vecUnactiveDoors.emplace_back(pObj);
		}

		m_pSpawnPoint = new CSpawnPoint(fpIn, m_rGameWorld);
	}
	if (fpIn) fclose(fpIn);
}

CMapLoader::~CMapLoader()
{
	Release();
}

void CMapLoader::Update(float _fDeltaTime)
{
}

void CMapLoader::LateUpdate(void)
{
	CollectGarbageObjects(m_listTriggersGroups);
}

void CMapLoader::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	/*for (int i = 0; i < ciMaxDrawLayerNum; i++) {
		for (auto& pGroup : m_vecAtlasObjsGroups[i]) {
			pGroup->Render(_hdc, _pCamera);
		}
	}
	for (auto& pGroup : m_vecCollidersGroups) {
		pGroup->Render(_hdc, _pCamera);
	}
	for (auto& pGroup : m_vecTriggersGroups) {
		pGroup->Render(_hdc, _pCamera);
	}
	for (auto& pObj : m_vecUnactiveDoors) {
		pObj->Render(_hdc, _pCamera);
	}
	m_pSpawnPoint->Render(_hdc, _pCamera);*/
}

void CMapLoader::Release(void)
{
	ClearObjs();
}

void CMapLoader::ActivateDoors(int _iGroupID)
{
	for (auto& pObj : m_vecUnactiveDoors) {
		if (pObj->GetGroupID() == _iGroupID) {
			m_vecActiveDoors.emplace_back(pObj);
		}
	}
}

void CMapLoader::UnactivateDoors(int _iGroupID)
{
	for (auto iter = m_vecActiveDoors.begin(); iter != m_vecActiveDoors.end(); ) {
		if ((*iter)->GetGroupID() == _iGroupID) {
			iter = m_vecActiveDoors.erase(iter);
		}
		else iter++;
	}
}

void CMapLoader::ClearObjs(void)
{
	for (int i = 0; i < ciMaxDrawLayerNum; i++) {
		DeleteVectorSafe(m_vecAtlasObjsGroups[i]);
	}
	DeleteVectorSafe(m_vecCollidersGroups);
	DeleteListSafe(m_listTriggersGroups);
	DeleteVectorSafe(m_vecUnactiveDoors);
	// 활성 문은 UnactiveDoors에서 받아오는 것이므로 동적해제를 할 이유가 없다.
	m_vecActiveDoors.clear();
	m_vecActiveDoors.shrink_to_fit();
	DeleteSafe(m_pSpawnPoint);
}
