#include "stdafx.h"
#include "CMapFileMgr.h"
#include "CAtlasLoader.h"

CMapFileMgr* CMapFileMgr::m_pInstance = nullptr;

CMapFileMgr::CMapFileMgr()
{
}


CMapFileMgr::~CMapFileMgr()
{
}

void CMapFileMgr::GenerateAtlasLoadersFromFile(const char* _szDirectory, vector<CAtlasLoader*>& _rAtlasLoaders)
{
	FILE* fpIn = nullptr;
	errno_t err = fopen_s(&fpIn, _szDirectory, "rt"); /*"../MapToolDatas/Atlas.txt"*/
	if (!err) {
		int iAtlasNum = 0;
		fscanf_s(fpIn, "%d ", &iAtlasNum);			// �ε��� ��Ʋ�� ��
		_atlas_loader_info stAtlasInfo;
		char szBuf[128];
		for (int i = 0; i < iAtlasNum; i++) {
			////// ���ڿ��� fgets�� �ϴ°� ������ �� ����.
			//do {
			//	fgets(szBuf, sizeof(szBuf), fpIn);
			//} while (strcmp(szBuf, "\n") == 0);	// ���๮�ڴ� �Ÿ���.
			//szBuf[strlen(szBuf) - 1] = '\0';	// ���๮�� ����

			//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szBuf, strlen(szBuf), stAtlasInfo.szAtlasKey, 128);

			fscanf_s(fpIn, " %d %d %f %d %d",
				&stAtlasInfo.iAtlasID,
				&stAtlasInfo.eLoaderType,
				&stAtlasInfo.fAtlasRatio,
				&stAtlasInfo.iTileWidth,
				&stAtlasInfo.iTileHeight);
			// i�� ��Ʋ�� �δ��� ���̵�� ��´�.
			_rAtlasLoaders.emplace_back(new CAtlasLoader(i, stAtlasInfo));
		}
	}
	if (fpIn) fclose(fpIn);
}

void CMapFileMgr::GenerateMapStructureFromFile(const char * _szDirectory, _map_structure_info & _rMapStructure)
{
	FILE* fpIn = nullptr;
	errno_t err = fopen_s(&fpIn, _szDirectory, "rt"); /*"../MapToolDatas/Atlas.txt"*/
	if (!err) {
		fscanf_s(fpIn, "%li %li %d %d %d %d",
			&_rMapStructure.lMapLeft,					// Ÿ�� ���� �»�� ���� X
			&_rMapStructure.lMapTop,					// Ÿ�� ���� �»�� ���� Y
			&_rMapStructure.iTileWidth,					// Ÿ�� ���� �����ϴ� Ÿ���� �ʺ� (�ȼ�)
			&_rMapStructure.iTileHeight,				// Ÿ�� ���� �����ϴ� Ÿ���� ���� (�ȼ�)
			&_rMapStructure.iMapWidth,					// ���η� ���� �� �ִ� ������Ʈ�� �ִ� ��
			&_rMapStructure.iMapHeight					// ���η� ���� �� �ִ� ������Ʈ�� �ִ� ��
		);
	}
	if (fpIn) fclose(fpIn);
}
