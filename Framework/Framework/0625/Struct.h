#pragma once

class CAtlasLoader;

struct _obj_render_info {
	vector<CAtlasLoader*> vecAtlasLoaders;			// 아틀라스들
	LONG lMapLeft = 0;								// 타일 맵의 좌상단 시작 X
	LONG lMapTop = 0;								// 타일 맵의 좌상단 시작 Y
	size_t iTileWidth = 73;							// 타일 맵을 구성하는 타일의 너비 (픽셀)
	size_t iTileHeight = 73;						// 타일 맵을 구성하는 타일의 높이 (픽셀)
	size_t iMapWidth = 10;							// 가로로 놓일 수 있는 오브젝트의 최대 수
	size_t iMapHeight = 10;							// 세로로 놓일 수 있는 오브젝트의 최대 수
};

// 아틀라스를 로드하는데 쓰이는 구조체
struct _atlas_loader_info {
	char szAtlasFileDirectory[256] = "";
	int iID = -1;
	ATLAS_LOADER::E_TYPE eLoaderType = ATLAS_LOADER::TYPE_UNIFORM;
	int iAtlasWidth = 100;
	int iAtlasHeight = 100;
	float fAtlasRatio = 1.f;
	int iTileWidth = 73;
	int iTileHeight = 73;
	vector<RECT> vecOutputArea;	// NON_UNIFORM일 경우에 쓰임
};

// 아틀라스 로드 후 아틀라스 이미지를 객체별로 분할/할당할때 쓰이는 구조체
struct _atlas_obj_info {
	int iAtlasID;					// 아틀라스 오브젝트가 속해있는 아틀라스의 아이디 (무효는 -1로 초기화된다.)
	RECT rcOutputArea;				// 아틀라스의 어느 부분의 이미지를 커버하는지
	size_t iCoveredWidthTiles;		// 해당 이미지가 타일맵에서 너비를 얼마나 차지하는지 (타일 단위) => 타일같은 경우 1이 될 것이다.
	size_t iCoveredHeightTiles;		// 해당 이미지가 타일맵에서 높이를 얼마나 차지하는지 (타일 단위) => 타일같은 경우 1이 될 것이다.
};

// 타일맵에서 차지하는 시작 지점 (타일이 위치한 행과 열을 반환한다. EditorObj는 모두 이를 갖는다.)
struct _pivot_point {
	int iRow;
	int iCol;
};
//// 타일맵에 오브젝트를 그릴때 쓰이는 구조체
//struct _tile_obj_info {
//	_pivot_point stPivotPoint;
//	_atlas_obj_info stAtlasObjInfo;		// 아틀라스 
//};




