#include "stdafx.h"
#include "CObj.h"


CObj::CObj(CGameWorld& _rGameWorld)
	:
	m_rGameWorld(_rGameWorld),
	m_bIsValid(true),
	m_fX(0.f),
	m_fY(0.f),
	m_iWidth(10),
	m_iHeight(10),
	m_fSpeed(0.f),
	m_pDrawFunc(Rectangle)
{
}

CObj::CObj(CGameWorld& _rGameWorld, float _fX /*= 0*/, float _fY /*= 0*/, size_t _iWidth /*= 10*/, size_t _iHeight /*= 10*/, float _fSpeed /*= 0.f*/, BOOL(__stdcall *_pDrawFunc) (HDC hdc, int _left, int _right, int _top, int _bottom) /*= Rectangle*/)
	:
	m_rGameWorld(_rGameWorld),
	m_bIsValid(true),
	m_fX(_fX),
	m_fY(_fY),
	m_iWidth(_iWidth),
	m_iHeight(_iHeight),
	m_fSpeed(_fSpeed),
	m_pDrawFunc(_pDrawFunc)
{
}


CObj::~CObj()
{
}