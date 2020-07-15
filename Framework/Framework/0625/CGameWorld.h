#pragma once

class CTimer;
class CSceneMgr;

class CGameWorld abstract
{
public:
	CGameWorld();
	~CGameWorld();

public:
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Ready(void) = 0;
	virtual void Update(void) = 0;
	virtual void LateUpdate(void) = 0;
	virtual void Render(void) = 0;
	virtual void Release(void) = 0;

public:
	bool ConfirmValidScene(void);
	void RunTick(void);
	void ClearWindow(void);
	void RenderWindow(void);
	HDC& GetHDC(void) { return m_hDC; }
	HDC& GetBackbufferDC(void) { return m_hBackbufferDC; }
	CTimer* GetTimer(void) { return m_pTimer; }
	CSceneMgr* GetSceneManager(void) { return m_pSceneManager; }

private:
	HDC m_hDC;
	HBITMAP m_hBackbuffer;
	HDC m_hBackbufferDC;
	CTimer* m_pTimer;
	CSceneMgr* m_pSceneManager;
};

