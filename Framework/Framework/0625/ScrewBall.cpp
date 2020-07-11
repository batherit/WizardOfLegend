#include "stdafx.h"
#include "ScrewBall.h"


CScrewBall::CScrewBall(CGameWorld& _rGameWorld, CObj * _pOwner, size_t _iWidth, size_t _iHeight, float _fSpeed, float _fDegree, float _fOffset)
	:
	m_pOwner(nullptr),
	CObj(_rGameWorld, 0.f, 0.f, _iWidth, _iHeight, _fSpeed, Ellipse),
	m_fScrewBallDegree(_fDegree),
	m_fScrewBallOffset(_fOffset)
{
	if (_pOwner) {
		m_pOwner = _pOwner;
		SetX(m_pOwner->GetX() + m_fScrewBallOffset * DIR_X_DEGREE(m_fScrewBallDegree));
		SetY(m_pOwner->GetY() + m_fScrewBallOffset * DIR_Y_DEGREE(m_fScrewBallDegree));
	}
}

CScrewBall::~CScrewBall()
{
	Release();
}

void CScrewBall::Ready(void)
{
}

int CScrewBall::Update(float _fDeltaTime)
{
	m_fScrewBallDegree += GetSpeed();

	if (m_pOwner) {
		SetX(m_pOwner->GetX() + m_fScrewBallOffset * DIR_X_DEGREE(m_fScrewBallDegree));
		SetY(m_pOwner->GetY() + m_fScrewBallOffset * DIR_Y_DEGREE(m_fScrewBallDegree));
	}

	return 0;
}

void CScrewBall::LateUpdate(void)
{
}

void CScrewBall::Render(const HDC & _hdc)
{
	Ellipse(_hdc,
		GetScrewBallX() - (GetWidth() >> 1),
		GetScrewBallY() - (GetHeight() >> 1),
		GetScrewBallX() + (GetWidth() >> 1),
		GetScrewBallY() + (GetHeight() >> 1));
}

void CScrewBall::Release(void)
{
}
