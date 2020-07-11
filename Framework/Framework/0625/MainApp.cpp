#include "stdafx.h"
#include "MainApp.h"
#include "Player.h"
#include "Monster.h"
#include "CSpace.h"
#include "CTimer.h"

CMainApp::CMainApp()
	:
	m_pPlayer(nullptr),
	m_pViewSpace(nullptr)
{
}


CMainApp::~CMainApp()
{
	Release();
}

void CMainApp::Ready(void)
{
	// 얻어왔으면 반드시 지워주기!
	//m_hDC = GetDC(g_hWND);

	srand((time_t)time(nullptr));

	if (!m_pPlayer) {
		m_pPlayer = new CPlayer(*this, WINCX >> 1, WINCY >> 1);
		m_pPlayer->Ready();
	}

	m_pViewSpace = new CSpace(*this, WINCX >> 1, WINCY >> 1, static_cast<int>(WINCX / 1.2f), static_cast<int>(WINCY / 1.2f), &m_listBullets);

	/*CObj* pMonster = nullptr;
	for (int i = 0; i < 4; i++) {
		pMonster = new CMonster(
			*this,
			GetNumberMinBetweenMax(m_pViewSpace->GetLeft(), m_pViewSpace->GetRight()),
			GetNumberMinBetweenMax(m_pViewSpace->GetTop(), m_pViewSpace->GetBottom()));
		m_listMonsters.emplace_back(pMonster);
	}*/

	m_vecLines.emplace_back(make_pair(0, 500));
	m_vecLines.emplace_back(make_pair(200, 500));
	m_vecLines.emplace_back(make_pair(400, 300));
	m_vecLines.emplace_back(make_pair(600, 500));
	m_vecLines.emplace_back(make_pair(800, 400));
}

void CMainApp::Update(void)
{
	float fDeltaTime = GetTimer()->GetElapsedTimePerFrame();
	for (auto& pBullet : m_listBullets) { pBullet->Update(fDeltaTime); }
	for (auto& pMonster : m_listMonsters) { pMonster->Update(fDeltaTime); }
	m_pPlayer->Update(fDeltaTime);
}

void CMainApp::LateUpdate(void)
{
	for (auto& pBullet : m_listBullets) { DO_IF_IS_VALID_OBJ(pBullet) { pBullet->LateUpdate(); } }
	for (auto& pMonster : m_listMonsters) { DO_IF_IS_VALID_OBJ(pMonster) { pMonster->LateUpdate(); } }
	DO_IF_IS_VALID_OBJ(m_pPlayer) { m_pPlayer->LateUpdate(); }


	CollectGarbageObjects(m_listBullets);
	CollectGarbageObjects(m_listMonsters);
	DO_IF_IS_NOT_VALID_OBJ(m_pPlayer) { DeleteSafe(m_pPlayer); }
}

void CMainApp::Render(void)
{
	ClearWindow();
	//InvalidateRect(g_hWND, nullptr, true);

	m_pViewSpace->Render(GetBackbufferDC());
	for (auto& pMonster : m_listMonsters) { pMonster->Render(GetBackbufferDC()); }
	for (auto& pBullet : m_listBullets) { pBullet->Render(GetBackbufferDC()); }
	m_pPlayer->Render(GetBackbufferDC());
	RenderLine(GetBackbufferDC());

	BitBlt(GetHDC(), 0, 0, WINCX, WINCY, GetBackbufferDC(), 0, 0, SRCCOPY);
}

void CMainApp::Release(void)
{
	DeleteSafe(m_pPlayer);
	DeleteSafe(m_pViewSpace);
	DeleteListSafe(m_listBullets);
	DeleteListSafe(m_listMonsters);

	//ReleaseDC(g_hWND, m_hDC);
}

void CMainApp::RenderLine(const HDC& _hdc)
{
	pair<float, float> pairFrom;
	pair<float, float> pairTo;

	for (int i = 0; i < m_vecLines.size() - 1; i++) {
		pairFrom = m_vecLines[i];
		pairTo = m_vecLines[i + 1];
		MoveToEx(_hdc, pairFrom.first, pairFrom.second, nullptr);
		LineTo(_hdc, pairTo.first, pairTo.second);
	}
}

//inline void CMainApp::ClearWindow(void)
//{
//	//IntersectRect()
//	Rectangle(GetHDC(), 0, 0, WINCX, WINCY);
//}
