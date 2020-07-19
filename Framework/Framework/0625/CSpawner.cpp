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
		SetValid(false);	// => Valid�� �ϸ� LateUpdate���� �÷��Ͱ� �����ذ�.
		return 1; 
	}
	return 0;
}