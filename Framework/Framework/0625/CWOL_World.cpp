#include "stdafx.h"
#include "CWOL_World.h"
#include "CUI_Cursor.h"
#include "CMapLoader.h"
#include "CCamera2D.h"
#include "CTimer.h"
#include "CPlayerWOL.h"
#include "CBitmapMgr.h"
#include "CTitleScene.h"
//#include "CUI_DamageText.h"	//테스트용
#include "CFireDragon.h"		//테스트용


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
}

void CWOL_World::Update(void)
{
	float fDeltaTime = GetTimer()->GetElapsedTimePerFrame();

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_P)) {
		m_plistUsedPlayerSkills.emplace_back(new CFireDragon(
			*this,
			m_pPlayer->GetX(), m_pPlayer->GetY(),
			m_pPlayer->GetToX(), m_pPlayer->GetToY(),
			TO_PLAYER_WOL(m_pPlayer)->GetLastAttackState()
		));
	}

	m_pCursor->Update(fDeltaTime);

	GetSceneManager()->Update(fDeltaTime);
	m_pCamera->Update(fDeltaTime);

	for (auto* pObj : m_plistParticles) {
		pObj->Update(fDeltaTime);
	}

	for (auto& pObj : m_plistUsedPlayerSkills) {
		pObj->Update(fDeltaTime);
	}

	for (auto& pObj : m_plistUsedMonsterSkills) {
		pObj->Update(fDeltaTime);
	}

	for (auto& pObj : m_plistUIs) {
		pObj->Update(fDeltaTime);
	}


	m_fElapsedTime += fDeltaTime;
	m_iFrameCount += 1;
	//m_pMap->Update(fDeltaTime);
}

void CWOL_World::LateUpdate(void)
{
	//m_pMap->LateUpdate();
	GetSceneManager()->LateUpdate();
	m_pCamera->LateUpdate();

	CollectGarbageObjects(m_plistUsedPlayerSkills);		// 무효화된 플레이어 스킬 제거
	CollectGarbageObjects(m_plistUsedMonsterSkills);	// 무효화된 몬스터 스킬 제거
	CollectGarbageObjects(m_plistUIs);					// 무효화된 UI를 제거
	CollectGarbageObjects(m_plistParticles);
}

void CWOL_World::Render(void)
{
	ClearWindow();

	GetSceneManager()->Render(GetBackbufferDC(), m_pCamera);

	for (auto* pObj : m_plistParticles) {
		pObj->Render(GetBackbufferDC(), m_pCamera);
	}
	for (auto& pObj : m_plistUsedPlayerSkills) {
		pObj->Render(GetBackbufferDC(), m_pCamera);
	}
	for (auto& pObj : m_plistUsedMonsterSkills) {
		pObj->Render(GetBackbufferDC(), m_pCamera);
	}
	for (auto& pObj : m_plistUIs) {
		pObj->Render(GetBackbufferDC(), m_pCamera);
	}

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
	DeleteListSafe(m_plistUsedPlayerSkills);
	DeleteListSafe(m_plistUsedMonsterSkills);
	DeleteListSafe(m_plistUIs);
	DeleteListSafe(m_plistParticles);
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

	// 타격 이펙트
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/HIT_EFFECT.bmp"), TEXT("HIT_EFFECT"));

	// 동전 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/COIN.bmp"), TEXT("COIN"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_MONEY.bmp"), TEXT("UI_MONEY"));

	// 스킬바 UI
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_SKILLBAR.bmp"), TEXT("UI_SKILLBAR"));
	// 스킬바 마법 아이콘
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ICE_BLAST_SKILLBAR.bmp"), TEXT("ICE_BLAST_SKILLBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/FIRE_DRAGON_SKILLBAR.bmp"), TEXT("FIRE_DRAGON_SKILLBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/GAIA_ARMOR_SKILLBAR.bmp"), TEXT("GAIA_ARMOR_SKILLBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ICE_CRYSTAL_SKILLBAR.bmp"), TEXT("ICE_CRYSTAL_SKILLBAR"));

	// 플레이어 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/BACK_COMPLETE.bmp"), TEXT("PLAYER_BACK"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/FRONT_COMPLETE.bmp"), TEXT("PLAYER_FRONT"));				// 비고. 죽음 장면까지 포함.
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/LEFT_COMPLETE.bmp"), TEXT("PLAYER_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/RIGHT_COMPLETE.bmp"), TEXT("PLAYER_RIGHT"));
	// 플레이어 공격 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/PLAYER_NORMAL_ATTACK.bmp"), TEXT("PLAYER_NORMAL_ATTACK"));
	// 플레이어 HP 
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_PLAYERBAR.bmp"), TEXT("UI_PLAYERBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_HPBAR.bmp"), TEXT("UI_HPBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_MANABAR.bmp"), TEXT("UI_MANABAR"));
	// 스킬 파이어 드래곤
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SKILL_FIREDRAGON_COM.bmp"), TEXT("SKILL_FIREDRAGON"));
	// 스킬 아이스 크리스탈
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SKILL_ICE_CRYSTAL.bmp"), TEXT("SKILL_ICE_CRYSTAL"));
	// 파이어 파티클
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/FIRE_PARTICLE.bmp"), TEXT("FIRE_PARTICLE"));

	// 스워드맨 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SWORDMAN_LEFT.bmp"), TEXT("SWORDMAN_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SWORDMAN_RIGHT.bmp"), TEXT("SWORDMAN_RIGHT"));
	// 스워드맨 공격 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SWORDMAN_ATTACK.bmp"), TEXT("SWORDMAN_ATTACK"));

	// 아쳐 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ARCHER_LEFT.bmp"), TEXT("ARCHER_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ARCHER_RIGHT.bmp"), TEXT("ARCHER_RIGHT"));
	// 아쳐 활/화살 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ARCHER_BOW_LEFT.bmp"), TEXT("ARCHER_BOW_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ARCHER_BOW_RIGHT.bmp"), TEXT("ARCHER_BOW_RIGHT"));

	// 위자드 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WIZARD_LEFT.bmp"), TEXT("WIZARD_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WIZARD_RIGHT.bmp"), TEXT("WIZARD_RIGHT"));
	// 위자드 파이어 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WIZARD_FIRE.bmp"), TEXT("WIZARD_FIRE"));


	// 위자드볼 렌더링
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WIZARD_BALL.bmp"), TEXT("WIZARD_BALL"));
}
