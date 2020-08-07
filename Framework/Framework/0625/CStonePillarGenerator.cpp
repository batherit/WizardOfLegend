#include "stdafx.h"
#include "CStonePillarGenerator.h"
#include "CStonePillar.h"




CStonePillarGenerator::CStonePillarGenerator(CGameWorld & _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, float _fIntervalDist, float _fIntervalTime, float _fDelayTime, float _fLifeTime)
	:
	CObj(_rGameWorld, _fX, _fY, 10, 10, _fToX, _fToY, 0.f),
	m_fIntervalDist(_fIntervalDist),
	m_fIntervalTime(_fIntervalTime),
	m_fDelayTime(_fDelayTime),
	m_fLifeTime(_fLifeTime)
{
	m_pColliders[COLLIDER::TYPE_WALL] = this;
}


CStonePillarGenerator::~CStonePillarGenerator()
{
}

int CStonePillarGenerator::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;

	if ((m_fElapsedTime += _fDeltaTime) >= m_fIntervalTime) {
		GetGameWorld().GetListObjs().emplace_back(new CStonePillar(GetGameWorld(), GetX(), GetY(), m_fDelayTime, m_fLifeTime));
		SetXY(GetX() + GetToX() * m_fIntervalDist, GetY() + GetToY() * m_fIntervalDist);
		m_fElapsedTime = 0.f;
	}
		

	return 0;
}

void CStonePillarGenerator::LateUpdate(void)
{
}

void CStonePillarGenerator::Render(HDC & _hdc, CCamera2D * _pCamera)
{
}

void CStonePillarGenerator::Release(void)
{
}

void CStonePillarGenerator::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT & _rcCollisionRect)
{
	switch (_pCollider->GetObjType()) {
	case OBJ::TYPE_WALL:
		SetValid(false);
		break;
	}
}
