#pragma once
class CMapFileMgr
{
public:
	CMapFileMgr();
	~CMapFileMgr();

public:
	static CMapFileMgr* GetInstance() {
		if (!m_pInstance)
			m_pInstance = new CMapFileMgr;

		return m_pInstance;
	}

	static void DestroyInstance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void GenerateAtlasLoadersFromFile(const char* _szDirectory, vector<CAtlasLoader*>& _rAtlasLoaders);
	void GenerateMapStructureFromFile(const char* _szDirectory, _map_structure_info& _rMapStructure);
private:
	static CMapFileMgr* m_pInstance;
};

