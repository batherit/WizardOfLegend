#pragma once
#include "CSpawner.h"
class CMonsterSpawner :
	public CSpawner
{
public:
	CMonsterSpawner(CGameWorld& _rGameWorld, list<CObj*>& _listMonsters, float _fX, float _fY, SPAWN::E_TYPE _eType);
	virtual ~CMonsterSpawner();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;

private:
	list<CObj*>& m_listMonsters;
	SPAWN::E_TYPE m_eType = SPAWN::TYPE_END;
};

