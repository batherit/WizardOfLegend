#pragma once
#include "CScene.h"

class CMapLoader;
class CSpawnerGenerator;

class CPlayScene :
	public CScene
{
public:
	CPlayScene(CGameWorld& _rGameWorld, const char* _szMapDirectory);
	virtual ~CPlayScene();

	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void Release(void) override;

private:
	CMapLoader* m_pMapLoader = nullptr;
	CObj* m_pPlayer = nullptr;
	list<CObj*> m_listMonsters;
	list<CSpawnerGenerator*> m_listSpawnerGenerators;
	list<CObj*> m_listSpawners;
	vector<CObj*> m_vecObjsToRender;
	const char* m_szMapDirectory = nullptr;
};

