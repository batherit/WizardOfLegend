#include "stdafx.h"
#include "CTitleScene.h"
#include "CBitmapMgr.h"
#include "CBitmapObj.h"
#include "CPlayScene.h"
#include "UI_Button.h"


CTitleScene::CTitleScene(CGameWorld& _rGameWorld)
	:
	CScene(_rGameWorld)
{
	m_eType = TYPE_READY;

	m_hDCTitle[TYPE_READY] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("READY_MENU"));
	m_hDCTitle[TYPE_QUIT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("QUIT_MENU"));
	m_hDCTitle[TYPE_START] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("MAIN_MENU"));

	const CBitmapObj* pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(TEXT("READY_MENU"));
	m_iWidth[TYPE_READY] = pBitmapObj->GetWitdh();
	m_iHeight[TYPE_READY] = pBitmapObj->GetHeight();
	pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(TEXT("QUIT_MENU"));
	m_iWidth[TYPE_QUIT] = pBitmapObj->GetWitdh();
	m_iHeight[TYPE_QUIT] = pBitmapObj->GetHeight();
	pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(TEXT("MAIN_MENU"));
	m_iWidth[TYPE_START] = pBitmapObj->GetWitdh();
	m_iHeight[TYPE_START] = pBitmapObj->GetHeight();

	//m_pButtons[TYPE_READY] = new CUI_Button<CTitleScene>(_rGameWorld, WINCX >> 1, WINCY >> 1, WINCX, WINCY, TEXT(""), this,;
	
	CSoundMgr::Get_Instance()->PlayBGM(TEXT("MAIN_MENU_BGM.mp3"));
}


CTitleScene::~CTitleScene()
{
	//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	Release();
	
}

void CTitleScene::ResetScene(void)
{
	// 타이틀씬은 딱히 Reset시킬만한 게 없음...
}

int CTitleScene::Update(float _fDeltaTime)
{
	switch (m_eType) {
	case TYPE_READY:
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
			CSoundMgr::Get_Instance()->PlaySound(TEXT("CLICK_MENU.mp3"), CSoundMgr::UI);
			m_eType = TYPE_START;
			//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
			//m_rGameWorld.GetSceneManager()->SetNextScene(new CPlayScene(m_rGameWorld, "../MapDatas/Maps/0/Game_Map.txt"));
		}
		break;
	case TYPE_START:

		break;
	case TYPE_QUIT:
		break;
	}

	/*if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
		CSoundMgr::Get_Instance()->PlaySound(TEXT("CLICK_MENU.mp3"),CSoundMgr::UI);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
		m_rGameWorld.GetSceneManager()->SetNextScene(new CPlayScene(m_rGameWorld, "../MapDatas/Maps/0/Game_Map.txt"));
	}*/

	return 0;
}

void CTitleScene::LateUpdate(void)
{
}

void CTitleScene::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	StretchBlt(_hdc, 0, 0, WINCX, WINCY, m_hDCTitle[m_eType], 0, 0, m_iWidth[m_eType], m_iHeight[m_eType], SRCCOPY);
}

void CTitleScene::Release(void)
{
	for (int i = 0; i < TYPE_END; i++) {
		DeleteSafe(m_pButtons[i]);
	}
}
