#pragma once
#include "CScene.h"

class CMapLoader;

class CPlayScene :
	public CScene
{
public:
	CPlayScene(CGameWorld& _rGameWorld, const char* _szMapDirectory);
	virtual ~CPlayScene();

	// CScene��(��) ���� ��ӵ�
	virtual void ResetScene(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void Release(void) override;

private:
	CMapLoader* m_pMapLoader = nullptr;
	CObj* m_pPlayer = nullptr;
	vector<CObj*> m_vecObjsToRender;
	const char* m_szMapDirectory = nullptr;
};

