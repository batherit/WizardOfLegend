#include "stdafx.h"
#include "Monster.h"
#include "Bullet.h"
#include "CSpace.h"
#include "UI_Gauge.h"
//#include "MainApp.h"
#include "CSpace.h"

CMonster::CMonster(CGameWorld& _rGameWorld)
	: 
	CObj(_rGameWorld, 0, 0, ciPlayerSize, ciPlayerSize, 0.f, 0.f, cfPlayerSpeed, Rectangle)
{
}


CMonster::CMonster(CGameWorld& _rGameWorld, float _fX, float _fY, size_t _iWidth /*= ciMonsterSize*/, size_t _iHeight /*= ciMonsterSize*/, float _fSpeed /*= cfMonsterSpeed*/, float _fHp /*= cfMonsterHp*/)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, 0.f, 0.f, _fSpeed, Rectangle),
	m_fHp(_fHp),
	m_pHpGauge(new CUI_Gauge(_rGameWorld, this, _iWidth * 2.f, 10.f, m_fHp, m_fHp, 0.f, -((m_iHeight >> 1) + 10.f)))
{
	SetToXnY(GetNumberMinBetweenMax(-1.f, 1.f), GetNumberMinBetweenMax(-1.f, 1.f));
}

CMonster::~CMonster()
{
}

void CMonster::Ready(void)
{
}

int CMonster::Update(float _fDeltaTime)
{
	//MoveTo(m_fToX * m_fSpeed, m_fToY * m_fSpeed);

	pair<float, float> pairPlayerXY = TO_GAMEWORLD(GetGameWorld()).GetPlayer()->GetXY();
	float fToX = pairPlayerXY.first - GetX();
	float fToY = pairPlayerXY.second - GetY();
	//NormalizeVector(fToX, fToY);
	float fRadian = acosf(fToX / GetVectorLength(fToX, fToY));
	if (fToY < 0) fRadian *= -1.f;
	//float fLength = GetVectorLength(fToX, fToY);
	
	MoveTo(m_fSpeed * DIR_X_RADIAN(fRadian), m_fSpeed * DIR_Y_RADIAN(fRadian));

	DO_IF_IS_VALID_OBJ(m_pHpGauge) { m_pHpGauge->Update(_fDeltaTime); }

	return 0;
}

void CMonster::LateUpdate(void)
{
	// 몬스터가 총알에 맞았는지 확인한다.
	RECT rc;
	
	OBJ::E_COLLIDED eCollidedPoint = OBJ::COL_END;
	float fCollidedDepth = 0.f;
	if (!(TO_SPACE((TO_GAMEWORLD(GetGameWorld()).GetViewSpace()))->IsObjInside(this, &eCollidedPoint, &fCollidedDepth))) {
		SetReverseDirectionOfCollision(eCollidedPoint, fCollidedDepth);
	}

	CBullet* pBullet = nullptr;
	for (auto& pObj : TO_GAMEWORLD(GetGameWorld()).GetBullets()) {
		DO_IF_IS_VALID_OBJ((pBullet = TO_BULLET(pObj))) {
			// CBullet 형태로 잘 변환되었고 해당 총알의 생명이 유효하다면,
			if (IsCollided(this, pBullet, rc)) {
				TakeDamage(pBullet->GetDamage());

				pBullet->SetValid(false);				// 사용된 총알은 생명이 끝났다.
				if (IsDied()) {
					SetValid(false);
					break;	// 죽은 몬스터는 생명이 끝났다. 생명이 끝났으므로 충돌 연산을 종료한다.
				}
			}
		}
	}
}

void CMonster::Render(const HDC & _hdc, CCamera2D* _pCamera)
{
	CObj::Render(_hdc, _pCamera);
	DO_IF_IS_VALID_OBJ(m_pHpGauge) { m_pHpGauge->Render(_hdc); }
}

void CMonster::Release(void)
{
	DeleteSafe(m_pHpGauge);
}

void CMonster::TakeDamage(float _fDamage)
{
	m_fHp -= _fDamage;
	DO_IF_IS_VALID_OBJ(m_pHpGauge) { m_pHpGauge->SetCurrentGauge(m_fHp); }
}

bool CMonster::IsDied(void)
{
	return m_fHp <= 0.f;
}

void CMonster::SetReverseDirectionOfCollision(OBJ::E_COLLIDED _CollidedPoint, float _fCollidedDepth)
{

	float fTemp;
	switch (_CollidedPoint) {
	case OBJ::COL_TOP:
	case OBJ::COL_BOTTOM:
		MoveTo(0.f, -_fCollidedDepth);
		fTemp = GetToY();
		fTemp *= -1.f;
		fTemp += GetNumberMinBetweenMax(-0.5f, 0.5f);
		SetToY(fTemp);
		break;
	case OBJ::COL_LEFT:
	case OBJ::COL_RIGHT:
		MoveTo(-_fCollidedDepth, 0.f);
		fTemp = GetToX();
		fTemp *= -1.f;
		fTemp += GetNumberMinBetweenMax(-0.5f, 0.5f);
		SetToX(fTemp);
		break;
	}
}