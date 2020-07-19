#include "stdafx.h"
#include "CTitleScene.h"
#include "CBitmapMgr.h"
#include "CBitmapObj.h"
#include "CPlayScene.h"


CTitleScene::CTitleScene(CGameWorld& _rGameWorld)
	:
	CScene(_rGameWorld)
{
	m_hMemdc = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("READY_MENU"));
	const CBitmapObj* pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(TEXT("READY_MENU"));
	m_iWidth = pBitmapObj->GetWitdh();
	m_iHeight = pBitmapObj->GetHeight();
}


CTitleScene::~CTitleScene()
{
}

void CTitleScene::ResetScene(void)
{
}

int CTitleScene::Update(float _fDeltaTime)
{
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
		m_rGameWorld.GetSceneManager()->SetNextScene(new CPlayScene(m_rGameWorld, "../MapDatas/Maps/0/Game_Map.txt"));
	}

	return 0;
}

void CTitleScene::LateUpdate(void)
{
}

void CTitleScene::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	StretchBlt(_hdc, 0, 0, WINCX, WINCY, m_hMemdc, 0, 0, m_iWidth, m_iHeight, SRCCOPY);
}

void CTitleScene::Release(void)
{
}
