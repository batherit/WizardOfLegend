#pragma once

class CBitmapObj;

class CAtlasLoader
{
public:
	CAtlasLoader(int _iID, _atlas_loader_info& _stAtlasInfo);
	~CAtlasLoader();

public:
	void Update(float _fDeltaTime);	// Ŭ�� ����
	void Render(HDC& _hdc, CCamera2D* _pCamera);

public:
	void RenderAtlas(HDC& _hdc, CCamera2D* _pCamera);
	void RenderGrid(HDC& _hdc, CCamera2D* _pCamera);
	
	const HDC& GetMemDC(void);
	
	LONG GetStretchedLeft(void) const { return 0; }
	LONG GetStretchedTop(void) const { return 0; }
	LONG GetStretchedBottom(void) const { return m_iStretchedAtlasHeight; }
	LONG GetStretchedRight(void) const { return m_iStretchedAtlasWidth; }

	_atlas_loader_info GetAtlasInfo(void) const { return m_stAtlasInfo; }
	
	void SetVisible(bool _bIsVisible) { m_bIsVisible = _bIsVisible; }
	bool IsVisible(void) const { return m_bIsVisible; }

	_atlas_obj_info GetDetectedTileRowCol(const POINT& _ptClicked);

private:
	int m_iID;

	//HBITMAP m_bitmapAtlas;
	//HBITMAP m_bitmapOldAtlas;
	//CBitmapObj* m_pBitmapObj;
	_atlas_loader_info m_stAtlasInfo;
	
	// ����� ������
	vector<_atlas_obj_info> m_vecAtlasObjInfos;

	// �ε�� ��Ʋ�� �����κ��� �����Ǵ� ������ (���� ����)
	size_t m_iStretchedAtlasWidth;
	size_t m_iStretchedAtlasHeight;

	bool m_bIsVisible = false;
	const TCHAR* ctBitmapKey[3] = { L"WOL_TILE_DUNGEON", L"WOL_TILE_HOMETOWN" };
};

