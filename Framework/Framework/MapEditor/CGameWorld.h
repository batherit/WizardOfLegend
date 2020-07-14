#pragma once

class CTimer;

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
	void RunTick(void);
	void ClearWindow(void);
	void RenderWindow(void);
	HDC& GetHDC(void) { return m_hDC; }
	HDC& GetBackbufferDC(void) { return m_hBackbufferDC; }
	CTimer* GetTimer(void) { return m_pTimer; }

private:
	HDC m_hDC;
	HBITMAP m_hBackbuffer;
	HDC m_hBackbufferDC;
	CTimer* m_pTimer;
};

