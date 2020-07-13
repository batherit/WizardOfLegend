#pragma once
#include "CTileMapObj.h"
class CAtlasObj :
	public CTileMapObj
{
public:
	CAtlasObj(const _obj_render_info & _rObjRenderInfo, int _iPivotRow, int _iPivotCol, const _atlas_obj_info& _rAtlasObjInfo);
	~CAtlasObj();

public:
	// CTileMapObj��(��) ���� ��ӵ�
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;

private:
	const _atlas_obj_info m_stAtlasObjInfo;

	
};

