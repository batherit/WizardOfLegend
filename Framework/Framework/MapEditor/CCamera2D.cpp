#include "stdafx.h"
#include "CCamera2D.h"


CCamera2D::CCamera2D(CGameWorld& _rGameWorld, CObj* _pOwner /*= nullptr*/, float _fX /*= 0.f*/, float _fY /*= 0.f*/)
	:
	m_pOwner(nullptr),
	CObj(_rGameWorld, _fX, _fY, 0.f, 0.f, 0.f, 0.f, 0.f, nullptr)
{
	if (_pOwner) {
		m_pOwner = _pOwner;
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}
	else {
		SetSpeed(500.0f);
	}
}


CCamera2D::~CCamera2D()
{
}

int CCamera2D::Update(float _fDeltaTime)
{
	if (m_pOwner) {
		SetX(m_pOwner->GetX());
		SetY(m_pOwner->GetY());
	}
	else {
		if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_Q)) {
			// ������ �̷����.
			ZoomIn(_fDeltaTime);
		}

		if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_E)) {
			// �ܾƿ��� �̷����.
			ZoomOut(_fDeltaTime);
		}

		if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_W)) {
			MoveTo(cfDeltaX[OBJ::DIR_UP] * GetSpeed() * 1.f / m_fZoomMultiple * _fDeltaTime, cfDeltaY[OBJ::DIR_UP] * GetSpeed() * 1.f / m_fZoomMultiple * _fDeltaTime);
		}

		if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_A)) {
			MoveTo(cfDeltaX[OBJ::DIR_LEFT] * GetSpeed()* 1.f / m_fZoomMultiple * _fDeltaTime, cfDeltaY[OBJ::DIR_LEFT] * GetSpeed()* 1.f / m_fZoomMultiple * _fDeltaTime);
		}

		if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_S)) {
			MoveTo(cfDeltaX[OBJ::DIR_DOWN] * GetSpeed()* 1.f / m_fZoomMultiple * _fDeltaTime, cfDeltaY[OBJ::DIR_DOWN] * GetSpeed()* 1.f / m_fZoomMultiple * _fDeltaTime);
		}

		if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_D)) {
			MoveTo(cfDeltaX[OBJ::DIR_RIGHT] * GetSpeed()* 1.f / m_fZoomMultiple * _fDeltaTime, cfDeltaY[OBJ::DIR_RIGHT] * GetSpeed()* 1.f / m_fZoomMultiple * _fDeltaTime);
		}
	}

	return 0;
}

void CCamera2D::ZoomIn(float _fDeltaTime)
{
	Clamp(&(m_fZoomMultiple -= (m_cfZoomSpeed * _fDeltaTime)), m_cfMaxZoomOut, m_cfMaxZoomIn);
}

void CCamera2D::ZoomOut(float _fDeltaTime)
{
	Clamp(&(m_fZoomMultiple += (m_cfZoomSpeed * _fDeltaTime)), m_cfMaxZoomOut, m_cfMaxZoomIn);
}

RECT CCamera2D::GetScreenRect(RECT& _rRectW)
{
	RECT rc;

	rc.right = (_rRectW.right - GetX()) * m_fZoomMultiple + (WINCX >> 1);
	rc.top = (_rRectW.top - GetY())* m_fZoomMultiple + (WINCY >> 1);
	rc.left = (_rRectW.left - GetX()) * m_fZoomMultiple + (WINCX >> 1);
	rc.bottom = (_rRectW.bottom - GetY())* m_fZoomMultiple + (WINCY >> 1);

	return rc;
}

pair<float, float> CCamera2D::GetScreenPoint(float _fXW, float _fYW)
{
	pair<float, float> pairScreenPoint;
	pairScreenPoint.first = (_fXW - GetX())* m_fZoomMultiple + (WINCX >> 1);
	pairScreenPoint.second = (_fYW - GetY())* m_fZoomMultiple + (WINCY >> 1);

	return pairScreenPoint;
}

pair<float, float> CCamera2D::GetWorldPoint(float _fXS, float _fYS)
{
	pair<float, float> pairWorldPoint;
	pairWorldPoint.first = (_fXS - (WINCX >> 1)) / m_fZoomMultiple + GetX();
	pairWorldPoint.second = (_fYS - (WINCY >> 1)) / m_fZoomMultiple + GetY();

	return pairWorldPoint;
}
