#pragma once
class CSpawnerGenerator
{
public:
	CSpawnerGenerator(CGameWorld & _rGameWorld, list<CObj*>& _listSpawners, list<CObj*>& _listMonsters, int _iGroupID);
	virtual ~CSpawnerGenerator();

public:
	int Update(float _fDeltaTime);		// ���� ������ ��. 1�� ��ȯ�ϸ� ���̻� ������ ���Ͱ� ���ٴ� ��.
	void Release(void);
	int GetGroupID(void) const { return m_iGroupID; }
	void DecreaseSpawnedMonstersNum(void) { if (m_iSpawnedMonstersNum > 0) m_iSpawnedMonstersNum--; }

private:
	CGameWorld& m_rGameWorld;
	vector<list<CObj*>> m_vecUnactiveSpawnersPerPhase;
	list<CObj*>& m_listSpawners;
	list<CObj*>& m_listMonserts;
	int m_iMaxPhase = 0;
	int m_iCurPhase = 0;
	int m_iGroupID = -1;
	int m_iSpawnedMonstersNum = 0;
};

