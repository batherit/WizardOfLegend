#include "stdafx.h"
#include "CBitmapMgr.h"
#include "CBitmapObj.h"

CBitmapMgr* CBitmapMgr::m_pInstance = nullptr;

CBitmapMgr::CBitmapMgr()
{
}


CBitmapMgr::~CBitmapMgr()
{
	Release();
}

const HDC CBitmapMgr::FindBitmapMemDC(const TCHAR * _szImageKey) const
{
	auto iter_find = m_mapBitmapObjs.find(_szImageKey);
	if (iter_find == m_mapBitmapObjs.end())
		return nullptr;

	return iter_find->second->GetMemDC();
}

const CBitmapObj * CBitmapMgr::GetBitmapObj(const TCHAR * _szImageKey) const
{
	auto iter_find = m_mapBitmapObjs.find(_szImageKey);
	if (iter_find == m_mapBitmapObjs.end())
		return nullptr;

	return iter_find->second;
}

void CBitmapMgr::InsertBitmap(const TCHAR * _pFilePath, const TCHAR * _pImageKey) // pImageKey Ex. L"Player"
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

void CBitmapMgr::Release(void)
{
	for (auto& rPair : m_mapBitmapObjs)
	{
		DeleteSafe(rPair.second);
	}
	m_mapBitmapObjs.clear();
}
