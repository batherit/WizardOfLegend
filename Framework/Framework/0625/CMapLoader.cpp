#include "stdafx.h"
#include "CMapLoader.h"
#include "CObj.h"
#include "CAtlasObj.h"
#include "CCollider.h"
#include "CTrigger.h"
#include "CMapFileMgr.h"


CMapLoader::CMapLoader(CGameWorld & _rGameWorld, const char* szMapDirectory)
	:
	m_rGameWorld(_rGameWorld)
{
	// 파일로부터 아틀라스 로더 생성 (맵 렌더 인포 구조체의 한 변수)
	CMapFileMgr::GetInstance()->GenerateAtlasLoadersFromFile("../MapDatas/AtlasLoaders.txt", m_stMapRenderInfo.vecAtlasLoaders);

	// 맵 구조 정보를 생성
	CMapFileMgr::GetInstance()->GenerateMapStructureFromFile("../MapDatas/MapStructure.txt", m_stMapRenderInfo.stMapStructureInfo);

	ClearObjs();

	FILE* fpIn = nullptr;
	errno_t err = fopen_s(&fpIn, szMapDirectory, "rt"); //"../MapDatas/Maps/0/Objs.txt"
	if (!err) {

		int iSize = 0;
		CObj* pObj = nullptr;
		for (int i = 0; i < ciMaxDrawLayerNum; i++) {
			fscanf_s(fpIn, " %d", &iSize);
			for (int j = 0; j < iSize; j++) {
				pObj = new CAtlasObj(fpIn, _rGameWorld, *this);
				m_listAtlasObjs[i].emplace_back(pObj);
			}
		}
		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			pObj = new CCollider(fpIn, _rGameWorld, *this);
			m_listColliders.emplace_back(pObj);
		}
		fscanf_s(fpIn, " %d", &iSize);
		for (int i = 0; i < iSize; i++) {
			pObj = new CTrigger(fpIn, _rGameWorld, *this);
			m_listTriggers.emplace_back(pObj);
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
}
