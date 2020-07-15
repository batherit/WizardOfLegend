#include "stdafx.h"
#include "CWOL_World.h"
#include "CMapLoader.h"
#include "CCamera2D.h"
#include "CTimer.h"


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
	m_pCamera = new CCamera2D(*this, nullptr, WINCX >> 1, WINCY >> 1);
	m_pMap = new CMapLoader(*this, "../MapDatas/Maps/0/Objs.txt");
}

void CWOL_World::Update(void)
{
	float fDeltaTime = GetTimer()->GetElapsedTimePerFrame();
	m_pMap->Update(fDeltaTime);
}

void CWOL_World::LateUpdate(void)
{
	m_pMap->LateUpdate();
}

void CWOL_World::Render(void)
{
	ClearWindow();

	m_pMap->Render(GetBackbufferDC(), m_pCamera);

	RenderWindow();
}

void CWOL_World::Release(void)
{
	DeleteSafe(m_pCamera);
	DeleteSafe(m_pMap);
}
