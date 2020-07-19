#include "stdafx.h"
#include "CWOL_World.h"
#include "CUI_Cursor.h"
#include "CMapLoader.h"
#include "CCamera2D.h"
#include "CTimer.h"
#include "CPlayerWOL.h"
#include "CBitmapMgr.h"
#include "CTitleScene.h"


CWOL_World::CWOL_World()
{
}


CWOL_World::~CWOL_World()
{
	Release();
}

LRESULT CWOL_World::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

void CWOL_World::Ready(void)
{
	LoadResources();

	m_pCursor = new CUI_Cursor(*this, CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("UI_MOUSE_CURSOR")));
	GetSceneManager()->SetNextScene(new CTitleScene(*this));
	m_pPlayer = new CPlayerWOL(*this, 0.f, 0.f);
	m_pCamera = new CCamera2D(*this, m_pPlayer, WINCX >> 1, WINCY >> 1);
	//m_pMap = new CMapLoader(*this, "../MapDatas/Maps/0/Objs.txt");
}

void CWOL_World::Update(void)
{
	float fDeltaTime = GetTimer()->GetElapsedTimePerFrame();
	m_pCursor->Update(fDeltaTime);

	GetSceneManager()->Update(fDeltaTime);
	m_pCamera->Update(fDeltaTime);

	m_fElapsedTime += fDeltaTime;
	m_iFrameCount += 1;
	//m_pMap->Update(fDeltaTime);
}

void CWOL_World::LateUpdate(void)
{
	//m_pMap->LateUpdate();
	GetSceneManager()->LateUpdate();
	m_pCamera->LateUpdate();
}

void CWOL_World::Render(void)
{
	ClearWindow();

	GetSceneManager()->Render(GetBackbufferDC(), m_pCamera);

	if (m_fElapsedTime >= 1.f) {
		TCHAR szBuf[64];
		
		swprintf_s(szBuf, TEXT("Wizard Of Legend (FPS : %d, RenderCnt : %d)"), m_iFrameCount, g_iRenderCount);
		SetWindowText(g_hWND, szBuf);
		//TextOut(GetBackbufferDC(), 0, 0, szBuf, lstrlen(szBuf));
		m_fElapsedTime = 0.f;
		m_iFrameCount = 0;
	}

	m_pCursor->Render(GetBackbufferDC(), m_pCamera);

	RenderWindow();
}

void CWOL_World::Release(void)
{
	DeleteSafe(m_pCursor);
	DeleteSafe(m_pCamera);
	CBitmapMgr::DestroyInstance();
	CKeyMgr::DestroyInstance();
}

void CWOL_World::LoadResources(void)
{
	// 마우스 커서 세팅
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_MOUSE_CURSOR.bmp"), TEXT("UI_MOUSE_CURSOR"));

	// 타이틀 화면
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/READY_MENU.bmp"), TEXT("READY_MENU"));					// 타이틀 화면

	// 맵 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_TILE_DUNGEON.bmp"), TEXT("WOL_TILE_DUNGEON"));		// 던젼용 아틀라스
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_TILE_HOMETOWN.bmp"), TEXT("WOL_TILE_HOMETOWN"));		// 마을용 아틀라스
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_OBJECT.bmp"), TEXT("WOL_OBJECT"));					// 오브젝트용 아틀라스
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/PRISON_HOR.bmp"), TEXT("PRISON_HOR"));					// 가로 형태의 철창
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/PRISON_VER.bmp"), TEXT("PRISON_VER"));					// 세로 형태의 철창

	// 스폰 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_PLAYER.bmp"), TEXT("SPAWN_PLAYER"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_CARD_ARCHER.bmp"), TEXT("SPAWN_CARD_ARCHER"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_CARD_SWORDMAN.bmp"), TEXT("SPAWN_CARD_SWORDMAN"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_CARD_WIZARD.bmp"), TEXT("SPAWN_CARD_WIZARD"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_CARD_WIZARDBALL.bmp"), TEXT("SPAWN_CARD_WIZARDBALL"));

	// 플레이어 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/BACK_COMPLETE.bmp"), TEXT("PLAYER_BACK"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/FRONT_COMPLETE.bmp"), TEXT("PLAYER_FRONT"));				// 비고. 죽음 장면까지 포함.
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/LEFT_COMPLETE.bmp"), TEXT("PLAYER_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/RIGHT_COMPLETE.bmp"), TEXT("PLAYER_RIGHT"));
}
