#include "stdafx.h"
#include "CSpawner.h"



CSpawner::CSpawner(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, 0, 0)
{

}

CSpawner::~CSpawner()
{
}

int CSpawner::Update(float _fDeltaTime)
{
	if (UpdateAnim(_fDeltaTime) == 1) {
		SetValid(false);	// => Valid로 하면 LateUpdate에서 컬렉터가 수거해감.
		return 1; 
	}
	return 0;
}