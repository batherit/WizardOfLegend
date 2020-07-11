#include "stdafx.h"
#include "ScrewBall.h"
#include "CCamera2D.h"


CScrewBall::CScrewBall(CGameWorld& _rGameWorld, CObj * _pOwner, size_t _iWidth, size_t _iHeight, float _fSpeed, float _fDegree, float _fOffset)
	:
	m_pOwner(nullptr),
	CObj(_rGameWorld, 0.f, 0.f, _iWidth, _iHeight, 0.f, 0.f, _fSpeed, Ellipse),
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

void CScrewBall::Render(const HDC & _hdc, CCamera2D* _pCamera)
{
	if (_pCamera) {
		RECT rc = _pCamera->TransformRect(GetScrewBallRect());
		Ellipse(_hdc, rc.left, rc.top, rc.right, rc.bottom);
	}
	else {
		Ellipse(_hdc,
			GetScrewBallLeft(),
			GetScrewBallTop(),
			GetScrewBallRight(),
			GetScrewBallBottom());
	}
}

void CScrewBall::Release(void)
{
}

RECT CScrewBall::GetScrewBallRect(void) const
{
	RECT rc;

	rc.left = GetScrewBallX() - (GetWidth() >> 1);
	rc.top = GetScrewBallY() - (GetHeight() >> 1);
	rc.right = GetScrewBallX() + (GetWidth() >> 1);
	rc.bottom = GetScrewBallY() + (GetHeight() >> 1);

	return rc;
}
