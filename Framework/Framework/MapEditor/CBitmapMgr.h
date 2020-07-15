#pragma once

class CBitmapObj;

class CBitmapMgr
{
public:
	static CBitmapMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CBitmapMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	CBitmapMgr();
	~CBitmapMgr();
public:
	HDC FindBitmapMemDC(const TCHAR* _pImageKey);
	void InsertBitmap(const TCHAR* _pFilePath, const TCHAR* _pImageKey);
	void Release(void);
private:
	static CBitmapMgr* m_pInstance;
	map<const TCHAR*, CBitmapObj*> m_mapBitmapObjs;
};

