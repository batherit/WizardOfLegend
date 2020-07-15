#include "stdafx.h"
#include "CWOL_World.h"
#include "CMapLoader.h"
#include "CCamera2D.h"
#include "CTimer.h"
#include "CBitmapMgr.h"
#include "CTitleScene.h"


CWOL_World::CWOL_World()
	:
	m_pCamera(nullptr),
	m_pMap(nullptr)
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
	GetSceneManager()->SetNextScene(new CTitleScene(*this));
	m_pCamera = new CCamera2D(*this, nullptr, WINCX >> 1, WINCY >> 1);
	//m_pMap = new CMapLoader(*this, "../MapDatas/Maps/0/Objs.txt");
}

void CWOL_World::Update(void)
{
	float fDeltaTime = GetTimer()->GetElapsedTimePerFrame();
	GetSceneManager()->Update(fDeltaTime);
	//m_pMap->Update(fDeltaTime);
}

void CWOL_World::LateUpdate(void)
{
	//m_pMap->LateUpdate();
	GetSceneManager()->LateUpdate();
}

void CWOL_World::Render(void)
{
	ClearWindow();

	GetSceneManager()->Render(GetBackbufferDC(), m_pCamera);
	//m_pMap->Render(GetBackbufferDC(), m_pCamera);

	RenderWindow();
}

void CWOL_World::Release(void)
{
	DeleteSafe(m_pCamera);
	DeleteSafe(m_pMap);
	CBitmapMgr::Destroy_Instance();
	CKeyMgr::DestroyInstance();
}

void CWOL_World::LoadResources(void)
{
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/READY_MENU.bmp"), TEXT("READY_MENU"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_TILE_DUNGEON.bmp"), TEXT("WOL_TILE_DUNGEON"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_TILE_HOMETOWN.bmp"), TEXT("WOL_TILE_HOMETOWN"));
}
