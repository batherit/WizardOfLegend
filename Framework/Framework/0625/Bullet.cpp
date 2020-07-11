#include "stdafx.h"
#include "Bullet.h"
#include "CSpace.h"
#include "MainApp.h"

CBullet::CBullet(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, size_t _iWidth /*= ciBulletSize*/, size_t _iHeight /*= ciBulletSize*/, float _fSpeed /*= cfBulletSpeed*/, float _fDamage /*= cfBulletSpeed*/)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight, _fSpeed, Ellipse),
	m_fToX(_fToX),
	m_fToY(_fToY),
	m_fDamage(_fDamage)
{
	// ���� ���� ����
	NormalizeVector(m_fToX, m_fToY);
}


CBullet::~CBullet()
{
}

void CBullet::Ready(void)
{
}

int CBullet::Update(float _fDeltaTime)
{
	MoveTo(m_fToX * m_fSpeed, m_fToY * m_fSpeed);

	return 0;
}

void CBullet::LateUpdate(void)
{
	DO_IF_IS_VALID_OBJ(TO_GAMEWORLD(GetGameWorld()).GetViewSpace()) {
		DO_IF_IS_VALID_OBJ(this) {
			if (!(TO_SPACE(TO_GAMEWORLD(GetGameWorld()).GetViewSpace())->IsObjInside(this))) {
				SetValid(false);
			}
		}
	}
}

void CBullet::Render(const HDC & _hdc)
{
	CObj::Render(_hdc);
}

void CBullet::Release(void)
{
}
