#pragma once
#include "CGameWorld.h"

class CUI_Cursor;
class CCamera2D;
class CMapLoader;

class CWOL_World :
	public CGameWorld
{
public:
	CWOL_World();
	~CWOL_World();

	// CGameWorld을(를) 통해 상속됨
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void Ready(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	CObj* GetPlayer(void) const { return m_pPlayer; }
	CCamera2D* GetCamera(void) const { return m_pCamera; }
	list<CObj*>& GetListUsedPlayerSkills(void) { return m_plistUsedPlayerSkills; }
	list<CObj*>& GetListUsedMonsterSkills(void) { return m_plistUsedMonsterSkills; }
	list<CObj*>& GetListUIs(void) { return m_plistUIs; }
	list<CObj*>& GetListParticles(void) { return m_plistParticles; }

private:
	void LoadResources(void);

private:
	CUI_Cursor* m_pCursor = nullptr;
	CCamera2D* m_pCamera = nullptr;
	float m_fElapsedTime = 0.f;
	int m_iFrameCount = 0;

private:
	// 월드 오브젝트
	CObj* m_pPlayer = nullptr;
	list<CObj*> m_plistUsedMonsterSkills;
	list<CObj*> m_plistUsedPlayerSkills;
	list<CObj*> m_plistParticles;
	list<CObj*> m_plistUIs;
};

