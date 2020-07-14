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
		fscanf_s(fpIn, "%d ", &iAtlasNum);			// 로드할 아틀라스 수
		_atlas_loader_info stAtlasInfo;
		for (int i = 0; i < iAtlasNum; i++) {
			// 문자열은 fgets로 하는게 안전한 것 같다.
			do {
				fgets(stAtlasInfo.szAtlasFileDirectory, sizeof(stAtlasInfo.szAtlasFileDirectory), fpIn);
			} while (strcmp(stAtlasInfo.szAtlasFileDirectory, "\n") == 0);	// 개행문자는 거른다.
			stAtlasInfo.szAtlasFileDirectory[strlen(stAtlasInfo.szAtlasFileDirectory) - 1] = '\0';	// 개행문자 제거
			int size = strlen(stAtlasInfo.szAtlasFileDirectory);
			fscanf_s(fpIn, " %d %d %d %d %f %d %d",
				&stAtlasInfo.iID,
				&stAtlasInfo.eLoaderType,
				&stAtlasInfo.iAtlasWidth,
				&stAtlasInfo.iAtlasHeight,
				&stAtlasInfo.fAtlasRatio,
				&stAtlasInfo.iTileWidth,
				&stAtlasInfo.iTileHeight);
			// i를 아틀라스 로더의 아이디로 삼는다.
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
			&_rMapStructure.lMapLeft,					// 타일 맵의 좌상단 시작 X
			&_rMapStructure.lMapTop,					// 타일 맵의 좌상단 시작 Y
			&_rMapStructure.iTileWidth,					// 타일 맵을 구성하는 타일의 너비 (픽셀)
			&_rMapStructure.iTileHeight,				// 타일 맵을 구성하는 타일의 높이 (픽셀)
			&_rMapStructure.iMapWidth,					// 가로로 놓일 수 있는 오브젝트의 최대 수
			&_rMapStructure.iMapHeight					// 세로로 놓일 수 있는 오브젝트의 최대 수
		);
	}
	if (fpIn) fclose(fpIn);
}
