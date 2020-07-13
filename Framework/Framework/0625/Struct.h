#pragma once

class CAtlasLoader;

struct _obj_render_info {
	vector<CAtlasLoader*> vecAtlasLoaders;			// ��Ʋ�󽺵�
	LONG lMapLeft = 0;								// Ÿ�� ���� �»�� ���� X
	LONG lMapTop = 0;								// Ÿ�� ���� �»�� ���� Y
	size_t iTileWidth = 73;							// Ÿ�� ���� �����ϴ� Ÿ���� �ʺ� (�ȼ�)
	size_t iTileHeight = 73;						// Ÿ�� ���� �����ϴ� Ÿ���� ���� (�ȼ�)
	size_t iMapWidth = 10;							// ���η� ���� �� �ִ� ������Ʈ�� �ִ� ��
	size_t iMapHeight = 10;							// ���η� ���� �� �ִ� ������Ʈ�� �ִ� ��
};

// ��Ʋ�󽺸� �ε��ϴµ� ���̴� ����ü
struct _atlas_loader_info {
	char szAtlasFileDirectory[256] = "";
	int iID = -1;
	ATLAS_LOADER::E_TYPE eLoaderType = ATLAS_LOADER::TYPE_UNIFORM;
	int iAtlasWidth = 100;
	int iAtlasHeight = 100;
	float fAtlasRatio = 1.f;
	int iTileWidth = 73;
	int iTileHeight = 73;
	vector<RECT> vecOutputArea;	// NON_UNIFORM�� ��쿡 ����
};

// ��Ʋ�� �ε� �� ��Ʋ�� �̹����� ��ü���� ����/�Ҵ��Ҷ� ���̴� ����ü
struct _atlas_obj_info {
	int iAtlasID;					// ��Ʋ�� ������Ʈ�� �����ִ� ��Ʋ���� ���̵� (��ȿ�� -1�� �ʱ�ȭ�ȴ�.)
	RECT rcOutputArea;				// ��Ʋ���� ��� �κ��� �̹����� Ŀ���ϴ���
	size_t iCoveredWidthTiles;		// �ش� �̹����� Ÿ�ϸʿ��� �ʺ� �󸶳� �����ϴ��� (Ÿ�� ����) => Ÿ�ϰ��� ��� 1�� �� ���̴�.
	size_t iCoveredHeightTiles;		// �ش� �̹����� Ÿ�ϸʿ��� ���̸� �󸶳� �����ϴ��� (Ÿ�� ����) => Ÿ�ϰ��� ��� 1�� �� ���̴�.
};

// Ÿ�ϸʿ��� �����ϴ� ���� ���� (Ÿ���� ��ġ�� ��� ���� ��ȯ�Ѵ�. EditorObj�� ��� �̸� ���´�.)
struct _pivot_point {
	int iRow;
	int iCol;
};
//// Ÿ�ϸʿ� ������Ʈ�� �׸��� ���̴� ����ü
//struct _tile_obj_info {
//	_pivot_point stPivotPoint;
//	_atlas_obj_info stAtlasObjInfo;		// ��Ʋ�� 
//};




