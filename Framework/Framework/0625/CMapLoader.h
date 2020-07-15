#pragma once

class CGameWorld;
class CCamera2D;

class CMapLoader final
{
public:
	explicit CMapLoader(CGameWorld& _rGameWorld, const char* szMapDirectory);
	~CMapLoader();

public:
	void Ready(void) {};
	void Update(float _fDeltaTime);
	void LateUpdate(void);
	void Render(HDC& _hdc, CCamera2D* _pCamera);
	void Release(void);

public:
	const vector<CAtlasLoader*>& GetAtlasLoaders(void) const { return m_stMapRenderInfo.vecAtlasLoaders; }
	const _map_structure_info& GetMapStructureInfo(void) const { return m_stMapRenderInfo.stMapStructureInfo; }

public:
	list<CObj*>& GetColliders(void) { return m_listColliders; }
	list<CObj*>& GetTriggers(void) { return m_listTriggers; }

private:
	void ClearObjs(void);

private:
	CGameWorld& m_rGameWorld;
	_map_render_info m_stMapRenderInfo;
	list<CObj*> m_listAtlasObjs[ciMaxDrawLayerNum];
	list<CObj*> m_listColliders;
	list<CObj*> m_listTriggers;
	//list<CObj*> m_listTriggers;
	//list<CObj*> 
};

