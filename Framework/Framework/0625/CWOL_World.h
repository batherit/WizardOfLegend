#pragma once
#include "CGameWorld.h"

class CCamera2D;
class CMapLoader;

class CWOL_World :
	public CGameWorld
{
public:
	CWOL_World();
	~CWOL_World();

	// CGameWorld을(를) 통해 상속됨
	virtual LRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void Ready(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	CObj* GetPlayer(void) { return m_pPlayer; }

private:
	void LoadResources(void);

private:
	CCamera2D* m_pCamera = nullptr;
	CObj* m_pPlayer = nullptr;
	float m_fElapsedTime = 0.f;
	int m_iFrameCount = 0;
};

