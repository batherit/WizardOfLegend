#include "stdafx.h"
#include "CMapLoader.h"
#include "CObj.h"
#include "CAtlasObj.h"
#include "CCollider.h"
#include "CTrigger.h"
#include "CDoor.h"
#include "CMapFileMgr.h"


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
		for (int i = 0; i < ciMaxDrawLayerNum; i++) {
			fscanf_s(fpIn, " %d", &iSize);
			for (int j = 0; j < iSize; j++) {
				pObj = new CAtlasObj(fpIn, _rGameWorld);
				m_listAtlasObjs[i].emplace_back(pObj);
			}
		}
		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			pObj = new CCollider(fpIn, _rGameWorld);
			m_listColliders.emplace_back(pObj);
		}
		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			pObj = new CTrigger(fpIn, _rGameWorld);
			m_listTriggers.emplace_back(pObj);
		}

		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			pObj = new CDoor(fpIn, _rGameWorld);
			m_listUnactiveDoors.emplace_back(pObj);
		}
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
}

void CMapLoader::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	for (int i = 0; i < ciMaxDrawLayerNum; i++) {
		for (auto& pObj : m_listAtlasObjs[i]) {
			pObj->Render(_hdc, _pCamera);
		}
	}
	for (auto& pObj : m_listColliders) {
		pObj->Render(_hdc, _pCamera);
	}
	for (auto& pObj : m_listTriggers) {
		pObj->Render(_hdc, _pCamera);
	}
	for (auto& pObj : m_listUnactiveDoors) {
		pObj->Render(_hdc, _pCamera);
	}
}

void CMapLoader::Release(void)
{
	ClearObjs();
}

void CMapLoader::ClearObjs(void)
{
	for (int i = 0; i < ciMaxDrawLayerNum; i++) {
		DeleteListSafe(m_listAtlasObjs[i]);
	}
	DeleteListSafe(m_listColliders);
	DeleteListSafe(m_listTriggers);
	DeleteListSafe(m_listUnactiveDoors);
	DeleteListSafe(m_listActiveDoors);
}
