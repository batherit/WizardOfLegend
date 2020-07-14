#include "stdafx.h"
#include "CMap.h"


CMap::CMap(CGameWorld & _rGameWorld, const char* szMapDirectory)
	:
	m_rGameWorld(_rGameWorld)
{
}

CMap::~CMap()
{
}

void CMap::Update(float _fDeltaTime)
{
}

void CMap::LateUpdate(void)
{
}

void CMap::Render(HDC & _hdc, CCamera2D * _pCamera2D)
{
}

void CMap::Release(void)
{
}
