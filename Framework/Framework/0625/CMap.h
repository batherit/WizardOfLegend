#pragma once

class CGameWorld;

class CMap final
{
public:
	explicit CMap(CGameWorld& _rGameWorld, const char* szMapDirectory);
	~CMap();

public:
	void Ready(void) {};
	void Update(float _fDeltaTime);
	void LateUpdate(void);
	void Render(HDC& _hdc, CCamera2D* _pCamera2D);
	void Release(void);

public:
	const vector<CAtlasLoader*>& GetAtlasLoaders(void) const { return m_stMapRenderInfo.vecAtlasLoaders; }

private:
	CGameWorld& m_rGameWorld;

	_map_render_info m_stMapRenderInfo;
	list<CObj*> m_listAtlasObjs[ciMaxDrawLayerNum];
	list<CObj*> m_listColliders;
	//list<CObj*> m_listTriggers;
	//list<CObj*> 
};

