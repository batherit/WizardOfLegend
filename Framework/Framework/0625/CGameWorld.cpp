#include "stdafx.h"
#include "CGameWorld.h"
#include "CTimer.h"
#include "CSceneMgr.h"


CGameWorld::CGameWorld()
	:
	m_pTimer(new CTimer(TIMER::TIMER_TYPE_WINDOWS)),
	m_pSceneManager(new CSceneMgr(*this))
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
	FillRect(m_hBackbufferDC, &rc, (HBRUSH)GetStockObject(GRAY_BRUSH));
}

void CGameWorld::RenderWindow(void)
{
	BitBlt(GetHDC(), 0, 0, WINCX, WINCY, GetBackbufferDC(), 0, 0, SRCCOPY);
}
