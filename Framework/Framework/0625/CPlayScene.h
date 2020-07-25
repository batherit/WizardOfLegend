#pragma once
#include "CScene.h"

class CMapLoader;
class CSpawnerGenerator;
class CUI_PlayerBar;
class CUI_SkillBar;
class CUI_SkillKeyBinding;
class CUI_Money;
class CUI_Minimap;

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

	virtual list<CObj*>* GetHitEffects() override { return &m_listHitEffects; } 
	virtual list<CObj*>* GetSpawners() override { return &m_listSpawners; }
	virtual list<CObj*>* GetMonsters() override { return &m_listMonsters; } 

private:
	CMapLoader* m_pMapLoader = nullptr;
	CObj* m_pPlayer = nullptr;
	CUI_PlayerBar* m_pPlayerBarUI = nullptr;
	CUI_SkillBar* m_pSkillBarUI = nullptr;
	CUI_SkillKeyBinding* m_pSkillKeyBindingUI = nullptr;
	CUI_Minimap* m_pMinimapUI = nullptr;
	CUI_Money* m_pMoneyUI = nullptr;
	list<CObj*> m_listItems;
	list<CObj*> m_listMonsters;
	list<CObj*> m_listHitEffects;
	list<CSpawnerGenerator*> m_listSpawnerGenerators;
	list<CObj*> m_listSpawners;
	vector<CObj*> m_vecObjsToRender;
	const char* m_szMapDirectory = nullptr;
};

