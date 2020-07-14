#include "stdafx.h"
#include "CBitmapManager.h"
#include "CBitmapObj.h"

CBitmapManager* CBitmapManager::m_pInstance = nullptr;

CBitmapManager::CBitmapManager()
{
}


CBitmapManager::~CBitmapManager()
{
	Release();
}

HDC CBitmapManager::FindBitmapMemDC(const TCHAR * _pImageKey)
{
	auto iter_find = m_mapBitmapObjs.find(_pImageKey);
	if (iter_find == m_mapBitmapObjs.end())
		return nullptr;

	return iter_find->second->GetMemDC();
}

void CBitmapManager::InsertBitmap(const TCHAR * _pFilePath, const TCHAR * _pImageKey) // pImageKey Ex. L"Player"
{
	// 키값으로 검사. 
	auto& iter_find = find_if(m_mapBitmapObjs.begin(), m_mapBitmapObjs.end(), [&](auto& _rPair)
	{
		return !lstrcmp(_pImageKey, _rPair.first);
	});
	// 키값이 end가 아니라는건 이미 이미지가 들어가 있다. 
	if (iter_find != m_mapBitmapObjs.end())
		return;
	//근데 키값이 end랴. 그럼? 없다. 집어넣어야지. 
	CBitmapObj* pBitmap = new CBitmapObj;
	pBitmap->LoadBmp(_pFilePath);
	m_mapBitmapObjs.emplace(_pImageKey, pBitmap);
}

void CBitmapManager::Release(void)
{
	for (auto& rPair : m_mapBitmapObjs)
	{
		DeleteSafe(rPair.second);
	}
	m_mapBitmapObjs.clear();
}
