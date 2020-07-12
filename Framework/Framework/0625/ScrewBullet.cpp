#include "stdafx.h"
#include "ScrewBullet.h"
#include "ScrewBall.h"


CScrewBullet::CScrewBullet(CGameWorld& _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, size_t _iWidth, size_t _iHeight, float _fSpeed, float _fDamage)
	:
	CBullet(_rGameWorld, _fX, _fY, _fToX, _fToY, _iWidth, _iHeight, _fSpeed, _fDamage),
	m_pScrewBall(new CScrewBall(_rGameWorld, this, 5, 5, 13.f, CLAMP_DEGREE(0.f), ((_iWidth > _iHeight ? _iWidth : _iHeight) >> 1) + 6.f))
{
}

CScrewBullet::~CScrewBullet()
{
	Release();
}

void CScrewBullet::Ready(void)
{

}

int CScrewBullet::Update(float _fDeltaTime)
{
	CBullet::Update(_fDeltaTime);
	DO_IF_IS_VALID_OBJ(m_pScrewBall) { m_pScrewBall->Update(_fDeltaTime); }
	return 0;
}

void CScrewBullet::LateUpdate(void)
{
	CBullet::LateUpdate();
	DO_IF_IS_VALID_OBJ(m_pScrewBall) { m_pScrewBall->LateUpdate(); }
}

void CScrewBullet::Render(HDC & _hdc, CCamera2D* _pCamera)
{
	CBullet::Render(_hdc, _pCamera);
	DO_IF_IS_VALID_OBJ(m_pScrewBall) { m_pScrewBall->Render(_hdc, _pCamera); }
}

void CScrewBullet::Release(void)
{
	DeleteSafe(m_pScrewBall);
}
