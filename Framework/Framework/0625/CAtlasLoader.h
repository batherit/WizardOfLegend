#pragma once
class CAtlasLoader
{
public:
	CAtlasLoader(_atlas_info& _stAtlasInfo);
	~CAtlasLoader();

public:
	void Update(float _fDeltaTime);	// 클릭 검출
	void Render(HDC& _hdc, CCamera2D* _pCamera);

public:
	void RenderAtlas(HDC& _hdc, CCamera2D* _pCamera);
	void RenderGrid(HDC& _hdc, CCamera2D* _pCamera);
	
	HBITMAP& GetBitmap(void) { return m_bitmapAtlas; }
	
	LONG GetStretchedLeft(void) const { return 0; }
	LONG GetStretchedTop(void) const { return 0; }
	LONG GetStretchedBottom(void) const { return m_iStretchedAtlasHeight; }
	LONG GetStretchedRight(void) const { return m_iStretchedAtlasWidth; }
	size_t GetStretchedTileWidth(void) const { return m_iStretchedTileWidth; }
	size_t GetStretchedTileHeight(void) const { return m_iStretchedTileHeight; }

	_atlas_info GetAtlasInfo(void) const { return m_stAtlasInfo; }
	
	void SetVisible(bool _bIsVisible) { m_bIsVisible = _bIsVisible; }
	bool IsVisible(void) const { return m_bIsVisible; }

	pair<int, int> GetDetectedTileRowCol(const POINT& _ptClicked);

private:
	HBITMAP m_bitmapAtlas;
	HBITMAP m_bitmapOldAtlas;
	_atlas_info m_stAtlasInfo;

	// 로드된 아틀라스 정보로부터 유도되는 데이터 (비율 영향)
	size_t m_iRow;
	size_t m_iCol;
	size_t m_iStretchedAtlasWidth;
	size_t m_iStretchedAtlasHeight;
	size_t m_iStretchedTileWidth;
	size_t m_iStretchedTileHeight;

	bool m_bIsVisible = false;
};

