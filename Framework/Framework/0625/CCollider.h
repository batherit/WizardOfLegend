#pragma once
#include "CTileMapObj.h"
class CCollider :
	public CTileMapObj
{
public:
	CCollider(const _obj_render_info & _rObjRenderInfo, int _iPivotRow, int _iPivotCol);
	~CCollider();

public:
	// CTileMapObj을(를) 통해 상속됨
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
};

