#pragma once

class CGameWorld;
class CCamera2D;
class CMapObjsGroup;

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
	// 플레이어와 충돌을 확인하기 위해 데이터 접근 함수를 만들어줌.
	const vector<CMapObjsGroup*>& GetAtlasObjsGroups(int _iLayerIndex) { return m_vecAtlasObjsGroups[_iLayerIndex]; }
	const vector<CMapObjsGroup*>& GetCollidersGroups(void) const { return m_vecCollidersGroups; }
	const vector<CMapObjsGroup*>& GetTriggersGroups(void) const { return m_vecTriggersGroups; }
	const vector<CObj*>& GetDoors(void) const { return m_vecActiveDoors; }
	const CObj* GetSpawnPoint(void) const { return m_pSpawnPoint; }

private:
	void ClearObjs(void);

private:
	CGameWorld& m_rGameWorld;

	vector<CMapObjsGroup*> m_vecAtlasObjsGroups[ciMaxDrawLayerNum];
	vector<CMapObjsGroup*> m_vecCollidersGroups;
	vector<CMapObjsGroup*> m_vecTriggersGroups;

	vector<CObj*> m_vecUnactiveDoors;
	vector<CObj*> m_vecActiveDoors;
	CObj* m_pSpawnPoint = nullptr;
};

