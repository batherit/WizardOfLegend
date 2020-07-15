#include "stdafx.h"
#include "CPlayScene.h"
#include "CMapLoader.h"


CPlayScene::CPlayScene(CGameWorld& _rGameWorld, const char* _szMapDirectory)
	:
	CScene(_rGameWorld),
	m_pMapLoader(nullptr)
{
	m_pMapLoader = new CMapLoader(_rGameWorld, _szMapDirectory);
}


CPlayScene::~CPlayScene()
{
	Release();
}

void CPlayScene::Ready(void)
{
	m_pMapLoader->Ready();
}

int CPlayScene::Update(float _fDeltaTime)
{
	return 0;
}

void CPlayScene::LateUpdate(void)
{
}

void CPlayScene::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	m_pMapLoader->Render(_hdc, _pCamera);
}

void CPlayScene::Release(void)
{
	DeleteSafe(m_pMapLoader);
}
