#include "stdafx.h"
#include "CGameWorld.h"
#include "CTimer.h"
#include "CSceneMgr.h"
#include "CSpace.h"


CGameWorld::CGameWorld()
	:
	m_pTimer(new CTimer(TIMER::TIMER_TYPE_WINDOWS)),
	m_pSceneManager(new CSceneMgr(*this)),
	// ViewSpace는 렌더 컬링할때 쓰일 수 있다.
	m_pViewSpace(new CSpace(*this, WINCX >> 1, WINCY >> 1, static_cast<int>(WINCX), static_cast<int>(WINCY)))
{
	if (m_pTimer) m_pTimer->Reset();
	m_hDC = GetDC(g_hWND);

	//Double Buffering
	m_hBackbuffer = CreateCompatibleBitmap(m_hDC, WINCX, WINCY);
	m_hBackbufferDC = CreateCompatibleDC(m_hDC);
	SelectObject(m_hBackbufferDC, m_hBackbuffer);
}


CGameWorld::~CGameWorld()
{
	DeleteDC(m_hBackbufferDC);
	DeleteObject(m_hBackbuffer);
	ReleaseDC(g_hWND, m_hDC);
	DeleteSafe(m_pTimer);
	DeleteSafe(m_pSceneManager);
	DeleteSafe(m_pViewSpace);
}

bool CGameWorld::ConfirmValidScene(void)
{
	return m_pSceneManager->ConfirmValidScene();
}

void CGameWorld::RunTick(void)
{
	if (m_pTimer) m_pTimer->RunTick();
}

void CGameWorld::ClearWindow(void)
{
	RECT rc{ 0,0, WINCX, WINCY };
	FillRect(m_hBackbufferDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
}

void CGameWorld::RenderWindow(void)
{
	BitBlt(GetHDC(), 0, 0, WINCX, WINCY, GetBackbufferDC(), 0, 0, SRCCOPY);
}
