#pragma once
#include "CTileMapObj.h"
class CAtlasObj :
	public CTileMapObj
{
public:
	CAtlasObj(const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol, const _atlas_obj_info& _rAtlasObjInfo);
	~CAtlasObj();

public:
	// CTileMapObj을(를) 통해 상속됨
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;

private:
	const _atlas_obj_info m_stAtlasObjInfo;

	
};

