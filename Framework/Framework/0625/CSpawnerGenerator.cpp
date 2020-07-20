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
		// �ش� �׷��� ������ ���Ͱ� ���� 
		for (auto& pObj : m_listMonserts) {
			if (pObj->GetGroupID() == m_iGroupID) {
				bOk = false;
				break;
			}
		}
		// �ش� �׷��� ������ ���� ���� ���ٸ�
		for (auto& pObj : m_listSpawners) {
			if (pObj->GetGroupID() == m_iGroupID) {
				bOk = false;
				break;
			}
		}
		// �����ʸ� �ܺ� ����Ʈ�� �����ϰ�, �ش� �������� ������ ����Ʈ�� ���ش�.
		// �׸��� ������ ����Ʈ�� �ø���.
		if (bOk) {
			for (auto& pObj : m_vecUnactiveSpawnersPerPhase[m_iCurPhase]) {
				m_listSpawners.emplace_back(pObj);
			}
			m_vecUnactiveSpawnersPerPhase[m_iCurPhase].clear();
			m_iCurPhase++;
		}
	}
	
	if(m_iCurPhase >= m_iMaxPhase) return 1;	// ������ ��������� ��� ��ȯ�� ���, ������ ��û�Ѵ�.
	return 0;									// ���� ��ȯ�� ����� ���������Ƿ� ������Ʈ�� ��� ��û�Ѵ�.
}

void CSpawnerGenerator::Release(void)
{
	for (auto& pList : m_vecUnactiveSpawnersPerPhase) {
		DeleteListSafe(pList);
	}
	m_vecUnactiveSpawnersPerPhase.clear();
	m_vecUnactiveSpawnersPerPhase.shrink_to_fit();
}
