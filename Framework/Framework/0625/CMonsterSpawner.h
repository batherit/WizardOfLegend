#pragma once
#include "CSpawner.h"

class CSpawnerGenerator;

class CMonsterSpawner :
	public CSpawner
{
public:
	CMonsterSpawner(CGameWorld& _rGameWorld/*, list<CObj*>& _listMonsters*/, float _fTimeToDelay, float _fX, float _fY, SPAWN::E_TYPE _eType, int _iGroupID, CSpawnerGenerator* _pSpawnerGenerator = nullptr, CObj* _pSummoner = nullptr);
	virtual ~CMonsterSpawner();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;

private:
	const float m_fTimeToDelay;
	float m_fElapsedTime = 0.f;
	//list<CObj*>& m_listMonsters;
	SPAWN::E_TYPE m_eType = SPAWN::TYPE_END;
	CSpawnerGenerator* m_pSpawnerGenerator = nullptr;
	CObj* m_pSummoner = nullptr;
	bool m_bIsAnimStarted = false;
};

