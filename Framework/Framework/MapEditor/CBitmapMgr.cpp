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

HDC CBitmapMgr::FindBitmapMemDC(const TCHAR * _pImageKey)
{
	auto iter_find = m_mapBitmapObjs.find(_pImageKey);
	if (iter_find == m_mapBitmapObjs.end())
		return nullptr;

	return iter_find->second->GetMemDC();
}

void CBitmapMgr::InsertBitmap(const TCHAR * _pFilePath, const TCHAR * _pImageKey) // pImageKey Ex. L"Player"
{
	// Ű������ �˻�. 
	auto& iter_find = find_if(m_mapBitmapObjs.begin(), m_mapBitmapObjs.end(), [&](auto& _rPair)
	{
		return !lstrcmp(_pImageKey, _rPair.first);
	});
	// Ű���� end�� �ƴ϶�°� �̹� �̹����� �� �ִ�. 
	if (iter_find != m_mapBitmapObjs.end())
		return;
	//�ٵ� Ű���� end��. �׷�? ����. ����־����. 
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