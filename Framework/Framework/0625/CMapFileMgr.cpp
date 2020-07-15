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
		for (int i = 0; i < iAtlasNum; i++) {
			// ���ڿ��� fgets�� �ϴ°� ������ �� ����.
			do {
				fgets(stAtlasInfo.szAtlasFileDirectory, sizeof(stAtlasInfo.szAtlasFileDirectory), fpIn);
			} while (strcmp(stAtlasInfo.szAtlasFileDirectory, "\n") == 0);	// ���๮�ڴ� �Ÿ���.
			stAtlasInfo.szAtlasFileDirectory[strlen(stAtlasInfo.szAtlasFileDirectory) - 1] = '\0';	// ���๮�� ����
			int size = strlen(stAtlasInfo.szAtlasFileDirectory);
			fscanf_s(fpIn, " %d %d %d %d %f %d %d",
				&stAtlasInfo.iID,
				&stAtlasInfo.eLoaderType,
				&stAtlasInfo.iAtlasWidth,
				&stAtlasInfo.iAtlasHeight,
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
