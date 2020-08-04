#pragma once

class CTimer;
class CSceneMgr;
class CSpace;
class CCamera2D;
class CObj;

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
	CTimer* GetTimer(void) const { return m_pTimer; }
	CSceneMgr* GetSceneManager(void) const { return m_pSceneManager; }
	CSpace* GetViewSpace(void) const { return m_pViewSpace; }
	virtual CCamera2D* GetCamera(void) const { return nullptr; }
	list<CObj*>& GetListObjs(void) { return m_listObjs; } // 객체의 생성과 소멸은 객체를 생성/소멸하는 주체가 담당한다.

private:
	HDC m_hDC;
	HDC m_hBackbufferDC;
	HDC m_hBackbufferClearDC;
	CTimer* m_pTimer;
	CSpace* m_pViewSpace;
	CSceneMgr* m_pSceneManager;
	list<CObj*> m_listObjs;
};

