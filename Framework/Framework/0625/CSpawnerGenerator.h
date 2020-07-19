#pragma once
class CSpawnerGenerator
{
public:
	CSpawnerGenerator(CGameWorld & _rGameWorld, list<CObj*>& _listSpawners, list<CObj*>& _listMonsters, int _iGroupID);
	virtual ~CSpawnerGenerator();

public:
	int Update(void);		// ���� ������ ��. 1�� ��ȯ�ϸ� ���̻� ������ ���Ͱ� ���ٴ� ��.
	void Release(void);

private:
	CGameWorld& m_rGameWorld;
	vector<list<CObj*>> m_vecUnactiveSpawnersPerPhase;
	list<CObj*>& m_listSpawners;
	list<CObj*>& m_listMonserts;
	int m_iMaxPhase = 0;
	int m_iCurPhase = 0;
};

