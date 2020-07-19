#include "stdafx.h"
#include "CSpawnerGenerator.h"
#include "CMonsterSpawner.h"


CSpawnerGenerator::CSpawnerGenerator(CGameWorld & _rGameWorld, list<CObj*>& _listSpawners, list<CObj*>& _listMonsters, int _iGroupID)
	:
	m_rGameWorld(_rGameWorld),
	m_listSpawners(_listSpawners),
	m_listMonserts(_listMonsters)
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
		m_vecUnactiveSpawnersPerPhase.reserve(m_iMaxPhase);
		for (int i = 0; i < m_iMaxPhase; i++) {
			while (true) {
				fscanf_s(fpIn, "%d %d %d", &iSpawnX, &iSpawnY, &eType);
				if (eType < 0) break;

				m_vecUnactiveSpawnersPerPhase[i].emplace_back(
					new CMonsterSpawner(m_rGameWorld, _listMonsters, iSpawnX, iSpawnY, eType));
			}
		}
	}
	if (fpIn) fclose(fpIn);
}

CSpawnerGenerator::~CSpawnerGenerator()
{
}

void CSpawnerGenerator::Update(void)
{
}

void CSpawnerGenerator::Release(void)
{
	for (auto& pList : m_vecUnactiveSpawnersPerPhase) {
		DeleteListSafe(pList);
	}
	m_vecUnactiveSpawnersPerPhase.clear();
	m_vecUnactiveSpawnersPerPhase.shrink_to_fit();
}
