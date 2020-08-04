#include "stdafx.h"
#include "CGameWorld.h"
#include "CTimer.h"
#include "CSceneMgr.h"
#include "CSpace.h"
#include "CBitmapMgr.h"


CGameWorld::CGameWorld()
	:
	m_pTimer(new CTimer(TIMER::TIMER_TYPE_WINDOWS)),
	m_pSceneManager(new CSceneMgr(*this)),
	// ViewSpace는 렌더 컬링할때 쓰일 수 있다.
	m_pViewSpace(new CSpace(*this, WINCX >> 1, WINCY >> 1, static_cast<int>(WINCX), static_cast<int>(WINCY)))
{
	srand((unsigned int)time(nullptr));
	if (m_pTimer) m_pTimer->Reset();
	m_hDC = GetDC(g_hWND);

	// 더블 버퍼링을 위한 준비
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/BACK_BUFFER.bmp"), TEXT("BACK_BUFFER"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/BACK_BUFFER.bmp"), TEXT("BACK_CLEAR"));

	m_hBackbufferDC = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BACK_BUFFER"));
	m_hBackbufferClearDC = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BACK_CLEAR"));
}


CGameWorld::~CGameWorld()
{
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
	BitBlt(m_hBackbufferDC, 0, 0, WINCX, WINCY, m_hBackbufferClearDC, 0, 0, SRCCOPY);
}

void CGameWorld::RenderWindow(void)
{
	BitBlt(m_hDC, 0, 0, WINCX, WINCY, m_hBackbufferDC, 0, 0, SRCCOPY);
}

