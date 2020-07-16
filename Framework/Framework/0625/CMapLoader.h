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
	// �÷��̾�� �浹�� Ȯ���ϱ� ���� ������ ���� �Լ��� �������.
	const vector<CMapObjsGroup*>& GetCollidersGroups(void) const { return m_vecCollidersGroups; }
	const vector<CMapObjsGroup*>& GetTriggersGroups(void) const { return m_vecTriggersGroups; }
	const vector<CObj*>& GetDoors(void) const { return m_vecActiveDoors; }

private:
	void ClearObjs(void);

private:
	CGameWorld& m_rGameWorld;

	vector<CMapObjsGroup*> m_vecAtlasObjsGroups[ciMaxDrawLayerNum];
	vector<CMapObjsGroup*> m_vecCollidersGroups;
	vector<CMapObjsGroup*> m_vecTriggersGroups;

	vector<CObj*> m_vecUnactiveDoors;
	vector<CObj*> m_vecActiveDoors;
};

