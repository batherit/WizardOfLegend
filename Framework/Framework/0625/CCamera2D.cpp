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
			// ÁÜÀÎÀÌ ÀÌ·ç¾îÁü.
			Clamp(&(m_fZoomMultiple -= (m_cfZoomSpeed * _fDeltaTime)), m_cfMaxZoomOut, m_cfMaxZoomIn);
		}

		if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_E)) {
			// ÁÜ¾Æ¿ôÀÌ ÀÌ·ç¾îÁü.
			Clamp(&(m_fZoomMultiple += (m_cfZoomSpeed * _fDeltaTime)), m_cfMaxZoomOut, m_cfMaxZoomIn);
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

RECT CCamera2D::TransformRect(RECT& _rRect)
{
	RECT rc;

	//_rRect.right - GetX() - GetX() 

	rc.right = (_rRect.right - GetX()) * m_fZoomMultiple + (WINCX >> 1);
	rc.top = (_rRect.top - GetY())* m_fZoomMultiple + (WINCY >> 1);
	rc.left = (_rRect.left - GetX()) * m_fZoomMultiple + (WINCX >> 1);
	rc.bottom = (_rRect.bottom - GetY())* m_fZoomMultiple + (WINCY >> 1);

	return rc;
}

pair<float, float> CCamera2D::TransformPoint(float _fX, float _fY)
{
	pair<float, float> pairPoint;
	pairPoint.first = (_fX - GetX())* m_fZoomMultiple + (WINCX >> 1);
	pairPoint.second = (_fY - GetY())* m_fZoomMultiple + (WINCY >> 1);

	return pairPoint;
}
