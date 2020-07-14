#pragma once

class CBitmapObj;

class CBitmapManager
{
public:
	static CBitmapManager* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CBitmapManager;
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
	CBitmapManager();
	~CBitmapManager();
public:
	HDC FindBitmapMemDC(const TCHAR* _pImageKey);
	void InsertBitmap(const TCHAR* _pFilePath, const TCHAR* _pImageKey);
	void Release(void);
private:
	static CBitmapManager* m_pInstance;
	map<const TCHAR*, CBitmapObj*> m_mapBitmapObjs;
};

