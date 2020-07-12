#include "stdafx.h"
#include "KeyMgr.h"

DWORD  CKeyMgr::m_dwOldKeyState = 0;
DWORD  CKeyMgr::m_dwCurrentKeyState = 0;
POINT CKeyMgr::m_ptOldClickedPoint = {0, 0};
CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
}


CKeyMgr::~CKeyMgr()
{
	Release();
}

void CKeyMgr::Update(void)
{
	m_dwOldKeyState = m_dwCurrentKeyState;

	for (int i = 0; i < KEY::KEY_END; i++) {
		if (IS_PRESS_(cdwKey[i])) {
			ON_FLAG(m_dwCurrentKeyState, cdwKeyMask[i]);
		}
		else {
			OFF_FLAG(m_dwCurrentKeyState, cdwKeyMask[i]);
		}
	}
}

void CKeyMgr::Release(void)
{
}

bool CKeyMgr::IsKeyUp(KEY::E_KEY _eKey)
{
	if (IS_ON_FLAG(m_dwOldKeyState, cdwKeyMask[_eKey]) && !IS_ON_FLAG(m_dwCurrentKeyState, cdwKeyMask[_eKey])) return true;
	return false;
}

bool CKeyMgr::IsKeyDown(KEY::E_KEY _eKey)
{
	if (!IS_ON_FLAG(m_dwOldKeyState, cdwKeyMask[_eKey]) && IS_ON_FLAG(m_dwCurrentKeyState, cdwKeyMask[_eKey])) return true;
	return false;
}

bool CKeyMgr::IsKeyPressing(KEY::E_KEY _eKey)
{
	if (IS_ON_FLAG(m_dwOldKeyState, cdwKeyMask[_eKey]) && IS_ON_FLAG(m_dwCurrentKeyState, cdwKeyMask[_eKey])) return true;
	return false;
}

bool CKeyMgr::IsKeyNone(KEY::E_KEY _eKey)
{
	if (!IS_ON_FLAG(m_dwOldKeyState, cdwKeyMask[_eKey]) && !IS_ON_FLAG(m_dwCurrentKeyState, cdwKeyMask[_eKey])) return true;
	return false;
}