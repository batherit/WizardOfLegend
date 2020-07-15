#include "stdafx.h"
#include "CTitleScene.h"
#include "CBitmapMgr.h"
#include "CBitmapObj.h"
#include "CPlayScene.h"


CTitleScene::CTitleScene(CGameWorld& _rGameWorld)
	:
	CScene(_rGameWorld)
{
}


CTitleScene::~CTitleScene()
{
}

void CTitleScene::Ready(void)
{
}

int CTitleScene::Update(float _fDeltaTime)
{
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
		m_rGameWorld.GetSceneManager()->SetNextScene(new CPlayScene(m_rGameWorld, "../MapDatas/Maps/0/Objs.txt"));
	}

	return 0;
}

void CTitleScene::LateUpdate(void)
{
}

void CTitleScene::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	HDC memdc = CBitmapMgr::GetInstance()->FindBitmapMemDC(TEXT("READY_MENU"));
	const CBitmapObj* pBitmapObj = CBitmapMgr::GetInstance()->GetBitmapObj(TEXT("READY_MENU"));

	StretchBlt(_hdc, 0, 0, WINCX, WINCY, memdc, 0, 0, pBitmapObj->GetWitdh(), pBitmapObj->GetHeight(), SRCCOPY);
}

void CTitleScene::Release(void)
{
}
