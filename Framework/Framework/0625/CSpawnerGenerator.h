#pragma once
class CSpawnerGenerator
{
public:
	CSpawnerGenerator(CGameWorld & _rGameWorld, list<CObj*>& _listSpawners, list<CObj*>& _listMonsters, int _iGroupID);
	virtual ~CSpawnerGenerator();

public:
	int Update(float _fDeltaTime);		// 몬스터 생성을 함. 1을 반환하면 더이상 생성할 몬스터가 없다는 뜻.
	void Release(void);

private:
	CGameWorld& m_rGameWorld;
	vector<list<CObj*>> m_vecUnactiveSpawnersPerPhase;
	list<CObj*>& m_listSpawners;
	list<CObj*>& m_listMonserts;
	int m_iMaxPhase = 0;
	int m_iCurPhase = 0;
	int m_iGroupID = -1;
};

