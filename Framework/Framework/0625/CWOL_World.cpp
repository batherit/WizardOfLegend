#include "stdafx.h"
#include "CWOL_World.h"
#include "CUI_Cursor.h"
#include "CMapLoader.h"
#include "CCamera2D.h"
#include "CTimer.h"
#include "CPlayerWOL.h"
#include "CBitmapMgr.h"
#include "CTitleScene.h"
//#include "CUI_DamageText.h"	//�׽�Ʈ��
#include "CFireDragon.h"		//�׽�Ʈ��


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

	if (m_bTimeFactorChange) {
		if ((m_fTimeFactorChangeElapsedTime += fDeltaTime) > m_fTimeFactorChangeTime) {
			m_fTimeFactor = 1;
			m_bTimeFactorChange = false;
		}
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_P)) {
		m_plistUsedPlayerSkills.emplace_back(new CFireDragon(
			*this,
			m_pPlayer->GetX(), m_pPlayer->GetY(),
			m_pPlayer->GetToX(), m_pPlayer->GetToY(),
			TO_PLAYER_WOL(m_pPlayer)->GetLastAttackState()
		));
	}

	m_pCursor->Update(fDeltaTime);

	GetSceneManager()->Update(fDeltaTime * m_fTimeFactor);
	m_pCamera->Update(fDeltaTime * m_fTimeFactor);

	for (auto* pObj : m_plistParticles) {
		pObj->Update(fDeltaTime* m_fTimeFactor);
	}

	for (auto& pObj : m_plistUsedPlayerSkills) {
		pObj->Update(fDeltaTime* m_fTimeFactor);
	}

	for (auto& pObj : m_plistUsedMonsterSkills) {
		pObj->Update(fDeltaTime* m_fTimeFactor);
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

	CollectGarbageObjects(m_plistUsedPlayerSkills);		// ��ȿȭ�� �÷��̾� ��ų ����
	CollectGarbageObjects(m_plistUsedMonsterSkills);	// ��ȿȭ�� ���� ��ų ����
	CollectGarbageObjects(m_plistUIs);					// ��ȿȭ�� UI�� ����
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

void CWOL_World::TemporarilyAdjustWorldTimeSpeed(float _fTimeFactorChangeTime, float _fTimeFactor)
{
	m_bTimeFactorChange = true;
	m_fTimeFactor = _fTimeFactor;
	m_fTimeFactorChangeTime = _fTimeFactorChangeTime;
	m_fTimeFactorChangeElapsedTime = 0.f;
}

void CWOL_World::LoadResources(void)
{
	// ���콺 Ŀ�� ����
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_MOUSE_CURSOR.bmp"), TEXT("UI_MOUSE_CURSOR"));

	// Ÿ��Ʋ ȭ��
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/READY_MENU.bmp"), TEXT("READY_MENU"));					// Ÿ��Ʋ ȭ��

	// �� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_TILE_DUNGEON.bmp"), TEXT("WOL_TILE_DUNGEON"));		// ������ ��Ʋ��
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_TILE_HOMETOWN.bmp"), TEXT("WOL_TILE_HOMETOWN"));		// ������ ��Ʋ��
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WOL_OBJECT.bmp"), TEXT("WOL_OBJECT"));					// ������Ʈ�� ��Ʋ��
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/PRISON_HOR.bmp"), TEXT("PRISON_HOR"));					// ���� ������ öâ
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/PRISON_VER.bmp"), TEXT("PRISON_VER"));					// ���� ������ öâ

	// ���� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_PLAYER.bmp"), TEXT("SPAWN_PLAYER"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_CARD_ARCHER.bmp"), TEXT("SPAWN_CARD_ARCHER"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_CARD_SWORDMAN.bmp"), TEXT("SPAWN_CARD_SWORDMAN"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_CARD_WIZARD.bmp"), TEXT("SPAWN_CARD_WIZARD"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SPAWN_CARD_WIZARDBALL.bmp"), TEXT("SPAWN_CARD_WIZARDBALL"));

	// Ÿ�� ����Ʈ
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/HIT_EFFECT.bmp"), TEXT("HIT_EFFECT"));

	// ��ų ��Ÿ��
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/COOLTIME_SHADOW.bmp"), TEXT("COOLTIME_SHADOW"));

	// ���� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/COIN.bmp"), TEXT("COIN"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_MONEY.bmp"), TEXT("UI_MONEY"));

	// ��ų�� UI
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_SKILLBAR.bmp"), TEXT("UI_SKILLBAR"));

	// �̴ϸ� UI
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_PLAYER_MINIMAP.bmp"), TEXT("UI_PLAYER_MINIMAP"));

	// ��ų�� ���� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/DASH_SKILLBAR.bmp"), TEXT("DASH_SKILLBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/NORMAL_ATTACK_SKILLBAR.bmp"), TEXT("NORMAL_ATTACK_SKILLBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ICE_BLAST_SKILLBAR.bmp"), TEXT("ICE_BLAST_SKILLBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/FIRE_DRAGON_SKILLBAR.bmp"), TEXT("FIRE_DRAGON_SKILLBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/GAIA_ARMOR_SKILLBAR.bmp"), TEXT("GAIA_ARMOR_SKILLBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ICE_CRYSTAL_SKILLBAR.bmp"), TEXT("ICE_CRYSTAL_SKILLBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ICE_BLAST_SKILLBAR.bmp"), TEXT("ICE_BLAST_SKILLBAR"));

	// ��ųŰ ���ε�
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/INVENTORY.bmp"), TEXT("INVENTORY"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/NORMAL_ATTACK_EX.bmp"), TEXT("NORMAL_ATTACK_EX"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/DASH_EX.bmp"), TEXT("DASH_EX"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/FIRE_DRAGON_EX.bmp"), TEXT("FIRE_DRAGON_EX"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ICE_CRYSTAL_EX.bmp"), TEXT("ICE_CRYSTAL_EX"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/GAIA_ARMOR_EX.bmp"), TEXT("GAIA_ARMOR_EX"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ICE_BLAST_EX.bmp"), TEXT("ICE_BLAST_EX"));

	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/NORMAL_ATTACK_ICON.bmp"), TEXT("NORMAL_ATTACK_ICON"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/DASH_ICON.bmp"), TEXT("DASH_ICON"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/FIRE_DRAGON_ICON.bmp"), TEXT("FIRE_DRAGON_ICON"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ICE_CRYSTAL_ICON.bmp"), TEXT("ICE_CRYSTAL_ICON"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/GAIA_ARMOR_ICON.bmp"), TEXT("GAIA_ARMOR_ICON"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ICE_BLAST_ICON.bmp"), TEXT("ICE_BLAST_ICON"));


	// �÷��̾� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/BACK_COMPLETE.bmp"), TEXT("PLAYER_BACK"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/FRONT_COMPLETE.bmp"), TEXT("PLAYER_FRONT"));				// ���. ���� ������ ����.
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/LEFT_COMPLETE.bmp"), TEXT("PLAYER_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/RIGHT_COMPLETE.bmp"), TEXT("PLAYER_RIGHT"));
	// �÷��̾� ���� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/PLAYER_NORMAL_ATTACK.bmp"), TEXT("PLAYER_NORMAL_ATTACK"));
	// �÷��̾� HP 
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_PLAYERBAR.bmp"), TEXT("UI_PLAYERBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_HPBAR.bmp"), TEXT("UI_HPBAR"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/UI_MANABAR.bmp"), TEXT("UI_MANABAR"));
	// ��ų ���̾� �巡��
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SKILL_FIREDRAGON_COM.bmp"), TEXT("SKILL_FIREDRAGON"));
	// ��ų ���̽� ũ����Ż
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SKILL_ICE_CRYSTAL.bmp"), TEXT("SKILL_ICE_CRYSTAL"));
	// ���̾� ��ƼŬ
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/FIRE_PARTICLE.bmp"), TEXT("FIRE_PARTICLE"));

	// ������� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SWORDMAN_LEFT.bmp"), TEXT("SWORDMAN_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SWORDMAN_RIGHT.bmp"), TEXT("SWORDMAN_RIGHT"));
	// ������� ���� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/SWORDMAN_ATTACK.bmp"), TEXT("SWORDMAN_ATTACK"));

	// ���� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ARCHER_LEFT.bmp"), TEXT("ARCHER_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ARCHER_RIGHT.bmp"), TEXT("ARCHER_RIGHT"));
	// ���� Ȱ/ȭ�� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ARCHER_BOW_LEFT.bmp"), TEXT("ARCHER_BOW_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/ARCHER_BOW_RIGHT.bmp"), TEXT("ARCHER_BOW_RIGHT"));

	// ���ڵ� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WIZARD_LEFT.bmp"), TEXT("WIZARD_LEFT"));
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WIZARD_RIGHT.bmp"), TEXT("WIZARD_RIGHT"));
	// ���ڵ� ���̾� ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WIZARD_FIRE.bmp"), TEXT("WIZARD_FIRE"));


	// ���ڵ庼 ������
	CBitmapMgr::GetInstance()->InsertBitmap(TEXT("../Textures/WIZARD_BALL.bmp"), TEXT("WIZARD_BALL"));
}
