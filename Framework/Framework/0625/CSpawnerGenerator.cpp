#include "stdafx.h"
#include "CSpawnerGenerator.h"
#include "CMonsterSpawner.h"


CSpawnerGenerator::CSpawnerGenerator(CGameWorld & _rGameWorld, list<CObj*>& _listSpawners, list<CObj*>& _listMonsters, int _iGroupID)
	:
	m_rGameWorld(_rGameWorld),
	m_listSpawners(_listSpawners),
	m_listMonserts(_listMonsters),
	m_iGroupID(_iGroupID)
{
	string strEventDirectory = "../EventDatas/";
	strEventDirectory += to_string(_iGroupID);
	strEventDirectory += ".txt";

	FILE* fpIn = nullptr;
	errno_t err = fopen_s(&fpIn, strEventDirectory.c_str(), "rt");

	if (!err) {
		int iSpawnX = 0;
		int iSpawnY = 0;
		SPAWN::E_TYPE eType;
		fscanf_s(fpIn, " %d", &m_iMaxPhase);
		m_vecUnactiveSpawnersPerPhase.resize(m_iMaxPhase);
		for (int i = 0; i < m_iMaxPhase; i++) {
			while (true) {
				fscanf_s(fpIn, "%d %d %d", &iSpawnX, &iSpawnY, &eType);
				if (eType < 0) break;

				m_vecUnactiveSpawnersPerPhase[i].emplace_back(
					new CMonsterSpawner(m_rGameWorld, _listMonsters, iSpawnX, iSpawnY, eType, _iGroupID));
			}
		}
	}
	if (fpIn) fclose(fpIn);
}

CSpawnerGenerator::~CSpawnerGenerator()
{
	Release();
}

int CSpawnerGenerator::Update(float _fDeltaTime)
{
	if (m_iCurPhase < m_iMaxPhase) {
		bool bOk = true;
		// 해당 그룹의 생성된 몬스터가 없고 
		for (auto& pObj : m_listMonserts) {
			if (pObj->GetGroupID() == m_iGroupID) {
				bOk = false;
				break;
			}
		}
		// 해당 그룹의 생성할 몬스터 또한 없다면
		for (auto& pObj : m_listSpawners) {
			if (pObj->GetGroupID() == m_iGroupID) {
				bOk = false;
				break;
			}
		}
		// 스포너를 외부 리스트에 전달하고, 해당 페이즈의 스포너 리스트를 없앤다.
		// 그리고 페이즈 포인트를 늘린다.
		if (bOk) {
			for (auto& pObj : m_vecUnactiveSpawnersPerPhase[m_iCurPhase]) {
				m_listSpawners.emplace_back(pObj);
			}
			m_vecUnactiveSpawnersPerPhase[m_iCurPhase].clear();
			m_iCurPhase++;
		}
	}
	
	if(m_iCurPhase >= m_iMaxPhase) return 1;	// 마지막 페이즈까지 모두 소환한 경우, 삭제를 요청한다.
	return 0;									// 아직 소환할 페이즈가 남아있으므로 업데이트를 계속 요청한다.
}

void CSpawnerGenerator::Release(void)
{
	for (auto& pList : m_vecUnactiveSpawnersPerPhase) {
		DeleteListSafe(pList);
	}
	m_vecUnactiveSpawnersPerPhase.clear();
	m_vecUnactiveSpawnersPerPhase.shrink_to_fit();
}
