#pragma once
class CAtlasLoader
{
public:
	CAtlasLoader(_atlas_info& _stAtlasInfo);
	~CAtlasLoader();

public:
	void Update(float _fDeltaTime);	// 클릭 검출
	void Render(HDC& _hdc, CCamera2D* _pCamera);

private:
	HBITMAP m_bitmapAtlas;
	HBITMAP m_bitmapOldAtlas;
};

