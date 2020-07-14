#pragma once

class CKeyMgr
{
public:
	static CKeyMgr* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new CKeyMgr;

		return m_pInstance;
	}
	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	CKeyMgr();
	~CKeyMgr();

public:
	void Update(void);
	void Release(void);

public:
	bool IsKeyUp(KEY::E_KEY _eKey);
	bool IsKeyDown(KEY::E_KEY _eKey);
	bool IsKeyPressing(KEY::E_KEY _eKey);
	bool IsKeyNone(KEY::E_KEY _eKey);
	void SetOldClickedPoint(POINT& _ptOld) { m_ptOldClickedPoint = _ptOld; }
	POINT GetOldClickedPoint(void) { return m_ptOldClickedPoint; }

private:
	static DWORD m_dwOldKeyState;
	static DWORD m_dwCurrentKeyState;
	static POINT m_ptOldClickedPoint;

private:
	static CKeyMgr* m_pInstance;
};
